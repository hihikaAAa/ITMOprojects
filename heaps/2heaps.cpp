#include <iostream>

using namespace std;

struct Node {
    int start;
    int end;
    long long money;
    int full_end;

    Node(int s, int e, long long m) {
        start = s;
        end = e;
        money = m;
        full_end = e + s;
    }

    Node() {
        start = end = full_end = 0;
        money = 0;
    }

};

struct Regulation {
    Node **arr;
    long long capacity;
    int heap_size;

    Regulation(long long cap) {
        arr = new Node *[cap];
        capacity = cap;
        heap_size = 0;
    }

    ~Regulation() {
        delete[] arr;
    }

    int parent(int i) {
        return (i - 1) / 2;
    }

    int left(int i) {
        return 2 * i + 1;
    }

    int right(int i) {
        return 2 * i + 2;
    }

    void insert(Node *node, int w) {
        if (heap_size == capacity) {
            capacity *= w;
            Node **new_arr = new Node *[capacity];
            for (int i = 0; i < heap_size; i++) {
                new_arr[i] = arr[i];
            }
            delete[] arr;
            arr = new_arr;
        }
        heap_size += 1;
        int i = heap_size - 1;
        arr[i] = node;
        while (i != 0 && arr[parent(i)]->full_end > arr[i]->full_end) {
            swap(arr[i], arr[parent(i)]);
            i = parent(i);
        }
    }

    void Min_Heapify(int i) {
        int l = left(i);
        int r = right(i);
        int smallest = i;
        if (l < heap_size && arr[l]->full_end < arr[smallest]->full_end) {
            smallest = l;
        }
        if (r < heap_size && arr[r]->full_end < arr[smallest]->full_end) {
            smallest = r;
        }
        if (smallest != i) {
            swap(arr[i], arr[smallest]);
            Min_Heapify(smallest);
        }
    }

    Node *extract_min() {
        if (heap_size != 0) {
            Node *root = arr[0];
            arr[0] = arr[heap_size - 1];
            heap_size--;
            Min_Heapify(0);
            return root;
        }
        return nullptr;
    }

    Node *top() {
        if (heap_size != 0) {
            return arr[0];
        }
        return nullptr;
    }
};

struct Heap {
    int *arr;
    long long capacity;
    int heap_size;

    Heap(long long cap) {
        arr = new int[cap];
        capacity = cap;
        heap_size = 0;
    }

    Heap() {
        arr = nullptr;
        capacity = 0;
        heap_size = 0;
    }

    ~Heap() {
        delete[] arr;
    }

    int parent(int i) {
        return (i - 1) / 2;
    }

    int left(int i) {
        return 2 * i + 1;
    }

    int right(int i) {
        return 2 * i + 2;
    }

    void insert(int val, int w) {
        if (heap_size == capacity) {
            capacity *= w;
            int *new_arr = new int[capacity];
            for (int i = 0; i < heap_size; i++) {
                new_arr[i] = arr[i];
            }
            delete[] arr;
            arr = new_arr;
        }
        heap_size += 1;
        int i = heap_size - 1;
        arr[i] = val;
        while (i != 0 && arr[parent(i)] > arr[i]) {
            swap(arr[i], arr[parent(i)]);
            i = parent(i);
        }
    }

    void Min_Hipify(int i) {
        int l = left(i);
        int r = right(i);
        int smallest = i;
        if (l < heap_size && arr[l] < arr[smallest]) {
            smallest = l;
        }
        if (r < heap_size && arr[r] < arr[smallest]) {
            smallest = r;
        }
        if (smallest != i) {
            swap(arr[i], arr[smallest]);
            Min_Hipify(smallest);
        }
    }

    int extract_min() {
        if (heap_size != 0) {
            int root = arr[0];
            arr[0] = arr[heap_size - 1];
            heap_size--;
            Min_Hipify(0);
            return root;
        }
        return -1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int workers;
    cin >> workers;
    int orders;
    cin >> orders;
    int salary;
    long long answer = 0;
    int start_time;
    int end_time;
    Regulation r(orders);
    Heap heap(workers);
    for (int i = 0; i < workers; i++) {
        cin >> salary;
        heap.insert(salary, workers);
    }
    for (int i = 0; i < orders; i++) {
        cin >> start_time;
        cin >> end_time;
        while (r.heap_size != 0 && r.top()->full_end <= start_time) {
            Node *freed_salary = r.extract_min();
            if(freed_salary!=nullptr) {
                heap.insert(freed_salary->money, workers);
            }
            delete freed_salary;
        }
        if (heap.heap_size != 0) {
            long long helper = heap.extract_min();
            Node* new_node = new Node(start_time, end_time, helper);
            r.insert(new_node,workers);
            answer += helper * end_time;
        }
    }
    cout << answer << endl;
}
