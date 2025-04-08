#include <iostream>

using namespace std;

int max_finder(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int min_finder(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

struct student {
    int data;
    student *left;
    student *right;

    student(int m) {
        data = m;
        left = nullptr;
        right = nullptr;
    }

    student() {
        data = 0;
        left = nullptr;
        right = nullptr;
    }
};

struct BST {
    student *root;

    BST() {
        root = nullptr;
    }

    void insertPart(student *p) {
        if (root == nullptr) {
            root = p;
        } else {
            student *ptr = root;
            while (ptr != nullptr) {
                if (ptr->data != p->data) {
                    if (p->data < ptr->data && ptr->left == nullptr) {
                        ptr->left = p;
                        break;
                    }
                    if (p->data < ptr->data) {
                        ptr = ptr->left;
                    } else if (p->data > ptr->data && ptr->right == nullptr) {
                        ptr->right = p;
                        break;
                    } else {
                        ptr = ptr->right;
                    }
                }
            }
        }
    }

    void in_order(student *x, int &prev_mark, int &min_size, bool &was_here) {
        if (x != nullptr) {
            in_order(x->left, prev_mark, min_size, was_here);
            if (!was_here) {
                if (x->data - prev_mark < min_size) {
                    min_size = x->data - prev_mark;
                }
            }
            was_here = false;
            prev_mark = x->data;
            in_order(x->right, prev_mark, min_size, was_here);
        }
    }
};

int main() {
    int students;
    cin >> students;
    int mark;
    int prev_mark = 0;
    int min_size = INT_MAX;
    bool was_here = true;
    BST bst;
    for (int i = 0; i < students; i++) {
        cin >> mark;
        bst.insertPart(new student(mark));
    }
    bst.in_order(bst.root, prev_mark, min_size, was_here);
    cout << min_size << endl;
}
