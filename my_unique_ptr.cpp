#include <utility> // For std::forward
#include <cstddef> // For std::nullptr_t
#include <iostream>

using namespace std;

template <typename T>
class my_unique_ptr
{
private:
    T *ptr; // Pointer to the managed object

public:
    // Constructor
    explicit my_unique_ptr(T *p = nullptr) : ptr(p) // explicit c'tor
    {
        cout << "C'tor called ----" << endl;
    }
    ~my_unique_ptr() // Destructor
    {
        delete ptr;
    }
    // Disable copy constructor and copy assignment operator
    my_unique_ptr(const my_unique_ptr &) = delete;            // deep copy disabled.
    my_unique_ptr &operator=(const my_unique_ptr &) = delete; // deep copy disabled.

    my_unique_ptr(my_unique_ptr &&rhs) noexcept // Move constructor shallow copy (not move copy constructor)
    {
        cout << "Move C'tor" << endl;
        ptr = rhs.ptr;
        rhs.ptr = nullptr;
    }
    my_unique_ptr &operator=(my_unique_ptr &&rhs) noexcept // Move assignment operator shallow copy
    {
        cout << "Move assignment Operator" << endl;
        if (this != &rhs)
        {
            delete ptr;
            ptr = rhs.ptr;
            rhs.ptr = nullptr;
        }
        return *this;
    }
    void reset(T *p) noexcept // Takes the ownership (of new object) and deletes the previous managed object.
    {
        if (ptr != p)
        {
            delete ptr;
            ptr = p;
        }
    }
    T *release() // Releases the ownership (of managed object)
    {
        T *temp = ptr;
        ptr = nullptr;
        return temp;
    }
    void swap(my_unique_ptr &rhs) noexcept // Swap the managed objects
    {
        std::swap(ptr, rhs.ptr);
    }
    // Dereference operator
    T &operator*() const // same as shared_ptr
    {
        return *ptr;
    }
    // Member access operator
    T *operator->() const // same as shared_ptr
    {
        return ptr;
    }
};

template <typename U, typename... Args>
my_unique_ptr<U> make_my_unique(Args &&...args)
{
    return my_unique_ptr<U>(new U(std::forward<Args>(args)...));
}

int main()
{
    cout << "my_unique_ptr 000 " << endl;

    int *rawPtr = new int(10);
    my_unique_ptr<int> obj1;
    my_unique_ptr<int> obj2(rawPtr);
    // my_unique_ptr<int> obj3 = new int(10);  // error, c'tor explicit
    my_unique_ptr<int> obj33(new int(10)); // c'tor called
    // Question why the below 4 statements doesnt work
    // my_unique_ptr<int> obj4 = make_unique<int>(10);
    // my_unique_ptr<int> obj44(make_unique<int>());
    my_unique_ptr<int> obj444 = make_my_unique<int>(20);
    // my_unique_ptr<int> obj4444(my_make_unique<int>());
    // my_unique_ptr<int> obj5(obj1);   // error, copy c'tor deleted
    // my_unique_ptr<int> obj55 = obj1;  // error, copy c'tor deleted
    my_unique_ptr<int> obj6 = std::move(obj1); // move copy c'tor called
    my_unique_ptr<int> obj66(std::move(obj1)); // move copy c'tor called
    my_unique_ptr<int> obj7;
    obj7 = std::move(obj1); // move assignment operator called

    /*
    //=========================================================================================
    int* rawPtr1 = new int(10);
    unique_ptr<int> ob1;
    unique_ptr<int> ob2(rawPtr1);
    //unique_ptr<int> ob3 = new int(10);  // error, constructor explicit
    unique_ptr<int> ob33(new int(10));    // parameterized c'tor called
    unique_ptr<int> ob4 = make_unique<int>(10);  // move c'tor called. rvalue
    unique_ptr<int> ob44(make_unique<int>());
    //unique_ptr<int> ob5(ob1);     // error, no copy c'tor
    //unique_ptr<int> ob5 = ob1;    // error, no copy c'tor

    // move c'tor call
    unique_ptr<int> ob6 = std::move(ob1);  // move c'tor called.
    unique_ptr<int> ob66(std::move(ob1));  // move c'tor called.

    //move assignment operator;
    unique_ptr<int> ob7;
    ob7 = std::move(ob1);

    unique_ptr<int> ob8;
    //ob8 = ob1;                  // error, assignment operator deleted
      int* rawPtr3;
    {
      unique_ptr<int> ob22(new int(10));
      rawPtr3 = ob2.release();  // ob2 is no longer is the owner.
    }
    *rawPtr3 = 5; // responsibility of coder to delete manually.

    cout << "value of rawPtr3 = " << *rawPtr3 << " " << endl;

    unique_ptr<int> ob9;
    ob9.reset(new int);  // New ownership
    *ob9=5;

    ob9.reset(); // deletes the managed object

    cout << "unique_ptr end -----" << endl;
    */
    //=============================================
    return 0;
}
