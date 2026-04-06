#include <bits/stdc++.h>
using namespace std;

// ─── House Robber (Linear) ───────────────────────────────────────────────────

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int solve(vector<int>& money, vector<int>& dp, int idx) {
        if (idx < 0) return 0;
        if (dp[idx] != -1) return dp[idx];
        dp[idx] = max(money[idx] + solve(money, dp, idx - 2),
                                   solve(money, dp, idx - 1));
        return dp[idx];
    }

    int houseRobber(vector<int>& money) {
        int n = money.size();
        vector<int> dp(n, -1);
        return solve(money, dp, n - 1);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
/*
class Solution {
public:
    int houseRobber(vector<int>& money) {
        int n = money.size();
        vector<int> dp(n);
        dp[0] = money[0];
        for (int i = 1; i < n; i++)
            dp[i] = max(money[i] + (i >= 2 ? dp[i - 2] : 0), dp[i - 1]);
        return dp[n - 1];
    }
};
*/

// Approach 3 — Space-Optimized
/*
class Solution {
public:
    int houseRobber(vector<int>& money) {
        int prev2 = 0, prev1 = 0;
        for (int x : money) {
            int curr = max(x + prev2, prev1);
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
};
*/

// ─── House Robber II (Circular) ───────────────────────────────────────────────

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int rob(vector<int>& money, int start, vector<int>& dp, int idx) {
        if (idx < start) return 0;
        if (dp[idx] != -1) return dp[idx];
        dp[idx] = max(money[idx] + rob(money, start, dp, idx - 2),
                                   rob(money, start, dp, idx - 1));
        return dp[idx];
    }

    int houseRobberCircular(vector<int>& money) {
        int n = money.size();
        if (n == 1) return money[0];
        vector<int> dp1(n, -1), dp2(n, -1);
        return max(rob(money, 0, dp1, n - 2),   // exclude last house
                   rob(money, 1, dp2, n - 1));  // exclude first house
    }
};
*/

// Approach 2 — Space-Optimized
class Solution {
public:
    int rob(vector<int>& money, int start, int end) {
        int prev2 = 0, prev1 = 0;
        for (int i = start; i <= end; i++) {
            int curr = max(money[i] + prev2, prev1);
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }

    int houseRobberCircular(vector<int>& money) {
        int n = money.size();
        if (n == 1) return money[0];
        return max(rob(money, 0, n - 2), rob(money, 1, n - 1));
    }
};
