#include <iostream>
#include <string>

using namespace std;

void CountingSort(string arr[], int size, int div) {
    string output[size];
    int count[256] = {0};
    for (int i = 0; i < size; i++) {
        int index =  arr[i][div];
        count[index] += 1;
    }
    for (int i = 1; i < 256; i++) {
        count[i] += count[i - 1];
    }
    for (int i = size - 1; i >= 0; i--) {
        output[count[arr[i][div]]-1] = arr[i];
        count[arr[i][div]] -= 1;
    }
    for (int i = 0; i < size; i++) {
        arr[i] = output[i];
    }
}

void RadixSort(int size, int t, int k, string arr[]) {
    for (int i = k - 1; i >= k - t; i--) {
        CountingSort(arr, size, i);
    }
}


int main() {
    int n;
    int k;
    int t;
    cin >> n >> k >> t;
    string arr[n];
    for (int i = 0; i < n; i++) {
        string word;
        cin >> word;
        arr[i] = word;
    }
    RadixSort(n, t, k, arr);
    for (int i = 0; i < n; i++) {
        cout << arr[i] << endl;
    }
}


