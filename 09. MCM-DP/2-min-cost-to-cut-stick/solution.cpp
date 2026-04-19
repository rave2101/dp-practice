#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(int i, int j, vector<int>& cuts, vector<vector<int>>& dp) {
        if (i > j) return 0;
        if (dp[i][j] != -1) return dp[i][j];
        int mini = INT_MAX;
        for (int k = i; k <= j; k++) {
            int current_cost = (cuts[j+1] - cuts[i-1]) + util(i, k-1, cuts, dp) + util(k+1, j, cuts, dp);
            mini = min(mini, current_cost);
        }
        return dp[i][j] = mini;
    }

    int minCost(int n, vector<int>& cuts) {
        cuts.push_back(0);
        cuts.push_back(n);
        sort(cuts.begin(), cuts.end());
        int SIZE = cuts.size();
        vector<vector<int>> dp(SIZE, vector<int>(SIZE, -1));
        return util(1, SIZE - 2, cuts, dp);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int minCost(int n, vector<int>& cuts) {
        cuts.push_back(0);
        cuts.push_back(n);
        sort(cuts.begin(), cuts.end());
        int SIZE = cuts.size();
        vector<vector<int>> dp(SIZE, vector<int>(SIZE, 0));
        for (int i = SIZE - 2; i >= 1; i--) {
            for (int j = i; j <= SIZE - 2; j++) {  // j starts at i, not i+1
                int mini = INT_MAX;
                for (int k = i; k <= j; k++) {
                    int current_cost = (cuts[j+1] - cuts[i-1]) + dp[i][k-1] + dp[k+1][j];
                    mini = min(mini, current_cost);
                }
                dp[i][j] = mini;
            }
        }
        return dp[1][SIZE - 2];
    }
};
