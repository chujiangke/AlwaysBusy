## 一、分支说明
这个分支是重新打包C语言中的信号量，使其用起来更方便。

## 二、使用简介
1. 下载这个分支。将Semaphore.h复制到你的项目中。

2. 初始化信号量的第一种方式
```
//包含头文件Semaphore.h
#include "Semaphore.h"
//定义全局信号量指针，方便在子线程的函数中使用
SEM::Semaphore *g_sem;
//初始化
int main(){
    ...
    g_sem = new SEM::Semaphore("sem_name",0);//在创建子线程之前初始化信号量
    ...
    delete g_sem;//销毁信号量
}
```

4. 在main.c中，基于这个信号量实现了一个简单的生产者-消费者模型。看那个更加完整易懂。


## 三、关于作者
```javascript
  var whoAmI = {
    name   : "Mannix1994",
    gitee  : "https://gitee.com/Mannix1994",
    github : "https://github.com/Mannix1994"
  }
```
