#include <iostream>
#include "ThreadsManager.h"


class Data{
public:
    Data(int xx,int yy):x(xx),y(yy),z(0){}//定义有参构造函数
    Data():x(0),y(0),z(0){}//若定义了有参构造函数，必须定义无参构造函数
    int x;//元素1
    int y;//元素2
    int z;//存储子线程中计算的结果
};//要处理的数据的类型定义

ThreadsManager<Data> *manager;  //线程管理器

/**
 * 在子线程执行的函数模板
 * @param i 子线程的的id，范围是0~n-1
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
        //取指针
        Data *point= manager->next();
        //做加法
        point->z = point->x + point->y;
    }
}

void demo(){
    //新建线程管理对象
    manager = new ThreadsManager<Data>(12);
    //创建线程
    manager->create(callback);

    //第一轮添加数据
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
           manager->add(Data(i,j));
        }
    }
    //等待处理完所有points里面的数据
    manager->join();
    //获取子线程处理完的数据
    std::cout << "****************************" << std::endl;
    for(ulong i=0;i<manager->size();i++){
        auto p = manager->get(i);
        printf("(%d+%d=%d)\n",p.x,p.y,p.z);
    }
    //必须清除旧的数据
    manager->clear();


    //第二轮添加数据
    for(int i=5;i<10;i++){
        for(int j=5;j<10;j++){
            manager->add(Data(i,j));
        }
    }
    //等待处理完
    manager->join();
    //获取子线程处理完的数据
    std::cout << "****************************" << std::endl;
    for(int i=0;i<manager->size();i++){ //打印数据
        auto p = manager->get(i);
        printf("(%d+%d=%d)\n",p.x,p.y,p.z);
    }
    //必须清除旧的数据
    manager->clear();

    //退出线程
    manager->kill();

    std::cout << "Hello, World!" << std::endl;
    delete manager;
}

int main() {
    demo();
    return 0;
}