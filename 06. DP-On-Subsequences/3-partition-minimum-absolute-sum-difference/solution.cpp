#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    bool subsetSumPossible(int idx, int target, vector<int>& arr, vector<vector<int>>& dp) {
        if (target == 0) return true;
        if (idx == 0)    return arr[0] == target;
        if (dp[idx][target] != -1) return dp[idx][target];

        bool not_pick = subsetSumPossible(idx - 1, target, arr, dp);
        bool pick = false;
        if (target >= arr[idx])
            pick = subsetSumPossible(idx - 1, target - arr[idx], arr, dp);

        return dp[idx][target] = pick || not_pick;
    }

    int minDifference(vector<int>& arr, int n) {
        int sum = 0;
        for (int& x : arr) sum += x;

        int target = sum / 2;
        vector<vector<int>> dp(n, vector<int>(target + 1, -1));

        for (int j = target; j >= 0; j--) {
            if (subsetSumPossible(n - 1, j, arr, dp))
                return abs(sum - 2 * j);
        }
        return sum;
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    void fillSubsetSumDP(vector<int>& arr, vector<vector<bool>>& dp, int target, int SIZE) {
        for (int idx = 1; idx <= SIZE; idx++) {
            for (int s = 1; s <= target; s++) {
                dp[idx][s] = dp[idx - 1][s];
                if (s - arr[idx - 1] >= 0)
                    dp[idx][s] = dp[idx][s] || dp[idx - 1][s - arr[idx - 1]];
            }
        }
    }

    int minDifference(vector<int>& arr, int n) {
        int sum = 0;
        for (int& num : arr) sum += num;

        int target = sum / 2;
        vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));
        for (int i = 0; i <= n; i++) dp[i][0] = true;

        fillSubsetSumDP(arr, dp, target, n);

        for (int j = target; j >= 0; j--) {
            if (dp[n][j]) return abs(sum - 2 * j);
        }
        return sum;
    }
};
