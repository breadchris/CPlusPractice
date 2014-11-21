#include <iostream>

using namespace std;

struct Node {
    Node (int data = 0, Node* link = nullptr) : data(data), link(link) {}
    int data;
    Node* link;
};

class Sequence {
public:
    Sequence() {
        head = nullptr;
        current = nullptr;
    }

    Sequence(const Sequence& seq) {
        if (seq.head == nullptr) {
            head = nullptr;
            current = nullptr;
        } else { 
            head = new Node(seq.head->data, nullptr);
            current = head;
            Node* tmp = seq.head;
            while (tmp != nullptr) {
                current->link = new Node(tmp->data, nullptr);
                current = current->link;
                tmp = tmp->link;
            }
        }
    }

    Sequence& operator=(Sequence& other) {
        if (this != &other) {
            Sequence* seq = new Sequence(other);
            return *seq;
        }
        return other;
    }

    void reset() {
        current = head;
    }

    void add(int _data) {
        if (current != nullptr) {
            Node* next = new Node(_data);
            next->link = current->link;
            current->link = next;
            current = current->link;
        } else {
            head = new Node(_data);
            current = head;
        }
    }

    void remove() {
        if (current != nullptr) {
            Node* prev = head;
            while (prev->link != nullptr && prev->link != current) {
                prev = prev->link;
            }
            Node* next = current->link;
            delete current;
            prev->link = next;
        }
    }

    int data() {
        return current->data;
    }

    void next() {
        if (current != nullptr) {
            current = current->link;
        }
    }

    bool valid() {
        return current != nullptr;
    }

    void clear() {
        Node* cur = head;
        Node* next = nullptr;
        while (next != nullptr) {
            next = cur->link;
            delete cur;
            cout << cur->data << endl;
            cur = next;
        }
        head = nullptr;
        current = nullptr;
    }

    void display(ostream& os = cout, const string& sep=" ") {
        Node* tmp = head;
        while (tmp != nullptr) {
            os << tmp->data << sep;
            tmp = tmp->link;
        }
        os << endl;
    }

private:
    Node* current;
    Node* head;
};

int main() {
    Sequence s;

    for (int i = 0; i < 6; ++i) s.add(i);
    s.display();
    cout << "============\n";
    s.reset();
    for (int i = 0; i < 3; ++i) s.next();
    s.add(42);
    s.display();
    cout << "============\n";
    s.reset();
    for (int i = 0; i < 2; ++i) s.next();
    s.remove();
    s.display();
    cout << "============\n";
    s.clear();
    s.display();
    cout << "============\n";
}
