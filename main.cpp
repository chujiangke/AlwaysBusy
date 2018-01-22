#include <iostream>
#include <mutex>
#include <list>
#include "ThreadsManager.h"

typedef struct _Point{
    _Point(int xx,int yy):x(xx),y(yy){};
    int x;
    int y;
}Postion;//要处理的数据的类型定义，我这里是处理点

std::mutex mCout;               //打印互斥锁
ThreadsManager<Postion> *manager; //线程管理器

void callback(int i){
    while(true){
        manager->wait();    //有资源的话会立即返回，没有资源会阻塞
        if(!manager->run()){//是否继续运行子线程
            break;
        }
        //取点
        Postion point = manager->pop();

        //处理数据
        MSLEEP(i*100);//休眠一段时间,代表处理数据时间
        mCout.lock();
        printf("Postion(%d,%d)\n",point.x,point.y);
        mCout.unlock();
    }
}

void demo(){
    manager = new ThreadsManager<Postion>(4);
    manager->create(callback);
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
           manager->add(Postion(i,j));
        }
    }
    manager->join();//等待处理完所有points里面的数据
    std::cout << "emmm" << std::endl;
    for(int i=5;i<10;i++){
        for(int j=5;j<10;j++){
            manager->add(Postion(i,j));
        }
    }
    manager->join();
    std::cout << "emmmmmm" << std::endl;
    for(int i=10;i<15;i++){
        for(int j=10;j<15;j++){
            manager->add(Postion(i,j));
        }
    }
    manager->kill();//退出线程

    std::cout << "Hello, World!" << std::endl;
    delete manager;
}

int main() {
    demo();
    return 0;
}