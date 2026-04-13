#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int lcsUtil(string s, string t, int N, vector<vector<int>>& dp) {
        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
                if (s[i-1] == t[j-1])
                    dp[i][j] = max(dp[i][j], 1 + dp[i-1][j-1]);
            }
        }
        return dp[N][N];
    }

    int lpsUtil(string s) {
        int SIZE = s.size();
        vector<vector<int>> dp(SIZE + 1, vector<int>(SIZE + 1, 0));
        string t = s;
        reverse(t.begin(), t.end());
        return lcsUtil(s, t, SIZE, dp);
    }

    int minInsertion(string s) {
        return s.size() - lpsUtil(s);
    }
};
