#include <iostream>

using namespace std;

void merge(int arr[], int temp[],int &left, int &mid, int &right, long long &ans) {
    int i = left;
    int j = mid + 1;
    int k = left;
    int Lsize = j - left;
    int current = 0;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i++;
            k++;
            current++;
        } else {
            temp[k] = arr[j];
            j++;
            k++;
            ans += Lsize - current;
        }
    }
    while (i <= mid) {
        temp[k] = arr[i];
        i++;
        k++;
    }
    while (j <= right) {
        temp[k] = arr[j];
        j++;
        k++;
    }
    for (int s = left; s <= right; s++) {
        arr[s] = temp[s];
    }
}


void merge_sort(int arr[],int temp[], int left, int right, long long &ans) {
    if (left < right) {
        int mid = (left + right) / 2;
        merge_sort(arr,temp, left, mid, ans);
        merge_sort(arr,temp, mid + 1, right, ans);
        merge(arr,temp, left, mid, right, ans);
    }
}

int main() {
    int size;
    cin >> size;
    int mas[size];
    int temp[size];
    long long a = 0;
    for (int i = 0; i < size; i++) {
        cin >> mas[i];
    }

    merge_sort(mas,temp, 0, size - 1, a);

    cout << a << endl;
}
