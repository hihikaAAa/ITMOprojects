#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int n1, n2, m;
vector<vector<int> > adjU, adjV;
vector<int> matchU, matchV;
vector<char> used;

bool kuhn(const int u) {
    for (const int v: adjU[u]) {
        if (used[v]) {
            continue;
        }
        used[v] = 1;
        if (matchV[v] == -1 || kuhn(matchV[v])) {
            matchU[u] = v;
            matchV[v] = u;
            return true;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n1 >> n2 >> m;
    adjU.assign(n1, {});
    adjV.assign(n2, {});
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        --u;
        --v;
        adjU[u].push_back(v);
        adjV[v].push_back(u);
    }
    matchU.assign(n1, -1);
    matchV.assign(n2, -1);
    for (int u = 0; u < n1; u++) {
        used.assign(n2, 0);
        kuhn(u);
    }
    vector<char> visU1(n1, 0), visV1(n2, 0); {
        queue<pair<int, int> > q;
        for (int u = 0; u < n1; u++) {
            if (matchU[u] == -1) {
                visU1[u] = 1;
                q.emplace(0, u);
            }
        }
        while (!q.empty()) {
            auto [side, x] = q.front();
            q.pop();
            if (side == 0) {
                for (int v: adjU[x]) {
                    if (matchU[x] == v) {
                        continue;
                    }
                    if (!visV1[v]) {
                        visV1[v] = 1;
                        q.emplace(1, v);
                    }
                }
            } else {
                int u = matchV[x];
                if (u != -1 && !visU1[u]) {
                    visU1[u] = 1;
                    q.emplace(0, u);
                }
            }
        }
    }
    vector<char> visU2(n1, 0), visV2(n2, 0); {
        queue<pair<int, int> > q;
        for (int v = 0; v < n2; v++) {
            if (matchV[v] == -1) {
                visV2[v] = 1;
                q.emplace(1, v);
            }
        }
        while (!q.empty()) {
            auto [side, x] = q.front();
            q.pop();
            if (side == 1) {
                for (int u: adjV[x]) {
                    if (matchV[x] == u) {
                        continue;
                    }
                    if (!visU2[u]) {
                        visU2[u] = 1;
                        q.emplace(0, u);
                    }
                }
            } else {
                int v = matchU[x];
                if (v != -1 && !visV2[v]) {
                    visV2[v] = 1;
                    q.emplace(1, v);
                }
            }
        }
    }
    for (int u = 0; u < n1; u++) {
        cout << (visU1[u] ? 'P' : 'N');
    }
    cout << "\n";
    for (int v = 0; v < n2; v++) {
        cout << (visV2[v] ? 'P' : 'N');
    }
    cout << "\n";
    return 0;
}
