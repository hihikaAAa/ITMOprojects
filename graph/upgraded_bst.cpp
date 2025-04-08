#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int min(int a, int b) {
    if (a > b)
        return b;
    else
        return a;
}

int max(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}

struct Node {
    int m_x;
    int m_y;
    int m_z;

    explicit Node(int x, int y, int z) {
        m_x = x;
        m_y = y;
        m_z = z;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int plants, energy;
    cin >> plants >> energy;
    int x_begin, y_begin, z_begin;
    cin >> x_begin >> y_begin >> z_begin;
    int x_end, y_end, z_end;
    cin >> x_end >> y_end >> z_end;
    if (x_begin == x_end && y_begin == y_end && z_begin == z_end) {
        cout << 0 << endl;
        return 0;
    } else if (plants == 0) {
        if (abs(x_end - x_begin) + abs(y_end - y_begin) + abs(z_end - z_begin) < energy) {
            cout << abs(x_end - x_begin) + abs(y_end - y_begin) + abs(z_end - z_begin) << endl;
            return 0;
        } else {
            cout << -1 << endl;
            return 0;
        }
    }
    int x_max = 102, y_max = 102, z_max = 202;
    vector<vector<vector<char> > > blocked(x_max, vector<vector<char> >(y_max, vector<char>(z_max, 'f')));
    for (int i = 0; i < plants; i++) {
        int x_i, y_i, z_i, h_i;
        cin >> x_i >> y_i >> z_i >> h_i;
        int indx_x = x_i;
        int indx_y = y_i;
        int startZ = z_i;
        int endZ = z_i + h_i - 1;
        startZ = max(startZ, 1);
        endZ = min(endZ, z_max-1);
        for (int z = startZ; z <= endZ; z++) {
            blocked[indx_x][indx_y][z] = 't';
        }
    }
    vector<vector<vector<int> > > dist(x_max, vector<vector<int> >(y_max, vector<int>(z_max, -1)));
    vector<Node> nodes;
    nodes.emplace_back(1,0,0);
    nodes.emplace_back(-1,0,0);
    nodes.emplace_back(0,-1,0);
    nodes.emplace_back(0,1,0);
    nodes.emplace_back(0,0,1);
    nodes.emplace_back(0,0,-1);
    queue<Node> q;
    dist[x_begin][y_begin][z_begin] = 0;
    q.push(Node(x_begin, y_begin, z_begin));
    while (!q.empty()) {
        Node current = q.front();
        q.pop();
        int d = dist[current.m_x][current.m_y][current.m_z];
        if (current.m_x == x_end && current.m_y == y_end && current.m_z == z_end) {
            if (d > energy) {
                cout << -1 << endl;
            } else {
                cout << d << endl;
            }
            return 0;
        }
        for (auto &help: nodes) {
            int currX = current.m_x + help.m_x;
            int currY = current.m_y + help.m_y;
            int currZ = current.m_z + help.m_z;
            if (currX < 0 || currX >= x_max || currY < 0 || currY >= y_max || currZ < 0 || currZ >= z_max)
                continue;
            if (blocked[currX][currY][currZ] == 't')
                continue;
            if (dist[currX][currY][currZ] != -1)
                continue;
            dist[currX][currY][currZ] = d + 1;
            q.push(Node(currX, currY, currZ));
        }
    }
    cout << -1 << endl;
    return 0;
}
