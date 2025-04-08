#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

constexpr long long INF = 1000000000000000000LL;

long long nod(long long a, long long b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

long long nod(long long x, long long y, long long z) {
    return nod(x, nod(y, z));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    long long max_height;
    cin >> max_height;
    long long a, b, c;
    cin >> a >> b >> c;
    const long long g = nod(a, b, c);
    const long long new_a = a / g;
    const long long new_b = b / g;
    const long long new_c = c / g;
    const long long H = (max_height - 1) / g;
    vector v = {new_a, new_b, new_c};
    sort(v.begin(), v.end());
    const long long step0 = v[0];
    long long step1 = v[1];
    long long step2 = v[2];
    if (step0 == 1) {
        cout << H + 1 << endl;
        return 0;
    }
    vector dp(step0, INF);
    dp[0] = 0;
    queue<long long> q;
    q.push(0);
    while (!q.empty()) {
        const long long r = q.front();
        q.pop();
        const long long start = dp[r];
        for (const long long next: {step1, step2}) {
            const long long end = start+ next;
            long long current = end % step0;

            if (end < dp[current]) {
                dp[current] = end;
                q.push(current);
            }
        }
    }
    long long answer = 0;
    for (int r = 0; r < step0; r++) {
        if (dp[r] <= H) {
            const long long helper = 1 + (H - dp[r]) / step0;
            answer += helper;
        }
    }
    cout << answer << endl;
}
