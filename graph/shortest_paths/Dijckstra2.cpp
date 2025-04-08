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
    int parent;
    vector<Edge> edgeList;

    Vertex() {
        state_id = -1;
        parent = -1;
    }

    explicit Vertex(const int s) {
        state_id = s;
        parent = -1;
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

void parent_search(vector<Vertex> &list, int indx) {
    if (list[indx].parent != -1) {
        parent_search(list, list[indx].parent);
    }
    cout <<list[indx].state_id << " ";
}

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
    for (int i = 0; i < cities; i++) {
        int nearest = -1;
        for (int j = 0; j < cities; j++) {
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
                g.vertices[edge.to].parent = nearest;
            }
        }
    }
    int a_indx = end - 1;
    if (dist[a_indx] == INF) {
        cout << "-1";
    } else {
        cout << dist[a_indx] << endl;
        parent_search(g.vertices,a_indx);
    }
    return 0;
}
