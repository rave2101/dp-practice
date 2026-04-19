#include <bits/stdc++.h>
using namespace std;

long long MOD = 1e9 + 7;

int util(string& s, int i, int j, int isTrue, vector<vector<vector<int>>>& dp) {
    if (i > j) return 0;
    if (i == j) {
        if (isTrue) return s[i] == 'T';
        else return s[i] == 'F';
    }
    if (dp[i][j][isTrue] != -1) return dp[i][j][isTrue];

    long long ways = 0;
    for (int k = i + 1; k <= j - 1; k += 2) {
        long long lt = util(s, i, k - 1, 1, dp);
        long long lf = util(s, i, k - 1, 0, dp);
        long long rt = util(s, k + 1, j, 1, dp);
        long long rf = util(s, k + 1, j, 0, dp);

        if (s[k] == '&') {
            if (isTrue) ways = (ways + (lt * rt) % MOD) % MOD;
            else        ways = (ways + (lt * rf) % MOD + (lf * rt) % MOD + (lf * rf) % MOD) % MOD;
        } else if (s[k] == '^') {
            if (isTrue) ways = (ways + (lt * rf) % MOD + (lf * rt) % MOD) % MOD;
            else        ways = (ways + (lt * rt) % MOD + (lf * rf) % MOD) % MOD;
        } else if (s[k] == '|') {
            if (isTrue) ways = (ways + (lt * rt) % MOD + (lt * rf) % MOD + (lf * rt) % MOD) % MOD;
            else        ways = (ways + (lf * rf) % MOD) % MOD;
        }
    }
    return dp[i][j][isTrue] = ways;
}

int evaluateExp(string& exp) {
    int SIZE = exp.size();
    vector<vector<vector<int>>> dp(SIZE, vector<vector<int>>(SIZE, vector<int>(2, -1)));
    return util(exp, 0, SIZE - 1, 1, dp);
}
