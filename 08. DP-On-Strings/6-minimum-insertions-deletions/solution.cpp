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

    int minOperations(string str1, string str2) {
        int SIZE1 = str1.size(), SIZE2 = str2.size();
        vector<vector<int>> dp(SIZE1 + 1, vector<int>(SIZE2 + 1, 0));
        return (SIZE1 + SIZE2) - 2 * lcsUtil(str1, str2, SIZE1, SIZE2, dp);
    }
};
