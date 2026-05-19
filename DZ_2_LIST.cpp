#include <iostream>
using namespace std;
// Задача на реализацию List мега классика для алгосов.Не помню что просили реализовать скорее всего convert как вариант еще вроде был двусвязный список



template<typename T>
class List {
private:
    struct Node {
        T data;
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
            Node *prikol = l.head;
            Node* head1 = new Node(prikol->data);
            head = head1;
            Node* p = head1;
            while (prikol->next) {
                prikol = prikol->next;
                Node* newNode = new Node(prikol->data);
                p->next = newNode;
                p = p->next;
            }

        }
        
    }
    ~List() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
            cout << "destruct";
        }
    }
    void push_front(const T& val) {
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
        size_++;
    }
    void push_back(const T& val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = newNode;
        }
        else {
            Node* curr = head;
            while (curr->next) { curr = curr->next; }
            curr->next = newNode;
        }
        size_++;
    }
    int size() const {
        return size_;
    }
    void pop_front() {
        if (!head)
            throw out_of_range("List is empty");

        Node* temp = head;
        head = head->next;
        delete temp;
        size_--;
    }
    T get(int id) const {
        if ((size_ < id and id>0) or id<0) {
            throw out_of_range("List is empty");
        }
        Node* curr = head;
        for (int i = 0; i < id; i++) {
            curr = curr->next;
        }
        return curr->data;
    }
    void pop_back() {
        if(!head)
            throw out_of_range("List is empty");
        if (!head->next) {
            delete head;
            head = nullptr;
            size_--;
        }
        else {
            Node* curr = head;
            while (curr->next->next) {
                curr = curr->next;
            }
            delete curr->next;
            curr->next = nullptr;
            size_--;
        }
    }
    template<typename T1, typename T2>
    friend bool operator == (const List<T1>& a, const List<T2>& b);
    template<typename T1, typename T2>
    friend bool operator != (const List<T1>& a, const List <T2>& b);

};
template<typename T1, typename T2>
bool operator==(const List<T1>& a, const List<T2>& b) {
    if (a.size_ != b.size_)
        return false;

    typename List<T1>::Node * n1 = a.head;
    typename List<T2>::Node * n2 = b.head;

    while (n1) {
        if (!(n1->data == n2->data))
            return false;

        n1 = n1->next;
        n2 = n2->next;
    }

    return true;
}
template<typename T2,typename T1>

List<T2> convert(const List<T1>& a) {
    List<T2> ret;
    for (int i = 0; i < a.size(); i++) {
        ret.push_back(static_cast<T2>(a.get(i)));
    }
    return ret;
}
template<typename T1, typename T2>
bool operator !=(const List<T1>& a, const List<T2>& b) {
    return !(a == b);
}


int main() {
    List<double> a;
    List<int> b;
    a.push_back(1);
    a.push_back(2);
    b.push_back(1);
    a.push_back(1);
    a.push_back(2);
    b.push_back(1);
    b.push_back(2);
    b.push_back(2);
       

}
