# DP on Subsequences

Problems involving subsets or subsequences of an array, including knapsack variants, subset sum, and coin change problems.

---

## Problems Covered

| # | Problem | Difficulty | Solution |
|---|---------|------------|----------|
| DP 14 | [Subset Sum](https://www.geeksforgeeks.org/problems/subset-sum-problem-1611555638/1) | Medium | [solution.cpp](1-subset-sum/solution.cpp) |
| DP 15 | [Partition Equal Subset Sum](https://leetcode.com/problems/partition-equal-subset-sum/) | Medium | [solution.cpp](2-partition-equal-subset-sum/solution.cpp) |
| DP 16 | [Partition — Min Absolute Sum Difference](https://www.geeksforgeeks.org/problems/minimum-sum-partition3317/1) | Medium | [solution.cpp](3-partition-minimum-absolute-sum-difference/solution.cpp) |
| DP 17 | [Count Subsets with Sum K](https://www.geeksforgeeks.org/problems/perfect-sum-problem5633/1) | Medium | [solution.cpp](4-count-subsets-with-sum-k/solution.cpp) |
| DP 18 | [Count Partitions with Given Difference](https://www.geeksforgeeks.org/problems/partitions-with-given-difference/1) | Medium | [solution.cpp](5-count-partitions-with-given-difference/solution.cpp) |
| DP 19 | [0-1 Knapsack](https://www.geeksforgeeks.org/problems/0-1-knapsack-problem/1) | Medium | [solution.cpp](6-0-1-knapsack/solution.cpp) |
| DP 20 | [Coin Change — Minimum Coins](https://leetcode.com/problems/coin-change/) | Medium | [solution.cpp](7-coin-change/solution.cpp) |
| DP 21 | [Target Sum](https://leetcode.com/problems/target-sum/) | Medium | [solution.cpp](8-target-sum/solution.cpp) |
| DP 22 | [Coin Change II — Number of Ways](https://leetcode.com/problems/coin-change-ii/) | Medium | [solution.cpp](9-coin-change-ii/solution.cpp) |
| DP 23 | [Unbounded Knapsack](https://www.geeksforgeeks.org/problems/knapsack-with-duplicate-items4201/1) | Medium | [solution.cpp](10-unbounded-knapsack/solution.cpp) |
| DP 24 | [Rod Cutting](https://www.geeksforgeeks.org/problems/rod-cutting0840/1) | Medium | [solution.cpp](11-rod-cutting/solution.cpp) |

---

## Key Patterns

**0-1 Knapsack (each item used at most once)**
```
dp[i][w] = max(dp[i-1][w], val[i] + dp[i-1][w - wt[i]])
```

**Unbounded Knapsack (item can be reused)**
```
dp[i][w] = max(dp[i-1][w], val[i] + dp[i][w - wt[i]])
           ↑ previous row          ↑ same row (reuse allowed)
```

**Subset Sum / Count variants** reduce to knapsack with a boolean or count table instead of a max table.

---

## Playlist

[Striver's DP Series — takeUforward](https://www.youtube.com/playlist?list=PLgUwDviBIf0qUlt5H_kiKYaNSqJ81PMMY)
