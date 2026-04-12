#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void printLCS(string str1, string str2, int i, int j, vector<vector<int>>& dp) {
        string result = "";
        while (i > 0 && j > 0) {
            if (str1[i-1] == str2[j-1]) {
                result = str1[i-1] + result;
                i--;
                j--;
            } else {
                (dp[i-1][j] >= dp[i][j-1]) ? i-- : j--;
            }
        }
        cout << "LCS :: " << "\t" << result;
    }

    string lcs(string str1, string str2) {
        int M = str1.size(), N = str2.size();
        vector<vector<int>> dp(M + 1, vector<int>(N + 1, 0));

        for (int i = 1; i <= M; i++) {
            for (int j = 1; j <= N; j++) {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
                if (str1[i-1] == str2[j-1])
                    dp[i][j] = max(dp[i][j], 1 + dp[i-1][j-1]);
            }
        }

        printLCS(str1, str2, M, N, dp);
        return "";  // length is dp[M][N]
    }
};
