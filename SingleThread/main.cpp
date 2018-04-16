#include <iostream>
#include <Array.h>

void testArray(){
    //初始化
    Array<int> int_arr;
    for(int i=0;i<10;i++){
        int_arr.push(i);//在数组末尾添加元素
    }
    //打印数组
    for(int i=0;i<int_arr.size();i++){
        std::cout<<int_arr[i]<<",";//访问元素
    }
    std::cout<<std::endl;

    //修改某个数据
    int_arr[0] = 23;
    int_arr.set(1,233);
    int *pInt = int_arr.ptr(2);
    *pInt = 2333;
    //打印数组
    for(int i=0;i<int_arr.size();i++){
        std::cout<<int_arr.get(i)<<",";//这样访问也ok

    }
}


int main() {
    testArray();
    return 0;
}