#ifndef ALWAYSBUSY_THREADSMANAGER_H
#define ALWAYSBUSY_THREADSMANAGER_H

#include <thread>
#include <vector>
#include <stdexcept>

#include "Semaphore.h"

#ifdef _WIN32
#include <windows.h>
#define SLEEP(x)  (Sleep(x)) //休眠xms
#else
#include <unistd.h>

#define SLEEP(x) (usleep((x*100))) //休眠x*100us
#endif

template <typename T>
class ThreadsManager {

public:
    typedef typename std::vector<T>::iterator Iterator;

    /**
     * 线程管理器的构造函数
     * @param count 线程总数
     */
    explicit ThreadsManager(int count):_run_or_not(true) {
        assert(count>0 && count<getCoreCount());
        _max_thread_count = count;
        _running_thread = count;
        _semaphore = new SEM::Semaphore("_semaphore",0);
        assert(_semaphore != nullptr);
        _data_pointer = 0;
    }

    ~ThreadsManager() {
        if(_semaphore != nullptr)
            delete _semaphore;
        _data_vector.clear();
    }

    /**
     * 创建线程
     * @param f 线程函数
     * @return
     */
    void create(void (*f)(int)) {
        for (int i=0;i<_max_thread_count;i++){
            _threads.push_back(std::thread(f,i));
        }
    }

    /**
     * 返回是否继续运行的标志
     * @return 继续运行返回true,否则返回false
     */
    bool run() {
        return _run_or_not;
    }

    /**
     * 等待所有子线程阻塞
     */
    void join() {
        SLEEP(5);//休眠500us,等待被唤醒的线程启动
        while (_running_thread != 0){
            SLEEP(1);//休眠500us之后再查询有无进程再运行
        }
    }

    /**
     * 子线程中调用,有资源时会立即返回
     */
    void wait() {
        _mutex.lock();
        _running_thread--;
        _mutex.unlock();
        _semaphore->wait();
        _mutex.lock();
        _running_thread++;
        _mutex.unlock();
    }

    /**
     * 唤醒一个阻塞的线程
     */
    void signal() {
        _semaphore->signal();
    }

    /**
     * 退出所有线程
     */
    void kill() {
        join();
        _run_or_not = false;
        _semaphore->signalAll();
        for (int i=0;i<_max_thread_count;i++){
            _threads[i].join();//等待所有线程退出
        }
    }

    /**
     * 添加数据
     * @param data 数据
     */
    void add(T data) {
        _mutex_data_vector.lock();
        _data_vector.emplace_back(data);
        _mutex_data_vector.unlock();
        signal();
    }

    /**
     * 取出数据
     * @return  数据
     */
    T pop() {
        _mutex_data_vector.lock();
        T data = _data_vector.front();        //取出第一个
        _data_vector.erase(_data_vector.begin());    //删除第一个
        _mutex_data_vector.unlock();
        return data;
    }

    /**
     * 返回链表中的下一个数据
     * @return 数据
     */
    T next(){
        int index=0;
        _mutex_data_vector.lock();
        if(_data_pointer < _data_vector.size()){
            index = _data_pointer;
            _data_pointer++;
        }
        _mutex_data_vector.unlock();
        T data = _data_vector[index];
        data.index = index;
        return data;
    }
    /**
     * 清空数据链表
     */
    void clear(){
        _mutex_data_vector.lock();
        _data_pointer = 0;
        _data_vector.clear();
        _mutex_data_vector.unlock();
    }

    /**
     * 返回数据
     * @param index
     * @return
     */
    T get(int index){
        if(index<_data_vector.size())
            return _data_vector[index];
        else
            throw std::out_of_range("index is out of range");
    }

    void set(T data){
        if(data.index<_data_vector.size())
            _data_vector[data.index] = data;
        else
            throw std::out_of_range("index is out of range");
    }

    /**
     * 返回数据vector大小
     * @return 大于0整数
     */
    ulong size(){
        return _data_vector.size();
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

private:
    int                         _max_thread_count;  //线程数量
    int                         _running_thread;    //正在运行的线程数量
    std::mutex                  _mutex;             //_running的互斥锁
    std::vector<std::thread>    _threads;           //线程数组容器
    SEM::Semaphore              *_semaphore;        //同步线程的信号量
    bool                        _run_or_not;        //是否继续运行子线程

    std::vector<T>              _data_vector;       //数据向量
    std::mutex                  _mutex_data_vector; //数据向量互斥锁
    int                         _data_pointer;      //下一个访问的数据位置
};


#endif //ALWAYSBUSY_THREADSMANAGER_H
