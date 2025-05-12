#include <iostream>
#include <vector>
using namespace std;

struct HashHelper {
  int key;
  char used;

  HashHelper() {
    key = -1;
    used = 'f';
  }
};

class HashTable {
private:
  vector<HashHelper> table;
  int table_size;

  [[nodiscard]] int HashFunction(const int key) const {
    return key % table_size;
  }

public:
  explicit HashTable(const int size) {
    table_size = 2*size;
    table.resize(table_size);
    for (int i = 0; i < table_size; i++) {
      table[i].used = 'f';
    }
  }

  [[nodiscard]] bool get(const int key) const {
    const int h = HashFunction(key);
    for (int i = 0; i < table_size; i++) {
      const int pos = (h + i) % table_size;
      if (table[pos].used == 'f')
        break;
      if (table[pos].used == 't' && table[pos].key == key)
        return true;
    }
    return false;
  }

  void insert(const int key) {
    const int h = HashFunction(key);
    for (int i = 0; i < table_size; i++) {
      const int pos = (h + i) % table_size;
      if (table[pos].used == 'f') {
        table[pos].used = 't';
        table[pos].key = key;
        return;
      }
      if (table[pos].used == 't' && table[pos].key == key) {
        return;
      }
    }
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int size;
  cin >> size;
  HashTable table_1(size);
  HashTable table_2(size);
  for (int i = 0; i < size; i++) {
    int curr_h;
    cin >> curr_h;
    table_1.insert(curr_h);
  }
  for (int i = 0; i < size; i++) {
    int curr_h;
    cin >> curr_h;
    if(!table_1.get(curr_h)) {
      cout<< "NO"<<endl;
      return 0;
    }
  }
  cout << "YES"<<endl;
}
