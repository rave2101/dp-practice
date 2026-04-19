# DP on Rectangles

Problems involving 2D binary matrices where the goal is to find or count rectangles/squares of all-1s.

---

## Problems Covered

| # | Problem | Difficulty | Solution |
|---|---------|------------|----------|
| — | [Maximal Rectangle](https://leetcode.com/problems/maximal-rectangle/) | Hard | [solution.cpp](1-maximal-rectangle/solution.cpp) |
| — | [Count Squares in a Binary Matrix](https://leetcode.com/problems/count-square-submatrices-with-all-ones/) | Medium | [solution.cpp](2-count-squares/solution.cpp) |

---

## Key Patterns

**Maximal Rectangle** — reduce each row to a histogram of heights (consecutive 1s from top), then run Largest Rectangle in Histogram on each row.

**Count Squares** — `dp[i][j]` = side length of the largest square with bottom-right corner at `(i, j)`. The recurrence `min(top, left, diagonal) + 1` captures the bottleneck constraint. Each `dp[i][j]` also directly counts how many new squares are introduced at that cell.
