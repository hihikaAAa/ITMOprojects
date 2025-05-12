#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int to;

    Edge() {
        to = -1;
    }

    explicit Edge(const int destVID = -1) {
        to = destVID;
    }
};

struct Vertex {
    int state_id;

    vector<Edge> edgeList;

    Vertex() {
        state_id = -1;
    }

    explicit Vertex(const int id) {
        state_id = id;
    }
};

struct Graph {
    vector<Vertex> vertices;
    vector<int> parent, rank, parity;

    explicit Graph(const int n) {
        vertices.resize(n);
        parent.resize(n);
        rank.resize(n);
        parity.resize(n);
        for (int i = 0; i < n; i++) {
            vertices[i].state_id = i + 1;
            parent[i] = i;
            rank[i] = 0;
            parity[i] = 0;
        }
    }


    void addEdgebyID(const int fromVertex, const int toVertex) {
        const int from = fromVertex - 1;
        int to = toVertex - 1;
        vertices[from].edgeList.emplace_back(to);
    }

    int findSet(const int v) {
        if (parent[v] == v) {
            return v;
        }
        const int p = parent[v];
        const int root = findSet(p);
        if(parity[v] != parity[p]) {
            parity[v] = 1;
        }
        else {
            parity[v] = 0;
        }
        return parent[v] = root;
    }

    int finder(const int f, const int t) {
        int from = f - 1, to = t - 1;
        int r_from = findSet(from), r_to = findSet(to);
        if (r_from == r_to) {
            if (parity[from] == parity[to]) {
                addEdgebyID(f, t);
                return 0;
            } else {
                addEdgebyID(f, t);
                return 1;
            }
        }
        if (rank[r_from] < rank[r_to]) {
            swap(r_from, r_to);
            swap(from, to);
        }
        parent[r_to] = r_from;
        if (parity[from] == parity[to]) {
            parity[r_to] = 1;
        } else {
            parity[r_to] = 0;
        }
        if (rank[r_from] == rank[r_to]) {
            rank[r_from]++;
        }
        addEdgebyID(f, t);
        return 1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int districts;
    int briges;
    cin >> districts >> briges;
    Graph g(districts);
    bool broken = false;
    string answer;
    for (int i = 0; i < briges; i++) {
        int f, t;
        cin >> f >> t;
        const int current = g.finder(f, t);
        if (!broken) {
            if (current == 0) {
                broken = true;
                answer += '0';
            } else {
                answer += '1';
            }
        } else {
            answer += '0';
        }
    }
    cout << answer;
    return 0;
}
