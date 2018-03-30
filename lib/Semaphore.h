#include <semaphore.h>

typedef struct Semaphore {
    char _name[200];//信号量名字,不超过200个字符
    sem_t _sem;
    struct Semaphore *this;

    void (*init)(struct Semaphore *sem,char name[], unsigned value);
    void (*wait)(struct Semaphore *sem);
    void (*wait_time)(struct Semaphore *sem,int ms);
    void (*signal)(struct Semaphore *sem);
    void (*signalAll)(struct Semaphore *sem);
    void (*name)(struct Semaphore *sem,char name[]);
    void (*destroy)(struct Semaphore *sem);
}Semaphore;

void initSemaphore(Semaphore *sem);
