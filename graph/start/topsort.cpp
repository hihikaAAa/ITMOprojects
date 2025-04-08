#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int DestinationVertexID;

    Edge() {
        DestinationVertexID = -1;
    }

    explicit Edge(int destVID) {
        DestinationVertexID = destVID;
    }

};

struct Vertex {
    int state_id;
    string color;

    vector<Edge> edgeList;

    Vertex() {
        state_id = -1;
        color = "white";
    }

    explicit Vertex(int id) {
        state_id = id;
        color = "white";
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
        vertices[from].edgeList.emplace_back(to);
    }

    void DFS(int indx, vector<int> &answer, int &max_indx) {
        Vertex &vertice = vertices[indx];
        if (vertice.color == "white") {
            vertice.color = "black";
            for (auto &edge: vertice.edgeList) {
                if (vertices[edge.DestinationVertexID].color != "black") {
                    DFS(edge.DestinationVertexID, answer, max_indx);
                }
            }
            answer[vertice.state_id - 1] = max_indx;
            max_indx -= 1;
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int gifts;
    int pairs;
    cin >> gifts >> pairs;
    vector<int> answer(gifts, 0);
    int max_indx = gifts;
    Graph graph(gifts);
    for (int i = 0; i < pairs; i++) {
        int fromVertex, toVertex;
        cin >> fromVertex >> toVertex;
        graph.addEdgebyID(fromVertex, toVertex);
    }
    for (int i = 0; i < gifts; i++) {
        if (graph.vertices[i].color == "white") {
            graph.DFS(i, answer, max_indx);
        };
    }
    for (int i = 0; i < gifts; i++) {
        cout << answer[i] << " ";
    }
    return 0;
}
