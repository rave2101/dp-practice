# Cherry Pickup II

## Problem

Given an `R x C` integer matrix where `matrix[i][j]` is the number of cherries at cell `(i, j)`.

Two robots start simultaneously:
- **Robot 1** at `(0, 0)` — top-left
- **Robot 2** at `(0, C-1)` — top-right

From `(i, j)` each robot can move to `(i+1, j-1)`, `(i+1, j)`, or `(i+1, j+1)`.  
If both robots land on the same cell, cherries are counted **only once**.  
Both robots must reach the bottom row.

Return the **maximum total cherries** both robots can collect.

---

## Example

```
        col=0  col=1  col=2
row=0: [  3,     1,     1  ]   Robot1 starts at (0,0), Robot2 at (0,2)
row=1: [  2,     5,     1  ]
row=2: [  1,     5,     5  ]
```

```
Robot1: (0,0)=3 → (1,0)=2 → (2,1)=5   total = 10
Robot2: (0,2)=1 → (1,1)=5 → (2,2)=5   total = 11

No overlapping cells → grand total = 10 + 11 = 21
```

**Output: 21**

---

## The Core Insight

Both robots move **one row at a time, simultaneously**. So the row `i` is always the same for both — only their **columns** differ.

State: `f(i, j1, j2)` = max cherries collectible from row `i` to row `R-1`, given robot1 is at column `j1` and robot2 is at column `j2`.

```
f(i, j1, j2) = cherries(i, j1, j2) + max over all 9 move combos:
                   f(i+1, j1+dj1, j2+dj2)   where dj1, dj2 ∈ {-1, 0, 1}
```

```
cherries(i, j1, j2) = matrix[i][j1] + (j1 != j2 ? matrix[i][j2] : 0)
```

**Base case:** `i == R-1` → collect cherries at current positions, no further moves.

**Answer:** `f(0, 0, C-1)` — fixed starting positions for both robots.

---

## Why fixed starting point?

Both robots always begin at **known, fixed columns** at row 0 (`j1=0`, `j2=C-1`).  
The ending columns (row `R-1`) are **variable** — they depend on the choices made along the way.

So the recursion starts from the **fixed top** `(0, 0, C-1)` and the base case handles the **variable bottom** (any `j1`, `j2` combination in the last row).

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<vector<int>>& matrix, vector<vector<vector<int>>>& dp,
         int i, int j1, int j2, int R, int C) {
    if (j1 < 0 || j1 >= C || j2 < 0 || j2 >= C) return -1e8; // invalid column
    if (dp[i][j1][j2] != -1) return dp[i][j1][j2];
    if (i == R-1) {
        int value = matrix[i][j1];
        if (j1 != j2) value += matrix[i][j2];
        return dp[i][j1][j2] = value;
    }
    int maxi = INT_MIN;
    for (int dj1 = -1; dj1 <= 1; dj1++) {
        for (int dj2 = -1; dj2 <= 1; dj2++) {
            int value = matrix[i][j1];
            if (j1 != j2) value += matrix[i][j2];
            maxi = max(maxi, value + util(matrix, dp, i+1, j1+dj1, j2+dj2, R, C));
        }
    }
    return dp[i][j1][j2] = maxi;
}

int cherryPickup(vector<vector<int>>& matrix) {
    int R = matrix.size(), C = matrix[0].size();
    vector<vector<vector<int>>> dp(R, vector<vector<int>>(C, vector<int>(C, -1)));
    return util(matrix, dp, 0, 0, C-1, R, C);
}
```

**Time: O(R × C² × 9) | Space: O(R × C²) + O(R) call stack**

---

## Recursion Tree Walkthrough

```
        col=0  col=1  col=2
row=0: [  3,     1,     1  ]
row=1: [  2,     5,     1  ]
row=2: [  1,     5,     5  ]
```

Entry call: `util(0, 0, 2)` — robot1 at col=0, robot2 at col=2.

### Base case (row 2) — filled first

```
f(2,0,0)=1   f(2,0,1)=6   f(2,0,2)=6
f(2,1,0)=6   f(2,1,1)=5   f(2,1,2)=10
f(2,2,0)=6   f(2,2,1)=10  f(2,2,2)=5
```

### Row 1 calls (selected)

```
f(1, 0, 2)   value = matrix[1][0] + matrix[1][2] = 2+1 = 3
  dj1=0, dj2=-1 → f(2,0,1) = 6  →  3+6 = 9
  dj1=0, dj2= 0 → f(2,0,2) = 6  →  3+6 = 9
  dj1=1, dj2=-1 → f(2,1,1) = 5  →  3+5 = 8
  dj1=1, dj2= 0 → f(2,1,2) = 10 →  3+10 = 13  ← best
  (dj1=-1 → nj1=-1, invalid; dj2=1 → nj2=3, invalid)
  dp[1][0][2] = 13

f(1, 0, 1)   value = 2+5 = 7
  dj1=0, dj2=-1 → f(2,0,0) = 1  →  7+1  = 8
  dj1=0, dj2= 0 → f(2,0,1) = 6  →  7+6  = 13  ← MEMOIZED
  dj1=0, dj2= 1 → f(2,0,2) = 6  →  7+6  = 13  ← MEMOIZED
  dj1=1, dj2=-1 → f(2,1,0) = 6  →  7+6  = 13
  dj1=1, dj2= 0 → f(2,1,1) = 5  →  7+5  = 12  ← MEMOIZED
  dj1=1, dj2= 1 → f(2,1,2) = 10 →  7+10 = 17  ← MEMOIZED — best
  dp[1][0][1] = 17

