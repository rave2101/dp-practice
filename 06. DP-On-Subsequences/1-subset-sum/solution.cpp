#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<int>& arr, vector<vector<int>>& dp, int target, int idx) {
        if (target == 0) return 1;
        if (idx == 0)    return 0;
        if (dp[idx][target] != -1) return dp[idx][target];

        int not_pick = util(arr, dp, target, idx - 1);
        int pick = 0;
        if (target - arr[idx - 1] >= 0)
            pick = util(arr, dp, target - arr[idx - 1], idx - 1);

        return dp[idx][target] = pick || not_pick;
    }

    bool isSubsetSum(vector<int> arr, int target) {
        int SIZE = arr.size();
        vector<vector<int>> dp(SIZE + 1, vector<int>(target + 1, -1));
        return util(arr, dp, target, SIZE);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    bool isSubsetSum(vector<int> arr, int target) {
        int SIZE = arr.size();
        vector<vector<bool>> dp(SIZE + 1, vector<bool>(target + 1, false));

        for (int i = 0; i <= SIZE; i++) dp[i][0] = true;

        for (int i = 1; i <= SIZE; i++) {
            for (int j = 1; j <= target; j++) {
                dp[i][j] = dp[i - 1][j];
                if (j >= arr[i - 1])
                    dp[i][j] = dp[i][j] || dp[i - 1][j - arr[i - 1]];
            }
        }

        return dp[SIZE][target];
    }
};
