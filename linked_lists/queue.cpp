#include <iostream>

using namespace std;

struct Node {
    long data;
    Node *next;

    Node(long d) {
        data = d;
        next = nullptr;
    }
};

struct DoubleLinkedList {
    Node *head;
    Node *tail;

    DoubleLinkedList() {
        head = nullptr;
        tail = nullptr;
    }


    void appendNode(long data) {
        if (head == nullptr) {
            head = tail = new Node(data);
        } else {
            Node *ptr = tail;
            tail = new Node(data);
            ptr->next = tail;
        }
    }

    long deleteFirstNode() {
        long ans = head->data;
        Node *ptr = head;
        head = head->next;
        delete ptr;
        return ans;
    }
};


int main() {
    cin.tie(0);
    int size;
    scanf("%d", &size);
    DoubleLinkedList s;
    long helper;
    char symb;
    for (int i = 0; i < size; i++) {
        scanf(" %c", &symb);
        if (symb == '+') {
            scanf("%ld", &helper);
            s.appendNode(helper);
        } else {
            printf("%ld\n", s.deleteFirstNode());
        }
    }
}

