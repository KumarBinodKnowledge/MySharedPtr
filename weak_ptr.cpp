#include <iostream>
#include <memory>

using namespace std;

class B; // Forward declaration

class A
{
public:
    shared_ptr<B> b_ptr;
    ~A() { cout << "A destroyed" << endl; }
};

class B
{
public:
    weak_ptr<A> a_ptr; // Use weak_ptr to avoid circular reference
    ~B() { cout << "B destroyed" << endl; }
};

class D; // Forward declaration

class C
{
public:
    shared_ptr<D> d_ptr;
    ~C() { cout << "C destroyed" << endl; }
};

class D
{
public:
    shared_ptr<C> c_ptr; // Changed to shared_ptr, creating a circular reference
    ~D() { cout << "D destroyed" << endl; }
};

int main()
{

    {
        shared_ptr<A> a = make_shared<A>();
        shared_ptr<B> b = make_shared<B>();
        a->b_ptr = b;
        b->a_ptr = a;
        cout << "A and B's objects are going out of scope" << endl;
    }
    // Both A and B should be destroyed here

    {
        shared_ptr<C> c = make_shared<C>();
        shared_ptr<D> d = make_shared<D>();
        c->d_ptr = d;
        d->c_ptr = c;
        cout << "C and D's objects are going out of scope" << endl;
    }
    // The destructore will not be called and there should be a memory leak now. :)
    cout << "End of main" << endl;
    return 0;
}
