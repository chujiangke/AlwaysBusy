#include "Semaphore.h"
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

struct Goods{
    int i;
};//产品

Semaphore g_sem;

struct Goods g_goods;//商品货架

pthread_mutex_t g_mutex;

void *producer(void *arg){

    int i=0;
    while (++i<5*5) {
        //休眠一段随机时间,代表生产过程
        sleep((unsigned)rand()%3+1);
        //生产
        pthread_mutex_lock(&g_mutex);
        g_goods.i = rand()%1000;
        printf("生产产品:%d\n",g_goods.i);
        g_sem.signal(g_sem.this);
        pthread_mutex_unlock(&g_mutex);
    }
}

void *costumer(void *arg){
    int i=0;
    int id = *((int *)arg);
    while (++i<5) {
        g_sem.wait(g_sem.this);//有资源会立即返回,没有资源则会等待
        //消费
        pthread_mutex_lock(&g_mutex);
        printf("消费者%d消费产品:%d\n",id,g_goods.i);
        pthread_mutex_unlock(&g_mutex);
    }
}
int main() {
    initSemaphore(&g_sem);
    g_sem.init(g_sem.this,"sem", 0);
    srand((unsigned)time(NULL));
    pthread_mutex_init(&g_mutex,NULL);

    pthread_t producer_t;
    pthread_t costumer_t[5];
    pthread_create(&producer_t,NULL,producer,NULL);
    for(int i=0;i<5;i++){
        pthread_t p;
        costumer_t[i] = p;
        pthread_create(&(costumer_t[i]),NULL,costumer,(void *)&i);
    }

    pthread_join(producer_t,NULL);

    for(int i=0;i<5;i++){
        pthread_join(costumer_t[i],NULL);
    }

    pthread_mutex_destroy(&g_mutex);
    g_sem.destroy(g_sem.this);
    return 0;
}