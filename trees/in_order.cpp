#include <iostream>

using namespace std;

struct TreePart {
    int data;
    TreePart *left;
    TreePart *right;

    TreePart() {
        data = 0;
        left = nullptr;
        right = nullptr;
    }

    TreePart(int v) {
        data = v;
        left = nullptr;
        right = nullptr;
    }
};

struct BST {
    TreePart *root;

    BST() {
        root = nullptr;
    }

    bool isEmpty() {
        return root == nullptr;
    }

    void insertPart(TreePart *p) {
        if (root == nullptr) {
            root = p;
        } else {
            TreePart *ptr = root;
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

    void printIn_order(TreePart *t) {
        if(t != nullptr) {
            printIn_order(t->left);
            cout << t->data << " ";
            printIn_order(t->right);
        }
    }
};

int main() {
    int s;
    cin>>s;
    int helper = 0;
    BST t;
    for (int i = 0; i < s; ++i) {
        cin>>helper;
        TreePart *p = new TreePart(helper);
        t.insertPart(p);
    }
    t.printIn_order(t.root);
}
