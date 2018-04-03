#include <string.h>
#include <time.h>
#include <malloc.h>
#include <stdlib.h>
#include "Semaphore.h"

/**
 * 初始化信号量
 * @param name
 */
void init(Semaphore *sem,char name[], unsigned value) {
    strcpy(sem->name,name);
    sem_init(&(sem->_sem), 0, value);
}

/**
 * 相当于信号量机制里面的P操作.
 * 有资源时,函数会立即返回,否则会阻塞调用此函数的线程.
 */
void wait(Semaphore *sem) {
    sem_wait(&(sem->_sem));
}

/**
 * 相当于信号量机制里面的P操作.
 * 有资源时,函数会立即返回,否则会阻塞调用此函数的线程.
 * 但如果等待时间超过ms指定的值，会唤醒所有阻塞线程.
 * @param ms 等待时间(ms)
 */
void wait_time(Semaphore *sem,int ms) {
    struct timespec t;
    t.tv_sec = ms / 1000;
    t.tv_nsec = (ms % 1000) * 1000;
    sem_timedwait(&(sem->_sem), &t);
}

/**
 * 如果有阻塞的线程,则随机唤醒一个线程，相当于
 * 信号量机制里面的V操作.否则立即返回.
 */
void signal(Semaphore *sem) {
    sem_post(&(sem->_sem));
}

/**
 * 销毁信号量
 */
void destroy(Semaphore *sem) {
    sem_destroy(&(sem->_sem));
    free(sem);
    sem = NULL;
}

/**
 * 初始化信号量结构体
 * @param sem 指向信号量的指针
 */
void init_semaphore(Semaphore *sem) {
    sem->this = sem;
    sem->init = init;
    sem->wait = wait;
    sem->wait_time = wait_time;
    sem->signal = signal;
    sem->destroy = destroy;
}

/**
 * 初始化信号量
 * @return 返回指向一个信号量的指针
 */
Semaphore *get_semaphore() {
    Semaphore *sem;
    sem = (Semaphore *)malloc(sizeof(Semaphore));
    if(sem == NULL){
        printf("申请内存失败，正在退出...");
        exit(-1);
    }

    sem->this = sem;
    sem->init = init;
    sem->wait = wait;
    sem->wait_time = wait_time;
    sem->signal = signal;
    sem->destroy = destroy;

    return sem;
}