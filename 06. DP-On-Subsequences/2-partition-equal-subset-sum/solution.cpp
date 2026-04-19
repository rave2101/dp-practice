#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    bool subsetSumPossible(int idx, int target, vector<int>& arr, vector<vector<int>>& dp) {
        if (target == 0) return true;
        if (idx == 0)    return arr[idx] == target;
        if (dp[idx][target] != -1) return dp[idx][target];

        bool not_pick = subsetSumPossible(idx - 1, target, arr, dp);
        bool pick = false;
        if (target >= arr[idx])
            pick = subsetSumPossible(idx - 1, target - arr[idx], arr, dp);

        return dp[idx][target] = pick || not_pick;
    }

    bool equalPartition(int n, vector<int> arr) {
        int sum = 0;
        for (int& x : arr) sum += x;
        if (sum % 2 != 0) return false;
        vector<vector<int>> dp(n, vector<int>((sum / 2) + 1, -1));
        return subsetSumPossible(n - 1, sum / 2, arr, dp);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    bool isSubsetSumPossible(vector<vector<bool>>& dp, vector<int>& arr, int target, int SIZE) {
        for (int idx = 1; idx <= SIZE; idx++) {
            for (int t = 1; t <= target; t++) {
                dp[idx][t] = dp[idx - 1][t];
                if (t - arr[idx - 1] >= 0)
                    dp[idx][t] = dp[idx][t] || dp[idx - 1][t - arr[idx - 1]];
            }
        }
        return dp[SIZE][target];
    }

    bool equalPartition(int n, vector<int> arr) {
        int sum = 0, target;
        for (int& num : arr) sum += num;
        if (sum % 2 != 0) return false;
        target = sum / 2;
        vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));
        for (int i = 0; i <= n; i++) dp[i][0] = true;
        return isSubsetSumPossible(dp, arr, target, n);
    }
};
