#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<int>& wt, vector<int>& val, vector<vector<int>>& dp, int idx, int W) {
        if (idx == 0) return 0;  // no items left
        if (dp[idx][W] != -1) return dp[idx][W];

        int not_pick = util(wt, val, dp, idx - 1, W);
        int pick = INT_MIN;
        if (W >= wt[idx - 1])
            pick = val[idx - 1] + util(wt, val, dp, idx, W - wt[idx - 1]);

        return dp[idx][W] = max(not_pick, pick);
    }

    int unboundedKnapsack(vector<int>& wt, vector<int>& val, int n, int W) {
        vector<vector<int>> dp(n + 1, vector<int>(W + 1, -1));
        return util(wt, val, dp, n, W);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int unboundedKnapsack(vector<int>& wt, vector<int>& val, int n, int W) {
        vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= W; j++) {
                int not_pick = dp[i - 1][j];
                int pick = INT_MIN;
                if (j >= wt[i - 1])
                    pick = val[i - 1] + dp[i][j - wt[i - 1]];
                dp[i][j] = max(not_pick, pick);
            }
        }
        return dp[n][W];
    }
};
