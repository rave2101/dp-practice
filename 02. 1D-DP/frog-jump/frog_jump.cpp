#include <iostream>
#include <vector>
using namespace std;

// Approach 1 — Memoization (Top-Down)
class Solution {
public:
    int frogJump(vector<int>& heights) {
        int N = heights.size();
        vector<int> dp(N, -1);
        return solve(heights, dp, N - 1);
    }

    int solve(vector<int>& heights, vector<int>& dp, int idx) {
        if (idx == 0) return 0;
        if (idx == 1) return abs(heights[1] - heights[0]);
        if (dp[idx] != -1) return dp[idx];
        int second_last = abs(heights[idx - 2] - heights[idx]) + solve(heights, dp, idx - 2);
        int last = abs(heights[idx - 1] - heights[idx]) + solve(heights, dp, idx - 1);
        dp[idx] = min(last, second_last);
        return dp[idx];
    }
};

// Approach 2 — Space-Optimized Tabulation (Bottom-Up)
class Solution2 {
public:
    int frogJump(vector<int>& heights) {
        int N = heights.size();
        if (N == 1) return 0;
        if (N == 2) return abs(heights[1] - heights[0]);
        int second_last = 0, last = abs(heights[1] - heights[0]);
        int current;
        for (int idx = 2; idx < N; idx++) {
            current = min(second_last + abs(heights[idx - 2] - heights[idx]),
                          last + abs(heights[idx - 1] - heights[idx]));
            second_last = last;
            last = current;
        }
        return current;
    }
};
