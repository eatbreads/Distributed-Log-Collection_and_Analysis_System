#include <iostream>
using namespace std;

class MyClass {
private:
    int id;
    static int count; // 静态成员变量

public:
    MyClass(int id){
        this->id=id;
        count++;
        cout << "Constructor: Object " << this->id << " created. Total objects: " << count << endl;
    }

    ~MyClass() {
        count--;
        cout << "Destructor: Object " << this->id << " destroyed. Total objects: " << count << endl;
    }

    void showId() const {
        cout << "Object ID: " << this->id << endl;
    }

    static int getCount() { // 静态成员函数
        return count;
    }
};

int MyClass::count = 0; // 初始化静态成员变量

int main() {
    MyClass obj1(1);
    {
        MyClass obj2(2);
        obj2.showId();
    }
    cout << "Total objects after block: " << MyClass::getCount() << endl;
    return 0;
}