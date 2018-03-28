#include <iostream>
#include <mutex>
#include <list>
#include <random>
#include "Semaphore.h"
#include <unistd.h>
#include <thread>

using namespace std;

class Goods{
public:
    explicit Goods(int ii):i(ii){
    }
    int i;
};//产品

SEM::Semaphore *g_sem;

list<Goods> g_goods;//商品货架

mutex g_mutex;

void producer(){
    default_random_engine e(static_cast<unsigned long>(time(0)));
    uniform_int_distribution<unsigned> u(0, 10);
    int i=0;
    while (++i<5) {
        //休眠一段随机时间,代表生产过程
        sleep(u(e));
        //生产
        g_mutex.lock();
        Goods good(u(e));
        g_goods.emplace_back(good);
        cout<<"生产产品:"<<good.i<<endl;
        g_sem->signal();
        g_mutex.unlock();
    }
}

void costumer(){
    int i=0;
    while (++i<5) {
        g_sem->wait();//有资源会立即返回,没有资源则会等待
        //消费
        g_mutex.lock();
        Goods good = g_goods.front();
        cout<<"消费产品:"<<good.i<<endl;
        g_goods.pop_front();
        g_mutex.unlock();
    }
}
int main() {
    g_sem = new SEM::Semaphore("sem_name",0);
    thread producer_t(producer);
    thread costumer_t(costumer);
    producer_t.join();
    costumer_t.join();
    delete g_sem;
    return 0;
}