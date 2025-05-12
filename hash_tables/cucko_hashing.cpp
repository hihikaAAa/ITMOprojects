#include <iostream>
#include <vector>

using namespace std;

int HashFunctionHorner(const std::string &s, const int table_size, const int key) {
    unsigned long long hash = 1469598103934665603ULL ^ static_cast<unsigned long long>(key);
    for (const unsigned char c : s) {
        hash ^= c;
        hash *= 1099511628211ULL;
    }
    hash ^= hash >> 33;
    hash *= 0xff51afd7ed558ccdULL;
    hash ^= hash >> 33;
    return static_cast<int>(hash % table_size);
}

struct HashFunction1 {
    int operator()(const std::string &s, const int table_size) const {
        return HashFunctionHorner(s, table_size, table_size - 1);
    }
};

struct HashFunction2 {
    int operator()(const std::string &s, const int table_size) const {
        return HashFunctionHorner(s, table_size, table_size + 1);
    }
};

struct HashNumbHelper {
    string key;
    int mask;

    HashNumbHelper() {
        key = "";
        mask = 0;
    }

    HashNumbHelper(const string &k, const int m) {
        key = k;
        mask = m;
    }
};

class HashTable {
    static constexpr int default_size = 8;
    static constexpr double max_load = 0.75;
    vector<HashNumbHelper*> arr1, arr2;
    int size;
    int capacity;
    HashFunction1 hf1;
    HashFunction2 hf2;

    void Rehash(const int expected) {
        const int old_cap = capacity;
        auto old1 = move(arr1);
        auto old2 = move(arr2);
        capacity = expected;
        arr1.assign(capacity, nullptr);
        arr2.assign(capacity, nullptr);
        size = 0;
        for (int i = 0; i < old_cap; ++i) {
            if (old1[i]) {
                insert(old1[i]->key, old1[i]->mask);
                delete old1[i];
            }
            if (old2[i]) {
                insert(old2[i]->key, old2[i]->mask);
                delete old2[i];
            }
        }
    }

    bool insert(const string &key, const int mask) {
        if (size + 1 > static_cast<int>(max_load * capacity)) {
            Rehash(capacity * 2);
        }
        int i1 = hf1(key, capacity);
        if ( arr1[i1] && arr1[i1]->key == key) {
            arr1[i1]->mask |= mask;
            return true;
        }
        int i2 = hf2(key, capacity);
        if (arr2[i2] && arr2[i2]->key == key) {
            arr2[i2]->mask |= mask;
            return true;
        }
        string cur_key = key;
        int cur_mask = mask;
        for (int kick = 0; kick < 64; ++kick) {
            const int h1 = hf1(cur_key, capacity);
            if (!arr1[h1]) {
                arr1[h1] = new HashNumbHelper(cur_key, cur_mask);
                ++size;
                return true;
            }
            if (arr1[h1]->key == cur_key) {
                arr1[h1]->mask |= cur_mask;
                return true;
            }
            swap(cur_key, arr1[h1]->key);
            swap(cur_mask, arr1[h1]->mask);
            const int h2 = hf2(cur_key, capacity);
            if (!arr2[h2]) {
                arr2[h2] = new HashNumbHelper(cur_key, cur_mask);
                ++size;
                return true;
            }
            if (arr2[h2]->key == cur_key) {
                arr2[h2]->mask |= cur_mask;
                return true;
            }
            swap(cur_key, arr2[h2]->key);
            swap(cur_mask, arr2[h2]->mask);
        }
        Rehash(capacity * 2);
        return insert(cur_key, cur_mask);
    }

public:
    HashTable() {
        size = 0;
        capacity = default_size;
        arr1.assign(capacity,nullptr);
        arr2.assign(capacity, nullptr);
    }
    void Add(const string &key, const int student) {
        insert(key, 1 << student);
    }

    [[nodiscard]] int get(const string &key) const {
        int i1 = hf1(key, capacity);
        if (arr1[i1] && arr1[i1]->key == key)
            return arr1[i1]->mask;
        int i2 = hf2(key, capacity);
        if (arr2[i2] && arr2[i2]->key == key)
            return arr2[i2]->mask;
        return 0;
    }

    [[nodiscard]] vector<HashNumbHelper *> entries() const {
        vector<HashNumbHelper *> enter;
        enter.reserve(size);
        for (int i = 0; i < capacity; i++) {
            if (arr1[i]) {
                enter.push_back(arr1[i]);
            }
            if (arr2[i]) {
                enter.push_back(arr2[i]);
            }

        }
        return enter;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int tasks;
    cin >> tasks;
    HashTable ht;
    string s;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < tasks; j++) {
            cin >> s;
            ht.Add(s, i);
        }
    }
    long long score[3] = {0,0,0};
    for (const auto rec : ht.entries()) {
        const int mask = rec->mask;
        const bool student1 = (mask & 1) != 0;
        const bool student2 = (mask & 2) != 0;
        const bool student3 = (mask & 4) != 0;
        int cnt = (student1 ? 1 : 0) + (student2 ? 1 : 0) + (student3 ? 1 : 0);
        if (cnt == 1) {
            if (student1) {
                score[0] += 3;
            }
            else if (student2) {
                score[1] += 3;
            }
            else
                score[2] += 3;
        }
        else if (cnt == 2) {
            if (student1 && student2) {
                score[0] += 1;
                score[1] += 1;
            }
            else if (student1 && student3) {
                score[0] += 1;
                score[2] += 1;
            }
            else {
                score[1] += 1;
                score[2] += 1;
            }
        }
    }
    cout << score[0] << " " << score[1] << " " << score[2];
    return 0;
}
