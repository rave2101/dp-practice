# DP on Grids

Problems that involve traversing a 2D grid to count paths, find minimum costs, or maximize values.

---

## Problems Covered

| # | Problem | Difficulty | Video | Article | Solution |
|---|---------|------------|-------|---------|----------|
| DP 8 | [Unique Paths](https://leetcode.com/problems/unique-paths/) | Medium | [YouTube](https://www.youtube.com/watch?v=sdE0A2Oxofw) | [TUF](https://takeuforward.org/data-structure/grid-unique-paths-dp-on-grids-dp8/) | [solution.cpp](1-unique-paths/solution.cpp) |
| DP 9 | [Unique Paths II](https://leetcode.com/problems/unique-paths-ii/) | Medium | [YouTube](https://www.youtube.com/watch?v=TmhpgXScLyY) | [TUF](https://takeuforward.org/data-structure/grid-unique-paths-2-dp-9/) | [solution.cpp](2-unique-paths-ii/solution.cpp) |
| DP 12 | [Minimum Falling Path Sum](https://leetcode.com/problems/minimum-falling-path-sum/) | Medium | [YouTube](https://www.youtube.com/watch?v=N_aJ5qQbYA0) | [TUF](https://takeuforward.org/data-structure/minimum-maximum-falling-path-sum-dp-12/) | [solution.cpp](3-minimum-falling-path-sum/solution.cpp) |
| DP 11 | [Triangle](https://leetcode.com/problems/triangle/) | Medium | [YouTube](https://www.youtube.com/watch?v=SrP-PiLSYC0) | [TUF](https://takeuforward.org/data-structure/minimum-path-sum-in-triangular-grid-dp-11/) | [solution.cpp](4-triangle/solution.cpp) |
| DP 13 | [Cherry Pickup II](https://leetcode.com/problems/cherry-pickup-ii/) | Hard | [YouTube](https://www.youtube.com/watch?v=QGfn7JeXK54) | [TUF](https://takeuforward.org/data-structure/3-d-dp-ninja-and-his-friends-dp-13/) | [solution.cpp](5-cherry-pickup-ii/solution.cpp) |

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
