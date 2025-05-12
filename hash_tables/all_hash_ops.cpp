#include <iostream>
#include <vector>

using namespace std;

struct HashStudentHelper {
    int isu;
    char used;
    int grade;

    HashStudentHelper() {
        isu = -1;
        used = 't';
        grade = 0;
    }
};

class HashTable {
private:
    static constexpr int default_size = 8;
    constexpr static double rehash_size = 0.75;
    HashStudentHelper **arr;
    int size;
    int buffer_size;
    int size_all_non_nullptr;
    int mid_grade;
    int max_grade;
    long long total_sum;
    int count;
    [[nodiscard]] int HashFunction(const int key) const {
        return key % buffer_size;
    }

public:
    explicit HashTable() {
        buffer_size = default_size;
        size = 0;
        size_all_non_nullptr = 0;
        mid_grade = 0;
        max_grade = 0;
        total_sum = 0;
        count = 0;
        arr = new HashStudentHelper *[buffer_size];
        for (int i = 0; i < buffer_size; i++) {
            arr[i] = nullptr;
        }
    }

    void Resize() {
        const int past_buffer_size = buffer_size;
        buffer_size *= 2;
        size_all_non_nullptr = 0;
        size = 0;
        mid_grade = 0;
        max_grade = 0;
        total_sum = 0;
        count = 0;
        auto **new_arr = new HashStudentHelper *[buffer_size];
        for (int i = 0; i < buffer_size; i++) {
            new_arr[i] = nullptr;
        }
        std::swap(arr, new_arr);
        for (int i = 0; i < past_buffer_size; i++) {
            if (new_arr[i] && new_arr[i]->used == 't') {
                Add(new_arr[i]->isu, new_arr[i]->grade);
            }
        }
        for (int i = 0; i < past_buffer_size; i++) {
            if (new_arr[i]) {
                delete new_arr[i];
            }
        }
        delete[] new_arr;
    }

    void Rehash() {
        size_all_non_nullptr = 0;
        size = 0;
        mid_grade = 0;
        max_grade = 0;
        total_sum = 0;
        count = 0;
        auto **new_arr = new HashStudentHelper *[buffer_size];
        for (int i = 0; i < buffer_size; i++) {
            new_arr[i] = nullptr;
        }
        std::swap(arr, new_arr);
        for (int i = 0; i < buffer_size; i++) {
            if (new_arr[i] && new_arr[i]->used == 't') {
                Add(new_arr[i]->isu, new_arr[i]->grade);
            }
        }
        for (int i = 0; i < buffer_size; i++) {
            if (new_arr[i]) {
                delete new_arr[i];
            }
        }
        delete[] new_arr;
    }

    bool Add(const int key, const int grade) {
        if (size + 1 > static_cast<int>(rehash_size * buffer_size)) {
            Resize();
        } else if (size_all_non_nullptr > 2 * size) {
            Rehash();
        }
        int h = HashFunction(key);
        int i = 0;
        int first_deleted = -1;
        while (arr[h] != nullptr && i < buffer_size) {
            if (arr[h]->isu == key && arr[h]->used == 't') {
                return false;
            }
            if (arr[h]->used == 'f' && first_deleted == -1) {
                first_deleted = h;
            }
            h = (h + 1) % buffer_size;
            ++i;
        }
        const int pos = (first_deleted == -1 ? h : first_deleted);
        if (first_deleted == -1) {
            arr[h] = new HashStudentHelper();
            arr[h]->isu = key;
        } else {
            arr[first_deleted]->isu = key;
            arr[first_deleted]->used = 't';
        }
        if (grade != -1) {
            arr[pos]->grade = grade;
            total_sum += grade;
            ++count;
            max_grade = max(max_grade, grade);
            mid_grade = static_cast<int>(total_sum / count);
        }
        ++size;
        return true;
    }

    bool Remove(const int key) {
        int h = HashFunction(key);
        int i = 0;
        while (arr[h] != nullptr && i < buffer_size) {
            if (arr[h]->isu == key && arr[h]->used == 't') {
                arr[h]->used = 'f';
                --size;
                const int removed_grade = arr[h]->grade;
                total_sum -= removed_grade;
                --count;
                mid_grade = (count > 0 ? static_cast<int>(total_sum / count) : 0);
                if (removed_grade == max_grade) {
                    max_grade = 0;
                    for (int j = 0; j < buffer_size; ++j) {
                        if (arr[j] && arr[j]->used == 't') {
                            max_grade = max(max_grade, arr[j]->grade);
                        }
                    }
                }
                return true;
            }
            h = (h + 1) % buffer_size;
            ++i;
        }
        return false;
    }

    [[nodiscard]] int get_max_grade() const {
        return max_grade;
    }

    [[nodiscard]] int get_mid_grade() const {
        return mid_grade;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int groups, requests;
    cin >> groups >> requests;
    vector<HashTable> tables(groups);
    for (int i = 0; i < requests; i++) {
        char line;
        cin >> line;
        switch (line) {
            case '+': {
                int group, isu, points;
                cin >> group >> isu >> points;
                tables[group - 1].Add(isu, points);
                break;
            }
            case '-': {
                int group, isu;
                cin >> group >> isu;
                tables[group - 1].Remove(isu);
                break;
            }
            case 'm': {
                int group;
                cin >> group;
                cout << tables[group - 1].get_max_grade() << '\n';
                break;
            }
            case 'a': {
                int group;
                cin >> group;
                cout << tables[group - 1].get_mid_grade() << '\n';
                break;
            }
            default:
                break;
        }
    }
    return 0;
}
