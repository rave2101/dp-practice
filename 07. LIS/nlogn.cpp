/*
 * Longest Increasing Subsequence — O(n log n) Patience Sorting (Binary Search)
 *
 * Approach: Maintain a `result` array where result[i] is the smallest possible
 * tail element of any increasing subsequence of length i+1.
 *
 * For each number:
 *   - If it's greater than the last element → append (extends LIS length)
 *   - Otherwise → find the first element >= current using lower_bound
 *                  and replace it (keeps tails as small as possible)
 *
 * Key insight: `result` is always sorted, so binary search (lower_bound) works.
 *              `result` does NOT store the actual LIS — only its length is valid.
 *
 * Complexity:
 *   Time  : O(n log n) — n elements, each with O(log n) binary search
 *   Space : O(n)       — result array of at most n elements
 *
 * Example: nums = [10, 9, 2, 5, 3, 7, 101, 18]
 *   num=10  → result=[10]
 *   num=9   → replace 10 → result=[9]
 *   num=2   → replace 9  → result=[2]
 *   num=5   → append     → result=[2, 5]
 *   num=3   → replace 5  → result=[2, 3]
 *   num=7   → append     → result=[2, 3, 7]
 *   num=101 → append     → result=[2, 3, 7, 101]
 *   num=18  → replace 101→ result=[2, 3, 7, 18]
 *   Answer: result.size() = 4 ✓
 *
 * See README.md for detailed explanation of lower_bound.
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int LIS(vector<int>& nums) {
        int SIZE = nums.size();
        vector<int> result;
        result.push_back(nums[0]);

        for (int idx = 1; idx < SIZE; idx++) {
            if (nums[idx] > result.back()) {
                // Extends the longest subsequence found so far
                result.push_back(nums[idx]);
            } else {
                // Find the first element >= nums[idx] and replace it
                // This keeps tails as small as possible for future elements
                int i = lower_bound(result.begin(), result.end(), nums[idx]) - result.begin();
                result[i] = nums[idx];
            }
        }
        return result.size();
    }
};
