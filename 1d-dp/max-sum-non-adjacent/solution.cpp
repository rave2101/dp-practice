#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<int>& nums, int idx, vector<int>& dp) {
        if (idx < 0) return 0;
        if (dp[idx] != -1) return dp[idx];
        dp[idx] = max(nums[idx] + util(nums, idx - 2, dp),
                                  util(nums, idx - 1, dp));
        return dp[idx];
    }

    int nonAdjacent(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n, -1);
        return util(nums, n - 1, dp);
    }
};
*/

// Approach 2 — Space-Optimized
class Solution {
public:
    int nonAdjacent(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return nums[0];
        if (n == 2) return max(nums[0], nums[1]);
        int second_last = nums[0], last = max(nums[0], nums[1]), current = 0;
        for (int idx = 2; idx < n; idx++) {
            current = max(nums[idx] + second_last, last);
            second_last = last;
            last = current;
        }
        return current;
    }
};
