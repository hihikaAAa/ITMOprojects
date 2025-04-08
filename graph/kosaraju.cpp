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
    int out_tact;
    int component_number;

    vector<Edge> edgeList;
    vector<Edge> rev_edge_list;

    Vertex() {
        state_id = -1;
        color = "white";
        out_tact = 0;
        component_number = -1;
    }

    explicit Vertex(int id) {
        state_id = id;
        color = "white";
        out_tact = 0;
        component_number = -1;
    }
};

struct Graph {
    vector<Vertex> vertices;
    vector<Vertex> order;
    vector<Vertex> components;

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
        vertices[to].rev_edge_list.emplace_back(from);
    }

    void DFS(int indx, int &tact) {
        Vertex &vertice = vertices[indx];
        vertice.color = "black";
        tact += 1;
        vertice.out_tact = tact;
        for (auto &edge: vertice.edgeList) {
            if (vertices[edge.to].color != "black") {
                DFS(edge.to, tact);
                tact += 1;
                vertice.out_tact = tact;
            }
        }
        order.push_back(vertice);
    }

    void inv_DFS(int indx, int &helper) {
        Vertex &vertice = vertices[indx];
        vertice.color = "white";
        for (auto &edge: vertice.rev_edge_list) {
            if (vertices[edge.to].color != "white") {
                inv_DFS(edge.to, helper);
            }
        }
        vertice.component_number = helper;
        components.push_back(vertice);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int grapes;
    int branches;
    int requests;
    cin >> grapes >> branches >> requests;
    Graph g(grapes);
    int mark1 = -1;
    int mark2 = -1;
    int fromVertex;
    int toVertex;
    for (int i = 0; i < branches; i++) {
        cin >> fromVertex >> toVertex;
        g.addEdgebyID(fromVertex, toVertex);
    }
    int tact = 0;
    for (int i = 0; i < grapes; i++) {
        if (g.vertices[i].color != "black") {
            g.DFS(i, tact);
        }
    }

    int helper = 0;
    for (int i = 0; i < (int)g.order.size(); i++) {
        int indx = g.order[g.order.size() - 1 - i].state_id-1;
        if (g.vertices[indx].color == "black") {
            g.inv_DFS(indx, helper);
            helper++;
        }
    }
    for(int i = 0; i < requests; i++) {
        cin >> fromVertex >> toVertex;
        mark1 = g.vertices[fromVertex - 1].component_number;
        mark2 = g.vertices[toVertex - 1].component_number;
        if (mark1 == mark2) {
            cout << "YES"<<endl;
        } else {
            cout << "NO" << endl;
        }
    }
}
