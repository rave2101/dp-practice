#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(int i, int j, vector<vector<int>>& dp) {
        if (i == 0 || j == 0) return 1;
        if (dp[i][j] != -1) return dp[i][j];
        return dp[i][j] = util(i - 1, j, dp) + util(i, j - 1, dp);
    }

    int uniquePaths(int m, int n) {
        vector<vector<int>> dp(m, vector<int>(n, -1));
        return util(m - 1, n - 1, dp);
    }
};
*/

// Approach 2 — Tabulation with Space Optimization
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<int> prev(n, 1);
        for (int i = 1; i < m; i++) {
            vector<int> current(n, 1);
            for (int j = 1; j < n; j++) {
                current[j] = prev[j] + current[j - 1];
            }
            prev = current;
        }
        return prev[n - 1];
    }
};
