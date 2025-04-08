#include <iostream>
#include <vector>
#include <queue>

using namespace std;
constexpr int INF = 100000000;

struct Edge {
    int to;
    int weight;

    Edge() {
        to = -1;
        weight = 0;
    }

    explicit Edge(const int t, const int w) {
        to = t;
        weight = w;
    }
};

struct Vertex {
    int state_id;
    vector<Edge> edgeList;

    Vertex() {
        state_id = -1;
    }

    explicit Vertex(const int s) {
        state_id = s;
    }
};

struct Graph {
    vector<Vertex> vertices;

    explicit Graph(const int n) {
        vertices.resize(n);
        for (int i = 0; i < n; i++) {
            vertices[i].state_id = i + 1;
        }
    }

    void addEdgebyID(const int fromVertex, const int toVertex, const int weight) {
        const int from = fromVertex - 1;
        int to = toVertex - 1;
        vertices[from].edgeList.emplace_back(to, weight);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int cities, roads;
    cin >> cities >> roads;
    Graph g(cities);
    for (int i = 0; i < roads; i++) {
        int from, to, weight;
        cin >> from >> to >> weight;
        g.addEdgebyID(from, to, weight);
        g.addEdgebyID(to, from, weight);
    }
    vector dist(cities, INF);
    vector visited(cities, 'f');
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<> > heap;
    dist[0] = 0;
    heap.emplace(0, 0);
    int answer = 0;
    while (!heap.empty()) {
        auto [cost, u] = heap.top();
        heap.pop();
        if (visited[u] == 't')
            continue;
        visited[u] = 't';
        answer += cost;
        for (const Edge &edge: g.vertices[u].edgeList) {
            int v = edge.to;
            int weight = edge.weight;
            if (visited[v] == 'f' && weight < dist[v]) {
                dist[v] = weight;
                heap.emplace(weight, v);
            }
        }
    }
    cout << answer << endl;
}
