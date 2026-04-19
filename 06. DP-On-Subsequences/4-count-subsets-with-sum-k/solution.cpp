#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    int util(vector<int>& arr, vector<vector<int>>& dp, int idx, int target) {
        if (target == 0) return 1;
        if (idx == 0)    return arr[0] == target ? 1 : 0;
        if (dp[idx][target] != -1) return dp[idx][target];

        const int MOD = 1e9 + 7;
        int not_pick = util(arr, dp, idx - 1, target);
        int pick = 0;
        if (target >= arr[idx])
            pick = util(arr, dp, idx - 1, target - arr[idx]);

        return dp[idx][target] = (pick + not_pick) % MOD;
    }

    int perfectSum(vector<int>& arr, int K) {
        int SIZE = arr.size();
        vector<vector<int>> dp(SIZE, vector<int>(K + 1, -1));
        return util(arr, dp, SIZE - 1, K);
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    int perfectSum(vector<int>& arr, int K) {
        int SIZE = arr.size();
        const int MOD = 1e9 + 7;
        vector<vector<int>> dp(SIZE + 1, vector<int>(K + 1, 0));
        dp[0][0] = 1;  // one way to form sum 0 using zero elements: the empty subset

        for (int idx = 1; idx <= SIZE; idx++) {
            for (int j = 0; j <= K; j++) {  // j=0 must be included to handle zeros in arr
                dp[idx][j] = dp[idx - 1][j];
                if (j >= arr[idx - 1])
                    dp[idx][j] = (dp[idx][j] + dp[idx - 1][j - arr[idx - 1]]) % MOD;
            }
        }
        return dp[SIZE][K];
    }
};
