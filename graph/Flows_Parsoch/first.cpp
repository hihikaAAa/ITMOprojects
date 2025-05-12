#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
typedef unsigned long long ull;
constexpr double EPS = 1e-12;

struct AuxEdge {
    double w;
    int c1, c2, v, mask;
};

struct vec {
    int m_x;
    int m_y;

    vec(const int x, const int y) {
        m_x = x;
        m_y = y;
    }
};

struct Edge {
    int to;

    explicit Edge(const int t) {
        to = t;
    }
};

struct Vertex {
    int state_id;
    int coord_x;
    int coord_y;
    std::vector<Edge> edges;
    std::vector<double> pair_weights;
    std::vector<std::vector<std::pair<int, int> > > pair_options;
    int best_mask = -1;
    std::vector<std::pair<int, int> > pairs;

    Vertex() {
        state_id = -1;
        coord_x = -1;
        coord_y = -1;
    }

    explicit Vertex(const int n, const int s, const int x, const int y) {
        state_id = s;
        coord_x = x;
        coord_y = y;
    }
};

ull deg_2(const int a) {
    return static_cast<ull>(a) * static_cast<ull>(a);
}

double find_angle(const Vertex &v1, const Vertex &v2, const Vertex &v3) {
    const auto v1_2 = vec(v2.coord_x - v1.coord_x, v2.coord_y - v1.coord_y);
    const auto v2_3 = vec(v3.coord_x - v2.coord_x, v3.coord_y - v2.coord_y);
    const int scalar_pr = (v1_2.m_x * v2_3.m_x) + (v1_2.m_y * v2_3.m_y);
    const auto norm1_2 = static_cast<double>(deg_2(v1_2.m_x) + deg_2(v1_2.m_y));
    const auto norm2_3 = static_cast<double>(deg_2(v2_3.m_x) + deg_2(v2_3.m_y));
    const double len1 = std::sqrt(norm1_2);
    const double len2 = std::sqrt(norm2_3);
    if (len1 == 0.0 || len2 == 0.0) {
        return 0.0;
    }
    double cos_theta = scalar_pr / (len1 * len2);
    if (cos_theta > 1.0)
        cos_theta = 1.0;
    if (cos_theta < -1.0)
        cos_theta = -1.0;
    if (std::fabs(cos_theta) > 1.0 - 1e-12) {
        return 0.0;
    }
    return std::acos(cos_theta);
}

struct DSU {
    std::vector<int> p;

    explicit DSU(const int n):
        p(n, -1) {
    }

    int find(const int x) {
        return p[x] < 0 ? x : p[x] = find(p[x]);
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b)
            return false;
        if (p[a] > p[b])
            std::swap(a, b);
        p[a] += p[b];
        p[b] = a;
        return true;
    }
};

struct Graph {
    int size;
    std::vector<Vertex> vertices;
    std::vector<std::pair<int, int> > edgesList;
    std::vector<std::vector<int> > vertexEdgeIds;

    explicit Graph(const int n) {
        size = n;
        vertices.resize(n);
        vertexEdgeIds.resize(n);
        for (int i = 0; i < n; i++) {
            vertices[i].state_id = i + 1;
        }
    }

    void addEdgebyID(int from, int to, const int id) {
        vertices[from].edges.emplace_back(to);
        vertices[to].edges.emplace_back(from);
        edgesList.emplace_back(from, to);
        vertexEdgeIds[from].push_back(id);
        vertexEdgeIds[to].push_back(id);
    }

    void removeEdge(const int u, const int v) {
        auto &eu = vertices[u].edges;
        eu.erase(std::remove_if(eu.begin(), eu.end(), [&](const Edge &e) { return e.to == v; }),
                 eu.end());

        auto &ev = vertices[v].edges;
        ev.erase(std::remove_if(ev.begin(), ev.end(),
                                [&](const Edge &e) { return e.to == u; }),
                 ev.end());
        auto kill_id = [&](const int vert, const int a, const int b) {
            auto &ids = vertexEdgeIds[vert];
            ids.erase(std::remove_if(ids.begin(), ids.end(), [&](const int id) {
                auto [uu, vv] = edgesList[id];
                return (uu == a && vv == b) || (uu == b && vv == a);
            }), ids.end());
        };
        kill_id(u, u, v);
        kill_id(v, u, v);
    }

