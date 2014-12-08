#include <cstdlib> // To allow NULL if no other includes
#include <iostream>

using namespace std;

struct Node {
	Node(int data = 0, Node* link = NULL) : data(data), link(link) {}
	int data;
	Node* link;
};

void printList (Node* head) {
    Node* tmp = head;
    while (tmp != NULL) {
        cout << tmp->data << " ";
        tmp = tmp->link;
    }
    cout << endl;
}

void splice(Node* target, Node* ptr) {
    Node* end = target;
    while (end->link != NULL) {
        end = end->link;
    }
    end->link = ptr->link;
    ptr->link = target;
}

Node* appendToHead(int val, Node*& headPtr) {
	Node* tmp = new Node(val, headPtr);
    headPtr = tmp;
    return tmp;
}

Node* findList(Node* search, Node* find) {
    if (find == NULL)
        return NULL;

    cout << "\nAttempt match: ";
    printList(find);

    Node* currentFind = NULL;
    Node* tmpSearch = search;
    Node* start = NULL;
    while (tmpSearch != NULL) {
        Node* tmpFind = tmpSearch;
        currentFind = find;
        while (currentFind != NULL && tmpFind != NULL) {
            if (currentFind->data != tmpFind->data) break;
            currentFind = currentFind->link;
            tmpFind = tmpFind->link;

        }
        if (currentFind == NULL) {
            start = tmpSearch;
            break;
        }
        tmpSearch = tmpSearch->link;
    }

    if (start != NULL) {
        return start;
    } else {
        cout << "Failed to match" << endl;
        return NULL;
    }
}

int main() {
    cout << "Part One:\n" << endl;
    Node* l1 = NULL;
    appendToHead(1, l1);
    appendToHead(9, l1);
    Node* ptr = appendToHead(7, l1);
    appendToHead(5, l1);

    Node* l2 = NULL;
    appendToHead(2, l2);
    appendToHead(3, l2);
    appendToHead(6, l2);

    cout << "L1: ";
    printList(l1);
    cout << "L2: ";
    printList(l2);

    splice(l2, ptr);

    cout << "L1: ";
    printList(l1);
    cout << "L2: ";
    printList(l2);

    cout << "======================\n" << endl;

    cout << "Part Two:" << endl;

    Node* target = NULL;
    appendToHead(6, target);
    appendToHead(5, target);
    appendToHead(4, target);
    appendToHead(2, target);
    appendToHead(3, target);
    appendToHead(2, target);
    appendToHead(3, target);
    appendToHead(2, target);
    appendToHead(1, target);

    cout << "Target: ";
    printList(target);

    Node* find1 = NULL;
    appendToHead(1, find1);

    printList(findList(target, find1));

    Node* find2 = NULL;
    appendToHead(3, find2);
    appendToHead(9, find2);

    printList(findList(target, find2));

    Node* find3 = NULL;
    appendToHead(3, find3);
    appendToHead(2, find3);
    
    printList(findList(target, find3));

    Node* find4 = NULL;
    appendToHead(6, find4);
    appendToHead(5, find4);
    appendToHead(4, find4);
    appendToHead(2, find4);
    
    printList(findList(target, find4));

    Node* find5 = NULL;
    appendToHead(4, find5);
    appendToHead(2, find5);
    appendToHead(3, find5);
    appendToHead(2, find5);
    
    printList(findList(target, find5));

    Node* find6 = NULL;
    appendToHead(7, find6);
    appendToHead(6, find6);
    appendToHead(5, find6);
    
    printList(findList(target, find6));

    return 0;
}
