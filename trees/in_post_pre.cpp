#include <iostream>

using namespace std;

struct Node {
    int value;
    Node *next;

    Node(int v) {
        value = v;
        next = nullptr;
    }
};

struct checker {
    Node *top;
    checker() {
        top = nullptr;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    void memory_cleaner() {
        while (top != nullptr) {
            Node *temp = top;
            top = top->next;
            delete temp;
        }
    }

    void prependNode(int data) {
        if (top == nullptr) {
            top = new Node(data);
        } else {
            Node *ptr = new Node(data);
            ptr->next = top;
            top = ptr;
        }
    }


    int pop_first() {
        int ans = top->value;
        Node *temp = top;
        top = top->next;
        delete temp;
        return ans;
    }

    bool pre_order_checker(int arr[], int size) {
        long long root = -1000000000000000;
        for (int i = 0; i < size; i++) {
            int current_value = arr[i];
            if (current_value < root) {
                return false;
            }
            if (top != nullptr) {
                while (!isEmpty() && top->value < current_value) {
                    root = top->value;
                    pop_first();
                }
            }
            prependNode(current_value);
        }
        return true;
    }

    bool post_order_checker(int arr[], int size) {
        long long root = 1000000000000000000;
        for (int i = size - 1; i >= 0; i--) {
            int current_value = arr[i];
            if (current_value > root)
                return false;
            while (!isEmpty() && top->value > current_value) {
                root = top->value;
                pop_first();
            }
            prependNode(current_value);
        }
        return true;
    }
};

bool in_order_checker(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int size;
    cin >> size;
    long long sum_pre = 0;
    long long sum_in = 0;
    long long sum_out = 0;
    bool pre = false;
    bool in = false;
    bool post = false;
    int helper;
    int *arr = new int[size];
    checker s;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < size; j++) {
            cin >> helper;
            arr[j] = helper;
            if (i == 0) {
                sum_pre += helper;
            } else if (i == 1) {
                sum_in += helper;
            } else if (i == 2) {
                sum_out += helper;
            }
        }
        if (i == 0) {
            if (s.pre_order_checker(arr, size)) {
                pre = true;
            }
            s.memory_cleaner();
        } else if (i == 1) {
            if (sum_pre == sum_in && in_order_checker(arr, size)) {
                in = true;
            }
        } else if (i == 2) {
            if (sum_pre == sum_out && s.post_order_checker(arr, size)) {
                post = true;
            }
        }
    }
    if (pre && in && post) {
        cout << "YES";
    } else {
        cout << "NO";
    }
    return 0;
}

