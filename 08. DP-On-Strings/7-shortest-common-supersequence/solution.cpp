#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int lcsUtil(string s, string t, int M, int N, vector<vector<int>>& dp) {
        for (int i = 1; i <= M; i++) {
            for (int j = 1; j <= N; j++) {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
                if (s[i-1] == t[j-1])
                    dp[i][j] = max(dp[i][j], 1 + dp[i-1][j-1]);
            }
        }
        return dp[M][N];
    }

    string shortestCommonSupersequence(string str1, string str2) {
        int M = str1.size(), N = str2.size();
        string result = "";
        vector<vector<int>> dp(M + 1, vector<int>(N + 1, 0));
        lcsUtil(str1, str2, M, N, dp);

        int i = M, j = N;
        while (i > 0 && j > 0) {
            if (str1[i-1] == str2[j-1]) {
                result = str1[i-1] + result;   // LCS character — include once
                i--; j--;
            } else if (dp[i-1][j] >= dp[i][j-1]) {
                result = str1[i-1] + result;   // str1 character not in LCS
                i--;
            } else {
                result = str2[j-1] + result;   // str2 character not in LCS
                j--;
            }
        }
        while (i > 0) { result = str1[i-1] + result; i--; }  // leftover from str1
        while (j > 0) { result = str2[j-1] + result; j--; }  // leftover from str2
        return result;
    }
};
