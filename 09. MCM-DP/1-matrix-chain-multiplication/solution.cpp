#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(int i, int j, vector<int>& nums, vector<vector<int>>& dp) {
        if (i == j) return 0;
        if (dp[i][j] != -1) return dp[i][j];
        int mini = INT_MAX;
        for (int k = i; k <= j - 1; k++) {
            int steps = (nums[i-1] * nums[k] * nums[j]) + util(i, k, nums, dp) + util(k+1, j, nums, dp);
            mini = min(mini, steps);
        }
        return dp[i][j] = mini;
    }

    int matrixMultiplication(vector<int>& nums) {
        int SIZE = nums.size();
        vector<vector<int>> dp(SIZE, vector<int>(SIZE, -1));
        return util(1, SIZE - 1, nums, dp);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int matrixMultiplication(vector<int>& nums) {
        int SIZE = nums.size();
        vector<vector<int>> dp(SIZE, vector<int>(SIZE, 0));
        for (int i = SIZE - 1; i >= 1; i--) {
            for (int j = i + 1; j < SIZE; j++) {
                int mini = INT_MAX;
                for (int k = i; k <= j - 1; k++) {
                    int steps = (nums[i-1] * nums[k] * nums[j]) + dp[i][k] + dp[k+1][j];
                    mini = min(mini, steps);
                }
                dp[i][j] = mini;
            }
        }
        return dp[1][SIZE - 1];
    }
};
