# 1D DP

Problems where the state depends on a single index, solved using a 1D array or a few variables.

---

## Problems Covered

| # | Problem | Difficulty | Video | Article | Solution |
|---|---------|------------|-------|---------|----------|
| DP 2 | [Climbing Stairs](https://leetcode.com/problems/climbing-stairs/) | Easy | [YouTube](https://youtu.be/mLfjzJsN8us) | [TUF](https://takeuforward.org/data-structure/dynamic-programming-climbing-stairs) | [solution.cpp](climbing-stairs/solution.cpp) |
| DP 3 | [Frog Jump](https://www.geeksforgeeks.org/problems/geek-jump/1) | Medium | [YouTube](https://www.youtube.com/watch?v=EgG3jsGoPvQ) | [TUF](https://takeuforward.org/data-structure/dynamic-programming-frog-jump-dp-3) | [solution.cpp](frog-jump/solution.cpp) |
| DP 4 | [Frog Jump — K Distances](https://www.geeksforgeeks.org/problems/minimal-cost/1) | Medium | [YouTube](https://www.youtube.com/watch?v=Kmh3rhyEtB8) | [TUF](https://takeuforward.org/data-structure/dynamic-programming-frog-jump-with-k-distances-dp-4) | [solution.cpp](frog-jump-k/solution.cpp) |
| DP 5 | [Max Sum of Non-Adjacent Elements](https://www.geeksforgeeks.org/problems/max-sum-without-adjacents/1) | Medium | [YouTube](https://www.youtube.com/watch?v=GrMBfJNk_NY) | [TUF](https://takeuforward.org/data-structure/maximum-sum-of-non-adjacent-elements-dp-5) | [solution.cpp](max-sum-non-adjacent/solution.cpp) |
| DP 6 | [House Robber](https://leetcode.com/problems/house-robber/) / [House Robber II](https://leetcode.com/problems/house-robber-ii/) | Medium | [YouTube](https://www.youtube.com/watch?v=3WaxQMELSkw) | [TUF](https://takeuforward.org/data-structure/dynamic-programming-house-robber-dp-6) | [solution.cpp](house-robber/solution.cpp) |

---

## Key Patterns

**Single index recurrence**
```
dp[i] = f(dp[i-1], dp[i-2], ...)
```
Every problem here reduces to: at index `i`, make a choice and recurse/look back.

**Optimization path**
```
Recursion → Memoization → Tabulation → Space-Optimized
```
Each step removes overhead from the previous. Most 1D DP problems reach O(1) space.

---

## Playlist

[Striver's DP Series — takeUforward](https://www.youtube.com/playlist?list=PLgUwDviBIf0qUlt5H_kiKYaNSqJ81PMMY)
