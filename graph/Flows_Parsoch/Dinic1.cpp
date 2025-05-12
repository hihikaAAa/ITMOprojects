#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef long long ll;
constexpr long long INF = 1e18;

struct Edge {
    int to;
    long long cap;
    int from;
};
struct Discount {
    int a, b;
    long long d;
};
class Dinic {
private:
    vector<vector<Edge>> g;
    vector<int> level, it;

    bool bfs(const int s, const int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            const int u = q.front(); q.pop();
            for (auto &e : g[u]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    long long dfs(const int u, const int t, const ll f) {
        if (u == t) {
            return f;
        }
        for (int &i = it[u]; i < static_cast<int>(g[u].size()); ++i) {
            Edge &e = g[u][i];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                ll ret = dfs(e.to, t, min(f, e.cap));
                if (ret > 0) {
                    e.cap -= ret;
                    g[e.to][e.from].cap += ret;
                    return ret;
                }
            }
        }
        return 0;
    }
public:
    explicit Dinic(const int n) {
        g.resize(n);
        level.resize(n);
        it.resize(n);
    }

    void add_edge(const int u, const int v, const ll c) {
        g[u].push_back({v, c, static_cast<int>(g[v].size())});
        g[v].push_back({u, 0, static_cast<int>(g[u].size()) - 1});
    }

    ll max_flow(const int s, const int t) {
        ll flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            ll f;
            while ((f = dfs(s, t, INF)) > 0) {
                flow += f;
            }
        }
        return flow;
    }

};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M, P;
    cin >> N >> M >> P;
    vector<ll> V(N);
    vector<vector<int>> need(N);
    for (int i = 0; i < N; i++) {
        int K;
        cin >> V[i] >> K;
        need[i].resize(K);
        for (int j = 0; j < K; j++) {
            cin >> need[i][j];
            --need[i][j];
        }
    }
    vector<ll> C(M);
    for (int j = 0; j < M; j++) {
        cin >> C[j];
    }
    vector<Discount> disc(P);
    for (int i = 0; i < P; i++) {
        cin >> disc[i].a >> disc[i].b >> disc[i].d;
        --disc[i].a; --disc[i].b;
    }
    const int s = N + P + M;
    const int t = s + 1;
    Dinic dinic(t + 1);
    ll totalPos = 0;
    for (int i = 0; i < N; i++) {
        dinic.add_edge(s, i, V[i]);
        totalPos += V[i];
        for (const int mj : need[i]) {
            dinic.add_edge(i, N + P + mj, INF);
        }
    }
    for (int k = 0; k < P; k++) {
        const int a = disc[k].a;
        const int b = disc[k].b;
        long long S = (C[a] + C[b] - disc[k].d);
        if (S > 0) {
            const int vk = N + k;
            dinic.add_edge(s, vk, S);
            totalPos += S;
            dinic.add_edge(vk, N + P + a, INF);
            dinic.add_edge(vk, N + P + b, INF);
        }
    }
    for (int j = 0; j < M; j++) {
        dinic.add_edge(N + P + j, t, C[j]);
    }
    const ll flow = dinic.max_flow(s, t);
    cout << totalPos - flow << "\n";
    return 0;
}
