/*
 * Number of Longest Increasing Subsequences
 *
 * Problem:
 *   Given an array, return the number of distinct longest increasing
 *   subsequences (LIS). Multiple LIS of the same maximum length may exist.
 *
 * Why dp[] alone is not enough:
 *   dp[i] = length of LIS ending at index i — this only tells you the length,
 *   not how many distinct ways you can form that length.
 *
 *   Example: nums = [1, 3, 5, 4, 7]
 *     dp = [1, 2, 3, 3, 4]
 *     dp[4]=4 tells you the LIS length is 4, but there are 2 ways:
 *       [1, 3, 5, 7]  and  [1, 3, 4, 7]
 *     You need a count[] array to track this.
 *
 * count[] array:
 *   count[i] = number of distinct LIS of length dp[i] ending at index i.
 *   Initialized to 1 (every element is a valid LIS of length 1 by itself).
 *
 * How count[] gets updated when nums[j] < nums[i]:
 *
 *   Case 1: dp[j]+1 > dp[i]  →  new longer length found
 *     dp[i]    = dp[j]+1
 *     count[i] = count[j]       ← all ways to reach j now flow into i
 *
 *   Case 2: dp[j]+1 == dp[i]  →  another path reaches the same length
 *     count[i] += count[j]      ← accumulate additional ways
 *
 * Final answer:
 *   Sum count[i] for all i where dp[i] == maxLen.
 *   (multiple indices can be endpoints of a maxLen LIS)
 *
 * Example: nums = [1, 3, 5, 4, 7]
 *
 *   i=1 (3): j=0(1): dp[0]+1=2>1 → dp[1]=2, count[1]=1
 *   i=2 (5): j=1(3): dp[1]+1=3>1 → dp[2]=3, count[2]=1
 *   i=3 (4): j=1(3): dp[1]+1=3>1 → dp[3]=3, count[3]=1
 *   i=4 (7): j=2(5): dp[2]+1=4>1 → dp[4]=4, count[4]=count[2]=1
 *            j=3(4): dp[3]+1=4==4 → count[4]+=count[3]=1 → count[4]=2
 *
 *   dp    = [1, 2, 3, 3, 4]
 *   count = [1, 1, 1, 1, 2]
 *   maxLen=4, dp[4]==4 → result = count[4] = 2  ✓
 *   ([1,3,5,7] and [1,3,4,7])
 *
 * Complexity:
 *   Time  : O(n²) — two nested loops
 *   Space : O(n)  — dp and count arrays
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int numberOfLIS(vector<int> nums) {
        int SIZE = nums.size(), maxLen = 1, result = 0;
        vector<int> dp(SIZE, 1);
        vector<int> count(SIZE, 1);

        for (int i = 1; i < SIZE; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[j] < nums[i]) {
                    if (dp[j] + 1 > dp[i]) {
                        dp[i] = dp[j] + 1;
                        count[i] = count[j];      // new best — inherit count
                    } else if (dp[j] + 1 == dp[i]) {
                        count[i] += count[j];     // same length — add ways
                    }
                }
            }
            maxLen = max(maxLen, dp[i]);
        }

        for (int i = 0; i < SIZE; i++)
            if (dp[i] == maxLen) result += count[i];

        return result;
    }
};
