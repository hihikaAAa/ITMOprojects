#include <iostream>
#include <vector>
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
    string status;

    vector<Edge> edgeList;
    vector<Edge> rev_edge_list;

    Vertex() {
        state_id = -1;
        color = "white";
        status = "leaf";
    }

    explicit Vertex(int id) {
        state_id = id;
        color = "white";
        status = "leaf";
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

    void DFS(int indx,int &helper, int parent = -1) {
        Vertex &vertice = vertices[indx];
        vertice.color = "black";
        helper += 1;
        for (auto &edge: vertice.edgeList) {
            if (edge.to == parent)
                continue;
            if (vertices[edge.to].color == "black") {
                cycleFound = true;
            } else {
                DFS(edge.to,helper, indx);
            }
        }
    }
};

int main() {
    int heads, necks;
    cin >> heads >> necks;
    if (heads != necks) {
        cout << "EUCLID\n";
        return 0;
    }
    Graph g(necks);
    for (int i = 0; i < necks; i++) {
        int fromVertex, toVertex;
        cin >> fromVertex >> toVertex;
        g.addEdgebyID(fromVertex, toVertex);
    }
    int helper = 0;
    for (int i = 0; i < heads; i++) {
        if (g.vertices[i].color != "black") {
            g.DFS(i,helper);
            if(helper!=heads || helper<3 || !cycleFound) {
                cout << "EUCLID\n";
                return 0;
            }
        }
    }
    if(cycleFound) {
        cout << "ARCHIMEDES\n";
    }
}
