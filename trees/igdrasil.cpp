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

    TreePart(int d) {
        data = d;
        left = nullptr;
        right = nullptr;
    }
};

struct BST {
    TreePart *root;

    BST() {
        root = nullptr;
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
                } else {
                    break;
                }
            }
        }
    }

    void search(TreePart *p, int current_lvl, int &max_lvl) {
        if (p == nullptr) {
            return;
        }
        if (current_lvl > max_lvl) {
            cout << p->data << ' ';
            max_lvl = current_lvl;
        }
        search(p->right, current_lvl+1, max_lvl);
        search(p->left, current_lvl+1, max_lvl);
    }
};

int main() {
    int count;
    int helper = 0;
    BST t;
    cin >> count;
    for (int i = 0; i < count; i++) {
        cin >> helper;
        t.insertPart(new TreePart(helper));
    }
    int max_lvl = -1;
    int current_lvl = 0;
    t.search(t.root, current_lvl, max_lvl);
    return 0;
}

