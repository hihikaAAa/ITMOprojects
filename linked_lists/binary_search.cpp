#include <iostream>

using namespace std;

bool helper(int chair , int arr[],int size, int max_people ) {
    int people  = 1;
    int guy = arr[0];
    for (int i =0; i < size; i++) {
        if (arr[i] - guy >= chair) {
            people+=1;
            guy = arr[i];
        }
    }
    return people >= max_people;
}

int solution(int *arr,int size, int max_people) {
    int first = 0;
    int last = arr[size-1] - arr[0]+1;
    while (last - first > 1) {
        int mid = (first + last) / 2;
        if(helper(mid,arr,size, max_people)) {
            first = mid;
        }
        else {
            last = mid;
        }
    }
    return first;
}

int main() {
    int n;
    int k;
    cin >> n >> k;
    int arr[n];
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    cout << solution(arr, n, k);
}

