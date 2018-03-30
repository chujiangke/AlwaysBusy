#include <string.h>
#include <time.h>
#include <malloc.h>
#include "Semaphore.h"

/**
 * 初始化信号量
 * @param name
 */
void init(Semaphore *sem,char name[], unsigned value) {
    strcpy(sem->_name,name);
    sem_init(&(sem->_sem), 0, value);
}

/**
 * 相当于信号量机制里面的P操作.
 * _count大于0(有资源)时,函数会立即返回,否则会阻塞调用此函数的线程.
 */
void wait(Semaphore *sem) {
    sem_wait(&(sem->_sem));
}

/**
 * 相当于信号量机制里面的P操作.
 * _count大于0(有资源)时,函数会立即返回,否则会阻塞调用此函数的线程.
 * 但如果等待时间超过seconds指定的值，会唤醒所有阻塞线程.
 * @param ms 等待时间(ms)
 */
void wait_time(Semaphore *sem,int ms) {
    struct timespec t;
    t.tv_sec = ms / 1000;
    t.tv_nsec = (ms % 1000) * 1000;
    sem_timedwait(&(sem->_sem), &t);
}

/**
 * 如果有阻塞的线程,则随机唤醒一个线程，相当于信号量机制里面的V操作.否则
 * 立即返回.
 */
void signal(Semaphore *sem) {
    sem_post(&(sem->_sem));
}

/**
 * 如果有线程阻塞,唤醒阻塞的所有线程;否则立即返回.
 */
void signalAll(Semaphore *sem) {
    int count = 0;
    sem_getvalue(&(sem->_sem), &count);
    while (count > 0) {
        signal(sem);
        count--;
    }
}

/**
 * 返回这个信号量的名字
 * @return 名字
 */
void name(Semaphore *sem,char name[]) {
    strcpy(name,sem->_name);
}

/**
 * 销毁信号量
 */
void destroy(Semaphore *sem) {
    sem_destroy(&(sem->_sem));
}

void initSemaphore(Semaphore *sem) {
    sem->this = sem;
    sem->init = init;
    sem->wait = wait;
    sem->wait_time = wait_time;
    sem->signal = signal;
    sem->signalAll = signalAll;
    sem->name = name;
    sem->destroy = destroy;
}
