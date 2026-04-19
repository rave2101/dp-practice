#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<vector<int>>& matrix, vector<vector<int>>& dp, int i, int j) {
        if (i < 0 || j < 0 || matrix[i][j] == 1) return 0;
        if (i == 0 && j == 0) return 1;
        if (dp[i][j] != -1) return dp[i][j];
        return dp[i][j] = util(matrix, dp, i-1, j) + util(matrix, dp, i, j-1);
    }

    int uniquePathsWithObstacles(vector<vector<int>>& matrix) {
        int R = matrix.size(), C = matrix[0].size();
        if (matrix[0][0] == 1 || matrix[R-1][C-1] == 1) return 0;
        vector<vector<int>> dp(R, vector<int>(C, -1));
        return util(matrix, dp, R-1, C-1);
    }
};
*/

// Approach 2 — Tabulation with Space Optimization
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& matrix) {
        int R = matrix.size(), C = matrix[0].size();
        if (matrix[0][0] == 1 || matrix[R-1][C-1] == 1) return 0;
        vector<int> prev(C, 0);
        prev[0] = 1;
        for (int j = 1; j < C; j++)
            prev[j] = matrix[0][j] == 1 ? 0 : prev[j-1];
        for (int i = 1; i < R; i++) {
            vector<int> current(C, 0);
            current[0] = matrix[i][0] == 1 ? 0 : prev[0];
            for (int j = 1; j < C; j++)
                current[j] = matrix[i][j] == 1 ? 0 : current[j-1] + prev[j];
            prev = current;
        }
        return prev[C-1];
    }
};
