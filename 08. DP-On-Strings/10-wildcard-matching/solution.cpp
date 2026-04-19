#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool wildCard(string str, string pat) {
        int M = str.size(), N = pat.size();
        vector<vector<bool>> dp(M + 1, vector<bool>(N + 1, false));

        // Base case 1: empty string matches empty pattern
        dp[0][0] = true;

        // Base case 2: empty string vs non-empty pattern
        // only possible if all characters so far are '*'
        for (int j = 1; j <= N; j++)
            dp[0][j] = (pat[j-1] == '*') && dp[0][j-1];

        // Base case 3: non-empty string vs empty pattern
        // always false — implicit from initialisation

        for (int i = 1; i <= M; i++) {
            for (int j = 1; j <= N; j++) {
                if (str[i-1] == pat[j-1] || pat[j-1] == '?') {
                    dp[i][j] = dp[i-1][j-1];          // exact match or '?' — consume both
                } else if (pat[j-1] == '*') {
                    dp[i][j] = dp[i-1][j]              // '*' matches one char — consume from str
                             || dp[i][j-1];             // '*' matches zero chars — skip '*'
                } else {
                    dp[i][j] = false;                   // real char, no match
                }
            }
        }
        return dp[M][N];
    }
};
