#include <iostream>
#include <vector>
#include <queue>
using namespace std;
const int INF = 1e9;

struct Edge {
    int to;
    int weight;

    Edge() {
        to = -1;
        weight = 1;
    }

    explicit Edge(int t) {
        to = t;
        weight = 1;
    }
};

struct Vertex {
    int state_id;
    int total;
    vector<Edge> edgeList;

    Vertex() {
        state_id = -1;
        total = 0;
    }

    explicit Vertex(int s) {
        state_id = s;
        total = 0;
    }
};

struct Graph {
    vector<Vertex> vertices;

    explicit Graph(int n) {
        vertices.resize(n);
        for (int i = 0; i < n; i++) {
            vertices[i].state_id = i + 1;
        }
    }

    void addEdgebyID(int fromVertex, int toVertex) {
        int from = fromVertex - 1;
        int to = toVertex - 1;
        vertices[from].edgeList.emplace_back(to);
        vertices[to].edgeList.emplace_back(from);
    }
};

void bfs(const Graph &g, int start, vector<int> &dist) {
    int n = g.vertices.size();
    queue<int> q;
    vector visited(n, false);
    dist[start] = 0;
    visited[start] = true;
    q.push(start);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto &edge: g.vertices[u].edgeList) {
            int v = edge.to;
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int islands;
    cin >> islands;
    Graph g(islands);
    int ways, days;
    cin >> ways >> days;
    for (int i = 0; i < ways; i++) {
        int fromVertex, toVertex;
        cin >> fromVertex >> toVertex;
        g.addEdgebyID(fromVertex, toVertex);
    }
    vector dist(islands, INF);
    bfs(g,0,dist);
    bool out = false;
    for (int i = 0; i < islands; i++) {
        if (dist[i] == days) {
            cout << i + 1 << '\n';
            out = true;
        }
    }
    if (!out) {
        cout << "NO\n";
    }
    return 0;
}
