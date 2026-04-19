#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestCommonSubstr(string str1, string str2) {
        int M = str1.size(), N = str2.size();
        int result = 0;
        vector<vector<int>> dp(M + 1, vector<int>(N + 1, 0));

        for (int i = 1; i <= M; i++) {
            for (int j = 1; j <= N; j++) {
                if (str1[i-1] == str2[j-1]) {
                    dp[i][j] = 1 + dp[i-1][j-1];
                    result = max(result, dp[i][j]);
                } else {
                    dp[i][j] = 0;
                }
            }
        }
        return result;
    }
};
