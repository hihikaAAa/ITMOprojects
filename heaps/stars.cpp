#include <iostream>

using namespace std;

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return 2 * i + 1;
}

int right(int i) {
    return 2 * i + 2;
}

struct maxHeap {
    int *arr;
    long long capacity;
    int heap_size;

    maxHeap(int cap) {
        capacity = cap;
        arr = new int[cap];
        heap_size = 0;
    }

    maxHeap() {
        arr = nullptr;
        capacity = 0;
        heap_size = 0;
    }

    ~maxHeap() {
        delete[] arr;
    }

    void insert(int val) {
        if (heap_size == capacity) {
            if (capacity == 0) {
                capacity = 1;
            } else {
                capacity *= 10;
            }
            int *new_arr = new int[capacity];
            for (int i = 0; i < heap_size; i++) {
                new_arr[i] = arr[i];
            }
            delete[] arr;
            arr = new_arr;
        }
        arr[heap_size] = val;
        heap_size += 1;
        int i = heap_size - 1;
        while (i != 0 && arr[parent(i)] < arr[i]) {
            swap(arr[i], arr[parent(i)]);
            i = parent(i);
        }
    }

    int extractMax() {
        if (heap_size <= 0) {
            return -1;
        }
        if (heap_size == 1) {
            heap_size--;
            return arr[0];
        }

        int root = arr[0];
        arr[0] = arr[heap_size - 1];
        heap_size--;
        Max_Heapify(0);
        return root;
    }

    void Max_Heapify(int i) {
        int l = left(i);
        int r = right(i);
        int largest = i;
        if (l < heap_size && arr[l] > arr[largest])
            largest = l;
        if (r < heap_size && arr[r] > arr[largest])
            largest = r;
        if (largest != i) {
            swap(arr[i], arr[largest]);
            Max_Heapify(largest);
        }
    }

    int top() {
        if (heap_size != 0) {
            return arr[0];
        }
        return -1;
    }
};

struct minHeap {
    int *arr;
    long long capacity;
    int heap_size;

    minHeap(int cap) {
        capacity = cap;
        arr = new int[cap];
        heap_size = 0;
    }

    minHeap() {
        arr = nullptr;
        capacity = 0;
        heap_size = 0;
    }

    ~minHeap() {
        delete[] arr;
    }

    void insert(int val) {
        if (heap_size == capacity) {
            if (capacity == 0) {
                capacity = 1;
            } else {
                capacity *= 10;
            }
            int *new_arr = new int[capacity];
            for (int i = 0; i < heap_size; i++) {
                new_arr[i] = arr[i];
            }
            delete[] arr;
            arr = new_arr;
        }
        arr[heap_size] = val;
        heap_size += 1;
        int i = heap_size - 1;
        while (i != 0 && arr[parent(i)] > arr[i]) {
            swap(arr[i], arr[parent(i)]);
            i = parent(i);
        }
    }

    int extractMin() {
        if (heap_size <= 0)
            return -1;
        if (heap_size == 1) {
            heap_size--;
            return arr[0];
        }

        int root = arr[0];
        arr[0] = arr[heap_size - 1];
        heap_size--;
        Min_Heapify(0);
        return root;
    }

    void Min_Heapify(int i) {
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
            Min_Heapify(smallest);
        }
    }

    int top() {
        if (heap_size != 0) {
            return arr[0];
        }
        return -1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int size;
    cin >> size;
    int value;
    maxHeap maximum(size / 2 + 1);
    minHeap minimum(size / 2 + 1);
    for (int i = 0; i < size; i++) {
        cin >> value;
        if (maximum.heap_size == 0 || value <= maximum.top()) {
            maximum.insert(value);
        } else {
            minimum.insert(value);
        }
        if (maximum.heap_size > minimum.heap_size + 1) {
            minimum.insert(maximum.extractMax());
        } else if (minimum.heap_size > maximum.heap_size) {
            maximum.insert(minimum.extractMin());
        }
        if (maximum.heap_size > minimum.heap_size) {
            cout << maximum.top() << "\n";
        } else {
            cout << (maximum.top() + minimum.top()) / 2 << "\n";
        }
    }
    return 0;
}
