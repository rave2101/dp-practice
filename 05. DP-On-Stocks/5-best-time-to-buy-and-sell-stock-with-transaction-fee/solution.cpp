#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<int>& arr, vector<vector<int>>& dp, int SIZE, int idx, int isBuy, int fee) {
        if (idx >= SIZE) return 0;
        if (dp[idx][isBuy] != -1) return dp[idx][isBuy];
        int profit = 0;
        if (isBuy) {
            int buy     = -arr[idx] + util(arr, dp, SIZE, idx+1, 0, fee);   // buy → isBuy=0
            int not_buy = util(arr, dp, SIZE, idx+1, 1, fee);                // skip → isBuy=1
            profit = max(buy, not_buy);
        } else {
            int sell     = (arr[idx] - fee) + util(arr, dp, SIZE, idx+1, 1, fee); // sell → isBuy=1
            int not_sell = util(arr, dp, SIZE, idx+1, 0, fee);                     // skip → isBuy=0
            profit = max(sell, not_sell);
        }
        return dp[idx][isBuy] = profit;
    }

    int stockBuySell(vector<int> arr, int n, int fee) {
        vector<vector<int>> dp(n, vector<int>(2, -1));
        return util(arr, dp, n, 0, 1, fee);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int stockBuySell(vector<int> arr, int n, int fee) {
        vector<vector<int>> dp(n+1, vector<int>(2, 0));
        for (int idx = n-1; idx >= 0; idx--) {
            for (int buy = 0; buy <= 1; buy++) {
                if (buy) {
                    int b  = -arr[idx] + dp[idx+1][0];
                    int nb = dp[idx+1][1];
                    dp[idx][buy] = max(b, nb);
                } else {
                    int sell     = (arr[idx] - fee) + dp[idx+1][1];
                    int not_sell = dp[idx+1][0];
                    dp[idx][buy] = max(sell, not_sell);
                }
            }
        }
        return dp[0][1];
    }
};
