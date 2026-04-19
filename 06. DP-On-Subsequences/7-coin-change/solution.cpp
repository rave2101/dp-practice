#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<int>& coins, vector<vector<int>>& dp, int idx, int amount) {
        if (idx == 0) return amount == 0 ? 0 : (int)1e9;  // no coins left
        if (dp[idx][amount] != -1) return dp[idx][amount];

        int not_pick = util(coins, dp, idx - 1, amount);
        int pick = (int)1e9;
        if (amount >= coins[idx - 1])
            pick = 1 + util(coins, dp, idx, amount - coins[idx - 1]);

        return dp[idx][amount] = min(pick, not_pick);
    }

    int MinimumCoins(vector<int>& coins, int amount) {
        int SIZE = coins.size();
        vector<vector<int>> dp(SIZE + 1, vector<int>(amount + 1, -1));
        int ans = util(coins, dp, SIZE, amount);
        return ans >= (int)1e9 ? -1 : ans;
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int MinimumCoins(vector<int>& coins, int amount) {
        int SIZE = coins.size();
        int INF = 1e9;
        vector<vector<int>> dp(SIZE + 1, vector<int>(amount + 1, INF));
        for (int i = 0; i <= SIZE; i++) dp[i][0] = 0;

        for (int i = 1; i <= SIZE; i++) {
            for (int j = 1; j <= amount; j++) {
                int not_pick = dp[i - 1][j];
                int pick = INF;
                if (j >= coins[i - 1])
                    pick = 1 + dp[i][j - coins[i - 1]];
                dp[i][j] = min(pick, not_pick);
            }
        }
        return dp[SIZE][amount] >= INF ? -1 : dp[SIZE][amount];
    }
};
