#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<int>& nums, vector<vector<int>>& dp, int idx, int target) {
        if (idx == 0) return target == 0 ? 1 : 0;
        if (dp[idx][target] != -1) return dp[idx][target];

        const int MOD = 1e9 + 7;
        int not_pick = util(nums, dp, idx - 1, target);
        int pick = 0;
        if (target >= nums[idx - 1])
            pick = util(nums, dp, idx - 1, target - nums[idx - 1]);

        return dp[idx][target] = (pick + not_pick) % MOD;
    }

    int targetSum(int n, int target, vector<int>& nums) {
        int sum = 0;
        for (int& num : nums) sum += num;
        if (abs(target) > sum || (sum + target) % 2 != 0) return 0;
        int T = (sum + target) / 2;
        vector<vector<int>> dp(n + 1, vector<int>(T + 1, -1));
        return util(nums, dp, n, T);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int countSubsetSum(vector<int>& nums, vector<vector<int>>& dp, int SIZE, int target) {
        const int MOD = 1e9 + 7;
        for (int idx = 1; idx <= SIZE; idx++) {
            for (int j = 0; j <= target; j++) {
                dp[idx][j] = dp[idx - 1][j];
                if (j >= nums[idx - 1])
                    dp[idx][j] = (dp[idx][j] + dp[idx - 1][j - nums[idx - 1]]) % MOD;
            }
        }
        return dp[SIZE][target];
    }

    int targetSum(int n, int target, vector<int>& nums) {
        int sum = 0;
        for (int& num : nums) sum += num;
        if (abs(target) > sum || (sum + target) % 2 != 0) return 0;
        int T = (sum + target) / 2;
        vector<vector<int>> dp(n + 1, vector<int>(T + 1, 0));
        dp[0][0] = 1;
        return countSubsetSum(nums, dp, n, T);
    }
};
