/**
 * Array是一个模板类,作用类似于一个vector，但是只实现了部分函数。
 * 最大的特点是在多线程环境下通过指针访问数组的元素的是安全的，不会
 * 像vector一样出现非法指针，详见博客：
 * 设计思想是，每当向Array数组的末尾push一个元素时，不是直接将数
 * 据本身放进数据容器，而是new一块空间，把数据的值放进去，由于new
 * 的空间需要手动释放才会失效，因此通过指针访问数组的元素时，除非手
 * 动释放了空间，都不会出现异常。然后将新分配空间的指针存储在vector
 * 里，这样就可以通过指针访问数组元素。这部分详见push()的实现。
 */

#ifndef MY_ARRAY_H
#define MY_ARRAY_H

#include <vector>
#include <stdexcept>

template<typename T>
class Array {
public:
    virtual ~Array() {
        clear();
    }

    /**
     * 存入一个元素
     * @param elem 元素
     */
    void push(T &elem) {
        auto node = new T;  //分配空间
        (*node) = elem;     //赋值
        _pointer_vector.push_back(node);//存下指针
    }

    /**
     * 获取第index个元素的指针
     * @param index 索引
     * @return 指针
     */
    T* ptr(std::size_t index) {
        if (index < _pointer_vector.size()) {
            return _pointer_vector[index];
        } else {
            clear();//释放内存
            throw std::out_of_range("Array::ptr():index超出范围");
        }
    }

    /**
     * 重载[]操作符号，提供类似于普通数组通过[]访问元素的功能。
     * @param index  索引
     * @return 第index个元素的引用
     */
    T& operator[](std::size_t index){
        if (index < _pointer_vector.size()) {
            return *(_pointer_vector[index]);
        } else {
            clear();//释放内存
            throw std::out_of_range("Array::ptr():index超出范围");
        }
    }

    /**
     * 获取第index个元素
     * @param index 索引
     * @return 元素
     */
    T get(std::size_t index) {
        if (index < _pointer_vector.size()) {
            return *(_pointer_vector[index]);
        } else {
            clear();//释放内存
            throw std::out_of_range("Array::get():index超出范围");
        }
    }

    /**
     * 用elem更新第index个元素
     * @param index 索引
     * @param elem 元素
     */
    void set(std::size_t index, T elem) {
        if (index < _pointer_vector.size()) {
            *(_pointer_vector[index]) = elem;
        } else {
            clear();//释放内存
            throw std::out_of_range("Array::set():index超出范围");
        }
    }

    /**
     * 清空所有元素，并释放内存
     */
    void clear() {
        for (std::size_t i = 0; i < _pointer_vector.size(); i++) {
            delete _pointer_vector[i];
        }
        _pointer_vector.clear();
    }

    /**
     * 返回元素个数
     * @return 大于等于0的数
     */
    size_t size() {
        return _pointer_vector.size();
    }

private:
    std::vector<T *> _pointer_vector;
};

#endif //MY_ARRAY_H
