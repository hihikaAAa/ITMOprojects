#include <iostream>
#include <vector>
#include <string>
using namespace std;

bool cycleFound = false;

struct Edge {
    int to;
    int from;

    Edge() {
        to = -1;
        from = -1;
    }

    explicit Edge(int t = -1, int f = -1) {
        to = t;
        from = f;
    }
};

struct Vertex {
    int state_id;
    string color;
    bool ran_there;
    int out_tact;

    vector<Edge> edgeList;
    vector<Edge> rev_edge_list;

    Vertex() {
        state_id = -1;
        color = "white";
        ran_there = false;
        out_tact = 0;
    }

    explicit Vertex(int id) {
        state_id = id;
        color = "white";
        ran_there = false;
        out_tact = 0;
    }
};

struct Graph {
    vector<Vertex> vertices;

    explicit Graph(int n) {
        vertices.resize(n);
        for (int i = 0; i < n; i++) {
            vertices[i].state_id = i + 1;
            vertices[i].color = "white";
        }
    }

    void addEdgebyID(int fromVertex, int toVertex) {
        int from = fromVertex - 1;
        int to = toVertex - 1;
        vertices[from].edgeList.emplace_back(to, to);
        vertices[to].edgeList.emplace_back(from, from);
    }

    void DFS(int indx,int parent = -1) {
        Vertex &vertice = vertices[indx];
        vertice.color = "black";
        for (auto &edge: vertice.edgeList) {
            if (edge.to == parent)
                continue;
            if (vertices[edge.to].color == "black") {
                cycleFound = true;
            } else {
                DFS(edge.to, indx);
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int houses, roads;
    cin >> houses >> roads;
    Graph g(houses);
    for (int i = 0; i < roads; i++) {
        int fromVertex, toVertex;
        cin >> fromVertex >> toVertex;
        g.addEdgebyID(fromVertex, toVertex);
    }

    for (int i = 0; i < houses; i++) {
        if (g.vertices[i].color != "black") {
            g.DFS(i);
        }
    }
    if(cycleFound) {
        cout << "YES" << endl;
        return 0;
    }
    cout << "NO" << endl;
}
