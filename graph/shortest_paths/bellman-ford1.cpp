#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
const int INF = -100000000;

struct Edge {
    int from;
    int to;
    int weight;

    Edge() {
        from = -1;
        to = -1;
        weight = 0;
    }

    explicit Edge(const int f, const int t, const int w) {
        from = f;
        to = t;
        weight = w;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int levels, portals;
    cin >> levels >> portals;
    vector<Edge> edges(portals);
    for (auto &[from,to,weight]: edges) {
        cin >> from >> to >> weight;
    }
    vector dist(levels, INF);
    dist[0] = 0;
    for (int i = 0; i < levels - 1; i++) {
        bool updated = false;
        for (const auto &edge: edges) {
            if (dist[edge.from - 1] != INF && dist[edge.to - 1] < dist[edge.from - 1] + edge.weight) {
                dist[edge.to - 1] = dist[edge.from - 1] + edge.weight;
                updated = true;
            }
        }
        if (!updated) {
            break;
        }
    }
    vector cycle(levels,'f');
    for (int i = 0; i < levels - 1; i++) {
        for (const auto &edge: edges) {
            if (dist[edge.from - 1] != INF && dist[edge.to - 1] < dist[edge.from - 1] + edge.weight) {
                cycle[edge.to-1]='t';
            }
            if(cycle[edge.from-1]=='t') {
                cycle[edge.to-1]='t';
            }
        }
    }
    if(cycle[levels-1]=='t') {
        cout << ":)";
    }
    else if (dist[levels - 1] == INF) {
        cout << ":(";
    } else {
        cout << dist[levels - 1];
    }
}
