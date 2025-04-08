#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int to;

    Edge() {
        to = -1;
    }

    explicit Edge(int destVID = -1) {
        to = destVID;
    }

};

struct Vertex {
    int state_id;
    string color;
    int component_number;

    vector<Edge> edgeList;

    Vertex() {
        state_id = -1;
        color = "white";
        component_number = -1;
    }

    explicit Vertex(int id) {
        state_id = id;
        color = "white";
        component_number = -1;
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

    bool ifEdgeExist(int fromVertex, int toVertex) {
        for (auto &i: vertices[fromVertex].edgeList) {
            if (i.to == toVertex) {
                return true;
            }
        }
        return false;
    }

    void addEdgebyID(int fromVertex, int toVertex) {
        int from = fromVertex - 1;
        int to = toVertex - 1;
        if (!ifEdgeExist(from, to)) {
            vertices[from].edgeList.emplace_back(to);
            vertices[to].edgeList.emplace_back(from);
        }
    }

    void DFS(int indx) {
        Vertex &vertice = vertices[indx];
        vertice.color = "black";
        for (auto &edge: vertice.edgeList) {
            if (vertices[edge.to].color != "black") {
                DFS(edge.to);
            }
        }
    }
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int parts, edges;
    cin >> parts >> edges;
    Graph g(parts);
    int helper = 0;
    for (int i = 0; i < edges; i++) {
        int fromVertex, toVertex;
        cin >> fromVertex >> toVertex;
        g.addEdgebyID(fromVertex, toVertex);
    }
    for (int i = 0; i < parts; i++) {
        if (g.vertices[i].color == "white") {
            g.DFS(i);
            helper++;
        }
    }
    cout << helper << endl;
    return 0;
}
