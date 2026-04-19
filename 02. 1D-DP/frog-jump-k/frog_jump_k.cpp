// Frog Jump with K distances
// GFG: https://www.geeksforgeeks.org/problems/minimal-cost/1
// A frog at step 0 wants to reach step n-1.
// It can jump 1 to k steps at a time.
// Cost of jumping from i to j is abs(heights[i] - heights[j]).
// Return the minimum total energy to reach the last step.

#include <vector>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
using namespace std;

// ─── Approach 1: Memoization (Top-Down) ──────────────────────────────────────
// Time: O(n*k)  |  Space: O(n) cache + O(n) call stack

int solve(vector<int>& heights, int k, vector<int>& dp, int idx) {
    if (idx == 0) return 0;
    if (idx == 1) return abs(heights[1] - heights[0]);
    if (dp[idx] != INT_MAX) return dp[idx];  // cache hit — already optimal

    for (int j = 1; j <= k; j++) {
        if (idx - j >= 0) {
            int cost = solve(heights, k, dp, idx - j) + abs(heights[idx - j] - heights[idx]);
            dp[idx] = min(dp[idx], cost);
        }
    }
    return dp[idx];
}

int frogJumpMemo(vector<int>& heights, int k) {
    int N = heights.size();
    vector<int> dp(N, INT_MAX);
    return solve(heights, k, dp, N - 1);
}

// ─── Approach 2: Tabulation (Bottom-Up) ──────────────────────────────────────
// Time: O(n*k)  |  Space: O(n)

int frogJumpTab(vector<int>& heights, int k) {
    int N = heights.size();
    if (N == 1) return 0;
    if (N == 2) return abs(heights[1] - heights[0]);

    vector<int> dp(N, INT_MAX);
    dp[0] = 0;
    dp[1] = abs(heights[1] - heights[0]);

    for (int idx = 2; idx < N; idx++) {
        for (int j = idx - 1; j >= max(idx - k, 0); j--) {
            dp[idx] = min(dp[idx], dp[j] + abs(heights[idx] - heights[j]));
        }
    }
    return dp[N - 1];
}

// ─── Approach 3: Space-Optimized Tabulation (circular buffer) ────────────────
// Time: O(n*k)  |  Space: O(k)
// dp[i % k] stores the min cost to reach step i.
// Key: compute into `curr` BEFORE writing to dp[idx % k],
//      because dp[idx % k] == dp[(idx-k) % k] — same slot, still needed.

int frogJumpSpaceOpt(vector<int>& heights, int k) {
    int N = heights.size();
    if (N == 1) return 0;
    if (N == 2) return abs(heights[1] - heights[0]);

    vector<int> dp(k, INT_MAX);
    dp[0] = 0;
    if (k > 1) dp[1 % k] = abs(heights[1] - heights[0]);

    for (int idx = 2; idx < N; idx++) {
        int curr = INT_MAX;
        for (int j = idx - 1; j >= max(idx - k, 0); j--) {
            if (dp[j % k] != INT_MAX)
                curr = min(curr, dp[j % k] + abs(heights[idx] - heights[j]));
        }
        dp[idx % k] = curr;  // safe to overwrite now — idx-k slot no longer needed
    }
    return dp[(N - 1) % k];
}

int main() {
    vector<int> heights = {10, 20, 30, 10, 10};
    int k = 3;

    cout << "Memoization:      " << frogJumpMemo(heights, k) << endl;  // 10
    cout << "Tabulation:       " << frogJumpTab(heights, k)  << endl;  // 10
    cout << "Space-Optimized:  " << frogJumpSpaceOpt(heights, k) << endl;  // 10

    return 0;
}
