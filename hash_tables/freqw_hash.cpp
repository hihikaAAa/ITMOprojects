#include <iostream>
#include <vector>

using namespace std;

int deg_2(const int a) {
    return a * 2;
}

struct HashChairHelper {
    int height;
    char used;
    long long freq;

    HashChairHelper() {
        height = -1;
        used = 't';
        freq = 0;
    }
};

class HashTable {
private:
    static constexpr int default_size = 8;
    constexpr static double rehash_size = 0.75;
    HashChairHelper **arr;
    int size;
    int buffer_size;
    int size_all_non_nullptr;

    [[nodiscard]] int HashFunction(const int key) const {
        int h = key % buffer_size;
        if (h < 0) {
            h += buffer_size;
        }
        return h;
    }

public:
    explicit HashTable(const int expected) {
        buffer_size = 1;
        while (buffer_size < expected * 2) {
            buffer_size = deg_2(buffer_size);
        }
        size = size_all_non_nullptr = 0;
        arr = new HashChairHelper*[buffer_size];
        for (int i = 0; i < buffer_size; i++) {
            arr[i] = nullptr;
        }
    }

    bool Add(const int key) {
        int h = HashFunction(key);
        int i = 0;
        int first_deleted = -1;
        while (arr[h] != nullptr && i < buffer_size) {
            if (arr[h]->height == key && arr[h]->used == 't') {
                arr[h]->freq++;
                return true;
            }
            if (arr[h]->used == 'f' && first_deleted == -1) {
                first_deleted = h;
            }
            h = (h + 1) % buffer_size;
            ++i;
        }
        const int pos = (first_deleted == -1 ? h : first_deleted);
        if (arr[pos] == nullptr) {
            arr[pos] = new HashChairHelper();
            arr[pos]->height = key;
            size_all_non_nullptr++;
        }
        arr[pos]->used = 't';
        arr[pos]->freq = 1;
        ++size;
        return true;
    }

    [[nodiscard]] long long GetFreq(const int key) const {
        int h = HashFunction(key);
        int i = 0;
        while (arr[h] != nullptr && i < buffer_size) {
            if (arr[h]->height == key && arr[h]->used == 't') {
                return arr[h]->freq;
            }
            h = (h + 1) % buffer_size;
            ++i;
        }
        return 0;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int chairs_count;
    cin >> chairs_count;
    vector<long long> chairs(chairs_count);
    for (int i = 0; i < chairs_count; i++) {
        cin >> chairs[i];
    }
    HashTable table(chairs_count);
    long long answer = 0;
    for (int i = 0; i < chairs_count; i++) {
        const long long key = chairs[i] - i;
        const long long prevCount = table.GetFreq(key);
        answer += prevCount;
        table.Add(key);
    }
    cout << answer << "\n";
    return 0;
}
