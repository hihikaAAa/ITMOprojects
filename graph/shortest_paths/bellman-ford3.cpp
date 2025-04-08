#include <iostream>
#include <vector>

using namespace std;
constexpr double INF = -1e18;

struct Edge {
    int from;
    int to;
    double course;
    double commition;

    Edge() {
        from = -1;
        to = -1;
        course = 0;
        commition = 0;
    }

    explicit Edge(const int f, const int t, const double c, const double com) {
        from = f;
        to = t;
        course = c;
        commition = com;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int vallets, spots, currency;
    double sum;
    cin >> vallets >> spots >> currency >> sum;
    vector<Edge> edges(2 * spots);
    for (int i = 0; i < spots; i++) {
        int a, b;
        double r_a, c_a, r_b, c_b;
        cin >> a >> b >> r_a >> c_a >> r_b >> c_b;
        edges[2 * i] = Edge(a, b, r_a, c_a);
        edges[2 * i + 1] = Edge(b, a, r_b, c_b);
    }
    vector dist(vallets, INF);
    dist[currency - 1] = sum;
    for (int i = 0; i < vallets - 1; i++) {
        bool updated = false;
        for (const auto &edge: edges) {
            if (dist[edge.from - 1] == INF) {
                continue;
            }
            const double cur_sum = (dist[edge.from - 1] - edge.commition) * edge.course;
            if (cur_sum > dist[edge.to - 1]) {
                dist[edge.to - 1] = cur_sum;
                updated = true;
            }
        }
        if (!updated) {
            break;
        }
    }
    bool cycleFound = false;
    for (int i = 0; i < vallets - 1; i++) {
        if (!cycleFound) {
            for (const auto &edge: edges) {
                if (dist[edge.from - 1] == INF) {
                    continue;
                }
                const double cur_sum = (dist[edge.from - 1] - edge.commition) * edge.course;
                if (cur_sum > dist[edge.to - 1]) {
                    cycleFound = true;
                    break;
                }
            }
        }
    }
    if (cycleFound) {
        cout << "YES" << endl;
        return 0;
    }
    cout << "NO" << endl;
}
