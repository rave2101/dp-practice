# Rod Cutting

## Problem

Given a rod of length `N` inches and an array `price[]` where `price[i-1]` is the value of a piece of length `i` (1-based), find the **maximum value** by cutting the rod into pieces and selling them.

Any number of cuts can be made, or none at all. The same length can be cut multiple times.

---

## Examples

```
price = [1, 6, 8, 9, 10, 19, 7, 20],  N = 8

Cut lengths 2 and 6 → price[1] + price[5] = 6 + 19 = 25
Output: 25
```

```
price = [1, 5, 8, 9],  N = 4

Cut lengths 2 and 2 → price[1] + price[1] = 5 + 5 = 10
Output: 10
```

---

## Connection to Unbounded Knapsack

Rod Cutting **is** Unbounded Knapsack with a specific mapping:

| Unbounded Knapsack | Rod Cutting |
|---|---|
| Item index `i` | Piece length `i` |
| Weight of item `i` | Length of piece = `i` (same as index) |
| Value of item `i` | `price[i-1]` |
| Knapsack capacity `W` | Rod length `N` |
| Reuse allowed | Yes — same length can be cut again |

The key insight: **the weight of piece `i` equals its index `i`** — no separate `wt[]` array needed. `rodLength = i` directly.

---

## The Core Insight

State: `dp[i][j]` = maximum value from a rod of length `j` using piece lengths `1..i`.

```
dp[i][j] = max(
    dp[i-1][j],                          ← skip length i (don't cut this size)
    price[i-1] + dp[i][j - i]            ← cut length i, STAY at i (reuse allowed)
)
```

Staying at row `i` after picking allows the same piece length to be cut again — identical to Unbounded Knapsack's `dp[i][j - wt[i-1]]`.

**Answer:** `dp[SIZE][N]`

---

## Base Case

The table uses **1-based indexing** — `i` runs from `1` to `SIZE`, where `i` represents the piece lengths considered so far (`1` to `i` inches).

`i = 0` means **no piece lengths available**. With no cuts possible, no value can be gained:

```
dp[0][j] = 0  for all j   ← implicit from zero-initialisation
```

Similarly, `dp[i][0] = 0` for all `i` — a rod of length `0` has no value regardless of available lengths.

When `i = 1` looks back at `dp[0][j]` via `not_pick`, it gets `0` — correctly representing "ignoring piece length 1 with no other options yields zero."

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<int>& price, vector<vector<int>>& dp, int idx, int n) {
    if (idx == 0) return 0;  // no piece lengths left
    if (dp[idx][n] != -1) return dp[idx][n];

    int not_pick = util(price, dp, idx - 1, n);
    int pick = INT_MIN;
    int rodLength = idx;
    if (n >= rodLength)
        pick = price[idx - 1] + util(price, dp, idx, n - rodLength);

    return dp[idx][n] = max(not_pick, pick);
}

int rodCutting(vector<int> price, int n) {
    int SIZE = price.size();
    vector<vector<int>> dp(SIZE + 1, vector<int>(n + 1, -1));
    return util(price, dp, SIZE, n);
}
```

**Time: O(SIZE × N) | Space: O(SIZE × N) + O(N) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int rodCutting(vector<int> price, int n) {
    int SIZE = price.size();
    vector<vector<int>> dp(SIZE + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= n; j++) {
            int not_pick = dp[i - 1][j];
            int rodLength = i;
            int pick = INT_MIN;
            if (j >= rodLength)
                pick = price[i - 1] + dp[i][j - rodLength];
            dp[i][j] = max(not_pick, pick);
        }
    }
    return dp[SIZE][n];
}
```

**Time: O(SIZE × N) | Space: O(SIZE × N)**

---

## Table Fill

```
price = [1, 5, 8, 9],  N = 4

        j=0  j=1  j=2  j=3  j=4
i=0:     0    0    0    0    0    ← no lengths available
i=1:     0    1    2    3    4    ← length=1, price=1  (cut as many 1s as needed)
i=2:     0    1    5    6   10    ← length=2, price=5
i=3:     0    1    5    8    9... wait

Let me retrace i=3 (length=3, price=8):
j=1: not_pick=dp[2][1]=1, 1<3 can't pick → dp[3][1]=1
j=2: not_pick=dp[2][2]=5, 2<3 can't pick → dp[3][2]=5
j=3: not_pick=dp[2][3]=6, pick=8+dp[3][0]=8 → dp[3][3]=8
j=4: not_pick=dp[2][4]=10, pick=8+dp[3][1]=9 → dp[3][4]=10

i=4 (length=4, price=9):
j=1: not_pick=1, 1<4 → dp[4][1]=1
j=2: not_pick=5, 2<4 → dp[4][2]=5
j=3: not_pick=8, 3<4 → dp[4][3]=8
j=4: not_pick=10, pick=9+dp[4][0]=9 → dp[4][4]=10

        j=0  j=1  j=2  j=3  j=4
i=0:     0    0    0    0    0
i=1:     0    1    2    3    4    ← length=1, price=1
i=2:     0    1    5    6   10    ← length=2, price=5  ← j=4: 5+dp[2][2]=10
i=3:     0    1    5    8   10    ← length=3, price=8  ← j=4: max(10, 8+1)=10
i=4:     0    1    5    8   10    ← length=4, price=9  ← j=4: max(10, 9)=10

return dp[4][4] = 10  ✓  (two cuts of length 2: 5+5=10)
```

Key cell `i=2, j=4`: `not_pick=dp[1][4]=4`, `pick=5+dp[2][2]=5+5=10` → `10` ✓

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(SIZE × N) | O(SIZE × N) + O(N) stack |
| Tabulation  | O(SIZE × N) | O(SIZE × N) |
