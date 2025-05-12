#include <iostream>
#include <vector>

using namespace std;

using namespace std;

int deg_2(const int a) {
    return a * 2;
}

struct Node {
    int x;
    int y;

    Node() {
        x = -1;
        y = -1;
    }

    Node(const int _x, const int _y) {
        x = _x;
        y = _y;
    }
};

struct Entry {
    int key;
    int val;
    char status;
};

class MapInt {
    Entry *arr;
    int cap;

public:
    explicit MapInt(const int expected) {
        cap = 1;
        while (cap < expected * 2) {
            cap = deg_2(cap);
        }
        arr = new Entry[cap];
        for (int i = 0; i < cap; ++i) {
            arr[i].status = 'u';
        }
    }

    ~MapInt() {
        delete[] arr;
    }

    [[nodiscard]] int hash1(const int key) const {
        int h = key & (cap - 1);
        if (h < 0) {
            h += cap;
        }
        return h;
    }

    [[nodiscard]] int hash2(const int key) const {
        int h = (key % (cap - 1));
        if (h < 0) {
            h += (cap - 1);
        }
        return h + 1;
    }

    void put(const int key, const int value) const {
        const int h1 = hash1(key);
        const int h2 = hash2(key);
        int index = h1;
        int firstTombstone = -1;
        for (int i = 0; i < cap; ++i) {
            if (arr[index].status == 'u') {
                const int target = (firstTombstone != -1 ? firstTombstone : index);
                arr[target].key = key;
                arr[target].val = value;
                arr[target].status = 'f';
                return;
            }
            if (arr[index].status == 'd') {
                if (firstTombstone == -1) {
                    firstTombstone = index;
                }
            } else if (arr[index].status == 'f' && arr[index].key == key) {
                arr[index].val = value;
                return;
            }
            index = (index + h2) & (cap - 1);
        }
    }

    [[nodiscard]] int get(const int key) const {
        const int h1 = hash1(key);
        const int h2 = hash2(key);
        int index = h1;
        for (int i = 0; i < cap; ++i) {
            if (arr[index].status == 'u')
                return -1;
            if (arr[index].status == 'f' && arr[index].key == key)
                return arr[index].val;
            index = (index + h2) & (cap - 1);
        }
        return -1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int size;
    cin >> size;
    if (size <= 3) {
        cout << "YES";
        return 0;
    }
    vector<Node> nails(size);
    for (int i = 0; i < size; ++i) {
        cin >> nails[i].x >> nails[i].y;
    }
    vector<int> nextX(size, -1), nextY(size, -1);
    const MapInt mapX(size);
    const MapInt mapY(size);
    for (int i = 0; i < size; ++i) {
        const int x = nails[i].x;
        int prev = mapX.get(x);
        nextX[i] = prev;
        mapX.put(x, i);
        const int y = nails[i].y;
        prev = mapY.get(y);
        nextY[i] = prev;
        mapY.put(y, i);
    }
    vector<char> active(size, 1);
    bool ok = false;
    for (int i = 0; i < 4 && i < size && !ok; ++i) {
        for (int j = i + 1; j < 4 && j < size && !ok; ++j) {
            for (int d1 = 0; d1 < 2 && !ok; ++d1) {
                vector<int> rem1;
                const int head1 = (d1 == 0 ? mapX.get(nails[i].x)
                                       : mapY.get(nails[i].y));
                for (int idx = head1; idx != -1; idx = (d1==0 ? nextX[idx] : nextY[idx])) {
                    if (active[idx]) {
                        active[idx] = 0;
                        rem1.push_back(idx);
                    }
                }
                for (int d2 = 0; d2 < 2 && !ok; ++d2) {
                    vector<int> rem2;
                    const int head2 = (d2 == 0 ? mapX.get(nails[j].x)
                                           : mapY.get(nails[j].y));
                    for (int idx = head2; idx != -1; idx = (d2==0 ? nextX[idx] : nextY[idx])) {
                        if (active[idx]) {
                            active[idx] = 0;
                            rem2.push_back(idx);
                        }
                    }
                    int fx = -1, fy = -1;
                    for (int t = 0; t < size; ++t) {
                        if (active[t]) {
                            fx = nails[t].x;
                            fy = nails[t].y;
                            break; }
                    }
                    bool good = true;
                    if (fx != -1) {
                        for (int t = 0; t < size; ++t) {
                            if (active[t] &&
                               nails[t].x != fx && nails[t].y != fy) {
                                good = false;
                                break;
                            }
                        }
                    }
                    if (good) {
                        ok = true;
                    }
                    for (const int index : rem2) {
                        active[index] = 1;
                    }
                }
                for (const int index : rem1) {
                    active[index] = 1;
                }
            }
        }
    }
    if(ok) {
        cout << "YES";
        return 0;
    }
    cout << "NO";
}
