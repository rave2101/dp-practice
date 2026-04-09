#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<int>& arr, vector<vector<vector<int>>>& dp, int SIZE, int idx, int isBuy, int remainingTransactions) {
        if (idx >= SIZE) return 0;
        if (remainingTransactions <= 0) return 0;
        if (dp[idx][isBuy][remainingTransactions] != -1)
            return dp[idx][isBuy][remainingTransactions];
        int profit = 0;
        if (isBuy) {
            int buy    = -arr[idx] + util(arr, dp, SIZE, idx+1, 0, remainingTransactions);
            int no_buy = util(arr, dp, SIZE, idx+1, 1, remainingTransactions);
            profit = max(buy, no_buy);
        } else {
            int sell    = arr[idx] + util(arr, dp, SIZE, idx+1, 1, remainingTransactions-1);
            int no_sell = util(arr, dp, SIZE, idx+1, 0, remainingTransactions);
            profit = max(sell, no_sell);
        }
        return dp[idx][isBuy][remainingTransactions] = profit;
    }

    int stockBuySell(vector<int> arr, int n) {
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(2, vector<int>(3, -1)));
        return util(arr, dp, n, 0, 1, 2);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int stockBuySell(vector<int> arr, int n) {
        vector<vector<vector<int>>> dp(n+1, vector<vector<int>>(2, vector<int>(3, 0)));
        for (int idx = n-1; idx >= 0; idx--) {
            for (int buy = 0; buy <= 1; buy++) {
                for (int rt = 2; rt >= 1; rt--) {
                    if (buy) {
                        int b  = -arr[idx] + dp[idx+1][0][rt];
                        int nb = dp[idx+1][1][rt];
                        dp[idx][buy][rt] = max(b, nb);
                    } else {
                        int sell    = arr[idx] + dp[idx+1][1][rt-1];
                        int no_sell = dp[idx+1][0][rt];
                        dp[idx][buy][rt] = max(sell, no_sell);
                    }
                }
            }
        }
        return dp[0][1][2];
    }
};
