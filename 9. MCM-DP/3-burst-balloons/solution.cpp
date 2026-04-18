#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(int i, int j, vector<int>& nums, vector<vector<int>>& dp) {
        if (i > j) return 0;
        if (dp[i][j] != -1) return dp[i][j];
        int maxi = INT_MIN;
        for (int k = i; k <= j; k++) {
            int profit = (nums[i-1] * nums[k] * nums[j+1]) + util(i, k-1, nums, dp) + util(k+1, j, nums, dp);
            maxi = max(maxi, profit);
        }
        return dp[i][j] = maxi;
    }

    int maxCoins(vector<int>& nums) {
        nums.insert(nums.begin(), 1);
        nums.push_back(1);
        int SIZE = nums.size();
        vector<vector<int>> dp(SIZE, vector<int>(SIZE, -1));
        return util(1, SIZE - 2, nums, dp);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int maxCoins(vector<int>& nums) {
        nums.insert(nums.begin(), 1);
        nums.push_back(1);
        int SIZE = nums.size();
        vector<vector<int>> dp(SIZE, vector<int>(SIZE, 0));  // 0 for empty ranges
        for (int i = SIZE - 2; i >= 1; i--) {
            for (int j = i; j <= SIZE - 2; j++) {
                int maxi = INT_MIN;
                for (int k = i; k <= j; k++) {
                    int profit = (nums[i-1] * nums[k] * nums[j+1]) + dp[i][k-1] + dp[k+1][j];
                    maxi = max(maxi, profit);
                }
                dp[i][j] = maxi;
            }
        }
        return dp[1][SIZE - 2];
    }
};
