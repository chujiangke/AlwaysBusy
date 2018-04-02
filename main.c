#include "Semaphore.h"
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

struct Goods{
    int id;//产品id
};//产品

Semaphore *g_sem;//信号量

struct Goods g_goods;//商品货架

pthread_mutex_t g_mutex;//互斥锁

/**
 * 生产这回调函数，生产商品
 * @param arg 未使用
 * @return 未使用
 */
void *producer(void *arg){

    int i=0;
    while (++i<5*5) {
        //休眠一段随机时间,代表生产过程
        sleep((unsigned)rand()%3+1);
        //生产产品
        pthread_mutex_lock(&g_mutex);
        g_goods.id = rand()%1000;
        printf("生产产品:%d\n",g_goods.id);
        pthread_mutex_unlock(&g_mutex);
        //唤醒一个阻塞的消费者线程
        g_sem->signal(g_sem->this);
    }
}

/**
 * 消费者回调函数
 * @param arg 消费者编号
 * @return 未使用
 */
void *costumer(void *arg){
    int i=0;
    int id = *((int *)arg);
    while (++i<5) {
        //有资源会立即返回,没有资源则会等待
        g_sem->wait(g_sem->this);
        //消费
        pthread_mutex_lock(&g_mutex);
        printf("消费者%d消费产品:%d\n",id,g_goods.id);
        pthread_mutex_unlock(&g_mutex);
    }
}

int main() {
    //获取信号量
    g_sem = getSemaphore();
    //初始化信号量
    g_sem->init(g_sem->this,"sem", 0);
    //设置随机数种子
    srand((unsigned)time(NULL));
    //初始化互斥锁
    pthread_mutex_init(&g_mutex,NULL);

    pthread_t producer_t;   //生产者线程
    pthread_t costumer_t[5];//5个消费者线程

    //创建生产者线程
    pthread_create(&producer_t,NULL,producer,NULL);

    //创建消费者线程
    for(int i=0;i<5;i++){
        pthread_t p;
        costumer_t[i] = p;
        pthread_create(&(costumer_t[i]),NULL,costumer,(void *)&i);
    }

    //等待生产者线程退出
    pthread_join(producer_t,NULL);

    //等待消费者线程退出
    for(int i=0;i<5;i++){
        pthread_join(costumer_t[i],NULL);
    }

    //销毁互斥锁
    pthread_mutex_destroy(&g_mutex);
    //销毁信号量
    g_sem->destroy(g_sem->this);
    return 0;
}