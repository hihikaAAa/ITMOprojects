#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

constexpr double INF = 1e9;

int deg_2(const int a) {
    return a * a;
}

void init_graph(vector<vector<double> > &graph) {
    for (auto &row: graph) {
        for (double &cell: row) {
            cell = 0;
        }
    }
}

struct Node {
    int m_x;
    int m_y;
    int parent;

    Node() {
        m_x = 0;
        m_y = 0;
        parent = 0;
    }

    explicit Node(const int x, const int y) {
        m_x = x;
        m_y = y;
        parent = -1;
    }
};

int main() {
    int cities;
    cin >> cities;
    vector g(cities, vector<double>(cities));
    init_graph(g);
    vector<Node> coords;
    for (int i = 0; i < cities; i++) {
        int x, y;
        cin >> x >> y;
        coords.emplace_back(x, y);
    }
    for (int i = 0; i < cities; i++) {
        for (int j = i + 1; j < cities; j++) {
            const Node node1 = coords[i];
            const Node node2 = coords[j];
            g[i][j] = sqrt(deg_2(node2.m_x - node1.m_x) + deg_2(node2.m_y - node1.m_y));
            g[j][i] = g[i][j];
        }
    }
    vector visited(cities, 'f');
    vector dist(cities, INF);
    dist[0] = 0.0;
    for (int i = 0; i < cities; i++) {
        int curr = -1;
        double min = INF;
        for (int j = 0; j < cities; j++) {
            if (visited[j] == 'f' && dist[j] < min) {
                min = dist[j];
                curr = j;
            }
        }
        visited[curr] = 't';
        for (int v = 0; v < cities; v++) {
            if (visited[v] == 'f' && g[curr][v] < dist[v]) {
                dist[v] = g[curr][v];
                coords[v].parent = curr;
            }
        }
    }
    double totalCost = 0.0;
    for (int i = 0; i < cities; i++) {
        totalCost += dist[i];
    }
    printf("%.6f\n", totalCost);
    cout << cities - 1 << endl;
    for (int i = 0; i < cities; i++) {
        if (coords[i].parent != -1) {
            cout << coords[i].parent + 1 << ' ' << i + 1 << endl;
        }
    }
    return 0;
}
