#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
private:
    int util(vector<vector<int>>& matrix, int day, int last, vector<vector<int>>& dp) {
        if (dp[day][last] != -1) return dp[day][last];
        if (day == 0) {
            int maxi = 0;
            for (int i = 0; i <= 2; i++) {
                if (i != last) maxi = max(maxi, matrix[0][i]);
            }
            return dp[0][last] = maxi;
        }
        int maxi = 0;
        for (int i = 0; i <= 2; i++) {
            if (i != last) {
                int activity = matrix[day][i] + util(matrix, day - 1, i, dp);
                maxi = max(maxi, activity);
            }
        }
        return dp[day][last] = maxi;
    }

public:
    int ninjaTraining(vector<vector<int>>& matrix) {
        int day = matrix.size();
        vector<vector<int>> dp(day, vector<int>(4, -1));
        return util(matrix, day - 1, 3, dp);
    }
};
*/

// Approach 2 — Tabulation with Space Optimization
class Solution {
public:
    int ninjaTraining(vector<vector<int>>& matrix) {
        int R = matrix.size(), C = matrix[0].size();
        vector<int> prev(C, 0);
        prev = matrix[0];
        for (int i = 1; i < R; i++) {
            vector<int> current(C, 0);
            for (int j = 0; j < C; j++) {
                for (int k = 0; k < C; k++) {
                    if (k == j) continue;
                    current[j] = max(current[j], matrix[i][j] + prev[k]);
                }
            }
            prev = current;
        }
        return *max_element(prev.begin(), prev.end());
    }
};
