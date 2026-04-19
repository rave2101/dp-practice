#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(string s, string t, int i, int j, vector<vector<long long>>& dp) {
        const int MOD = 1e9 + 7;
        if (j <= 0) return 1;
        if (i <= 0) return 0;
        if (dp[i][j] != -1) return dp[i][j];
        if (s[i-1] == t[j-1])
            dp[i][j] = (util(s, t, i-1, j-1, dp) + util(s, t, i-1, j, dp)) % MOD;
        else
            dp[i][j] = util(s, t, i-1, j, dp);
        return dp[i][j];
    }

    int distinctSubsequences(string s, string t) {
        int M = s.size(), N = t.size();
        vector<vector<long long>> dp(M + 1, vector<long long>(N + 1, -1));
        return util(s, t, M, N, dp);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int distinctSubsequences(string s, string t) {
        int M = s.size(), N = t.size();
        const int MOD = 1e9 + 7;
        vector<vector<long long>> dp(M + 1, vector<long long>(N + 1, 0));
        for (int i = 0; i <= M; i++) dp[i][0] = 1;
        for (int i = 1; i <= M; i++) {
            for (int j = 1; j <= N; j++) {
                if (s[i-1] == t[j-1])
                    dp[i][j] = (dp[i-1][j-1] + dp[i-1][j]) % MOD;
                else
                    dp[i][j] = dp[i-1][j];
            }
        }
        return dp[M][N];
    }
};
