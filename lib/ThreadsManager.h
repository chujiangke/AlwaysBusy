#ifndef ALWAYSBUSY_THREADSMANAGER_H
#define ALWAYSBUSY_THREADSMANAGER_H

#include <thread>
#include <list>
#include "Semaphore.h"
#include "tools.h"

#ifdef _WIN32
#include <windows.h>
#define SLEEP(x)  (Sleep(x)) //休眠xms
#else
#include <unistd.h>
#define SLEEP(x) (usleep((x*100))) //休眠x*100us
#endif

using namespace SEM;

template <typename DataType>
class ThreadsManager {

private:
    int _count;                 //线程数量
    int _running;               //正在运行的线程数量
    std::mutex _mutex;          //_running的互斥锁
    std::thread *threads;       //线程数组指针
    Semaphore *semaphore;       //信号量
    bool _run;                  //是否继续运行子线程
    std::list<DataType> _dataList;      //数据链表
    std::mutex _mutexDataList;          //数据链表互斥锁
    int _coreCount;             //CPU核心数量

public:
    /**
     * 构造函数，获取逻辑核心数量和新建用来管理资源的信号量
     * @param count 线程数量
     */
    explicit ThreadsManager(int count):threads(nullptr),_run(true) {
        _coreCount = getCoreCount();
        ASSERT(count>0 && count<_coreCount,"线程数量不正确");//
        _count = count;
        _running = count;
        semaphore = new Semaphore("semaphore",0);
        ASSERT(semaphore != nullptr,"新建信号量失败");
    }

    ~ThreadsManager() {
        if(threads != nullptr)
            delete[] threads;
        if(semaphore != nullptr)
            delete semaphore;
    }

    /**
     * 创建子线程
     * @param f 子线程中执行的函数，函数的形式需要满足一定的格式
     * @return 没啥用
     */
    bool create(void (*f)(int)) {
        threads = new std::thread[_count];
        if(threads == nullptr){
            std::cout<<"Failed to create threads"<<std::endl;
            exit(-1);
        }
        for (int i=0;i<_count;i++){
            threads[i] = std::thread(f,i);
        }
        return true;
    }

    /**
     * 返回是否继续运行的标志_run
     * @return 还要继续运行线程_run的值为true，返回的是true，否则返回false
     */
    bool run() {
        return _run;
    }

    /**
     * 等待子线程处理完数据
     */
    void join() {
        SLEEP(5);//休眠500us,等待被唤醒的线程启动
        while (_running != 0){
            SLEEP(1);//休眠500us之后再查询有无进程再运行
        }
    }

    /**
     * wait操作
     */
    void wait() {
        _mutex.lock();
        _running--;
        _mutex.unlock();
        semaphore->wait();
        _mutex.lock();
        _running++;
        _mutex.unlock();
    }

    /**
     * signal操作
     */
    void signal() {
        semaphore->signal();
    }

    /**
     * 通知所有线程退出
     */
    void kill() {
        join();
        _run = false;
        semaphore->signalAll();
        for (int i=0;i<_count;i++){
            threads[i].join();//等待所有线程退出
        }
    }

    /**
     * 向队列中添加数据
     * @param data 数据
     */
    void add(DataType data) {
        _mutexDataList.lock();
        _dataList.emplace_back(data);
        _mutexDataList.unlock();
        signal();
    }

    /**
     * 从队列中取出数据
     * @return 数据
     */
    DataType pop() {
        _mutexDataList.lock();
        DataType data = _dataList.front();   //取出第一个
        _dataList.pop_front();             //删除第一个
        _mutexDataList.unlock();
        return data;
    }

    /**
     * 获取CPU逻辑核心数量
     * @return 核心数量
     */
    static int getCoreCount()
    {
        int count = 1; // 至少一个
#ifdef _WIN32
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        count = si.dwNumberOfProcessors;
#else
        count = static_cast<int>(sysconf(_SC_NPROCESSORS_CONF));
#endif
        return count;
    }
};


#endif //ALWAYSBUSY_THREADSMANAGER_H
