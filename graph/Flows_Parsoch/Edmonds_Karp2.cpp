#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;
typedef long long ll;

struct Edge {
    int to;
    ll cap;
    int from;
};

class MaxFlow {
    int N;
    vector<vector<Edge>> adj;
public:
    explicit MaxFlow(const int n){
        N = n;
        adj.resize(N);
    }

    void addEdge(const int u, const int v, const ll cap) {
        adj[u].push_back({v, cap, static_cast<int>(adj[v].size())});
        adj[v].push_back({u, 0,   static_cast<int>(adj[u].size())-1});
    }

    ll bfs(int s, const int t, vector<int>& parent, vector<int>& parentEdge) {
        fill(parent.begin(), parent.end(), -1);
        queue<pair<int,ll>> q;
        q.emplace(s, LLONG_MAX);
        parent[s] = -2;
        while (!q.empty()) {
            const int u = q.front().first;
            ll flow = q.front().second;
            q.pop();
            for (int i = 0; i < adj[u].size(); i++) {
                auto &e = adj[u][i];
                if (parent[e.to] == -1 && e.cap > 0) {
                    parent[e.to] = u;
                    parentEdge[e.to] = i;
                    ll new_flow = min(flow, e.cap);
                    if (e.to == t)
                        return new_flow;
                    q.emplace(e.to, new_flow);
                }
            }
        }
        return 0;
    }

    ll maxflow(const int s, const int t) {
        ll flow = 0, new_flow;
        vector<int> parent(N), parentEdge(N);
        while ((new_flow = bfs(s, t, parent, parentEdge))) {
            flow += new_flow;
            int cur = t;
            while (cur != s) {
                const int p = parent[cur];
                const int ei = parentEdge[cur];
                adj[p][ei].cap -= new_flow;
                adj[cur][adj[p][ei].from].cap += new_flow;
                cur = p;
            }
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M;
    ll w, b, g;
    cin >> N >> M >> w >> b >> g;
    vector<string> grid(N);
    for (int i = 0; i < N; i++)
        cin >> grid[i];
    const int V = N * M;
    const int S = V;
    const int T = V + 1;
    MaxFlow mf(V + 2);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            const int v = i * M + j;
            if (grid[i][j] == 'W') {
                mf.addEdge(S, v, 0);
                mf.addEdge(v, T, b);
            } else {
                mf.addEdge(S, v, w);
                mf.addEdge(v, T, 0);
            }
        }
    }
    constexpr int dx[2] = {1, 0};
    constexpr int dy[2] = {0, 1};
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            const int u = i * M + j;
            for (int d = 0; d < 2; d++) {
                const int ni = i + dx[d];
                const int nj = j + dy[d];
                if (ni < 0 || ni >= N || nj < 0 || nj >= M) {
                    continue;
                }
                const int v = ni * M + nj;
                mf.addEdge(u, v, g);
                mf.addEdge(v, u, g);
            }
        }
    }
    const ll answer = mf.maxflow(S, T);
    cout << answer << "\n";
    return 0;
}
