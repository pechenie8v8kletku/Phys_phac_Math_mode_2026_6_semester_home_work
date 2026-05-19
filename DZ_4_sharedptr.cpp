#include <iostream>
using namespace std;
//На самом деле не сложное задание помоему меня просили реализовать мув семантику сделана через оператор присваивания и конструктор 
// move semantic ñþäà
template<typename T>
class SharedPtr {
private:
    T* ptr_;
    size_t* count_;

public:
    explicit SharedPtr(T* ptr = nullptr)
        : ptr_(ptr), count_(nullptr)
    {
        if (ptr_) {
            count_ = new std::size_t(1);
        }
    }
    SharedPtr(const SharedPtr& other)
        : ptr_(other.ptr_), count_(other.count_)
    {
        if (count_) {
            ++(*count_);
        }
    }
    SharedPtr(SharedPtr&& other)
        : ptr_(other.ptr_), count_(other.count_)
    {
        other.ptr_ = nullptr;
        other.count_ = nullptr;
    }
    ~SharedPtr() {
        release();
    }

    void release() {
        if (count_) {
            --(*count_);
            if (*count_ == 0) {
                delete ptr_;
                delete count_;
            }
        }
    }
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            count_ = other.count_;
            if (count_) {
                ++(*count_);
            }
        }
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other) {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            count_ = other.count_;
            other.ptr_ = nullptr;
            other.count_ = nullptr;
        }
        return *this;
    }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }

    size_t use_count() const {
        return count_ ? *count_ : 0;
    }
    void swap(SharedPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(count_, other.count_);
    }
    void reset(T* ptr) {
        release();
        if (ptr) {
            ptr_ = ptr;
            count_ = new std::size_t(1);
        }
    }

};
template<typename T>
void swap(SharedPtr<T>& a, SharedPtr<T>& b) noexcept {
    a.swap(b);
}
template<typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
struct Test {
    int value;

    Test(int v) : value(v) {
        cout << "Construct " << value << "\n";
    }

    ~Test() {
        cout << "Destruct " << value << "\n";
    }
};

int main() {
    cout << "TEST 1 copy\n";
    {
        SharedPtr<Test> p1(new Test(10));
        cout << "count = " << p1.use_count() << "\n";

        SharedPtr<Test> p2 = p1;
        cout << "count = " << p1.use_count() << "\n";

        SharedPtr<Test> p3 = p2;
        cout << "count = " << p1.use_count() << "\n";
    }

    cout << "\n TEST 2 assign\n";
    {
        SharedPtr<Test> p1(new Test(30));
        SharedPtr<Test> p2;
        p2 = p1;
        swap(p1, p2);

        cout << "p1 count = " << p1.use_count() << "\n";
        cout << "p2 count = " << p2.use_count() << "\n";
    }
    cout << "\n TEST 3 self assing \n";
    {
        SharedPtr<Test> p1(new Test(50));
        p1 = p1;
        cout << "count = " << p1.use_count() << "\n";
    }

    cout << "\nTEST4 nullptr\n";
    {
        SharedPtr<Test> p1;
        cout << "count = " << p1.use_count() << "\n";
    }
    cout << "\nTEST move\n";
    {
        SharedPtr<Test> p1(new Test(60));
        SharedPtr<Test> p2;
        p2 = move(p1);
        cout << "p1 count = " << p1.use_count() << "\n"; 
        cout << "p2 count = " << p2.use_count() << "\n"; 
        SharedPtr<Test> p3(new Test(60));
        SharedPtr<Test> p4(move(p3));
        cout << "p1 count = " << p3.use_count() << "\n"; 
        cout << "p2 count = " << p4.use_count() << "\n"; 
    }
    return 0;
}
