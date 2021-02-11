#include <initializer_list>
#include <stdexcept>
#include <string>
#include <tuple>
#include <iostream>

using std::string;
using std::exception;
using std::cout;
using std::endl;

int function(int* a, int b, int c, string d)
{
    try {

    }
    catch(...) {

    }
    throw std::runtime_error("");

}
/*
class base {
    public:
    virtual void fun1() = 0;
    virtual int fun2(int x) = 0;
    virtual string fun3() = 0;
    virtual void add() = 0;

    private:
    int i;

    public:
    static int counter;
};
*/
class son1 {
    public:
    void fun1() ;
    int fun2(int x);
    string fun3();
    son1() = default;
    void add(void);
};

using pson = son1*;

/*
class son2 : public base {
    public:
    void fun1() { cout << "son2.fun1() call" <<endl;}
    int fun2(int x) { cout << "son2.fun2() call, x = " << x <<endl;}
    //string fun3() { cout << "son2.fun3() call " <<endl; return "__";}
    son2() = default;
};
*/