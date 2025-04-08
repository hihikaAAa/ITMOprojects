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
        vertices.resize(2 * n);
        for (int i = 0; i < n; i++) {
            vertices[i].state_id = i + 1;
        }
    }

    void addEdgebyID(const int fromVertex, const int toVertex, const int weight) {
        const int from = fromVertex;
        int to = toVertex;
        vertices[from].edgeList.emplace_back(to, weight);
        vertices[to].edgeList.emplace_back(from, weight);
    }
};

int main() {
    int galaxies;
    cin >> galaxies;
    int indx = 0, sub_indx = 0;
    Graph g(galaxies);
    for (int i = 0; i < galaxies * galaxies; i++) {
        int current;
        cin >> current;
        if (sub_indx == galaxies) {
            sub_indx = 0;
            indx++;
        }
        if (current != -1 && current != 0) {
            g.addEdgebyID(indx, sub_indx, current);
        }
        sub_indx++;
    }
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<> > heap;
    int exentricitet[galaxies];
    int curr_max = -INF;
    for (int i = 0; i < galaxies; i++) {
        vector dist(galaxies, INF);
        dist[g.vertices[i].state_id - 1] = 0;
        heap.emplace(0, g.vertices[i].state_id - 1);
        while (!heap.empty()) {
            auto [currentDist, vertice] = heap.top();
            heap.pop();
            if (currentDist > dist[vertice])
                continue;
            for (const auto &edge: g.vertices[vertice].edgeList) {
                const int cur_sum = currentDist + edge.weight;
                if (dist[edge.to] > cur_sum) {
                    dist[edge.to] = cur_sum;
                    heap.emplace(dist[edge.to], edge.to);
                }
            }
        }
        for (int j = 0; j < galaxies; j++) {
            curr_max = max(curr_max, dist[j]);
        }
        exentricitet[i] = curr_max;
        curr_max = -INF;
    }
    int min = INF;
    int max = -INF;
    for (int i = 0; i < galaxies; i++) {
        if (exentricitet[i] > max) {
            max = exentricitet[i];
        }
        if (exentricitet[i] < min) {
            min = exentricitet[i];
        }
    }
    cout << max << endl << min << endl;
}
