#include <iostream>
#include <mutex>
#include <list>
#include "ThreadsManager.h"

class Point{
public:
    Point(int xx,int yy):x(xx),y(yy),z(0){};
    int index = 0;
    int x;
    int y;
    int z;
};//要处理的数据的类型定义，我这里是处理点

std::mutex mCout;                   //打印互斥锁
ThreadsManager<Point> *manager;  //线程管理器

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
        //取点

        //处理数据，可以自定义的部分
        SLEEP(i*100);//休眠一段时间,代表处理数据时间
//        mCout.lock();
        Point point= manager->next();
        point.z = point.x + point.y;
        manager->set(point);
//        printf("Position:%d(%d,%d,%d)\n",point.index,point.x,point.y,point.z);
//        point = nullptr;
//        mCout.unlock();
    }
}

void demo(){
    manager = new ThreadsManager<Point>(4);  //新建线程管理对象
    manager->create(callback);                  //创建线程
    for(int i=0;i<5;i++){                       //添加点
        for(int j=0;j<5;j++){
           manager->add(Point(i,j));
        }
    }
    manager->join();                    //等待处理完所有points里面的数据
    std::cout << "emmm" << std::endl;
    for(ulong i=0;i<manager->size();i++){
        auto p = manager->get(i);
        printf("(%d %d %d)\n",p.x,p.y,p.z);
    }
    manager->clear();//清除旧的数据
    for(int i=5;i<10;i++){              //再次添加点
        for(int j=5;j<10;j++){
            manager->add(Point(i,j));
        }
    }
    manager->join();                    //等待处理完
    std::cout << "emmmmmm" << std::endl;
    for(int i=0;i<manager->size();i++){ //打印数据
        auto p = manager->get(i);
        printf("(%d %d %d)\n",p.x,p.y,p.z);
    }
    manager->clear();//清除旧的数据
    for(int i=10;i<15;i++){             //再次添加数据
        for(int j=10;j<15;j++){
            manager->add(Point(i,j));
        }
    }
    manager->kill();//退出线程
    for(ulong i=0;i<manager->size();i++){
        auto p = manager->get(i);
        printf("(%d %d %d)\n",p.x,p.y,p.z);
    }

    std::cout << "Hello, World!" << std::endl;
    delete manager;
}

int main() {
    demo();
    return 0;
}