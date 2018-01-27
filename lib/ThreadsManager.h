#ifndef ALWAYSBUSY_THREADSMANAGER_H
#define ALWAYSBUSY_THREADSMANAGER_H

#include <thread>
#include <list>
#include <unistd.h>
#include "Semaphore.h"
#include "tools.h"

#define SLEEP(x) (usleep((x*100))) //休眠x*100us

using namespace SEM;

template <typename DataType>
class ThreadsManager {
public:
    explicit ThreadsManager(int count):threads(nullptr),_run(true) {
        ASSERT(count>0 && count<12);//
        _count = count;
        _running = count;
        semaphore = new Semaphore("semaphore",0);
        ASSERT(semaphore != nullptr);
    }

    ~ThreadsManager() {
        if(threads != nullptr)
            delete[] threads;
        if(semaphore != nullptr)
            delete semaphore;
    }

    /**
     * 创建线程
     * @param f 线程执行的函数
     * @return 创建成功返回true
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
     * 是否继续运行线程
     * @return 继续的话返回true
     */
    bool run() {
        return _run;
    }

    /**
     * 主动查询是否还有线程再执行，没有任务执行时才返回，相当于阻塞
     */
    void join() {
        SLEEP(10);//休眠1000us,等待被唤醒的线程启动
        while (_running != 0){
            SLEEP(1);//休眠100us之后再查询有无进程再运行
        }
    }

    /**
     * 子线程中调用，没数据时等待，有数据时会立即返回
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
     * 唤醒一个线程处理数据
     */
    void signal() {
        semaphore->signal();
    }

    /**
     * 等待所有线程执行完任务，然后设置运行标志_run为false，
     * 当线程再次唤醒时，检查_run标志，然后决定是否退出程序
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
     * 向数据链表中添加待处理的数据
     * @param data 自定义的数据类型
     */
    void add(DataType data) {
        _mutexDataList.lock();
        _dataList.emplace_back(data);
        _mutexDataList.unlock();
        signal();
    }

    /**
     * 取出数据链表中的第一个数据，并且删除第一个数据
     * @return 数据
     */
    DataType pop() {
        _mutexDataList.lock();
        DataType data = _dataList.front();   //取出第一个
        _dataList.pop_front();             //删除第一个
        _mutexDataList.unlock();
        return data;
    }

private:
    int _count;                 //线程数量
    int _running;               //正在运行的线程数量
    std::mutex _mutex;          //_running的互斥锁
    std::thread *threads;       //线程数组指针
    Semaphore *semaphore;       //信号量
    bool _run;                  //是否继续运行子线程
    std::list<DataType> _dataList;      //数据链表
    std::mutex _mutexDataList;          //数据链表互斥锁
};


#endif //ALWAYSBUSY_THREADSMANAGER_H
