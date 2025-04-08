#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int to;

    Edge() {
        to = -1;
    }

    explicit Edge(int t = -1) {
        to = t;
    }
};

struct Vertex {
    int state_id;
    string color;
    int out_tact;
    int scc_number;

    vector<Edge> edgeList;
    vector<Edge> rev_edge_list;

    Vertex() {
        state_id = -1;
        color = "white";
        out_tact = 0;
    }

    explicit Vertex(int id) {
        state_id = id;
        color = "white";
        out_tact = 0;
    }
};

struct Graph {
    vector<Vertex> vertices;
    vector<int> order;

    explicit Graph(int n) {
        vertices.resize(n);
        int indx = 0;
        for (int i = 0; i < n / 2; i++) {
            vertices[indx].state_id = i + 1;
            vertices[indx].color = "white";
            indx++;
        }
        for (int i = 0; i < n / 2; i++) {
            vertices[indx].state_id = -(i + 1);
            vertices[indx].color = "white";
            indx++;
        }
    }

    int indexFromID(int x) {
        int n = (int) vertices.size();
        int half = n / 2;
        if (x > 0) {
            return x - 1;
        } else {
            return (abs(x) - 1) + half;
        }
    }

    void addEdgebyID(int fromVertex, int toVertex) {
        int from = indexFromID(fromVertex);
        int to = indexFromID(toVertex);
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
        order.push_back(indx);
    }

    void inv_DFS(int indx, int &scc_indx) {
        Vertex &vertice = vertices[indx];
        vertice.color = "white";
        vertice.scc_number = scc_indx;
        for (auto &edge: vertice.rev_edge_list) {
            if (vertices[edge.to].color != "white") {
                inv_DFS(edge.to, scc_indx);
            }
        }
    }
};

int main() {
    int ingredients, conditions;
    cin >> ingredients >> conditions;
    Graph g(2 * ingredients);
    bool potion = true;
    for (int i = 0; i < conditions; i++) {
        int first = 0, second = 0;
        string operation;
        cin >> operation;
        if (operation[0] != '!') {
            int helping_indx = operation.length() - 1;
            int degree = 1;
            while (helping_indx != -1) {
                first += (operation[helping_indx] - 48) * degree;
                helping_indx--;
                degree *= 10;
            }
            cin >> operation;
            cin >> second;
        } else {
            size_t helping_indx = operation.length() - 1;
            int degree = 1;
            while (operation[helping_indx] != '!') {
                first += (operation[helping_indx] - 48) * degree;
                degree *= 10;
                helping_indx--;
            }
            operation = "!";
        }
        if (second == 0) {
            g.addEdgebyID(first, -first);
        } else {
            switch (operation[0]) {
                case '|':
                    g.addEdgebyID(-first, second);
                    g.addEdgebyID(-second, first);
                    break;
                case '-':
                    g.addEdgebyID(first, second);
                    g.addEdgebyID(-second, -first);
                    break;
                case '&':
                    g.addEdgebyID(-first, first);
                    g.addEdgebyID(-second, second);
                    break;
                case '^':
                    g.addEdgebyID(-first, second);
                    g.addEdgebyID(-second, first);
                    g.addEdgebyID(first, -second);
                    g.addEdgebyID(second, -first);
                    break;
                default:
                    break;
            }
        }
    }
    int tact = 0;
    for (int i = 0; i < 2 * ingredients; i++) {
        if (g.vertices[i].color != "black") {
            g.DFS(i, tact);
        }
    }
    int scc_count = 0;
    for (int i = (int)g.order.size() - 1; i >= 0; i--) {
        int indx = g.order[i];
        if (g.vertices[indx].color == "black") {
            g.inv_DFS(indx, scc_count);
            scc_count++;
        }
    }
    for (int i = 0; i < ingredients; i++) {
        if (g.vertices[i].scc_number == g.vertices[i + ingredients].scc_number) {
            potion = false;
            break;
        }
    }
    if (potion) {
        cout << "YES" << endl;
        return 0;
    }
    cout << "NO" << endl;
}
