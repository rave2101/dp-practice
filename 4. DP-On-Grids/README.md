# DP on Grids

Problems that involve traversing a 2D grid to count paths, find minimum costs, or maximize values.

---

## Problems Covered

| # | Problem | Difficulty | Video | Article | Solution |
|---|---------|------------|-------|---------|----------|
| DP 8 | [Unique Paths](https://leetcode.com/problems/unique-paths/) | Medium | [YouTube](https://www.youtube.com/watch?v=sdE0A2Oxofw) | [TUF](https://takeuforward.org/data-structure/grid-unique-paths-dp-on-grids-dp8/) | [solution.cpp](unique-paths/solution.cpp) |

---

## Key Patterns

**Grid recurrence**
```
dp[i][j] = f(dp[i-1][j], dp[i][j-1])
```
Each cell depends only on the cell above and the cell to the left.

**Space optimization**
When `dp[i][j]` only depends on the previous row, replace the full 2D table with two 1D arrays (`prev` and `current`), reducing space from O(m×n) to O(n).

---

## Playlist

[Striver's DP Series — takeUforward](https://www.youtube.com/playlist?list=PLgUwDviBIf0qUlt5H_kiKYaNSqJ81PMMY)
