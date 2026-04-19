# Unique Paths II

## Problem

Given an `m x n` grid where each cell is either `0` (open) or `1` (blocked), find the number of unique paths from **top-left** `(0,0)` to **bottom-right** `(m-1, n-1)`.

You can only move **right** or **down**. No path can pass through a blocked cell (`1`).

---

## Examples

**Example 1**
```
matrix = [[0, 0, 0],
          [0, 1, 0],
          [0, 0, 0]]
```
```
  (0,0) → (0,1) → (0,2)
    ↓        X       ↓
  (1,0)   (1,1)   (1,2)
    ↓               ↓
  (2,0) → (2,1) → (2,2)
```
```
Path 1: right → right → down → down
Path 2: down → down → right → right
```
**Output: 2**

**Example 2**
```
matrix = [[0, 0, 0],
          [0, 0, 1],
          [0, 1, 0]]
```
Both routes to `(2,2)` are blocked. **Output: 0**

---

## The Core Insight

Same recurrence as Unique Paths I, with one extra rule — if a cell is blocked, it contributes 0 paths:

```
dp[i][j] = 0                          if matrix[i][j] == 1
dp[i][j] = dp[i-1][j] + dp[i][j-1]   otherwise
```

**Base case:** `(0,0)` = 1 if open, 0 if blocked (handled by the edge-case guard).

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<vector<int>>& matrix, vector<vector<int>>& dp, int i, int j) {
    if (i < 0 || j < 0 || matrix[i][j] == 1) return 0;
    if (i == 0 && j == 0) return 1;
    if (dp[i][j] != -1) return dp[i][j];
    return dp[i][j] = util(matrix, dp, i-1, j) + util(matrix, dp, i, j-1);
}

int uniquePathsWithObstacles(vector<vector<int>>& matrix) {
    int R = matrix.size(), C = matrix[0].size();
    if (matrix[0][0] == 1 || matrix[R-1][C-1] == 1) return 0;
    vector<vector<int>> dp(R, vector<int>(C, -1));
    return util(matrix, dp, R-1, C-1);
}
```

**Time: O(m×n) | Space: O(m×n) + O(m+n) call stack**

---

## Recursion Tree Walkthrough

Using Example 1: `matrix = [[0,0,0],[0,1,0],[0,0,0]]`

Entry call: `util(2, 2)`

```
util(2, 2)
├── util(1, 2)
│   ├── util(0, 2)
│   │   ├── util(-1, 2) → i<0, return 0
│   │   └── util(0, 1)
│   │       ├── util(-1, 1) → i<0, return 0
│   │       └── util(0, 0) → base case, return 1
│   │       dp[0][1] = 0 + 1 = 1,  return 1
│   │   dp[0][2] = 0 + 1 = 1,  return 1
│   │
│   └── util(1, 1) → matrix[1][1] == 1, return 0
│
│   dp[1][2] = 1 + 0 = 1,  return 1
│
└── util(2, 1)
    ├── util(1, 1) → matrix[1][1] == 1, return 0   ← OBSTACLE
    └── util(2, 0)
        ├── util(1, 0)
        │   ├── util(0, 0) → base case, return 1
        │   └── util(1, -1) → j<0, return 0
        │   dp[1][0] = 1 + 0 = 1,  return 1
        └── util(2, -1) → j<0, return 0
        dp[2][0] = 1 + 0 = 1,  return 1
    dp[2][1] = 0 + 1 = 1,  return 1

dp[2][2] = 1 + 1 = 2,  return 2
```

### dp table after all calls

```
       col=0  col=1  col=2
row=0:  -1     1      1     ← (0,0) is base case, never cached
row=1:   1     0*     1     ← 0* = obstacle, returned directly without caching
row=2:   1     1      2     ← answer
```

### How the obstacle blocks paths

`util(1,1)` returns `0` immediately due to `matrix[i][j] == 1`. Any call that tries to go through `(1,1)` gets 0 — effectively cutting off all paths through that cell.

---

## Approach 2 — Tabulation with Space Optimization

```cpp
int uniquePathsWithObstacles(vector<vector<int>>& matrix) {
    int R = matrix.size(), C = matrix[0].size();
    if (matrix[0][0] == 1 || matrix[R-1][C-1] == 1) return 0;
    vector<int> prev(C, 0);
    prev[0] = 1;
    for (int j = 1; j < C; j++)
        prev[j] = matrix[0][j] == 1 ? 0 : prev[j-1];
    for (int i = 1; i < R; i++) {
        vector<int> current(C, 0);
        current[0] = matrix[i][0] == 1 ? 0 : prev[0];
        for (int j = 1; j < C; j++)
            current[j] = matrix[i][j] == 1 ? 0 : current[j-1] + prev[j];
        prev = current;
    }
    return prev[C-1];
}
```

### Table fill for Example 1

```
prev    = [1, 1, 1]       ← row 0: prev[0]=1, no obstacles

i=1:
  current[0] = prev[0] = 1
  j=1: matrix[1][1]==1 → current[1] = 0
  j=2: matrix[1][2]==0 → current[2] = current[1] + prev[2] = 0 + 1 = 1
  prev = [1, 0, 1]

i=2:
  current[0] = prev[0] = 1
  j=1: matrix[2][1]==0 → current[1] = current[0] + prev[1] = 1 + 0 = 1
  j=2: matrix[2][2]==0 → current[2] = current[1] + prev[2] = 1 + 1 = 2
  prev = [1, 1, 2]

return prev[2] = 2
```

**Time: O(m×n) | Space: O(n)**

---

## Difference from Unique Paths I

| | Unique Paths I | Unique Paths II |
|---|---|---|
| Grid values | all open | 0 = open, 1 = blocked |
| Extra check | none | `matrix[i][j]==1 → return 0` |
| Edge case | none | guard if start/end blocked |

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(m×n) | O(m×n) + O(m+n) stack |
| Tabulation (space-optimized) | O(m×n) | O(n) |
