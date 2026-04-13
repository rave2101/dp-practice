#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int editDistance(string start, string target) {
        int M = start.size(), N = target.size();
        vector<vector<int>> dp(M + 1, vector<int>(N + 1, 0));

        for (int i = 0; i <= M; i++) dp[i][0] = i;  // delete all chars of start
        for (int j = 0; j <= N; j++) dp[0][j] = j;  // insert all chars of target

        for (int i = 1; i <= M; i++) {
            for (int j = 1; j <= N; j++) {
                if (start[i-1] == target[j-1]) {
                    dp[i][j] = dp[i-1][j-1];                              // no operation needed
                } else {
                    dp[i][j] = 1 + min({dp[i-1][j],                       // delete
                                        dp[i][j-1],                        // insert
                                        dp[i-1][j-1]});                    // replace
                }
            }
        }
        return dp[M][N];
    }
};
