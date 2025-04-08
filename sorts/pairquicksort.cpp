#include <iostream>


using namespace std;


void partition(long a[], long l, long r) {
    long left = l, right = r;
    long mid;
    if ((left + right) / 2 % 2 != 0) {
        mid = a[(left + right) / 2 - 1];
    } else {
        mid = a[(left + right) / 2];
    }
    while (left <= right) {
        while (a[left] < mid) {
            left += 2;
        }
        while (a[right] > mid) {
            right -= 2;
        }
        if (left <= right) {
            swap(a[left], a[right]);
            swap(a[left + 1], a[right + 1]);
            left += 2;
            right -= 2;
        }
    }
    if (l < right) {
        partition(a, l, right);
    }
    if (r > left) {
        partition(a, left, r);
    }
}


const int N = 100000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int size;
    cin >> size;
    long arr[N * 2];
    long ans = 0;
    for (int i = 0; i < size * 2; i++) {
        cin >> arr[i];
    }
    partition(arr, 0, size * 2 - 2);
    int f_elem = arr[0], s_elem = arr[1];
    for (int i = 2; i < size * 2; i += 2) {
        if (arr[i] > f_elem) {
            if (arr[i] > s_elem) {
                ans += s_elem - f_elem + 1;;
                s_elem = arr[i + 1];
            } else {
                ans += arr[i] - f_elem;
            }

            if (arr[i + 1] > s_elem) {
                s_elem = arr[i + 1];
            }
            f_elem = arr[i];
        } else if (arr[i + 1] > s_elem) {
            s_elem = arr[i + 1];
        }
    }
    ans += s_elem - f_elem + 1;
    cout << ans;
}


