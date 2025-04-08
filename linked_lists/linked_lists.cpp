#include <iostream>

using namespace std;


struct Node {
    double data;
    Node *next;


    Node(double d) {
        data = d;
        next = nullptr;
    }
};

struct DoubleLinkedList {
    Node *head;
    Node *current;
    Node *copy;

    DoubleLinkedList() {
        head = nullptr;
        current = nullptr;
        copy = nullptr;
    }

    void partition(double arr[], int l, int r) {
        int left = l;
        int right = r;
        double mid = arr[(left + right) / 2];
        while (left <= right) {
            while (arr[left] < mid) {
                left++;
            }
            while (arr[right] > mid) {
                right--;
            }
            if (left <= right) {
                swap(arr[left], arr[right]);
                left++;
                right--;
            }
        }
        if (l < right) {
            partition(arr, l, right);
        }
        if (r > left) {
            partition(arr, left, r);
        }
    }

    void appendNode(Node *n) {
        if (head == nullptr) {
            head = copy = current = n;
        } else {
            Node *ptr = current;
            current = n;
            ptr->next = current;
        }
    }


    void memory_cleaner() {
        while (head != nullptr) {
            Node *ptr = head;
            head = head->next;
            delete ptr;
        }
    }

    void counter(double *arr, int length, int numb) {
        if (head != nullptr) {
            Node *a = head;
            for (int i = 0; i < length; i++) {
                if (a->data == head->data) {
                    a = a->next;
                }
                arr[i] = head->data / a->data;
                a = a->next;
                if (a == nullptr) {
                    head = head->next;
                    a = head;
                }
            }
        }
        partition(arr, 0, length);
        Node *ptr = copy;
        while (copy->next != nullptr) {
            if (ptr->data == copy->data) {
                ptr = ptr->next;
            }
            if (copy->data / ptr->data == arr[numb]) {
                cout << copy->data << " " << ptr->data << endl;
            }
            ptr = ptr->next;
            if (ptr == nullptr) {
                copy = copy->next;
                ptr = copy;
            }
        }
    }
};


int main() {
    int size;
    scanf("%d", &size);
    int numb;
    scanf("%d", &numb);
    DoubleLinkedList s;
    double arr[size * (size - 1) / 2];
    for (int i = 0; i < size; i++) {
        double helper;
        scanf("%lf", &helper);
        Node *n1 = new Node(helper);
        s.appendNode(n1);
    }
    s.counter(arr, size * (size - 1) / 2, numb);
}


