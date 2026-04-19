#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<int>& coins, vector<vector<int>>& dp, int idx, int amount) {
        if (idx == 0) return amount % coins[0] == 0 ? 1 : 0;
        if (dp[idx][amount] != -1) return dp[idx][amount];

        const int MOD = 1e9 + 7;
        int not_pick = util(coins, dp, idx - 1, amount);
        int pick = 0;
        if (amount >= coins[idx - 1])
            pick = util(coins, dp, idx, amount - coins[idx - 1]);

        return dp[idx][amount] = (pick + not_pick) % MOD;
    }

    int count(vector<int>& coins, int N, int amount) {
        vector<vector<int>> dp(N + 1, vector<int>(amount + 1, -1));
        return util(coins, dp, N, amount);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int count(vector<int>& coins, int N, int amount) {
        const int MOD = 1e9 + 7;
        vector<vector<int>> dp(N + 1, vector<int>(amount + 1, 0));
        for (int i = 0; i <= N; i++) dp[i][0] = 1;

        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= amount; j++) {
                dp[i][j] = dp[i - 1][j];
                if (j - coins[i - 1] >= 0)
                    dp[i][j] = (dp[i][j] + dp[i][j - coins[i - 1]]) % MOD;
            }
        }
        return dp[N][amount];
    }
};
