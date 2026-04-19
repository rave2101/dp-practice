/*
 * Longest Increasing Subsequence — Tabulation (Bottom-Up DP) + Reconstruction
 *
 * Approach:
 *   dp[i]     = length of LIS ending at index i
 *   parent[i] = index of the previous element in the LIS ending at i
 *               (initialized to i itself → means "no predecessor")
 *
 *   For every i, look back at all j < i:
 *     if nums[j] < nums[i] && dp[j]+1 > dp[i]:
 *       dp[i]     = dp[j] + 1   (extend the LIS ending at j)
 *       parent[i] = j            (remember where we came from)
 *
 *   Base case: dp[i] = 1, parent[i] = i  (every element is its own LIS)
 *
 *   Track maxIdx = index where the overall LIS ends (needed to backtrack).
 *
 * Reconstruction (backtracking):
 *   Start at maxIdx, follow parent[] until parent[i] == i (the start).
 *   Collect elements in reverse, then reverse the result.
 *
 * Example: nums = [2, 5, 3, 7]
 *   After DP:
 *     dp     = [1, 2, 2, 3]
 *     parent = [0, 0, 0, 1]   ← parent[3]=1 means 7 came after 5
 *
 *   Backtrack from maxIdx=3:
 *     i=3 → push 7, move to parent[3]=1
 *     i=1 → push 5, move to parent[1]=0
 *     i=0 → push 2, parent[0]==0 → stop
 *   Collected: [7, 5, 2] → reverse → [2, 5, 7]  ✓
 *
 * ⚠️  BUG to avoid — initialize maxLen=1, maxIdx=0 (NOT maxLen=0):
 *   The loop starts at i=1, so index 0 is never compared against maxLen.
 *   If maxLen=0, the first update happens at i=1, making index 0 unreachable
 *   as maxIdx. For a fully decreasing input like [10,8,6,3], every dp[i]=1
 *   and maxIdx would wrongly settle at index 1 instead of 0.
 *   Fix: treat index 0 as the default answer (maxLen=1, maxIdx=0).
 *
 * Complexity:
 *   Time  : O(n²) — two nested loops
 *   Space : O(n)  — dp and parent arrays of size n
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> longestIncreasingSubsequence(vector<int>& nums) {
        int SIZE = nums.size(), maxLen = 1, maxIdx = 0;
        vector<int> dp(SIZE, 1);
        vector<int> parent(SIZE);
        vector<int> result;

        for (int i = 0; i < SIZE; i++) parent[i] = i;

        for (int i = 1; i < SIZE; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[i] > nums[j] && dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                    parent[i] = j;
                }
            }
            if (dp[i] > maxLen) {
                maxLen = dp[i];
                maxIdx = i;
            }
        }

        // Backtrack from maxIdx to reconstruct the LIS
        int i = maxIdx;
        while (parent[i] != i) {
            result.push_back(nums[i]);
            i = parent[i];
        }
        result.push_back(nums[i]);

        reverse(result.begin(), result.end());
        return result;
    }
};
