#include <iostream>

using namespace std;

template <class Object>
class LList;

template <class Object>
class LListItr;

template <class Object>
class LListNode
{
    LListNode( const Object & theElement = Object( ), LListNode * n = NULL )
    : element( theElement ), next( n ) { }
    
    Object element;
    LListNode *next;
    
    friend class LList<Object>;
    friend class LListItr<Object>;
};

template <class Object>
class LListItr
{
public:
    LListItr( ) : current( NULL ) { }

    void operator++() {
        current = current->next;
    }

    void operator++(int) {
        current = current->next;
    }

    const Object& operator*() const {
        return current->element;
    }
    
    bool operator==(LListItr<Object> rhs) {
        return current == rhs.current;
    }

    bool operator!=(LListItr<Object> rhs) {
        return current != rhs.current;
    }
    
private:
    LListNode<Object> *current;
    LListItr(LListNode<Object> *theNode) : current( theNode ) {}
    
    bool isPastEnd() const
    { return current == NULL; }
    
    friend class LList<Object>;
};

template <class Object>
class LList
{
public:
    typedef LListItr<Object> iterator;
    
    LList( ):listSize(0){ header = new LListNode<Object>; }
    
    LListItr<Object> begin() const { return LListItr<Object>(header->next); }
    LListItr<Object> end() const { return LListItr<Object>(); }
    bool isEmpty() const { return header->next == NULL; }
    int size() { return listSize; }
    
    void insert_before(const Object& x, LListItr<Object> p) {
        iterator temp = header;
         while(temp.current->next != NULL && temp.current->next != p.current) {
            temp++;
         }
         temp.current->next = new LListNode<Object>(x, temp.current->next);
    }

    void push_front(const Object& x) {
        LListNode<Object>* prevNext = header->next;
        header->next = new LListNode<Object>(x);
        header->next->next = prevNext;
        listSize++;
    }

    void pop_front() {
        if (!this->isEmpty()) {
            LListNode<Object>* pop = header->next;
            header->next = pop->next;
            delete pop; listSize--;
        }
    }

    LListItr<Object> erase_after(LListItr<Object> position) {
        LListNode<Object>* current = position.current;
        LListNode<Object>* remove = current->next;
        current->next = current->next->next;
        if (remove != NULL) {
            delete remove; listSize--;
        }
        return ++position;
    }

    template <class Predicate>
    void remove_if(Predicate pred) {
        LListItr<Object> itr = LListItr<Object>(header);
        while (itr.current->next != NULL) {
            if (pred(itr.current->next->element)) {
                erase_after(itr);
            }
        }
    }

    void remove_last (const Object & x) {
        iterator temp = header;
        iterator beforeLast, last;
        while(temp.current->next != NULL) {
            if(temp.current->next->element == x) {
                beforeLast = temp;
            }
            temp++;
        }
        last = beforeLast;
        last++;
        beforeLast.current->next = last.current->next;
        delete last.current;
    }

private:
    int listSize;
    LListNode<Object> *header;
    LListItr<Object> zeroth( ) const { return LListItr<Object>( header ); }
};

int main() {
    LList<int> a = LList<int>();
    cout << a.size() << endl;
    a.push_front(1);
    a.push_front(2);
    a.push_front(3);
    cout << a.size() << endl;
    a.pop_front();
    cout << a.size() << endl;
}

