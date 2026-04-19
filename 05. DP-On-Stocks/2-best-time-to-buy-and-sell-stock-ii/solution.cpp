#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<int>& arr, vector<vector<int>>& dp, int isBuy, int idx, int SIZE) {
        if (idx == SIZE) return 0;
        if (dp[idx][isBuy] != -1) return dp[idx][isBuy];
        int profit = 0;
        if (isBuy) {
            int take     = -arr[idx] + util(arr, dp, 0, idx+1, SIZE);
            int not_take = util(arr, dp, 1, idx+1, SIZE);
            profit = max(take, not_take);
        } else {
            int sell     = arr[idx] + util(arr, dp, 1, idx+1, SIZE);
            int not_sell = util(arr, dp, 0, idx+1, SIZE);
            profit = max(sell, not_sell);
        }
        return dp[idx][isBuy] = profit;
    }

    int stockBuySell(vector<int> arr, int n) {
        vector<vector<int>> dp(n, vector<int>(2, -1));
        return util(arr, dp, 1, 0, n);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int stockBuySell(vector<int> arr, int n) {
        vector<vector<int>> dp(n+1, vector<int>(2, 0));
        for (int i = n-1; i >= 0; i--) {
            for (int buy = 0; buy <= 1; buy++) {
                if (buy) {
                    int take     = -arr[i] + dp[i+1][0];
                    int not_take = dp[i+1][1];
                    dp[i][buy] = max(take, not_take);
                } else {
                    int sell     = arr[i] + dp[i+1][1];
                    int not_sell = dp[i+1][0];
                    dp[i][buy] = max(sell, not_sell);
                }
            }
        }
        return dp[0][1];
    }
};
