#include <iostream>
#include <vector>

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
    int total;
    vector<Edge> edgeList;

    Vertex() {
        state_id = -1;
        total = 0;
    }

    explicit Vertex(const int s) {
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

    void addEdgebyID(const int fromVertex, const int toVertex, const int weight) {
        const int from = fromVertex;
        int to = toVertex;
        vertices[from].edgeList.emplace_back(to, weight);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int districts, start, end;
    cin >> districts >> start >> end;
    Graph g(districts);
    int indx = 0, sub_indx = 0;
    for (int i = 0; i < districts * districts; i++) {
        int current;
        cin >> current;
        if (sub_indx == districts) {
            sub_indx = 0;
            indx++;
        }
        if (current != -1 && current != 0) {
            g.addEdgebyID(indx, sub_indx, current);
        }
        sub_indx++;
    }
    vector dist(districts, INF);
    dist[start-1] = 0;
    vector visited(districts, 'f');
    for (int i = 0; i < districts; i++) {
        int nearest = -1;
        for (int j = 0; j < districts; j++) {
            if (visited[j] == 'f' && (nearest == -1 || dist[nearest] > dist[j])) {
                nearest = j;
            }
        }
        if (dist[nearest] == INF) {
            break;
        }
        visited[nearest] = 't';
        for (auto &edge: g.vertices[nearest].edgeList) {
            if (dist[edge.to] > dist[nearest] + edge.weight) {
                dist[edge.to] = dist[nearest] + edge.weight;
            }
        }
    }
    if (dist[end - 1] == INF) {
        cout << "-1";
    } else {
        cout << dist[end - 1];
    }
    return 0;
}
