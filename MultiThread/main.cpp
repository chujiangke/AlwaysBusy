#include <iostream>
#include <thread>

#define MULTI_THREAD
#include <Array.h>
#include <zconf.h>

Array<int> int_arr;

void callback(){
    for(int i=0;i<10;i++){
        usleep(500);
        int_arr.push(i);
    }
}

void testArray(){
    std::thread thread(callback);
    std::thread thread2(callback);
    sleep(1);
    //打印数组
    for(int i=0;i<int_arr.size();i++){
        std::cout<<int_arr[i]<<",";//访问元素
    }
    thread.join();
    thread2.join();
    std::cout<<std::endl;
}


int main() {
    testArray();
    return 0;
}
