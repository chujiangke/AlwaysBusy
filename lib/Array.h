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
    T *ptr(std::size_t index) {
        if (index < _pointer_vector.size()) {
            return _pointer_vector[index];
        } else {
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
            throw std::out_of_range("Array::get():index超出范围");
        }
    }

    /**
     * 用elem更新第index个元素
     * @param index 索引
     * @param elem 元素
     */
    void set(std::size_t index, T &elem) {
        if (index < _pointer_vector.size()) {
            *(_pointer_vector[index]) = elem;
        } else {
            throw std::out_of_range("Array::set():index超出范围");
        }
    }

    /**
     * 清空所有元素
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
