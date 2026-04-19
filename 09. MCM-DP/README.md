# MCM DP (Matrix Chain Multiplication / Partition DP)

Problems that involve trying every split point `k` in a range `[i, j]`, computing the cost of both halves, and combining — identical in structure to Matrix Chain Multiplication.

---

## Problems Covered

| # | Problem | Difficulty | Solution |
|---|---------|------------|----------|
| DP 48 | [Matrix Chain Multiplication](https://www.geeksforgeeks.org/problems/matrix-chain-multiplication0303/1) | Hard | [solution.cpp](1-matrix-chain-multiplication/solution.cpp) |
| DP 49 | [Min Cost to Cut a Stick](https://leetcode.com/problems/minimum-cost-to-cut-a-stick/) | Hard | [solution.cpp](2-min-cost-to-cut-stick/solution.cpp) |
| DP 50 | [Burst Balloons](https://leetcode.com/problems/burst-balloons/) | Hard | [solution.cpp](3-burst-balloons/solution.cpp) |
| DP 51 | [Palindrome Partitioning II](https://leetcode.com/problems/palindrome-partitioning-ii/) | Hard | [solution.cpp](4-palindrome-partitioning/solution.cpp) |
| DP 52 | [Boolean Parenthesization](https://www.geeksforgeeks.org/problems/boolean-parenthesization5610/1) | Hard | [solution.cpp](5-boolean-parenthesization/solution.cpp) |
| —    | [Partition Array for Maximum Sum](https://leetcode.com/problems/partition-array-for-maximum-sum/) | Medium | [solution.cpp](6-partition-array-maximum-sum/solution.cpp) |

---

## Key Pattern

```
for k in [i+1, j-1]:          // try every split point
    cost = solve(i, k) + solve(k, j) + combine(i, k, j)
    answer = min/max(answer, cost)
```

The subproblems `[i, k]` and `[k, j]` overlap heavily across different split points, which is why memoization gives a polynomial solution over exponential brute force.

---

## Playlist

[Striver's DP Series — takeUforward](https://www.youtube.com/playlist?list=PLgUwDviBIf0qUlt5H_kiKYaNSqJ81PMMY)
