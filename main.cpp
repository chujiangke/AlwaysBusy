/**
 * 本分支由ptr_no_create分支演化而来，打算使用lambda函数作为参数
*/
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

void demo1(bool kill){
    int a[13] = {0};
    static auto func = [&](ThreadsManager<Data> *m,int i)->void {
        while(true){
            m->wait();    //有资源的话会立即返回，没有资源会阻塞
            if(!m->run()){//是否继续运行子线程
                break;
            }
            //处理数据，可以自定义的部分

            std::this_thread::sleep_for(std::chrono::microseconds(100*100));//休眠一段时间,代表处理数据时间
            //取指针
            Data *point= m->next();
            //做加法
            point->z = point->x + point->y;

            a[i] = i;
        }
    };
    static ThreadsManager<Data> manager1(func,12);
    //第一轮添加数据
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            manager1.add(Data(i,j));
        }
    }
    manager1.join();
    //获取子线程处理之后的数据
    std::cout << "****************************" << std::endl;
    for(ulong i=0;i<manager1.size();i++){
        auto p = manager1.get(i);
        printf("(%d+%d=%d)\n",p.x,p.y,p.z);
    }
    //打印a
    for(ulong i=0;i<13;i++){
        printf("a[%ld]=%d\n",i,a[i]);
    }
    //必须清除旧的数据
    manager1.clear();
    if(kill)
        manager1.kill();
}

int main() {
//    demo();
    demo1(false);
    demo1(false);
    demo1(true);
    return 0;
}