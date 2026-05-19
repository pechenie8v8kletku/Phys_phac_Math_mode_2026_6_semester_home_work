#include <iostream>
#include <unordered_set>
#include <numeric>
#include <algorithm>
#include <fstream>
using namespace std;
//Реализовать лист с форвард итераторм уникальных элементов, может показаться что лучше использовать set так и есть по сути, петрович попросил переделать на то чтобы в
// итераторе ничего не хранилось поэтому тут добавлена bool is visited что по сути своей не правильно с точки зрения STL т.к. мы меняем контейнер по которому проходимя
// но да Бог с ним. В целом не сложная задача 
template<typename T>
class List {
private:
    struct Node {
        T data;
        bool is_visited=false;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* head;
    int size_;

public:
    List() : head(nullptr), size_(0) {}
    List(const List& l) {
        size_ = l.size_;
        if (size_ > 0) {
            Node* prikol = l.head;
            head = new Node(prikol->data);
            Node* p = head;

            while (prikol->next) {
                prikol = prikol->next;
                Node* newNode = new Node(prikol->data);
                p->next = newNode;
                p = p->next;
            }
        }
        else {
            head = nullptr;
        }
    }

    ~List() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void push_back(const T& val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = newNode;
        }
        else {
            Node* curr = head;
            while (curr->next) curr = curr->next;
            curr->next = newNode;
        }
        size_++;
    }

    void push_front(const T& val) {
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
        size_++;
    }

    void pop_front() {
        if (!head) throw out_of_range("List is empty");
        Node* temp = head;
        head = head->next;
        delete temp;
        size_--;
    }

    int size() const { return size_; }

    T get(int id) const {
        if (id < 0 || id >= size_) throw out_of_range("Index out of range");
        Node* curr = head;
        for (int i = 0; i < id; i++) curr = curr->next;
        return curr->data;
    }
    class UniqueForwardIterator {
    private:
        Node* current;
        Node* head;


        void set_visited_by_value(Node* visit) {
            T val = visit->data;
            Node* temp = head;
            while (temp) {
                if (temp->data == val) {
                    temp->is_visited = true;
                }
               temp = temp->next;
            }

        }
        void skip_duplicates() {
            while (current && current->is_visited) {
                current = current->next;
            }
            if (current) {
                set_visited_by_value(current);
            }
        }

    public:
        using iterator_category = forward_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        UniqueForwardIterator() : current(nullptr), head(nullptr) {}

        UniqueForwardIterator(Node* node, Node* head_) : current(node), head(head_) {
            skip_duplicates();
        }

        reference operator*() const { return current->data; }
        pointer operator->() const { return &(current->data); }

        UniqueForwardIterator& operator++() {
            if (current) {
                current = current->next;
                skip_duplicates();
            }
            return *this;
        }

        UniqueForwardIterator operator++(int) {
            UniqueForwardIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const UniqueForwardIterator& other) const {
            return current == other.current;
        }

        bool operator!=(const UniqueForwardIterator& other) const {
            return !(*this == other);
        }
    };

    UniqueForwardIterator begin_unique() {
        return UniqueForwardIterator(head, head);
    }
    UniqueForwardIterator end_unique() { return UniqueForwardIterator(); }
};
int main() {
    {
        List<int> lst;
        lst.push_back(1);
        lst.push_back(1);
        lst.push_back(2);
        lst.push_back(3);
        lst.push_back(2);
        lst.push_back(4);
        lst.push_back(88);
    cout << "Test 1: Base " << endl;
    cout << "Unique elements: ";
    for (auto it = lst.begin_unique(); it != lst.end_unique(); ++it) {
        cout << *it << " ";
    }
    }
    { List<int> lst;
    lst.push_back(1);
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    lst.push_back(2);
    lst.push_back(4);
    lst.push_back(88);
        
        cout << "\nSum of unique: " << accumulate(lst.begin_unique(), lst.end_unique(), 0) << " (Expected: 98)" << endl;

    }
    {
        List<int> lst;
        lst.push_back(1);
        lst.push_back(1);
        lst.push_back(2);
        lst.push_back(3);
        lst.push_back(2);
        lst.push_back(4);
        lst.push_back(88);
        cout << "\nTest 2: Distance" << endl;
    cout << "Distance: "
        << distance(lst.begin_unique(), lst.end_unique())
        << " (Expected: 5)" << endl;

    }
    {
        List<int> lst;
        lst.push_back(1);
        lst.push_back(1);
        lst.push_back(2);
        lst.push_back(3);
        lst.push_back(2);
        lst.push_back(4);
        lst.push_back(88);
        cout << "\nTest 3: Find" << endl;
    auto it = find(lst.begin_unique(), lst.end_unique(), 3);
    cout << "Found 3? " << (it != lst.end_unique()) << endl;
    }
    { List<int> lst;
    lst.push_back(1);
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    lst.push_back(2);
    lst.push_back(4);
    lst.push_back(88);
    lst.push_back(44);
    lst.push_back(3);
    ofstream fout("output.txt");

    auto begin = lst.begin_unique();
    auto end = lst.end_unique();

    std::copy(begin, end, ostream_iterator<int>(fout, " "));

    fout.close();
    }
    return 0;
    }