    void sortAdj() {
        for (int v = 0; v < size; ++v) {
            auto &ed = vertices[v].edges;
            std::sort(ed.begin(), ed.end(), [&](const Edge &a, const Edge &b) {
                const int dx1 = vertices[a.to].coord_x - vertices[v].coord_x;
                const int dy1 = vertices[a.to].coord_y - vertices[v].coord_y;
                const int dx2 = vertices[b.to].coord_x - vertices[v].coord_x;
                const int dy2 = vertices[b.to].coord_y - vertices[v].coord_y;
                return std::atan2(static_cast<double>(dy1), static_cast<double>(dx1)) <
                       std::atan2(static_cast<double>(dy2), static_cast<double>(dx2));
            });
        }
    }

    void computePairings() {
        for (auto &V: vertices) {
            if (V.edges.size() != 4)
                continue;
            std::vector<std::pair<double, int> > tmp;
            tmp.reserve(4);
            for (const auto &e: V.edges) {
                int u = e.to;
                double ang = std::atan2(
                        vertices[u].coord_y - V.coord_y,
                        vertices[u].coord_x - V.coord_x
                        );
                tmp.emplace_back(ang, u);
            }
            std::sort(tmp.begin(), tmp.end(), [](auto &a, auto &b) { return a.first < b.first; });
            std::vector<int> nbr(4);
            for (int i = 0; i < 4; ++i) {
                nbr[i] = tmp[i].second;
            }
            V.pair_weights.clear();
            V.pair_options.clear();
            for (int mask = 0; mask < 3; mask++) {
                int a1, b1, a2, b2;
                if (mask == 0) {
                    a1 = 0;
                    b1 = 1;
                    a2 = 2;
                    b2 = 3;
                } else if (mask == 1) {
                    a1 = 0;
                    b1 = 2;
                    a2 = 1;
                    b2 = 3;
                } else {
                    a1 = 0;
                    b1 = 3;
                    a2 = 1;
                    b2 = 2;
                }
                double s = find_angle(vertices[nbr[a1]], V, vertices[nbr[b1]])
                           + find_angle(vertices[nbr[a2]], V, vertices[nbr[b2]]);
                V.pair_weights.push_back(s);
                V.pair_options.push_back({{nbr[a1], nbr[b1]}, {nbr[a2], nbr[b2]}});
            }
            V.best_mask = std::min_element(V.pair_weights.begin(), V.pair_weights.end()) - V.pair_weights.begin();
            V.pairs = V.pair_options[V.best_mask];
        }
    }

