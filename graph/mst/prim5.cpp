#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef long long ll;

struct Node {
    int state_id;
    int m_x;
    int m_y;
    ll m_cost;
    int m_factor;
    int parent;

    Node() {
        state_id = 0;
        m_x = 0;
        m_y = 0;
        m_cost = 0;
        m_factor = 0;
        parent = -1;
    }

    explicit Node(const int st, const int x, const int y) {
        state_id = st;
        m_x = x;
        m_y = y;
        m_cost = 0;
        m_factor = 0;
        parent = -1;
    }

    bool operator>(const Node &other) const {
        return m_cost > other.m_cost;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int n;
    cin >> n;
    vector<Node> cities;
    cities.reserve(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        cities.emplace_back(i + 1, x, y);
    }
    vector<ll> dist(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> cities[i].m_cost;
        dist[i] = cities[i].m_cost;
    }
    for (int i = 0; i < n; i++) {
        cin >> cities[i].m_factor;
    }
    vector visited(n, 'f');
    priority_queue<Node, vector<Node>, greater<>> pq;
    queue<Node> self_constructed;
    queue<Node> parent_constructed;
    for (int i = 0; i < n; i++) {
        pq.push(cities[i]);
    }
    ll total_cost = 0;
    while (!pq.empty()) {
        const Node curr = pq.top();
        pq.pop();
        const ll curr_id = curr.state_id - 1;
        if (visited[curr_id] == 't' || curr.m_cost != dist[curr_id])
            continue;
        visited[curr_id] = 't';
        total_cost += curr.m_cost;
        for (int i = 0; i < n; i++) {
            if (visited[i] == 'f') {
                ll dx = abs(cities[curr_id].m_x - cities[i].m_x);
                ll dy = abs(cities[curr_id].m_y - cities[i].m_y);
                ll cost_edge = (dx + dy) * (cities[curr_id].m_factor + cities[i].m_factor);
                if (cost_edge < dist[i]) {
                    dist[i] = cost_edge;
                    cities[i].parent = cities[curr_id].state_id;
                    cities[i].m_cost = cost_edge;
                    pq.push(cities[i]);
                }
            }
        }
        if (curr.parent == -1) {
            self_constructed.push(curr);
        } else {
            parent_constructed.push(curr);
        }
    }
    cout << total_cost << "\n";
    cout << self_constructed.size() << "\n";
    if(!self_constructed.empty()) {
        while (!self_constructed.empty()) {
            cout<<self_constructed.front().state_id << ' ';
            self_constructed.pop();
        }
        cout<<"\n";
    }
    cout <<parent_constructed.size() << "\n";
    if(!parent_constructed.empty()) {
      while (!parent_constructed.empty()) {
            cout<<parent_constructed.front().state_id << ' ' <<parent_constructed.front().parent;
            cout<<"\n";
            parent_constructed.pop();
        }
    }
    return 0;
}
