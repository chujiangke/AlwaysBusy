#include <iostream>
#include <mutex>
#include <list>
#include <thread>

using namespace std;

class Position {
public:
    Position(int xx, int yy) : x(xx), y(yy) {};
    int x;
    int y;

    void member(int i) {
        printf("member:%d\n", i);
        x = 100;
    }

    friend void pointer(Position *p, int i);
    friend void reference(Position &p, int i);
};

void pointer(Position *p, int i) {
    printf("pointer:%d(%d,%d)\n", i, p->x, p->y);
    p->x = 200;
}

void reference(Position &p, int i) {
    printf("reference:%d(%d,%d)\n", i, p.x, p.y);
    p.x = 300;
}

int main() {
    Position p(1, 2);

    //使用类的成员函数作为子线程的参数
    //参数说明：//参数1是成员函数的指针，参数2是Position对象
    //值传递,子线程中对p的修改不会影响主线程中的对象p。
    thread t1(&Position::member, p, 233);
    t1.join();
    cout << "p.x=" << p.x << endl;

    //使用类的友元函数作为参数
    //友元函数参见：https://blog.csdn.net/mary19920410/article/details/70036878
    //参数说明：参数1是Position的友元函数reference，参数2是用ref包装过的引用，参数3是传给reference的值。
    //在这种方式中，由于p是以引用的方式传递给子线程，在子线程中对p的修改相当于修改主线程中的p。
    std::thread t2(reference, ref(p), 1);
    t2.join();
    cout << "p.x=" << p.x << endl;

    //使用类的友元函数作为参数
    //参数说明：参数1是Position的友元函数pointer，参数2是p的指针，参数3是传给pointer的参数。
    //在这种方式中，由于p是以引用的方式传递给子线程，在子线程中对p的修改相当于修改主线程中的p。
    std::thread t3(pointer, &p, 1);
    t3.join();
    cout << "p.x=" << p.x << endl;

    //通过lambda传递引用参数
    //关于lambda函数的说明:https://blog.csdn.net/u010984552/article/details/53634513
    auto lambda_func = [&p](int i) -> void {
        printf("lambda_func:%d(%d,%d)\n", i, p.x, p.y);
        p.x = 400;
    };
    //参数说明：参数1是lambda表达式,参数2是传递给lambda函数的参数
    //由于在lambda_func是以引用的方式捕获p，所以在子线程中p是主线程中p的引用，在子线程中修改p
    //相当于主线程中的p。
    thread t4(lambda_func, 123);
    t4.join();
    cout << "p.x=" << p.x << endl;

    return 0;
}