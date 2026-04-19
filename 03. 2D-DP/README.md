# 2D DP

Problems where the state depends on two variables, requiring a 2D table (or two 1D arrays) to store subproblem results.

---

## Problems Covered

| # | Problem | Difficulty | Video | Article | Solution |
|---|---------|------------|-------|---------|----------|
| DP 7 | [Ninja's Training](https://takeuforward.org/plus/dsa/problems/ninja's-training) | Medium | [YouTube](https://www.youtube.com/watch?v=AE39gJYuRog) | [TUF](https://takeuforward.org/data-structure/dynamic-programming-ninjas-training-dp-7) | [solution.cpp](ninja-training/solution.cpp) |

---

## Key Patterns

**Two-variable state**
```
dp[i][j] = f(dp[i-1][...])
```
The answer at row `i` depends on the previous row and some constraint carried forward (e.g. last activity, last column).

**Space optimization**
When `dp[i]` only depends on `dp[i-1]`, replace the full 2D table with two 1D arrays (`prev` and `current`), reducing space from O(n×m) to O(m).

---

## Playlist

[Striver's DP Series — takeUforward](https://www.youtube.com/playlist?list=PLgUwDviBIf0qUlt5H_kiKYaNSqJ81PMMY)
