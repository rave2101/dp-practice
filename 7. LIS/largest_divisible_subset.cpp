/*
 * Largest Divisible Subset
 *
 * Problem:
 *   Given an array of positive integers, find the largest subset such that
 *   every pair (a, b) satisfies a % b == 0 or b % a == 0.
 *
 * Key Insight — why sorting helps:
 *   If we sort the array, then for any pair (nums[j], nums[i]) where j < i,
 *   nums[j] <= nums[i]. So the divisibility condition simplifies to just:
 *     nums[i] % nums[j] == 0
 *   We never need to check both directions after sorting.
 *
 *   Also: divisibility is transitive on a sorted array.
 *   If nums[i] % nums[j] == 0 and nums[j] % nums[k] == 0,
 *   then nums[i] % nums[k] == 0 automatically.
 *   → We only need to check nums[i] % nums[j] == 0 (not every pair).
 *
 * Approach (same structure as LIS tabulation):
 *   dp[i]     = length of largest divisible subset ending at index i
 *   parent[i] = index of the previous element in the subset ending at i
 *
 *   For every i, look back at all j < i:
 *     if nums[i] % nums[j] == 0 && dp[j]+1 > dp[i]:
 *       dp[i]     = dp[j] + 1
 *       parent[i] = j
 *
 *   Track maxIdx to backtrack and reconstruct the actual subset.
 *
 * Example: nums = [3, 5, 10, 20]  (already sorted)
 *   i=1 (5):  no j where 5%nums[j]==0         → dp[1]=1
 *   i=2 (10): 10%5==0 → dp[2]=dp[1]+1=2       → dp[2]=2, parent[2]=1
 *   i=3 (20): 20%5==0 → dp[3]=dp[1]+1=2
 *             20%10==0 → dp[3]=dp[2]+1=3       → dp[3]=3, parent[3]=2
 *
 *   dp     = [1, 1, 2, 3]
 *   parent = [0, 1, 1, 2]
 *   maxIdx = 3
 *
 *   Backtrack: 3→2→1, parent[1]==1 stop
 *   Collected: [20, 10, 5] → reverse → [5, 10, 20]  ✓
 *
 * Complexity:
 *   Time  : O(n² + n log n) — sorting + two nested loops
 *   Space : O(n)            — dp and parent arrays
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> largestDivisibleSubset(vector<int> nums) {
        int SIZE = nums.size(), maxLen = 1, maxIdx = 0;
        sort(nums.begin(), nums.end());

        vector<int> dp(SIZE, 1);
        vector<int> parent(SIZE);
        vector<int> result;

        for (int i = 0; i < SIZE; i++) parent[i] = i;

        for (int i = 1; i < SIZE; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[i] % nums[j] == 0 && dp[i] < dp[j] + 1) {
                    dp[i] = dp[j] + 1;
                    parent[i] = j;
                }
            }
            if (maxLen < dp[i]) {
                maxLen = dp[i];
                maxIdx = i;
            }
        }

        // Backtrack from maxIdx to reconstruct the subset
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
