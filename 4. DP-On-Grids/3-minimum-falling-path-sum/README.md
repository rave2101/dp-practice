# Minimum Falling Path Sum

## Problem

Given an `R x C` integer matrix, return the **minimum sum** of any falling path through the matrix.

A falling path starts at **any cell in the first row** and ends at **any cell in the last row**. From cell `(i, j)` you can move to `(i+1, j)`, `(i+1, j-1)`, or `(i+1, j+1)` — straight down, down-left, or down-right.

---

## Example

```
        col=0  col=1  col=2
row=0: [  2,     1,     3  ]
row=1: [  6,     5,     4  ]
row=2: [  7,     8,     9  ]
```

Two paths tie for minimum (both = 13):

```
Path A: (0,1)=1 → (1,1)=5 → (2,0)=7   total = 13
Path B: (0,1)=1 → (1,2)=4 → (2,1)=8   total = 13
```

**Output: 13**

---

## The Core Insight

`dp[i][j]` = minimum cost to reach cell `(i, j)` from any cell in row 0.

A cell can only be reached from the row above — directly above or one diagonal:

```
dp[i][j] = matrix[i][j] + min(dp[i-1][j], dp[i-1][j-1], dp[i-1][j+1])
```

**Base case:** Row 0 — cost is just the cell value itself (`dp[0][j] = matrix[0][j]`).

**Answer:** `min(dp[R-1][j])` for all `j` — cheapest cell in the last row.

### dp table for the example

```
        col=0  col=1  col=2
row=0:    2      1      3      ← copied directly from matrix
row=1:    7      6      5      ← e.g. dp[1][0] = 6 + min(2, ∞, 1) = 7
row=2:   13     13     14      ← e.g. dp[2][1] = 8 + min(7, 6, 5) = 13

Answer = min(13, 13, 14) = 13
```

---

## Approach 1 — Memoization (Top-Down)

**Direction:** starts at each cell in the **last row**, recurses **upward** to row 0.  
**Fixed starting point:** variable — any column in the last row.  
**Variable ending point:** row 0, any column.

`util(i, j)` = minimum path cost to reach `(i, j)` from row 0.

Out-of-bounds columns return `1e5` (a large sentinel) so they're never chosen as the minimum.

```cpp
int util(vector<vector<int>>& matrix, vector<vector<int>>& dp, int i, int j, int R, int C) {
    if (j < 0 || j >= C) return 1e5;          // out-of-bounds column → invalid
    if (dp[i][j] != -1) return dp[i][j];       // already computed
    if (i == 0) return dp[i][j] = matrix[i][j]; // base case: row 0

    return dp[i][j] = matrix[i][j] + min({ util(matrix, dp, i-1, j,   R, C),
                                           util(matrix, dp, i-1, j-1, R, C),
                                           util(matrix, dp, i-1, j+1, R, C) });
}

int minFallingPathSum(vector<vector<int>>& matrix) {
    int R = matrix.size(), C = matrix[0].size();
    vector<vector<int>> dp(R, vector<int>(C, -1));
    int mini = INT_MAX;
    for (int j = 0; j < C; j++)
        mini = min(mini, util(matrix, dp, R-1, j, R, C));
    return mini;
}
```

**Time: O(R×C) | Space: O(R×C) + O(R) call stack**

---

## Recursion Tree Walkthrough

Using the 3×3 example. Entry: `util(2,0)`, `util(2,1)`, `util(2,2)` — one call per last-row column.

```
matrix = [[2, 1, 3],
           [6, 5, 4],
           [7, 8, 9]]
```

### util(2, 0)

```
util(2, 0)
├── util(1, 0)
│   ├── util(0, 0)  →  i==0, return dp[0][0] = 2
│   ├── util(1,-1)  →  j<0,  return 1e5
│   └── util(0, 1)  →  i==0, return dp[0][1] = 1
│   dp[1][0] = 6 + min(2, 1e5, 1) = 6 + 1 = 7
│
├── util(1,-1)  →  j<0, return 1e5
│
└── util(1, 1)
    ├── util(0, 0)  →  dp[0][0] cached = 2          ← MEMOIZED
    ├── util(0, 1)  →  dp[0][1] cached = 1          ← MEMOIZED
    └── util(0, 2)  →  i==0, return dp[0][2] = 3
    dp[1][1] = 5 + min(2, 1, 3) = 5 + 1 = 6

dp[2][0] = 7 + min(dp[1][0]=7, 1e5, dp[1][1]=6) = 7 + 6 = 13
```

