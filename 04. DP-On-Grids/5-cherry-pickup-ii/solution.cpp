#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<vector<int>>& matrix, vector<vector<vector<int>>>& dp, int i, int j1, int j2, int R, int C) {
        if (j1 < 0 || j1 >= C || j2 < 0 || j2 >= C) return -1e8;
        if (dp[i][j1][j2] != -1) return dp[i][j1][j2];
        if (i == R-1) {
            int value = matrix[i][j1];
            if (j1 != j2) value += matrix[i][j2];
            return dp[i][j1][j2] = value;
        }
        int maxi = INT_MIN;
        for (int dj1 = -1; dj1 <= 1; dj1++) {
            for (int dj2 = -1; dj2 <= 1; dj2++) {
                int value = matrix[i][j1];
                if (j1 != j2) value += matrix[i][j2];
                maxi = max(maxi, value + util(matrix, dp, i+1, j1+dj1, j2+dj2, R, C));
            }
        }
        return dp[i][j1][j2] = maxi;
    }

    int cherryPickup(vector<vector<int>>& matrix) {
        int R = matrix.size(), C = matrix[0].size();
        vector<vector<vector<int>>> dp(R, vector<vector<int>>(C, vector<int>(C, -1)));
        return util(matrix, dp, 0, 0, C-1, R, C);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int cherryPickup(vector<vector<int>>& matrix) {
        int R = matrix.size(), C = matrix[0].size();
        vector<vector<vector<int>>> dp(R, vector<vector<int>>(C, vector<int>(C, 0)));

        // Base case: last row
        for (int j1 = 0; j1 < C; j1++)
            for (int j2 = 0; j2 < C; j2++) {
                dp[R-1][j1][j2] = matrix[R-1][j1];
                if (j1 != j2) dp[R-1][j1][j2] += matrix[R-1][j2];
            }

        // Fill bottom-up
        for (int i = R-2; i >= 0; i--) {
            for (int j1 = 0; j1 < C; j1++) {
                for (int j2 = 0; j2 < C; j2++) {
                    int maxi = INT_MIN;
                    int value = matrix[i][j1];
                    if (j1 != j2) value += matrix[i][j2];
                    for (int dj1 = -1; dj1 <= 1; dj1++) {
                        for (int dj2 = -1; dj2 <= 1; dj2++) {
                            int nj1 = j1 + dj1, nj2 = j2 + dj2;
                            if (nj1 >= 0 && nj1 < C && nj2 >= 0 && nj2 < C)
                                maxi = max(maxi, value + dp[i+1][nj1][nj2]);
                        }
                    }
                    dp[i][j1][j2] = maxi;
                }
            }
        }

        return dp[0][0][C-1];
    }
};
