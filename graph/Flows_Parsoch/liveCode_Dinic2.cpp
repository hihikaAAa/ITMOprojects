#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct Edge {
    int to, cap, rev;

    Edge(const int t, const int c, const int r) {
        to = t;
        cap = c;
        rev = r;
    }
};

struct Graph {
    vector<vector<Edge> > vertices;
    vector<int> level, ptr;
    int size;

    explicit Graph(const int n) {
        size = n;
        vertices.assign(n, {});
        level.assign(n, 0);
        ptr.assign(n, 0);
    }

    void addEdge(int from, int to, int c) {
        vertices[from].emplace_back(to, c, vertices[to].size());
        vertices[to].emplace_back(from, 0, vertices[from].size() - 1);
    }

    bool bfs(const int s, const int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            const int to = q.front();
            q.pop();
            for (auto &e: vertices[to]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[to] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    int dfs(const int f, const int t, const int pushed) {
        if (f == t || pushed == 0)
            return pushed;
        for (int &cid = ptr[f]; cid < static_cast<int>(vertices[f].size()); ++cid) {
            Edge &e = vertices[f][cid];
            if (e.cap > 0 && level[e.to] == level[f] + 1) {
                int tr = dfs(e.to, t, min(pushed, e.cap));
                if (tr > 0) {
                    e.cap -= tr;
                    vertices[e.to][e.rev].cap += tr;
                    return tr;
                }
            }
        }
        return 0;
    }

    int maxflow(const int s, const int t) {
        int flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (const int pushed = dfs(s, t, INT_MAX)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int rows, collums;
    cin >> rows >> collums;
    vector<string> G(rows);
    for (int i = 0; i < rows; i++) {
        cin >> G[i];
    }
    auto val = [&](const char c) {
        if (c == 'H')
            return 1;
        if (c == 'O')
            return 2;
        if (c == 'N')
            return 3;
        if (c == 'C')
            return 4;
        return 0;
    };
    vector id(rows, vector(collums, -1));
    int V = 0;
    long long sumL = 0, sumR = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < collums; j++) {
            if (G[i][j] != '.') {
                id[i][j] = V++;
                if ((i + j) % 2 == 0)
                    sumL += val(G[i][j]);
                else
                    sumR += val(G[i][j]);
            }
        }
    }
    if (V == 0 || sumL != sumR) {
        cout << "Invalid\n";
        return 0;
    }
    constexpr int dx[4] = {1, -1, 0, 0};
    constexpr int dy[4] = {0, 0, 1, -1};
    const int S = V;
    const int T = V + 1;
    Graph flow(V + 2);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < collums; j++) {
            if (G[i][j] == '.')
                continue;
            const int u = id[i][j];
            const int d = val(G[i][j]);
            if ((i + j) % 2 == 0) {
                flow.addEdge(S, u, d);
                for (int k = 0; k < 4; k++) {
                    int x = i + dx[k], y = j + dy[k];
                    if (x >= 0 && x < rows && y >= 0 && y < collums && G[x][y] != '.') {
                        const int v = id[x][y];
                        flow.addEdge(u, v, 1);
                    }
                }
            } else {
                flow.addEdge(u, T, d);
            }
        }
    }
    int f = flow.maxflow(S, T);
    if (f == sumL) {
        cout << "Valid";
    } else {
        cout << "Invalid";
    }
    return 0;
}