### util(2, 1)

```
util(2, 1)
├── util(1, 0)  →  dp[1][0] cached = 7              ← MEMOIZED
├── util(1, 1)  →  dp[1][1] cached = 6              ← MEMOIZED
└── util(1, 2)
    ├── util(0, 1)  →  dp[0][1] cached = 1          ← MEMOIZED
    ├── util(0, 2)  →  dp[0][2] cached = 3          ← MEMOIZED
    └── util(1, 3)  →  j>=C, return 1e5
    dp[1][2] = 4 + min(1, 3, 1e5) = 4 + 1 = 5

dp[2][1] = 8 + min(dp[1][0]=7, dp[1][1]=6, dp[1][2]=5) = 8 + 5 = 13
```

### util(2, 2)

```
util(2, 2)
├── util(1, 1)  →  dp[1][1] cached = 6              ← MEMOIZED
├── util(1, 2)  →  dp[1][2] cached = 5              ← MEMOIZED
└── util(1, 3)  →  j>=C, return 1e5

dp[2][2] = 9 + min(dp[1][1]=6, dp[1][2]=5, 1e5) = 9 + 5 = 14
```

### Final answer

```
mini = min(dp[2][0]=13, dp[2][1]=13, dp[2][2]=14) = 13
```

### dp table after all calls

```
        col=0  col=1  col=2
row=0:    2      1      3      ← set by base case hits
row=1:    7      6      5      ← set by recursive returns
row=2:   13     13     14      ← set by top-level loop
```

Fill order (leaves before parents):
```
dp[0][0]=2 → dp[0][1]=1 → dp[1][0]=7 → dp[0][2]=3 → dp[1][1]=6 → dp[2][0]=13
           → dp[1][2]=5 → dp[2][1]=13 → dp[2][2]=14
```

---

## Why the recursion starts from the last row

The problem asks for the minimum path starting from **any** cell in row 0. Since the path ends at row R-1, we call `util(R-1, j)` for every `j` and take the minimum.

There is **no fixed ending column** — just "any cell in the last row" — so the recursion runs once per column in the last row and picks the best.

Alternatively, the recursion could start from row 0 with `util(0, j)` for each `j`, defining `util(i, j)` as "minimum cost from `(i,j)` to any cell in row R-1", with the base case at row R-1. Both directions are valid; Striver's version goes top-down (row 0 = base, row R-1 = entry).

---

## Approach 2 — Tabulation with Space Optimization

Fill row by row, top to bottom. Each row only needs the previous row, so we keep just one 1D array.

```cpp
int minFallingPathSum(vector<vector<int>>& matrix) {
    int R = matrix.size(), C = matrix[0].size();
    vector<int> prev(matrix[0]);             // row 0: base case
    for (int i = 1; i < R; i++) {
        vector<int> current(C);
        for (int j = 0; j < C; j++) {
            int up    = prev[j];
            int left  = (j > 0)   ? prev[j-1] : INT_MAX;
            int right = (j < C-1) ? prev[j+1] : INT_MAX;
            current[j] = matrix[i][j] + min({up, left, right});
        }
        prev = current;
    }
    return *min_element(prev.begin(), prev.end());
}
```

### Table fill for the example

```
prev    = [2, 1, 3]              ← row 0

i=1:
  j=0: current[0] = 6 + min(2, INT_MAX, 1) = 7
  j=1: current[1] = 5 + min(2, 1, 3)       = 6   (up=1, left=2, right=3 from prev)
  j=2: current[2] = 4 + min(1, 3, INT_MAX) = 5
  prev = [7, 6, 5]

i=2:
  j=0: current[0] = 7 + min(7, INT_MAX, 6) = 13
  j=1: current[1] = 8 + min(7, 6, 5)       = 13
  j=2: current[2] = 9 + min(6, 5, INT_MAX) = 14
  prev = [13, 13, 14]

return min(13, 13, 14) = 13
```

**Time: O(R×C) | Space: O(C)**

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(R×C) | O(R×C) + O(R) stack |
| Tabulation (space-optimized) | O(R×C) | O(C) |
