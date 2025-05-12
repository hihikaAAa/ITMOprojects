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
                Edge &e = adj[u][i];
                if (parent[e.to] == -1 && e.cap > 0) {
                    parent[e.to] = u;
                    parentEdge[e.to] = i;
                    ll new_flow = min(flow, e.cap);
                    if (e.to == t) {
                        return new_flow;
                    }
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
    int n;
    cin >> n;
    vector<ll> wins(n), rem(n);
    for (int i = 0; i < n; i++) cin >> wins[i];
    for (int i = 0; i < n; i++) cin >> rem[i];
    vector mat(n, vector<ll>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> mat[i][j];
    const ll maxWins1 = wins[0] + rem[0];
    for (int i = 1; i < n; i++) {
        if (wins[i] > maxWins1) {
            cout << "NO\n";
            return 0;
        }
    }
    int matchCount = 0;
    vector matchNode(n, vector(n, -1));
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (i == 0 || j == 0) {
                continue;
            }
            if (mat[i][j] > 0)
                matchNode[i][j] = matchCount++;
        }
    }
    constexpr int S = 0;
    constexpr int matchStart = 1;
    const int teamStart = matchStart + matchCount;
    const int T = teamStart + n;
    MaxFlow mf(T + 1);
    ll totalMatches = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (i == 0 || j == 0) {
                continue;
            }
            ll cnt = mat[i][j];
            if (cnt > 0) {
                int mnode = matchStart + matchNode[i][j];
                mf.addEdge(S, mnode, cnt);
                totalMatches += cnt;
                mf.addEdge(mnode, teamStart + i, cnt);
                mf.addEdge(mnode, teamStart + j, cnt);
            }
        }
    }
    for (int i = 1; i < n; i++) {
        const ll cap = maxWins1 - wins[i];
        mf.addEdge(teamStart + i, T, cap);
    }
    const ll flow = mf.maxflow(S, T);
    cout << (flow == totalMatches ? "YES\n" : "NO\n");
    return 0;
}
