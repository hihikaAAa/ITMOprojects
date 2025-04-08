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

    TreePart *findMin(TreePart *p) {
        while (p->left != nullptr) {
            p = p->left;
        }
        return p;
    }

    TreePart *deletePart(TreePart *p, int d) {
        if (p == nullptr) {
            return nullptr;
        }
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
        return p;
    }

    void search(int d) {
        TreePart *ptr = root;
        while (ptr != nullptr) {
            if (d == ptr->data) {
                cout << "true\n";
                return;
            }
            if (d < ptr->data) {
                ptr = ptr->left;
            } else {
                ptr = ptr->right;
            }
        }
        cout << "false\n";
    }

    void finderMax(int d) {
        int result = -1;
        TreePart *ptr = root;
        while (ptr != nullptr) {
            if (d < ptr->data) {
                result = ptr->data;
                ptr = ptr->left;
            } else {
                ptr = ptr->right;
            }
        }
        if (result!=-1) {
            cout << result << '\n';
            return;
        }
        cout << "none\n";
    }

    void finderMin(int d) {
        int result = -1;
        TreePart *ptr = root;
        while (ptr != nullptr) {
            if (d > ptr->data) {
                result = ptr->data;
                ptr = ptr->right;
            } else {
                ptr = ptr->left;
            }
        }
        if (result!=-1) {
            cout << result << '\n';
            return;
        }
        cout << "none\n";
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int num;
    int helper;
    BST t;
    while (cin >> num) {
        if (num == 0) {
            cin >> helper;
            t.insertPart(new TreePart(helper));
        } else if (num == 1) {
            cin >> helper;
            t.root = t.deletePart(t.root, helper);
        } else if (num == 2) {
            cin >> helper;
            t.search(helper);
        } else if (num == 3) {
            cin >> helper;
            t.finderMax(helper);
        } else if (num == 4) {
            cin >> helper;
            t.finderMin(helper);
        }
    }
}



