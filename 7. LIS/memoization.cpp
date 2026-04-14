/*
 * Longest Increasing Subsequence — Memoization (Top-Down DP)
 *
 * Approach:
 *   State: dp[idx][prev]
 *     - idx  : current position (1-indexed, 1 to n)
 *     - prev : last picked index (0 = none picked, k = nums[k-1] was last picked)
 *
 *   Recurrence:
 *     notPick = dp[idx+1][prev]
 *     pick    = 1 + dp[idx+1][idx]   if prev==0 || nums[idx-1] > nums[prev-1]
 *     dp[idx][prev] = max(notPick, pick)
 *
 *   Base case: idx > n → return 0
 *
 * Complexity:
 *   Time  : O(n^2) — n*n unique states, O(1) per state
 *   Space : O(n^2) — dp table of size (n+1) x (n+1)
 *
 * ❌ WHY THIS GETS TLE/MLE FOR LARGE INPUTS (n = 10^5):
 *
 *   dp table size = (n+1) x (n+1)
 *                = (10^5+1) x (10^5+1)
 *                ≈ 10^10 cells
 *
 *   - Memory  : 10^10 integers x 4 bytes ≈ 40 GB  → MLE (limit is ~256 MB)
 *   - Time    : 10^10 operations            → TLE (limit is ~10^8 ops/sec)
 *
 *   Even before hitting the time limit, the judge runs out of memory.
 *   The dp table simply cannot be allocated for n = 10^5.
 *
 *   This approach only works for n ≤ ~3000 (where n^2 ≈ 9 x 10^6).
 *
 * ✅ For n = 10^5, use O(n log n) patience sorting — see nlogn.cpp
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int util(vector<int>& nums, int idx, int prev, int SIZE, vector<vector<int>>& dp) {
        if (idx > SIZE) return 0;
        if (dp[idx][prev] != -1) return dp[idx][prev];

        int not_pick = util(nums, idx + 1, prev, SIZE, dp);
        int pick = INT_MIN;
        if (prev == 0 || nums[idx - 1] > nums[prev - 1]) {
            pick = 1 + util(nums, idx + 1, idx, SIZE, dp);
        }
        return dp[idx][prev] = max(pick, not_pick);
    }

    int LIS(vector<int>& nums) {
        int SIZE = nums.size();
        vector<vector<int>> dp(SIZE + 1, vector<int>(SIZE + 1, -1));
        return util(nums, 1, 0, SIZE, dp);
    }
};
