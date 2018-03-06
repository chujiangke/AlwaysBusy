#include <iostream>
#include <mutex>
#include <list>
#include "ThreadsManager.h"

typedef struct _Point{
    _Point(int xx,int yy):x(xx),y(yy){};
    int x;
    int y;
}Position;//要处理的数据的类型定义，我这里是处理点

std::mutex mCout;                   //打印互斥锁
ThreadsManager<Position> *manager;  //线程管理器

/**
 * 在子线程执行的函数模板
 * @param i 子线程的的id，范围是1~n
 */
void callback(int i){
    while(true){
        manager->wait();    //有资源的话会立即返回，没有资源会阻塞
        if(!manager->run()){//是否继续运行子线程
            break;
        }
        //取数据
        Position point = manager->pop();

        //处理数据，可以自定义的部分
        SLEEP(i*100);//休眠一段时间,代表处理数据时间
        mCout.lock();
        printf("Position(%d,%d)\n",point.x,point.y);
        mCout.unlock();
    }
}

void demo(){
    manager = new ThreadsManager<Position>(4);  //新建线程管理对象
    manager->create(callback);                  //创建线程
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
           manager->add(Position(i,j));         //添加待处理数据
        }
    }
    manager->join();                    //等待处理完所有数据

//    std::cout << "emmm" << std::endl;
    for(int i=5;i<10;i++){              //再次添加数据
        for(int j=5;j<10;j++){
            manager->add(Position(i,j));
        }
    }
    manager->join();                    //等待处理完所有数据

//    std::cout << "emmmmmm" << std::endl;
    for(int i=10;i<15;i++){             //再次添加数据
        for(int j=10;j<15;j++){
            manager->add(Position(i,j));
        }
    }
    manager->kill();//杀死所有子线程

    std::cout << "Hello, World!" << std::endl;
    delete manager;
}

int main() {
    demo();
    return 0;
}