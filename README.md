# 中文文档
## 一、项目简介
本项目是一个简单的C++多线程模型，在Windows和Linux下均测试通过。

## 二、模型简介
模型的核心是ThreadManager模板类，可用它来创建n个（最多不超过
CPU逻辑核心数）在后台驻留的子线程。ThreadManager的实例会维护
一个数据队列，子线程从队列中取出数据来处理，没有数据时会阻塞等
待数据，直到主线程通知退出。

## 三、使用简介
1. 点击[链接](https://codeload.github.com/Mannix1994/AlwaysBusy/zip/master),
下载本项目，然后将压缩包中的lib文件夹拷贝到你的项目中。

2. 在代码中定义要在子线程中处理的数据结构，可以是任何类型的数据
```
typedef struct _Point{
    _Point(int xx,int yy):x(xx),y(yy){};
    int x;
    int y;
}Position;//要处理的数据的类型定义，我这里是处理点
``` 

3. 在代码中包含头文件ThreadsManager.h并声明一个指针
```cplusplus
#include "ThreadsManager.h"
//声明一个指针
std::mutex mCout;  //打印互斥锁，不是必须设置的变量
ThreadsManager<Position> *manager;  //线程管理器
```

4. 声明一个在子线程执行的回调函数，必须按照下面的结构来写
```cplusplus
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
        //取数据
        Position point = manager->pop();
        //处理数据，后面就是可以自定义的部分
        SLEEP(i*100);//休眠一段时间,代表处理数据时间
        mCout.lock();
        printf("Position(%d,%d)\n",point.x,point.y);
        mCout.unlock();
    }
}
```

5. 在主程序中编写下列代码
```cplusplus
//新建线程管理对象，参数为子线程数量。
manager = new ThreadsManager<Position>(4);
//创建线程，参数为上一步定义的回调函数
manager->create(callback);
//添加待处理数据
for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
       manager->add(Position(i,j));
    }
}
//等待处理完所有数据
manager->join();
//杀死所有子线程
manager->kill();
delete manager;
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
