#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<vector<int>>& triangle, vector<vector<int>>& dp, int i, int j, int R) {
        if (i == R-1) return triangle[i][j];
        if (dp[i][j] != -1) return dp[i][j];
        return dp[i][j] = triangle[i][j] + min(util(triangle, dp, i+1, j,   R),
                                                util(triangle, dp, i+1, j+1, R));
    }

    int minTriangleSum(vector<vector<int>>& triangle) {
        int R = triangle.size();
        vector<vector<int>> dp(R, vector<int>(R, -1));
        return util(triangle, dp, 0, 0, R);
    }
};
*/

// Approach 2 — Tabulation with Space Optimization
class Solution {
public:
    int minTriangleSum(vector<vector<int>>& triangle) {
        int R = triangle.size();
        vector<int> dp = triangle[R-1];
        for (int i = R-2; i >= 0; i--) {
            for (int j = 0; j < (int)triangle[i].size(); j++) {
                dp[j] = triangle[i][j] + min(dp[j], dp[j+1]);
            }
        }
        return dp[0];
    }
};
