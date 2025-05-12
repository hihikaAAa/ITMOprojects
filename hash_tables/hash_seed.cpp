#include <iostream>
#include <vector>

using namespace std;
typedef unsigned long long ull;
static constexpr ull FIXED_RANDOM = 123456789ULL;

int deg_2(const int a) {
    return a * 2;
}

struct HashNumbHelper {
    int key;
    char used;
    int count;

    HashNumbHelper() {
        key = -1;
        used = 't';
        count = 1;
    }
};

ull split64(ull x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

class HashTable {
private:
    static constexpr int default_size = 8;
    constexpr static double rehash_size = 0.75;
    HashNumbHelper **arr;
    int size;
    int buffer_size;
    int size_all_non_nullptr;

    [[nodiscard]] int HashFunction(const int key) const {
        const ull h64 = split64(static_cast<ull>(key) + FIXED_RANDOM);
        int h = static_cast<int>(h64 % buffer_size);
        if (h < 0)
            h += buffer_size;
        return h;
    }

public:
    explicit HashTable() {
        buffer_size = default_size;
        size = 0;
        size_all_non_nullptr = 0;
        arr = new HashNumbHelper *[buffer_size];
        for (int i = 0; i < buffer_size; i++) {
            arr[i] = nullptr;
        }
    }

    ~HashTable() {
        for (int i = 0; i < buffer_size; ++i)
            delete arr[i];
        delete[] arr;
    }

    void Resize() {
        const int past_buffer_size = buffer_size;
        buffer_size *= 2;
        size_all_non_nullptr = 0;
        size = 0;
        auto **new_arr = new HashNumbHelper *[buffer_size];
        for (int i = 0; i < buffer_size; i++) {
            new_arr[i] = nullptr;
        }
        std::swap(arr, new_arr);
        for (int i = 0; i < past_buffer_size; i++) {
            if (new_arr[i] && new_arr[i]->used == 't') {
                int k = new_arr[i]->key;
                int c = new_arr[i]->count;
                for (int j = 0; j < c; j++)
                    Add(k);
            }
            delete new_arr[i];
        }
        delete[] new_arr;
    }

    void Rehash() {
        const int past_buffer_size = buffer_size;
        size_all_non_nullptr = 0;
        size = 0;
        auto **new_arr = new HashNumbHelper *[buffer_size];
        for (int i = 0; i < buffer_size; i++) {
            new_arr[i] = nullptr;
        }
        std::swap(arr, new_arr);
        for (int i = 0; i < past_buffer_size; i++) {
            if (new_arr[i] && new_arr[i]->used == 't') {
                int k = new_arr[i]->key;
                int c = new_arr[i]->count;
                for (int j = 0; j < c; j++)
                    Add(k);
            }
            delete new_arr[i];
        }
        delete[] new_arr;
    }

    bool Add(const int key) {
        if (size + 1 > static_cast<int>(rehash_size * buffer_size)) {
            Resize();
        } else if (size_all_non_nullptr > 2 * size) {
            Rehash();
        }
        int h = HashFunction(key);
        int i = 0;
        int first_deleted = -1;
        while (arr[h] != nullptr && i < buffer_size) {
            if (arr[h]->key == key && arr[h]->used == 't') {
                arr[h]->count++;
                return true;
            }
            if (arr[h]->used == 'f' && first_deleted == -1) {
                first_deleted = h;
            }
            h = (h + 1) % buffer_size;
            ++i;
        }
        const int slot = (first_deleted == -1 ? h : first_deleted);
        if (arr[slot] == nullptr) {
            arr[slot] = new HashNumbHelper();
            arr[slot]->key = key;
            ++size_all_non_nullptr;
        }
        arr[slot]->used = 't';
        ++size;
        return true;
    }

    [[nodiscard]] int get(const int key) const {
        const int h = HashFunction(key);
        for (int i = 0; i < buffer_size; i++) {
            const int pos = (h + i) % buffer_size;
            if (arr[pos] == nullptr)
                break;
            if (arr[pos]->used == 't' && arr[pos]->key == key)
                return arr[pos]->count;
        }
        return 0;
    }

};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int first;
    cin >> first;
    vector<int> a(first);
    for (int i = 0; i < first; i++) {
        cin >> a[i];
    }
    int second;
    cin >> second;
    vector<int> b(second);
    for (int i = 0; i < second; i++) {
        cin >> b[i];
    }
    int l = 0;
    int best = min(first, second);
    int answer = 0;
    vector<ull> hA(first), hB(second);
    for (int i = 0; i < first; i++)
        hA[i] = split64(static_cast<ull>(a[i]) + FIXED_RANDOM);
    for (int i = 0; i < second; i++)
        hB[i] = split64(static_cast<ull>(b[i]) + FIXED_RANDOM);
    auto check = [&](const int len) {
        if (len == 0) {
            return true;
        }
        if (len > first || len > second) {
            return false;
        }
        HashTable sigs;
        ull sum = 0;
        for (int i = 0; i < len; i++)
            sum += hA[i];
        sigs.Add(static_cast<int>(sum ^ (sum >> 32)));
        for (int i = len; i < first; i++) {
            sum -= hA[i - len];
            sum += hA[i];
            sigs.Add(static_cast<int>(sum ^ (sum >> 32)));
        }
        sum = 0;
        for (int i = 0; i < len; i++) {
            sum += hB[i];
        }
        if (sigs.get(static_cast<int>(sum ^ (sum >> 32))) > 0) {
            return true;
        }
        for (int i = len; i < second; i++) {
            sum -= hB[i - len];
            sum += hB[i];
            if (sigs.get(static_cast<int>(sum ^ (sum >> 32))) > 0)
                return true;
        }
        return false;
    };
    while (l <= best) {
        int mid = (l + best) / 2;
        if (check(mid)) {
            answer = mid;
            l = mid + 1;
        } else {
            best = mid - 1;
        }
    }
    cout << answer << endl;
    return 0;
}