f(1, 1, 2)   value = 5+1 = 6
  dj1=-1, dj2=-1 → f(2,0,1) = 6  →  6+6  = 12  ← MEMOIZED
  dj1=-1, dj2= 0 → f(2,0,2) = 6  →  6+6  = 12  ← MEMOIZED
  dj1= 0, dj2=-1 → f(2,1,1) = 5  →  6+5  = 11  ← MEMOIZED
  dj1= 0, dj2= 0 → f(2,1,2) = 10 →  6+10 = 16  ← MEMOIZED — best
  dj1= 1, dj2=-1 → f(2,2,1) = 10 →  6+10 = 16  ← MEMOIZED — best
  dj1= 1, dj2= 0 → f(2,2,2) = 5  →  6+5  = 11  ← MEMOIZED
  (dj2=1 → nj2=3, always invalid)
  dp[1][1][2] = 16

f(1, 1, 1)   value = 5 (j1==j2, count once)
  dj1= 0, dj2= 1 → f(2,1,2) = 10 →  5+10 = 15  ← best
  dj1= 1, dj2= 0 → f(2,2,1) = 10 →  5+10 = 15  ← best
  dp[1][1][1] = 15
```

### Root call

```
f(0, 0, 2)   value = matrix[0][0] + matrix[0][2] = 3+1 = 4
  dj1=-1 → nj1=-1, all 3 moves invalid
  dj1= 0, dj2=-1 → f(1,0,1) = 17 →  4+17 = 21  ← best
  dj1= 0, dj2= 0 → f(1,0,2) = 13 →  4+13 = 17
  dj1= 0, dj2=+1 → nj2=3, invalid
  dj1=+1, dj2=-1 → f(1,1,1) = 15 →  4+15 = 19
  dj1=+1, dj2= 0 → f(1,1,2) = 16 →  4+16 = 20
  dj1=+1, dj2=+1 → nj2=3, invalid

dp[0][0][2] = 21
```

**Answer: 21**

### Winning path decoded

`f(0,0,2)=21` came from `dj1=0, dj2=-1` → `f(1,0,1)=17`  
`f(1,0,1)=17` came from `dj1=1, dj2=1` → `f(2,1,2)=10`  

```
Robot1: col 0 → col 0 → col 1   cells: (0,0)=3, (1,0)=2, (2,1)=5  →  total 10
Robot2: col 2 → col 1 → col 2   cells: (0,2)=1, (1,1)=5, (2,2)=5  →  total 11
Grand total = 21  ✓
```

---

## Approach 2 — Tabulation (Bottom-Up)

**Direction:** memoization recurses top→bottom, so tabulation fills bottom→top.

**Step 1:** Initialize `dp[R-1]` — base case (last row).  
**Step 2:** Fill `dp[i]` using `dp[i+1]` for rows `R-2` down to `0`.  
**Answer:** `dp[0][0][C-1]`.

```cpp
int cherryPickup(vector<vector<int>>& matrix) {
    int R = matrix.size(), C = matrix[0].size();
    vector<vector<vector<int>>> dp(R, vector<vector<int>>(C, vector<int>(C, 0)));

    // Base case: last row
    for (int j1 = 0; j1 < C; j1++)
        for (int j2 = 0; j2 < C; j2++) {
            dp[R-1][j1][j2] = matrix[R-1][j1];
            if (j1 != j2) dp[R-1][j1][j2] += matrix[R-1][j2];
        }

    // Fill bottom-up
    for (int i = R-2; i >= 0; i--) {
        for (int j1 = 0; j1 < C; j1++) {
            for (int j2 = 0; j2 < C; j2++) {
                int maxi = INT_MIN;
                int value = matrix[i][j1];
                if (j1 != j2) value += matrix[i][j2];
                for (int dj1 = -1; dj1 <= 1; dj1++)
                    for (int dj2 = -1; dj2 <= 1; dj2++) {
                        int nj1 = j1 + dj1, nj2 = j2 + dj2;
                        if (nj1 >= 0 && nj1 < C && nj2 >= 0 && nj2 < C)
                            maxi = max(maxi, value + dp[i+1][nj1][nj2]);
                    }
                dp[i][j1][j2] = maxi;
            }
        }
    }

    return dp[0][0][C-1];
}
```

### Table fill for the example

```
--- Base case: dp[2] ---
       j2=0  j2=1  j2=2
j1=0: [  1,    6,    6  ]
j1=1: [  6,    5,   10  ]
j1=2: [  6,   10,    5  ]

--- dp[1] (using dp[2]) ---
       j2=0  j2=1  j2=2
j1=0: [  ?,   17,   13  ]    (j2=0: robot2 at col0 = valid but suboptimal paths)
j1=1: [  ?,   15,   16  ]
j1=2: [  ?,    ?,    ?  ]    (robot2 starts at C-1=2, robot1 can't reach col2 in 1 step from col0)

--- dp[0] (using dp[1]) ---
       j2=2
j1=0: [ 21 ]   ← dp[0][0][2] = answer
```

**Time: O(R × C² × 9) | Space: O(R × C²)**

---

## Why is dp sized `R x C x C` and not `R x C x R`?

Both parameters that change per robot are **column indices** — rows are always in sync (`i` is the same for both). So the three dimensions are:  
`dp[row][col_robot1][col_robot2]` — each column index ranges from `0` to `C-1`.

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(R × C² × 9) | O(R × C²) + O(R) stack |
| Tabulation | O(R × C² × 9) | O(R × C²) |
