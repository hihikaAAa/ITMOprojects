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
    DoubleLinkedList() {
        head = nullptr;
        tail = nullptr;
    }

    Node *head;
    Node *tail;


    void prependNode(long data) {
        tail = new Node(data);
        tail->next = head;
        head = tail;
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
    char symb;
    long helper;
    for (int i = 0; i < size; i++) {
        scanf(" %c", &symb);
        if (symb == '+') {
            scanf("%ld", &helper);
            s.prependNode(helper);
        } else {
            printf("%ld\n", s.deleteFirstNode());
        }
    }
    return 0;
}
