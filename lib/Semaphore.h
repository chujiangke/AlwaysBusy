#include <semaphore.h>

typedef struct Semaphore {
    char _name[200];//信号量结构体名字,不超过200个字符
    sem_t _sem;//标准信号量
    struct Semaphore *this;//指向自己的指针

    //接下来的这几行，每行定义了一个函数指针，指向一个函数，不了解的可以去了解一下。
    void (*init)(struct Semaphore *sem,char name[], unsigned value);
    void (*wait)(struct Semaphore *sem);
    void (*wait_time)(struct Semaphore *sem,int ms);
    void (*signal)(struct Semaphore *sem);
    void (*signalAll)(struct Semaphore *sem);
    void (*name)(struct Semaphore *sem,char name[]);
    void (*destroy)(struct Semaphore *sem);
}Semaphore;

//初始化信号量的第一种方式
void initSemaphore(Semaphore *sem);
//初始化信号量的第二种方式
Semaphore *getSemaphore();
