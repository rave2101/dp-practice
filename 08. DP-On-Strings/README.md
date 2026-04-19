# DP on Strings

Problems involving one or two strings, including Longest Common Subsequence, Edit Distance, Wildcard Matching, and related variants.

---

## Problems Covered

| # | Problem | Difficulty | Solution |
|---|---------|------------|----------|
| DP 25 | [Longest Common Subsequence](https://leetcode.com/problems/longest-common-subsequence/) | Medium | [solution.cpp](1-longest-common-subsequence/solution.cpp) |
| DP 26 | [Print LCS](https://www.geeksforgeeks.org/problems/print-lcs-sequence/1) | Medium | [solution.cpp](2-print-lcs/solution.cpp) |
| DP 27 | [Longest Common Substring](https://www.geeksforgeeks.org/problems/longest-common-substring1452/1) | Medium | [solution.cpp](3-longest-common-substring/solution.cpp) |
| DP 28 | [Longest Palindromic Subsequence](https://leetcode.com/problems/longest-palindromic-subsequence/) | Medium | [solution.cpp](4-longest-palindromic-subsequence/solution.cpp) |
| DP 29 | [Minimum Insertions to Make String Palindrome](https://leetcode.com/problems/minimum-insertion-steps-to-make-a-string-palindrome/) | Hard | [solution.cpp](5-minimum-insertions-palindrome/solution.cpp) |
| DP 30 | [Minimum Insertions & Deletions to Convert String](https://www.geeksforgeeks.org/problems/minimum-number-of-deletions-and-insertions/1) | Medium | [solution.cpp](6-minimum-insertions-deletions/solution.cpp) |
| DP 31 | [Shortest Common Supersequence](https://leetcode.com/problems/shortest-common-supersequence/) | Hard | [solution.cpp](7-shortest-common-supersequence/solution.cpp) |
| DP 32 | [Distinct Subsequences](https://leetcode.com/problems/distinct-subsequences/) | Hard | [solution.cpp](8-distinct-subsequences/solution.cpp) |
| DP 33 | [Edit Distance](https://leetcode.com/problems/edit-distance/) | Medium | [solution.cpp](9-edit-distance/solution.cpp) |
| DP 34 | [Wildcard Matching](https://leetcode.com/problems/wildcard-matching/) | Hard | [solution.cpp](10-wildcard-matching/solution.cpp) |

---

## Key Pattern

Most problems reduce to LCS or a variation of it:

```
dp[i][j] = answer for s1[0..i-1] and s2[0..j-1]

if s1[i-1] == s2[j-1]:  dp[i][j] = 1 + dp[i-1][j-1]
else:                    dp[i][j] = f(dp[i-1][j], dp[i][j-1])
```

The `else` branch changes per problem — max for LCS, min for Edit Distance, sum for Distinct Subsequences.

---

## Playlist

[Striver's DP Series — takeUforward](https://www.youtube.com/playlist?list=PLgUwDviBIf0qUlt5H_kiKYaNSqJ81PMMY)
