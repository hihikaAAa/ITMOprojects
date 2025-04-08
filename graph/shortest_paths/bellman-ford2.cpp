#include <iostream>
#include <vector>

using namespace std;
const long long INF = 1000000000000000000LL;

struct Edge {
    int from;
    int to;
    long long weight;

    Edge() {
        from = -1;
        to = -1;
        weight = 0;
    }

    explicit Edge(const int f, const int t, const long long w) {
        from = f;
        to = t;
        weight = w;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int systems, tunnels, start;
    cin >> systems >> tunnels >> start;
    vector<Edge> edges(tunnels);
    for (auto &[from,to,weight]: edges) {
        cin >> from >> to >> weight;
    }
    vector dist(systems, INF);
    dist[start - 1] = 0;
    for (int i = 0; i < systems - 1; i++) {
        bool updated = false;
        for (const auto &edge: edges) {
            if (dist[edge.from - 1] == INF) {
                continue;
            }
            const long long cur_weight = dist[edge.from - 1] + edge.weight;
            if (dist[edge.to - 1] > cur_weight) {
                dist[edge.to - 1] = cur_weight;
                updated = true;
            }
        }
        if (!updated) {
            break;
        }
    }
    vector cycle(systems, 'f');
    for (int i = 0; i < systems - 1; i++) {
        for (const auto &edge: edges) {
            if (dist[edge.from - 1] == INF) {
                continue;
            }
            const long long cur_weight = dist[edge.from - 1] + edge.weight;
            if (dist[edge.to - 1] > cur_weight) {
                cycle[edge.to - 1] = 't';
            }
            if (cycle[edge.from - 1] == 't') {
                cycle[edge.to - 1] = 't';
            }
        }
    }
    for (int i = 0; i < systems; i++) {
        if (cycle[i] == 't') {
            cout << "-\n";
        } else {
            if (dist[i] == INF) {
                cout << "*\n";
            } else {
                cout << dist[i] << "\n";
            }
        }
    }
}
