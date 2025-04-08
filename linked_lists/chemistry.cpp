    #include <iostream>
    #include <string>

    using namespace std;

    struct Components {
        char key;
        int data;
        Components *next;
        Components *previous;

        Components() {
            key = 0;
            data = 0;
            next = nullptr;
            previous = nullptr;
        }

        Components(char k, int d) {
            key = k;
            data = d;
            next = nullptr;
            previous = nullptr;
        }
    };

    struct Mnoz {
        int data;
        Mnoz *next;
        Mnoz *previous;

        Mnoz() {
            data = 0;
            next = nullptr;
            previous = nullptr;
        }

        Mnoz(int m) {
            data = m;
            next = nullptr;
            previous = nullptr;
        }
    };

    struct DoubleLinkedList {
        Components *open_s;
        Components *close_s;
        Mnoz *open;
        Mnoz *close;

        DoubleLinkedList() {
            open_s = nullptr;
            close_s = nullptr;
            open = nullptr;
            close = nullptr;
        }

        void appendComponent(Components *n) {
            if (open_s == nullptr) {
                open_s = close_s = n;
            } else {
                Components *ptr = close_s;
                close_s = n;
                close_s->previous = ptr;
                ptr->next = close_s;
            }
        }

        void appendMnoz(Mnoz *n) {
            if (open == nullptr) {
                open = close = n;
            } else {
                Mnoz *ptr = close;
                close = n;
                close->previous = ptr;
                ptr->next = close;
            }
        }

        int pop() {
            if (close != nullptr) {
                Mnoz *ptr = close;
                close = close->previous;
                if(close==nullptr) {
                    open = close;
                }
                int c = ptr->data;
                delete ptr;
                return c;

            }
        }


        void solved(int arr[]) {
            if (open_s != nullptr) {
                Components *c = open_s;
                int current = 1;
                int mnoz = 1;
                while (c != nullptr) {
                    if (c->key != '(' && c->key != ')') {
                        c->data = c->data * mnoz;
                        arr[c->key - 65] += c->data;
                    }
                    if (c->key == ')') {
                        Mnoz *n1 = new Mnoz(c->data);
                        appendMnoz(n1);
                        mnoz = mnoz * c->data;
                    }
                    if (c->key == '(') {
                        current = pop();
                        mnoz = mnoz / current;
                    }
                    c = c->next;
                }
            }
        }


        void printlist(int arr[]) {
            for (int i = 0; i < 26; i++) {
                char alpha = 65 + i;
                if (arr[i] != 0) {
                    if (arr[i] == 1) {
                        cout << alpha;
                    } else {
                        cout << alpha << arr[i];
                    }
                }
            }
        }
    };

    int main() {
        string formula;
        cin >> formula;
        char key = ' ';
        int data = 0;
        int size = formula.size();
        int arr[26] = {0};
        int step = 1;
        DoubleLinkedList s;
        for (int i = size - 1; i >= 0; i--) {
            if (48 <= (int) formula[i] && 57 >= (int) formula[i]) {
                while (48 <= (int) formula[i] && 57 >= (int) formula[i]) {
                    data += ((int) formula[i] - 48) * step;
                    step *= 10;
                    i--;
                }
                key = formula[i];
            } else {
                key = formula[i];
                data = 1;
            }
            Components *c1 = new Components(key, data);
            s.appendComponent(c1);
            data = 0;
            key = ' ';
            step = 1;
        }
        s.solved(arr);
        s.printlist(arr);
    }

