/**
 * semaphore类似于一个信号量类，利用mutex和condition_variable来实现
 */
#include <mutex>
#include <condition_variable>
#include <string>
#include <cassert>
#include <chrono>
#include <iostream>
#include <utility>

namespace SEM{
	class Semaphore {
    private:
        int _count;                             //等待线程数量
        std::mutex _mutex;                      //互斥锁
        std::condition_variable _condition;     //条件变量
        std::string _name;                      //信号量名字

	public:
		Semaphore(std::string name,int value = 0) :
                _name(std::move(name)),_count(value){}

        /**
         * 阻塞一个线程，相当于信号量机制里面的P操作
         */
		void wait() {
			std::unique_lock<std::mutex> lock(_mutex);
            //std::cout<<_name+":"<<_count<<std::endl;
			if (--_count<0) { // _count is not enough ?
				_condition.wait(lock); // suspend and wait ...
			}
		}

        /**
         * 阻塞一个线程，但如果等待时间超过seconds指定的值，会唤醒所有阻塞线程
         * @param ms 等待时间(ms)
         */
		void wait(int ms) {
			assert(ms>0);
			std::unique_lock<std::mutex> lock(_mutex);
			//std::cout<<_name+":"<<_count<<std::endl;
			if (--_count<0) {                            // _count is not enough ?
				std::cv_status status = _condition.wait_for(lock,
				std::chrono::milliseconds(ms));
				if(status == std::cv_status::timeout)   //timeout
				{
                    std::cout<<_name+":timeout"<<std::endl;
                    _count = 0;
                    _condition.notify_all();
				}
			}
		}

        /**
         * 随机唤醒一个线程，相当于信号量机制里面的V操作
         */
		void signal() {
			std::lock_guard<std::mutex> lock(_mutex);
			if (++_count <= 0) {         // have some thread suspended ?
				_condition.notify_one(); // notify one !
			}
		}

        /**
         * 唤醒阻塞的所有线程
         */
		void signalAll() {
			std::lock_guard<std::mutex> lock(_mutex);
			while (++_count <= 0) {      // have some thread suspended ?
				_condition.notify_one(); // notify one !
			}
			_count=0;
		}
	};
};