# Triangle — Minimum Falling Path Sum

## Problem

Given a triangular 2D array where row `i` has `i+1` elements, return the **minimum path sum** from the top to the bottom.

From cell `(i, j)` you can only move to `(i+1, j)` (bottom) or `(i+1, j+1)` (bottom-right).

---

## Example

```
      [2]           row 0
     [3, 4]         row 1
    [6, 5, 7]       row 2
   [4, 1, 8, 3]     row 3
```

```
Winning path: (0,0)=2 → (1,0)=3 → (2,1)=5 → (3,1)=1
Total = 2 + 3 + 5 + 1 = 11
```

**Output: 11**

---

## The Core Insight

`util(i, j)` = minimum path sum starting from `(i, j)` down to the last row.

From `(i, j)` you can go to `(i+1, j)` or `(i+1, j+1)`, so:

```
util(i, j) = triangle[i][j] + min(util(i+1, j), util(i+1, j+1))
```

**Base case:** `i == R-1` (last row) → return `triangle[i][j]` directly.

**Answer:** `util(0, 0)` — the single entry point at the top.

---

## Why fixed start, variable end?

- **Fixed start:** always `(0, 0)` — the top of the triangle is a single cell, no choice.
- **Variable end:** the path can land on any cell in the last row — the recursion naturally reaches whichever cell minimizes the sum.

This is why the recursion starts from `(0, 0)` and the base case is at the last row (`i == R-1`), opposite to Minimum Falling Path Sum where any last-row cell was the entry point.

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<vector<int>>& triangle, vector<vector<int>>& dp, int i, int j, int R) {
    if (i == R-1) return triangle[i][j];   // base case: last row
    if (dp[i][j] != -1) return dp[i][j];  // already computed
    return dp[i][j] = triangle[i][j] + min(util(triangle, dp, i+1, j,   R),
                                           util(triangle, dp, i+1, j+1, R));
}

int minTriangleSum(vector<vector<int>>& triangle) {
    int R = triangle.size();
    vector<vector<int>> dp(R, vector<int>(R, -1));  // R×R — last row has R elements
    return util(triangle, dp, 0, 0, R);
}
```

**Time: O(R²) | Space: O(R²) + O(R) call stack**

---

## Recursion Tree Walkthrough

```
      [2]
     [3, 4]
    [6, 5, 7]
   [4, 1, 8, 3]
```

Entry call: `util(0, 0)`

```
util(0, 0)
├── util(1, 0)
│   ├── util(2, 0)
│   │   ├── util(3, 0)  →  base case, return 4
│   │   └── util(3, 1)  →  base case, return 1
│   │   dp[2][0] = 6 + min(4, 1) = 7
│   │
│   └── util(2, 1)
│       ├── util(3, 1)  →  dp[3][1] cached = 1          ← MEMOIZED
│       └── util(3, 2)  →  base case, return 8
│       dp[2][1] = 5 + min(1, 8) = 6
│
│   dp[1][0] = 3 + min(dp[2][0]=7, dp[2][1]=6) = 3 + 6 = 9
│
└── util(1, 1)
    ├── util(2, 1)  →  dp[2][1] cached = 6              ← MEMOIZED
    └── util(2, 2)
        ├── util(3, 2)  →  dp[3][2] cached = 8          ← MEMOIZED
        └── util(3, 3)  →  base case, return 3
        dp[2][2] = 7 + min(8, 3) = 10

    dp[1][1] = 4 + min(dp[2][1]=6, dp[2][2]=10) = 4 + 6 = 10

dp[0][0] = 2 + min(dp[1][0]=9, dp[1][1]=10) = 2 + 9 = 11
```

**Answer: 11**

### dp table after all calls

```
        j=0   j=1   j=2   j=3
row=0:   11    -1    -1    -1
row=1:    9    10    -1    -1
row=2:    7     6    10    -1
row=3:    4     1     8     3   ← base cases, returned directly
```

Row 3 values are returned but not written into dp (base case returns before the cache write).  
Fill order (leaves first, root last):

```
(3,0)=4 → (3,1)=1 → (2,0)=7 → (3,2)=8 → (2,1)=6 → (1,0)=9
        → (3,3)=3 → (2,2)=10 → (1,1)=10 → (0,0)=11
```

---

## Approach 2 — Tabulation with Space Optimization

Fill bottom-up. Start with the last row, then for each row above, replace `dp[j]` with the best choice going forward.

Because row `i` has fewer elements than row `i+1`, `dp[j+1]` (the bottom-right neighbor) always exists in the array when needed.

```cpp
int minTriangleSum(vector<vector<int>>& triangle) {
    int R = triangle.size();
    vector<int> dp = triangle[R-1];      // start with last row
    for (int i = R-2; i >= 0; i--) {
        for (int j = 0; j < (int)triangle[i].size(); j++) {
            dp[j] = triangle[i][j] + min(dp[j], dp[j+1]);
        }
    }
    return dp[0];
}
```

### Table fill for the example

```
dp = [4, 1, 8, 3]      ← row 3 (initial)

i=2  (row [6, 5, 7]):
  j=0: dp[0] = 6 + min(dp[0]=4, dp[1]=1) = 7
  j=1: dp[1] = 5 + min(dp[1]=1, dp[2]=8) = 6
  j=2: dp[2] = 7 + min(dp[2]=8, dp[3]=3) = 10
dp = [7, 6, 10, 3]

i=1  (row [3, 4]):
  j=0: dp[0] = 3 + min(dp[0]=7, dp[1]=6) = 9
  j=1: dp[1] = 4 + min(dp[1]=6, dp[2]=10) = 10
dp = [9, 10, 10, 3]

i=0  (row [2]):
  j=0: dp[0] = 2 + min(dp[0]=9, dp[1]=10) = 11
dp = [11, 10, 10, 3]

return dp[0] = 11
```

Why can we reuse the same array in-place? Row `i` has `i+1` elements. When computing `dp[j]` for row `i`, we need `dp[j]` and `dp[j+1]` from the row below — both are still untouched since we fill left to right and `j+1 > j`.

**Time: O(R²) | Space: O(R)**

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(R²) | O(R²) + O(R) stack |
| Tabulation (space-optimized) | O(R²) | O(R) |
