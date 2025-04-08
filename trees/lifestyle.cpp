#include <iostream>

using namespace std;

int max_finder(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int min_finder(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int main() {
    int size;
    cin >> size;
    int answer = 0;
    int arr[size];
    for (int i = 0; i < size; i++) {
        cin >> arr[i];
    }
    int l_max[size];
    l_max[0] = arr[0];
    int r_max[size];
    r_max[size - 1] = arr[size - 1];
    for (int i = 1; i < size; i++) {
        l_max[i] = max_finder(l_max[i - 1], arr[i]);
    }
    for (int i = size - 2; i >= 0; i--) {
        r_max[i] = max_finder(r_max[i + 1], arr[i]);
    }
    for (int i = 0; i < size; i++) {
        answer += max_finder(0, min_finder(l_max[i], r_max[i]) - arr[i]);
    }
    cout << answer;
}
