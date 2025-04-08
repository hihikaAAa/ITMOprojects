#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

constexpr double INF = 1e9;

int deg_2(const int a) {
    return a * a;
}

struct Node {
    int m_x;
    int m_y;


    Node() {
        m_x = 0;
        m_y = 0;
    }

    explicit Node(const int x, const int y) {
        m_x = x;
        m_y = y;
    }
};

int main() {
    int cities;
    cin >> cities;
    vector<Node> coords;
    for (int i = 0; i < cities; i++) {
        int x, y;
        cin >> x >> y;
        coords.emplace_back(x, y);
    }
    vector visited(cities, 'f');
    vector dist(cities, INF);
    dist[0] = 0.0;
    for (int i = 0; i < cities; i++) {
        int curr = -1;
        double min = INF;
        for (int j = 0; j < cities; j++) {
            if (visited[j]=='f' && dist[j] < min) {
                min = dist[j];
                curr = j;
            }
        }
        visited[curr] = 't';
        for (int v = 0; v < cities; v++) {
            if (visited[v]=='f') {
                const Node node1 = coords[curr];
                const Node node2 = coords[v];
                double d = sqrt(deg_2(node2.m_x - node1.m_x) + deg_2(node2.m_y - node1.m_y));
                if (d < dist[v]) {
                    dist[v] = d;
                }
            }
        }
    }
    double totalCost = 0.0;
    for (int i = 0; i < cities; i++) {
        totalCost += dist[i];
    }
    printf("%.6f\n", totalCost);
    return 0;
}
