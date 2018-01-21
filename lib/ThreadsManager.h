#ifndef ALWAYSBUSY_THREADSMANAGER_H
#define ALWAYSBUSY_THREADSMANAGER_H

#include <thread>
#include <list>
#include <unistd.h>
#include "Semaphore.h"

#define MSLEEP(x) (usleep((x*100))) //休眠x*100us

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

public:
    explicit ThreadsManager(int count):threads(nullptr),_run(true) {
        assert(count>0 && count<12);
        _count = count;
        _running = count;
        semaphore = new Semaphore("semaphore",0);
        assert(semaphore != nullptr);
    }

    ~ThreadsManager() {
        if(threads != nullptr)
            delete[] threads;
    }

    bool create(void (*f)(int)) {
        threads = new std::thread[_count];
        if(threads == nullptr){
            std::cout<<"Failed to create threads"<<std::endl;
            exit(-1);
        }
        for (int i=0;i<_count;i++){
            threads[i] = std::thread(f,i);
        }
        return false;
    }

    bool run() {
        return _run;
    }

    void join() {
        MSLEEP(10);//休眠500us,等待被唤醒的线程启动
        while (_running != 0){
            MSLEEP(5);//休眠500us之后再查询有无进程再运行
        }
    }

    void wait() {
        _mutex.lock();
        _running--;
        _mutex.unlock();
        semaphore->wait();
        _mutex.lock();
        _running++;
        _mutex.unlock();
    }

    void signal() {
        semaphore->signal();
    }

    void kill() {
        join();
        _run = false;
        semaphore->signalAll();
        for (int i=0;i<_count;i++){
            threads[i].join();//等待所有线程退出
        }
    }

    void add(DataType data) {
        _mutexDataList.lock();
        _dataList.emplace_back(data);
        _mutexDataList.unlock();
        signal();
    }

    DataType pop() {
        _mutexDataList.lock();
        DataType data = _dataList.front();   //取出第一个
        _dataList.pop_front();             //删除第一个
        _mutexDataList.unlock();
        return data;
    }
};


#endif //ALWAYSBUSY_THREADSMANAGER_H
