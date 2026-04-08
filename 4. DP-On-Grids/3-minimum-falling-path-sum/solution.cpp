#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<vector<int>>& matrix, vector<vector<int>>& dp, int i, int j, int R, int C) {
        if (j < 0 || j >= C) return 1e5;
        if (dp[i][j] != -1) return dp[i][j];
        if (i == 0) return dp[i][j] = matrix[i][j];
        return dp[i][j] = matrix[i][j] + min({ util(matrix, dp, i-1, j,   R, C),
                                               util(matrix, dp, i-1, j-1, R, C),
                                               util(matrix, dp, i-1, j+1, R, C) });
    }

    int minFallingPathSum(vector<vector<int>>& matrix) {
        int R = matrix.size(), C = matrix[0].size();
        vector<vector<int>> dp(R, vector<int>(C, -1));
        int mini = INT_MAX;
        for (int j = 0; j < C; j++)
            mini = min(mini, util(matrix, dp, R-1, j, R, C));
        return mini;
    }
};
*/

// Approach 2 — Tabulation with Space Optimization
class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int R = matrix.size(), C = matrix[0].size();
        vector<int> prev(matrix[0]);
        for (int i = 1; i < R; i++) {
            vector<int> current(C);
            for (int j = 0; j < C; j++) {
                int up    = prev[j];
                int left  = (j > 0)   ? prev[j-1] : INT_MAX;
                int right = (j < C-1) ? prev[j+1] : INT_MAX;
                current[j] = matrix[i][j] + min({up, left, right});
            }
            prev = current;
        }
        return *min_element(prev.begin(), prev.end());
    }
};
