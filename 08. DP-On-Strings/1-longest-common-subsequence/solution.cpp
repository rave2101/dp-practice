#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(string str1, string str2, int idx1, int idx2, vector<vector<int>>& dp) {
        if (idx1 <= 0 || idx2 <= 0) return 0;
        if (dp[idx1][idx2] != -1) return dp[idx1][idx2];

        int not_pick = max(util(str1, str2, idx1 - 1, idx2, dp),
                           util(str1, str2, idx1, idx2 - 1, dp));
        int pick = INT_MIN;
        if (str1[idx1 - 1] == str2[idx2 - 1])
            pick = 1 + util(str1, str2, idx1 - 1, idx2 - 1, dp);

        return dp[idx1][idx2] = max(not_pick, pick);
    }

    int lcs(string str1, string str2) {
        int M = str1.size(), N = str2.size();
        vector<vector<int>> dp(M + 1, vector<int>(N + 1, -1));
        return util(str1, str2, M, N, dp);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int lcs(string str1, string str2) {
        int M = str1.size(), N = str2.size();
        vector<vector<int>> dp(M + 1, vector<int>(N + 1, 0));

        for (int i = 1; i <= M; i++) {
            for (int j = 1; j <= N; j++) {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                if (str1[i - 1] == str2[j - 1])
                    dp[i][j] = max(dp[i][j], 1 + dp[i - 1][j - 1]);
            }
        }
        return dp[M][N];
    }
};
