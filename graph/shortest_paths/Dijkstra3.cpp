#include <iostream>
#include <vector>
#include <queue>

using namespace std;
const int INF = 100000000;

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

    explicit Graph(int n) {
        vertices.resize(n);
        for (int i = 0; i < n; i++) {
            vertices[i].state_id = i + 1;
        }
    }

    void addEdgebyID(const int fromVertex, const int toVertex, const int weight) {
        const int from = fromVertex - 1;
        int to = toVertex - 1;
        vertices[from].edgeList.emplace_back(to, weight);
        vertices[to].edgeList.emplace_back(from, weight);
    }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int cities, roads;
    cin >> cities >> roads;
    Graph g(cities);
    int start, end;
    cin >> start >> end;
    for (int i = 0; i < roads; i++) {
        int from, to, weight;
        cin >> from >> to >> weight;
        g.addEdgebyID(from, to, weight);
    }
    vector dist(cities, INF);
    dist[start - 1] = 0;
    vector visited(cities, 'f');
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> heap;
    heap.emplace(0, start - 1);

    while (!heap.empty()) {
        auto [currentDist, vertice] = heap.top();
        heap.pop();

        if (currentDist > dist[vertice])
            continue;

        for (const auto &edge : g.vertices[vertice].edgeList) {
            if (dist[edge.to] > currentDist + edge.weight) {
                dist[edge.to] = currentDist + edge.weight;
                heap.emplace(dist[edge.to], edge.to);
            }
        }
    }
    int a_indx = end - 1;
    if (dist[a_indx] == INF) {
        cout << "-1";
    } else {
        cout << dist[a_indx] << endl;
    }
    return 0;
}
