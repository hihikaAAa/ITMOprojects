#include <iostream>

using namespace std;

int main() {
    int size;
    cin >> size;
    long long arr[size];
    bool answer = true;
    for (int i = 0; i < size; i++) {
        cin >> arr[i];
    }
    for (int i = 0; i < (size-1)/2; i++) {
        if (arr[i] > arr[2*i + 1] || arr[i] > arr[2*i + 2]) {
            answer = false;
            break;
        }
    }
    if (answer) {
        cout << "YES";
    } else {
        cout << "NO";
    }
    return 0;
}
