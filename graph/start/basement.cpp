#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int MAX_HEIGHT = 1000000;

struct Node {
    int low;
    int high;

    Node() {
        low = 0;
        high = 0;
    }

    explicit Node(int l, int h) {
        low = l;
        high = h;
    }

    bool operator==(const Node &other) const {
        return low == other.low && high == other.high;
    }
};

struct Edge {
    int to;
    int l, r;

    explicit Edge(int t, int li, int ri) {
        to = t;
        l = li;
        r = ri;
    }
};

struct Vertex {
    int state_id;
    vector<Edge> edgeList;

    Vertex() {
        state_id = -1;
    }

    explicit Vertex(int s) {
        state_id = s;
    }
};

void quickSort(vector<int>& arr, int l, int r) {
    if (l >= r)
        return;
    int left = l, right = r;
    int pivot = arr[(l + r) / 2];
    while (left <= right) {
        while (arr[left] < pivot)
            left++;
        while (arr[right] > pivot)
            right--;
        if (left <= right) {
            swap(arr[left], arr[right]);
            left++;
            right--;
        }
    }
    if (l < right)
        quickSort(arr, l, right);
    if (left < r)
        quickSort(arr, left, r);
}

struct Graph {
    vector<Vertex> vertices;

    explicit Graph(int n) {
        vertices.resize(n);
        for (int i = 0; i < n; i++) {
            vertices[i].state_id = i + 1;
        }
    }

    void addEdgebyID(int fromVertex, int toVertex, int li, int ri) {
        int from = fromVertex - 1;
        int to = toVertex - 1;
        vertices[from].edgeList.emplace_back(to, li, ri);
        vertices[to].edgeList.emplace_back(from, li, ri);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int rooms, ways;
    cin >> rooms >> ways;
    vector<pair<int, int>> edgeEndpoints(ways);
    for (int i = 0; i < ways; i++) {
        int from, to;
        cin >> from >> to;
        edgeEndpoints[i] = {from, to};
    }

    vector<Node> edgeIntervals(ways);
    for (int i = 0; i < ways; i++) {
        int low, high;
        cin >> low >> high;
        edgeIntervals[i] = Node(low, high);
    }

    Graph graph(rooms);
    for (int i = 0; i < ways; i++) {
        int fromVertex = edgeEndpoints[i].first;
        int toVertex = edgeEndpoints[i].second;
        int low_it = edgeIntervals[i].low;
        int high_it = edgeIntervals[i].high;
        graph.addEdgebyID(fromVertex, toVertex, low_it, high_it);
    }

    vector<int> break_points;
    break_points.push_back(1);
    break_points.push_back(MAX_HEIGHT + 1);
    for (int i = 0; i < ways; i++) {
        break_points.push_back(edgeIntervals[i].low);
        break_points.push_back(edgeIntervals[i].high + 1);
    }
    quickSort(break_points, 0, break_points.size() - 1);
    vector<int> unique_break_points;
    if (!break_points.empty()) {
        unique_break_points.push_back(break_points[0]);
        for (size_t i = 1; i < break_points.size(); i++) {
            if (break_points[i] != break_points[i - 1])
                unique_break_points.push_back(break_points[i]);
        }
    }
    int ans = 0;
    for (auto X : unique_break_points) {
        if (X > MAX_HEIGHT)
            break;
        vector<int> best(rooms, 0);
        best[0] = MAX_HEIGHT;
        priority_queue<pair<int, int>> pq;
        pq.emplace(best[0], 0);
        while (!pq.empty()) {
            auto [curr, u] = pq.top();
            pq.pop();
            if (curr != best[u])
                continue;
            for (Edge &edge : graph.vertices[u].edgeList) {
                if (edge.l <= X) {
                    int candidate = min(curr, edge.r);
                    if (candidate > best[edge.to] && candidate >= X) {
                        best[edge.to] = candidate;
                        pq.emplace(best[edge.to], edge.to);
                    }
                }
            }
        }
        if (best[rooms - 1] >= X) {
            int count = best[rooms - 1] - X + 1;
            ans = max(ans, count);
        }
    }
    cout << (ans == 0 ? -1 : ans) << "\n";
    return 0;
}
