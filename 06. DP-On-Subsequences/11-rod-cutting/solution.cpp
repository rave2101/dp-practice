#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<int>& price, vector<vector<int>>& dp, int idx, int n) {
        if (idx == 0) return 0;  // no piece lengths left
        if (dp[idx][n] != -1) return dp[idx][n];

        int not_pick = util(price, dp, idx - 1, n);
        int pick = INT_MIN;
        int rodLength = idx;
        if (n >= rodLength)
            pick = price[idx - 1] + util(price, dp, idx, n - rodLength);

        return dp[idx][n] = max(not_pick, pick);
    }

    int rodCutting(vector<int> price, int n) {
        int SIZE = price.size();
        vector<vector<int>> dp(SIZE + 1, vector<int>(n + 1, -1));
        return util(price, dp, SIZE, n);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int rodCutting(vector<int> price, int n) {
        int SIZE = price.size();
        vector<vector<int>> dp(SIZE + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= SIZE; i++) {
            for (int j = 1; j <= n; j++) {
                int not_pick = dp[i - 1][j];
                int rodLength = i;
                int pick = INT_MIN;
                if (j >= rodLength)
                    pick = price[i - 1] + dp[i][j - rodLength];
                dp[i][j] = max(not_pick, pick);
            }
        }
        return dp[SIZE][n];
    }
};
