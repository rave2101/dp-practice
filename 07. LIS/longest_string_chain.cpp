/*
 * Longest String Chain
 *
 * Problem:
 *   Given an array of words, return the length of the longest chain where
 *   each word is a predecessor of the next. wordA is a predecessor of wordB
 *   if you can insert exactly one letter anywhere in wordA (without changing
 *   the order of other characters) to get wordB.
 *
 * Key Insight — why sort by LENGTH (not lexicographically):
 *   A predecessor is always shorter than its successor by exactly 1.
 *   The DP requires all valid predecessors to have a lower index than the
 *   current word. Lexicographic sort breaks this — "ab" < "b" lexicographically,
 *   so "ab" gets a lower index than "b", and "b" → "ab" is never discovered.
 *
 *   Sorting by length guarantees: for any valid pair (wordA, wordB) where
 *   wordA is a predecessor of wordB, wordA always appears before wordB.
 *
 * compare(a, b) — is `a` a predecessor of `b`?
 *   b must be exactly 1 character longer than a: SIZE2 - SIZE1 != 1 → false.
 *
 *   Two-pointer approach: try to match all of `a` inside `b`.
 *   - If a[i] == b[j]: match found, advance both
 *   - If a[i] != b[j]: skip b[j] (this is the "inserted" character), advance only j
 *   If i reaches SIZE1, all of `a` was matched → `a` is a predecessor of `b`.
 *
 *   Example: a="ab", b="acb"  (insert 'c' in the middle)
 *     i=0,j=0: 'a'=='a' → i=1, j=1
 *     i=1,j=1: 'b'!='c' → j=2   (skip inserted 'c', keep i)
 *     i=1,j=2: 'b'=='b' → i=2, j=3
 *     i==SIZE1 → true  ✓
 *
 * ⚠️  Common Bug — advancing both i and j on mismatch:
 *   if(a[i]!=b[j]) mismatch++;
 *   i++; j++;           ← WRONG: skips one char from a too
 *
 *   This only works when the inserted character is at the very end.
 *   For mid-string insertions it counts 2 mismatches and returns false.
 *
 *   Fix: advance only j on mismatch (skip the inserted char in b, keep a's pointer).
 *
 * ⚠️  Common Bug — using SIZE2-SIZE1>=2 instead of SIZE2-SIZE1!=1:
 *   SIZE2-SIZE1>=2 fails to reject:
 *     - equal-length words (SIZE2-SIZE1 = 0)
 *     - cases where a is longer than b (SIZE2-SIZE1 is negative)
 *   Use != 1 to handle all invalid cases in one check.
 *
 * Approach (LIS pattern):
 *   dp[i] = length of longest chain ending at words[i]
 *   For each i, check all j < i: if words[j] is predecessor of words[i],
 *     dp[i] = max(dp[i], dp[j] + 1)
 *   Answer: max over all dp[i]
 *
 * Complexity:
 *   Time  : O(n² * L) — n² pairs, each compare is O(L) where L = max word length
 *   Space : O(n)      — dp array
 */

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool compare(string& a, string& b) {
        int SIZE1 = a.size(), SIZE2 = b.size(), i = 0, j = 0;
        if (SIZE2 - SIZE1 != 1) return false;
        while (i < SIZE1 && j < SIZE2) {
            if (a[i] == b[j]) i++;
            j++;
        }
        return i == SIZE1;
    }

    int longestStringChain(vector<string>& words) {
        int SIZE = words.size(), result = 1;

        // Sort by length so predecessors always appear before successors
        sort(words.begin(), words.end(), [](const string& a, const string& b) {
            return a.size() < b.size();
        });

        vector<int> dp(SIZE, 1);

        for (int i = 1; i < SIZE; i++) {
            for (int j = 0; j < i; j++) {
                if (compare(words[j], words[i])) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            result = max(result, dp[i]);
        }

        return result;
    }
};
