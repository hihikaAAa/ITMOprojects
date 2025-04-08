#include <iostream>

using namespace std;

struct TreePart {
    int data;
    int size;
    TreePart *left;
    TreePart *right;

    TreePart() {
        data = 0;
        size = 0;
        left = nullptr;
        right = nullptr;
    }

    TreePart(int d) {
        data = d;
        size = 1;
        left = nullptr;
        right = nullptr;
    }
};

struct BST {
    TreePart *root;

    BST() {
        root = nullptr;
    }

    void updateSize(TreePart *p) {
        p->size = 1;
        if (p->left != nullptr) {
            p->size += p->left->size;
        }
        if (p->right != nullptr) {
            p->size += p->right->size;
        }
    }

    TreePart *insertPart(TreePart *p, int data) {
        if (p == nullptr) {
            return new TreePart(data);
        }
        if (data < p->data) {
            p->left = insertPart(p->left, data);
        } else if (data > p->data) {
            p->right = insertPart(p->right, data);
        }
        updateSize(p);
        return p;
    }

    TreePart *findMin(TreePart *p) {
        while (p->left != nullptr) {
            p = p->left;
        }
        return p;
    }

    TreePart *deletePart(TreePart *p, int d) {
        if (d < p->data) {
            p->left = deletePart(p->left, d);
        } else if (d > p->data) {
            p->right = deletePart(p->right, d);
        } else {
            if (p->left == nullptr) {
                TreePart *temp = p->right;
                delete p;
                return temp;
            }
            if (p->right == nullptr) {
                TreePart *temp = p->left;
                delete p;
                return temp;
            }
            TreePart *temp = findMin(p->right);
            p->data = temp->data;
            p->right = deletePart(p->right, temp->data);
        }
        updateSize(p);
        return p;
    }

    int search(TreePart *p, int d) {
        if (p != nullptr) {
            int r_size = 0;
            if (p->right != nullptr) {
                r_size = p->right->size;
            }
            if (d == r_size + 1) {
                return p->data;
            }
            if (d <= r_size) {
                return search(p->right, d);
            }
            return search(p->left, d - r_size - 1);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int count = 0;
    int operation;
    int helper;
    int ans;
    cin >> count;
    BST t;
    for (int i = 0; i < count; i++) {
        cin >> operation;
        if (operation == 1) {
            cin >> helper;
            t.root = t.insertPart(t.root, helper);
        } else if (operation == -1) {
            cin >> helper;
            t.root = t.deletePart(t.root, helper);
        } else if (operation == 0) {
            cin >> helper;
            ans = t.search(t.root, helper);
            cout << ans << '\n';
        }
    }
}

