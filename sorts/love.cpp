#include <iostream>

using namespace std;

void partition(long a[], long l, long r) {
    long left = l, right = r;
    long mid = a[(left + right) / 2];
    while (left <= right) {
        while (a[left] > mid) {
            left++;
        }
        while (a[right] < mid) {
            right--;
        }
        if (left <= right) {
            swap(a[left], a[right]);
            left++;
            right--;
        }
    }
    if (l < right) {
        partition(a, l, right);
    }
    if (r > left) {
        partition(a, left, r);
    }
}


int main() {
    long size;
    cin >> size;
    long mas[size];
    long length = 0;
    long width = 0;
    long ans = 0;
    for (int i = 0; i < size; i++) {
        cin >> mas[i];
    }
    partition(mas, 0, size - 1);
    for (int i = 0; i < size - 1; i++) {
        if (length == 0) {
            if (mas[i] == mas[i + 1] || mas[i] - 1 == mas[i + 1]) {
                length = mas[i + 1];
                i += 2;
            }
        }
        if (length > 0) {
            if (mas[i] == mas[i + 1] || mas[i] - 1 == mas[i + 1]) {
                width = mas[i + 1];
                i++;
            }
        }

        if (length > 0 && width > 0) {
            ans += length * width;
            length = 0;
            width = 0;
        }
    }
    cout << ans << endl;
    return 0;
}


