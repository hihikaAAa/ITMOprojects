#include <iostream>

using namespace std;

void insertionSort(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        int sorted = i - 1;
        while (arr[sorted] > arr[sorted + 1] && sorted > -1) {
            int helper = arr[sorted];
            arr[sorted] = arr[sorted + 1];
            arr[sorted + 1] = helper;
            sorted--;
        }
    }
}

int main() {
    int size;
    cin >> size;
    int mas[size];
    for (int i = 0; i < size; i++) {
        cin >> mas[i];
    }
    insertionSort(mas, size);
    for (int i = 0; i < size; i++) {
        cout << mas[i] << " ";
    }
    return 0;
}
