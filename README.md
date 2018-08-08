# 中文文档
## 一、项目简介
本项目是一个简单的C++多线程模型，在Windows和Linux下均测试通过。

## 二、模型简介
模型的核心是ThreadManager模板类，可用它来创建n个（最多不超过
CPU逻辑核心数）在后台驻留的子线程。ThreadManager的实例会维护
一个数据队列，子线程从队列中取出数据来处理，没有数据时会阻塞等
待数据，直到主线程通知退出。

## 三、使用简介
1. 点击[链接](https://codeload.github.com/Mannix1994/AlwaysBusy/zip/ptr_no_create),
下载本项目，然后将压缩包中的lib文件夹拷贝到你的项目中。

2. 在代码中定义要在子线程中处理的数据结构，可以是任何类型的数据
```
class Data{
public:
    Data(int xx,int yy):x(xx),y(yy),z(0){
    };
    Data():x(0),y(0),z(0){}//必须有一个无参构造函数
    int x;//元素1
    int y;//元素2
    int z;//存储子线程的运算结果
};//要处理的数据的类型定义
``` 

3. 在代码中包含头文件ThreadsManager.h并声明一个指针
```cplusplus
#include "ThreadsManager.h"
std::mutex mCout;  //打印互斥锁，不是必须设置的变量
//声明一个指针
ThreadsManager<Data> *manager;  //线程管理器
```

4. 声明一个在子线程执行的函数，必须按照下面的结构来写
```cplusplus
/**
 * 在子线程执行的函数模板
 * @param i 子线程的的id，范围是0~n-1
 */
void callback(ThreadsManager<Data> *m, int i){
    while(true){
        m->wait();    //有资源的话会立即返回，没有资源会阻塞
        if(!m->run()){//是否继续运行子线程
            break;
        }
        //处理数据，可以自定义的部分
        SLEEP(i*100);//休眠一段时间,代表处理数据时间
        //取指针
        Data *point= manager->next();
        //做加法
        point->z = point->x + point->y;
    }
}
```

5. 在主程序中编写下列代码
```cplusplus
void demo(){
    //新建线程管理对象
    manager = new ThreadsManager<Data>(callback, 12);

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
        for(int j=5;j<1000;j++){
            manager->add(Data(i,j));
        }
    }
    //等待处理完
    manager->join();
    //获取子线程处理完的数据
    std::cout << "****************************" << std::endl;
    for(int i=0;i<manager->size();i++){ //打印数据
        auto p = manager->get(i);
    }
    //必须清除旧的数据
    manager->clear();

    //退出线程
    manager->kill();

    std::cout << "Hello, World!" << std::endl;
    delete manager;
}
```

完整的demo在main.cpp中，看那个更加完整易懂。

## 四、问题反馈
在使用中有任何问题，欢迎开issue

## 五、关于作者
```javascript
  var whoAmI = {
    name   : "Mannix1994",
    gitee  : "https://gitee.com/Mannix1994",
    github : "https://github.com/Mannix1994"
  }
```