    void resolvePairingsByMST() {
        const int E = static_cast<int>(edgesList.size());
        const int H = 2 * E;
        std::vector<int> nextHalf(H, -1), cycleId(H, -1);
        auto halfId = [&](const int edgeIdx, const int atVertex) {
            auto [u,v] = edgesList[edgeIdx];
            return (atVertex == u) ? edgeIdx * 2 : edgeIdx * 2 + 1;
        };
        for (int v = 0; v < size; v++) {
            if (vertices[v].edges.size() == 2) {
                const int e0 = vertexEdgeIds[v][0], e1 = vertexEdgeIds[v][1];
                const int h0 = halfId(e0, v), h1 = halfId(e1, v);
                nextHalf[h0] = h1;
                nextHalf[h1] = h0;
            }
        }
        for (int v = 0; v < size; v++) {
            if (vertices[v].edges.size() != 4)
                continue;
            for (auto &[fst, snd]: vertices[v].pairs) {
                int id_u = -1, id_w = -1;
                for (const int eid: vertexEdgeIds[v]) {
                    auto [uu,vv] = edgesList[eid];
                    if ((uu == v && vv == fst) || (uu == fst && vv == v))
                        id_u = eid;
                    if ((uu == v && vv == snd) || (uu == snd && vv == v))
                        id_w = eid;
                }
                if (id_u == -1 || id_w == -1) {
                    continue;
                }
                const int h_u = halfId(id_u, v), h_w = halfId(id_w, v);
                nextHalf[h_u] = h_w;
                nextHalf[h_w] = h_u;
            }
        }
        int ncycles = 0;
        for (int h = 0; h < H; ++h) {
            if (cycleId[h] != -1) {
                continue;
            }
            int curr = h;
            bool broken = false;
            while (true) {
                cycleId[curr] = ncycles;
                int twin = 0;
                if (curr % 2 == 0) {
                    twin = curr + 1;
                } else {
                    twin = curr - 1;
                }
                if (nextHalf[twin] == -1) {
                    broken = true;
                    break;
                }
                curr = nextHalf[twin];
                if (curr == h) {
                    break;
                }
            }
            ++ncycles;
        }
        std::vector<AuxEdge> aux;
        for (int v = 0; v < size; v++) {
            if (vertices[v].edges.size() != 4) {
                continue;
            }
            auto &V = vertices[v];
            for (int mask = 0; mask < 3; mask++) {
                if (mask == V.best_mask) {
                    continue;
                }
                const double diff = V.pair_weights[mask] - V.pair_weights[V.best_mask];
                for (int i = 0; i < 2; ++i) {
                    auto [fst, snd] = V.pair_options[mask][i];
                    int id_u = -1, id_w = -1;
                    for (const int eid: vertexEdgeIds[v]) {
                        auto [uu,vv] = edgesList[eid];
                        if ((uu == v && vv == fst) || (uu == fst && vv == v))
                            id_u = eid;
                        if ((uu == v && vv == snd) || (uu == snd && vv == v))
                            id_w = eid;
                    }
                    if (id_u == -1 || id_w == -1) {
                        continue;
                    }
                    const int c_u = cycleId[halfId(id_u, v)];
                    const int c_w = cycleId[halfId(id_w, v)];
                    if (c_u == -1 || c_w == -1) {
                        continue;
                    }
                    aux.push_back({diff, c_u, c_w, v, mask});
                }
            }
        }
        std::sort(aux.begin(), aux.end(), [](auto &a, auto &b) { return a.w < b.w; });
        DSU dsu(ncycles);
        int need = ncycles - 1;
        for (auto &[w, c1, c2, v, mask]: aux) {
            if (need <= 0)
                break;
            if (dsu.unite(c1, c2)) {
                auto &V = vertices[v];
                V.best_mask = mask;
                V.pairs = V.pair_options[mask];
                --need;
            }
        }
        for (auto &V: vertices) {
            std::vector<double>().swap(V.pair_weights);
            std::vector<std::vector<std::pair<int, int> > >().swap(V.pair_options);
        }
    }
};

void euler(Graph &g, int &used, const int roads, double &answer, const int parent, const int child,
           const int first_child) {
    if (used == roads) {
        answer += find_angle(g.vertices[parent], g.vertices[child], g.vertices[first_child]);
        return;
    }
    if (used == 0) {
        used++;
        g.removeEdge(parent, child);
        euler(g, used, roads, answer, parent, child, first_child);
    } else {
        int best_next = -1;
        double angle = 0.0;
        auto &V = g.vertices[child];
        auto edge_alive = [&](int a, int b) {
            for (const auto &e: g.vertices[a].edges)
                if (e.to == b)
                    return true;
            return false;
        };
        for (auto &[fst, snd]: V.pairs) {
            if (fst == parent && edge_alive(child, snd))
                best_next = snd;
            else
                if (snd == parent && edge_alive(child, fst))
                    best_next = fst;
            if (best_next != -1)
                break;
        }
        if (best_next != -1) {
            angle = find_angle(g.vertices[parent], V, g.vertices[best_next]);
        } else {
            double min_angle = 1e18;
            for (const auto &e: V.edges) {
                const double a = find_angle(g.vertices[parent], V, g.vertices[e.to]);
                if (a < min_angle) {
                    min_angle = a;
                    best_next = e.to;
                }
            }
            angle = min_angle;
        }
        used++;
        g.removeEdge(child, best_next);
        answer += angle;
        euler(g, used, roads, answer, child, best_next, first_child);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int points, roads;
    std::cin >> points >> roads;
    Graph g(points);
    for (int i = 0; i < points; ++i) {
        std::cin >> g.vertices[i].coord_x >> g.vertices[i].coord_y;
    }
    for (int i = 0; i < roads; ++i) {
        int from, to;
        std::cin >> from >> to;
        g.addEdgebyID(from, to, i);
    }
    g.sortAdj();
    g.computePairings();
    g.resolvePairingsByMST();
    double best = std::numeric_limits<double>::infinity();
    constexpr int start = 0;
    for (const auto &edge: g.vertices[start].edges) {
        Graph g2 = g;
        int used = 0;
        double ans = 0.0;
        euler(g2, used, roads, ans, start, edge.to, edge.to);
        best = std::min(best, ans);
    }
    std::cout << std::setprecision(11) << best;
    return 0;
}
