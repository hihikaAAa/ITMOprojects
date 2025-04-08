#include <iostream>

using namespace std;

void partition(long int *arr, int l, int r) {
    int left = l;
    int right = r;
    int mid = arr[(left+right)/2];
    while (left <= right) {
        while(arr[left]<mid) {
            left++;
        }
        while(arr[right]>mid) {
            right--;
        }
        if(left<=right) {
            swap(arr[left],arr[right]);
            left++;
            right--;
        }
    }
    if (l<right) {
        partition(arr, l, right);
    }
    if (r>left) {
        partition(arr,left,r);
    }
}

int main() {
    int size; cin >> size;
    long mas[size];
    for (int i = 0; i < size; i++) {
        cin >> mas[i];
    }
    partition(mas,0,size);
    for (int i = 0; i < size; i++) {
        cout << mas[i] << " ";
    }
    return 0;
}
