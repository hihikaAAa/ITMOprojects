#include <iostream>

using namespace std;

void Anti_Qsort(int *newmas, int size) {
    if (size > 1) {
        newmas[0] = 1;
        newmas[1] = 2;
        for (int i = 2; i < size; i++) {
            newmas[i] = i + 1;
            swap(newmas[i / 2], newmas[i]);
        }
    } else {
        newmas[0] = 1;
    }
}

int main() {
    int size;
    cin >> size;
    int mas[size];
    Anti_Qsort(mas, size);
    for (int i = 0; i < size; i++) {
        cout << mas[i] << " ";
    }
}
