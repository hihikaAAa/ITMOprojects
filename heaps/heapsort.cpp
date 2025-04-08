#include <iostream>

using namespace std;

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

    int parent(int i) {
        return (i - 1) / 2;
    }

    int left(int i) {
        return 2 * i + 1;
    }

    int right(int i) {
        return 2 * i + 2;
    }


    void insert(int val) {
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

    void heap_sort() {
        int temp[capacity];
        for (int i = 0; i < capacity; i++) {
            temp[i] = extractMin();
            cout << temp[i] << " ";
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int size;
    cin >> size;
    minHeap heap(size);
    for (int i = 0; i < size; i++) {
        int val;
        cin >> val;
        heap.insert(val);
    }
    heap.heap_sort();
}
