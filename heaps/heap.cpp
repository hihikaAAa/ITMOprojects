#include <iostream>

using namespace std;

struct Heap {
    long long *arr;
    long long capacity;
    int heap_size;

    Heap(long long cap) {
        arr = new long long[cap];
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

    void insert(int val) {
        if (heap_size == capacity) {
            if (capacity == 0) {
                capacity = 1;
            } else {
                capacity *= 10;
            }
            long long *new_arr = new long long[capacity];
            for (int i = 0; i < heap_size; i++) {
                new_arr[i] = arr[i];
            }
            delete[] arr;
            arr = new_arr;
        }
        arr[heap_size ] = val;
        heap_size+=1;
        int i = heap_size - 1;
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

    void extract_min() {
        if (heap_size != 0) {
            int root = arr[0];
            arr[0] = arr[heap_size - 1];
            heap_size--;
            Min_Hipify(0);
            cout << root << "\n";
        } else {
            cout << "*" << "\n";
        }
    }

    void merge(Heap *queue1, Heap *queue2) {
        heap_size = 0;
        for (int i = 0; i < queue1->heap_size; i++) {
            insert(queue1->arr[i]);
        }
        for (int i = 0; i < queue2->heap_size; i++) {
            insert(queue2->arr[i]);
        }
    }

    void decrease_key(int x, int y) {
        int indx = -1;
        for (int i = 0; i < heap_size; i++) {
            if (arr[i] == x) {
                indx = i;
                break;
            }
        }
        if (indx == -1) {
            return;
        }
        arr[indx] = y;
        while (indx != 0 && arr[parent(indx)] > arr[indx]) {
            swap(arr[indx], arr[parent(indx)]);
            indx = parent(indx);
        }
    }

};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    string operation_name;
    long long mas_capasity = 10;
    Heap **mas = new Heap *[mas_capasity];
    long long queue_count = 0;
    long long queue_number_1;
    long long queue_number_2;
    int element;
    int new_element;
    while (cin >> operation_name) {
        if (queue_count + 1 == mas_capasity) {
            mas_capasity *= 10;
            Heap **new_mas = new Heap *[mas_capasity];
            for (int i = 0; i < queue_count; i++) {
                new_mas[i] = mas[i];
            }
            delete[] mas;
            mas = new_mas;
        }
        if (operation_name == "create") {
            mas[queue_count] = new Heap(0);
            queue_count++;
        } else if (operation_name == "merge") {
            cin >> queue_number_1 >> queue_number_2;
            mas[queue_count] = new Heap(mas[queue_number_1]->heap_size + mas[queue_number_2]->heap_size);
            mas[queue_count]->merge(mas[queue_number_1], mas[queue_number_2]);
            queue_count++;
        } else if (operation_name == "insert") {
            cin >> queue_number_1;
            cin >> element;
            mas[queue_number_1]->insert(element);
        } else if (operation_name == "decrease-key") {
            cin >> queue_number_1;
            cin >> element;
            cin >> new_element;
            mas[queue_number_1]->decrease_key(element, new_element);
        } else if (operation_name == "extract-min") {
            cin >> queue_number_1;
            mas[queue_number_1]->extract_min();
        }
    }
}
