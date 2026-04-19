# Unique Paths

## Problem

Given a grid of `m` rows and `n` columns, find the number of unique paths from the **top-left** `(0,0)` to the **bottom-right** `(m-1, n-1)`.

You can only move **right** or **down** at each step.

---

## Example

```
Input: m = 3, n = 2

Grid (3 rows, 2 columns):

  (0,0) → (0,1)
    ↓       ↓
  (1,0) → (1,1)
    ↓       ↓
  (2,0) → (2,1) ← destination
```

**3 unique paths:**
```
1) right → down → down:   (0,0)→(0,1)→(1,1)→(2,1)
2) down → right → down:   (0,0)→(1,0)→(1,1)→(2,1)
3) down → down → right:   (0,0)→(1,0)→(2,0)→(2,1)
```

**Output: 3**

---

## The Core Insight

To reach cell `(i, j)`, you must have come from either:
- `(i-1, j)` — the cell above (moved down)
- `(i, j-1)` — the cell to the left (moved right)

So the number of ways to reach `(i, j)` is:

```
dp[i][j] = dp[i-1][j] + dp[i][j-1]
```

**Base case:** Any cell in the first row or first column has exactly **1** way to reach it — only one direction is possible (all right or all down).

```
dp[0][j] = 1  for all j
dp[i][0] = 1  for all i
```

### dp table for m=3, n=2

```
       col=0  col=1
row=0:   1      1
row=1:   1      2     ← dp[1][1] = dp[0][1] + dp[1][0] = 1 + 1 = 2
row=2:   1      3     ← dp[2][1] = dp[1][1] + dp[2][0] = 2 + 1 = 3
```

Answer = `dp[2][1]` = **3**

---

## Approach 1 — Memoization (Top-Down)

Ask: *"how many ways can I reach (i, j)?"*  
Recurse from `(m-1, n-1)` toward `(0, 0)`.

```cpp
int util(int i, int j, vector<vector<int>>& dp) {
    if (i == 0 || j == 0) return 1;
    if (dp[i][j] != -1) return dp[i][j];
    return dp[i][j] = util(i - 1, j, dp) + util(i, j - 1, dp);
}

int uniquePaths(int m, int n) {
    vector<vector<int>> dp(m, vector<int>(n, -1));
    return util(m - 1, n - 1, dp);
}
```

**Time: O(m×n) | Space: O(m×n) + O(m+n) call stack**

---

## Recursion Tree Walkthrough

Using `m=3, n=2`. Entry call: `util(2, 1)`

```
util(2, 1)
├── util(1, 1)
│   ├── util(0, 1)
│   │   └── i==0 → return 1
│   │
│   └── util(1, 0)
│       └── j==0 → return 1
│
│   dp[1][1] = 1 + 1 = 2,  return 2
│
└── util(2, 0)
    └── j==0 → return 1

dp[2][1] = 2 + 1 = 3,  return 3
```

### Call stack at deepest point

```
util(2, 1)          ← waiting for util(1,1) + util(2,0)
  util(1, 1)        ← waiting for util(0,1) + util(1,0)
    util(0, 1)      ← base case: i==0, return 1  ✓
```

### dp table after all calls

```
       col=0  col=1
row=0:  -1     -1    ← base cases, returned directly without caching
row=1:  -1      2    ← dp[1][1] cached
row=2:  -1      3    ← dp[2][1] cached (answer)
```

### Why are row 0 and col 0 never cached?

The base case `if (i == 0 || j == 0) return 1` triggers before the cache check is ever needed — these cells are hit at most once per path in this small example. For larger grids they could be reached multiple times but since the answer is always `1`, it doesn't affect correctness, just a minor inefficiency.

---

## Approach 2 — Tabulation with Space Optimization

`dp[i][j]` only depends on the row above (`prev[j]`) and the cell to the left in the current row (`current[j-1]`). So we only need two 1D arrays.

```cpp
int uniquePaths(int m, int n) {
    vector<int> prev(n, 1);          // first row: all 1s
    for (int i = 1; i < m; i++) {
        vector<int> current(n, 1);   // first col: always 1
        for (int j = 1; j < n; j++) {
            current[j] = prev[j] + current[j - 1];
        }
        prev = current;
    }
    return prev[n - 1];
}
```

### Table fill for m=3, n=2

```
prev    = [1, 1]          ← row 0 initialized
i=1:
  current = [1, 1]        ← col 0 = 1
  j=1: current[1] = prev[1] + current[0] = 1 + 1 = 2
  prev = [1, 2]

i=2:
  current = [1, 1]
  j=1: current[1] = prev[1] + current[0] = 2 + 1 = 3
  prev = [1, 3]

return prev[1] = 3
```

**Time: O(m×n) | Space: O(n)**

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(m×n) | O(m×n) + O(m+n) stack |
| Tabulation (space-optimized) | O(m×n) | O(n) |
