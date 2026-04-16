/*
 * Longest Increasing Subsequence — Tabulation (Bottom-Up DP)
 *
 * Approach:
 *   dp[i] = length of LIS ending at index i
 *
 *   For every i, look back at all j < i:
 *     if nums[j] < nums[i] → nums[i] can extend the subsequence ending at j
 *     dp[i] = max(dp[i], dp[j] + 1)
 *
 *   Base case: dp[i] = 1 for all i (every element is an LIS of length 1 by itself)
 *   Answer: max element in dp[]
 *
 * Example: nums = [10, 9, 2, 5, 3, 7, 101, 18]
 *   i=0 → dp=[1, 1, 1, 1, 1, 1, 1, 1]
 *   i=1 → no j where nums[j]<9 with j<1      → dp[1]=1
 *   i=2 → no j where nums[j]<2 with j<2      → dp[2]=1
 *   i=3 → nums[2]=2 < 5 → dp[3]=dp[2]+1=2   → dp[3]=2
 *   i=4 → nums[2]=2 < 3 → dp[4]=dp[2]+1=2   → dp[4]=2
 *   i=5 → nums[2]=2,nums[3]=5,nums[4]=3 < 7  → dp[5]=dp[3]+1=3
 *   i=6 → all previous < 101                  → dp[6]=dp[5]+1=4
 *   i=7 → nums[2..5] < 18                     → dp[7]=dp[5]+1=4
 *   Answer: max(dp) = 4  ✓  (LIS: 2, 3, 7, 101 or 2, 3, 7, 18)
 *
 * Complexity:
 *   Time  : O(n²) — two nested loops
 *   Space : O(n)  — dp array of size n
 *
 * Comparison with memoization.cpp:
 *   Same O(n²) time but O(n) space instead of O(n²),
 *   because tabulation only needs dp[i] (not a 2D table).
 *   The 2D table in memoization comes from tracking both idx and prev;
 *   tabulation reframes the state as "LIS ending at i", needing only 1D.
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int LIS(vector<int>& nums) {
        int SIZE = nums.size();
        vector<int> dp(SIZE, 1);

        for (int i = 1; i < SIZE; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[j] < nums[i]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
        }

        return *max_element(dp.begin(), dp.end());
    }
};
