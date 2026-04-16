/*
 * Longest Bitonic Sequence
 *
 * Problem:
 *   Given an array, find the length of the longest bitonic subsequence.
 *   A bitonic subsequence first strictly increases then strictly decreases.
 *   A purely increasing or purely decreasing sequence also counts as bitonic.
 *
 * Approach — Two LIS passes:
 *   For each index i, a bitonic subsequence through i has two parts:
 *     - An increasing subsequence ending   at i  (i is the peak)
 *     - A decreasing subsequence starting  at i  (i is the peak)
 *
 *   Run LIS twice:
 *     dp1[i] = LIS length ending   at i  (left to right)
 *     dp2[i] = LIS length starting at i  (right to left = LIS on reversed array)
 *
 *   For each i:  bitonic length through i = dp1[i] + dp2[i] - 1
 *                                           (subtract 1 because i is counted twice)
 *
 * Example: arr = [1, 11, 2, 10, 4, 5, 2, 1]
 *
 *   dp1 (LIS ending at i, left→right):
 *     i=0 (1):  dp1=[1,...]
 *     i=1 (11): 11>1  → dp1[1]=2
 *     i=2 (2):  2>1   → dp1[2]=2
 *     i=3 (10): 10>1,2→ dp1[3]=3
 *     i=4 (4):  4>1,2 → dp1[4]=3
 *     i=5 (5):  5>1,2,4→dp1[5]=4
 *     i=6 (2):  2>1   → dp1[6]=2
 *     i=7 (1):  none  → dp1[7]=1
 *     dp1 = [1, 2, 2, 3, 3, 4, 2, 1]
 *
 *   dp2 (LIS starting at i, right→left):
 *     i=7 (1):  dp2=[...,1]
 *     i=6 (2):  2>1  → dp2[6]=2
 *     i=5 (5):  5>2,1→ dp2[5]=3
 *     i=4 (4):  4>2,1→ dp2[4]=3
 *     i=3 (10): 10>5,4,2,1→dp2[3]=5
 *     i=2 (2):  2>1  → dp2[2]=2
 *     i=1 (11): 11>all→dp2[1]=6
 *     i=0 (1):  none → dp2[0]=1
 *     dp2 = [1, 6, 2, 5, 3, 3, 2, 1]
 *
 *   dp1+dp2-1 = [1, 7, 3, 7, 5, 6, 3, 1]
 *   Answer: max = 7  ✓  (subsequence: 1, 2, 10, 5, 2, 1  or  1, 11, 10, 5, 2, 1)
 *
 * Why subtract 1?
 *   dp1[i] counts i as the last element of the increasing part.
 *   dp2[i] counts i as the first element of the decreasing part.
 *   So index i is counted in both — subtract 1 to avoid double-counting the peak.
 *
 * Complexity:
 *   Time  : O(n²) — two LIS passes, each O(n²)
 *   Space : O(n)  — dp1 and dp2 arrays
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int LongestBitonicSequence(vector<int> arr) {
        int SIZE = arr.size(), result = 0;
        vector<int> dp1(SIZE, 1); // LIS ending at i   (left → right)
        vector<int> dp2(SIZE, 1); // LIS starting at i (right → left)

        // Forward pass: LIS ending at each index
        for (int i = 1; i < SIZE; i++) {
            for (int j = 0; j < i; j++) {
                if (arr[i] > arr[j]) {
                    dp1[i] = max(dp1[i], dp1[j] + 1);
                }
            }
        }

        // Backward pass: LIS starting at each index (= LIS on reversed array)
        for (int i = SIZE - 2; i >= 0; i--) {
            for (int j = SIZE - 1; j > i; j--) {
                if (arr[i] > arr[j]) {
                    dp2[i] = max(dp2[i], dp2[j] + 1);
                }
            }
        }

        // Peak at i: increasing part (dp1[i]) + decreasing part (dp2[i]) - 1 (peak counted twice)
        for (int i = 0; i < SIZE; i++) {
            result = max(result, dp1[i] + dp2[i] - 1);
        }

        return result;
    }
};
