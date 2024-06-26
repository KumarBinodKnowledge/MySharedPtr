#include <iostream>
#include <utility> // For std::move
using namespace std;

template <typename T>
class my_shared_ptr
{
private:
    T *ptr;     // Pointer to the resource
    int *count; // Reference count

public:
    // Constructor
    explicit my_shared_ptr(T *p = nullptr) : ptr(p), count(new int(1))
    {
        std::cout << "Created my_shared_ptr. Count = " << *count << std::endl;
    }

    // Copy constructor
    my_shared_ptr(const my_shared_ptr &other) : ptr(other.ptr), count(other.count)
    {
        (*count)++;
        std::cout << "Copied my_shared_ptr. Count = " << *count << std::endl;
    }

    // Move constructor
    my_shared_ptr(my_shared_ptr &&other) noexcept : ptr(other.ptr), count(other.count)
    {
        other.ptr = nullptr;
        other.count = nullptr;
        std::cout << "Moved my_shared_ptr. Count = " << *count << std::endl;
    }

    // Copy assignment operator
    my_shared_ptr &operator=(const my_shared_ptr &other)
    {
        if (this != &other)
        {
            if (count != nullptr && --(*count) == 0)
            {
                delete ptr;
                delete count;
            }
            ptr = other.ptr;
            count = other.count;
            (*count)++;
        }
        return *this;
    }

    // // Move assignment operator
    my_shared_ptr &operator=(my_shared_ptr &&other) noexcept
    {
        cout << "Move assignment operator ---------------" << endl;
        if (this != &other)
        {
            if (count != nullptr && --(*count) == 0)
            {
                delete ptr;
                delete count;
            }
            ptr = other.ptr;
            count = other.count;
            other.ptr = nullptr;
            other.count = nullptr;
        }
        return *this;
    }

    // Accessors
    T *get() const { return ptr; }
    int use_count() const { return count ? *count : 0; }

    // Reset the shared pointer
    void reset(T *p = nullptr)
    {
        if (count != nullptr && --(*count) == 0)
        {
            delete ptr;
            delete count;
        }
        ptr = p;
        count = new int(1);
    }

    // Destructor
    ~my_shared_ptr()
    {
        if (count != nullptr && --(*count) == 0)
        {
            delete ptr;
            delete count;
            std::cout << "Deleted resource." << std::endl;
        }
    }
};

int main()
{
    // Creating a new shared pointer
    my_shared_ptr<int> sp1(new int(10));
    std::cout << "Initial use count of sp1: " << sp1.use_count() << std::endl;

    // Using move constructor
    my_shared_ptr<int> sp2 = std::move(sp1);
    std::cout << "Use count of sp2 after move: " << sp2.use_count() << std::endl;

    std::cout << "Use count of sp1 after move: " << sp1.use_count() << std::endl; // Should be 0

    // Assigning a new value to sp1 and then using move assignment
    sp1.reset(new int(20));
    std::cout << "Use count of sp1 after reset: " << sp1.use_count() << std::endl;

    // Using move assignment operator
    sp1 = std::move(sp2);
    std::cout << "Use count of sp1 after move assignment: " << sp1.use_count() << std::endl;
    std::cout << "Use count of sp2 after move assignment: " << sp2.use_count() << std::endl; // Should be 0

    return 0;
}
