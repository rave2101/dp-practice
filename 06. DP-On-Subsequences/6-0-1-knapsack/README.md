# 0-1 Knapsack

## Problem

Given two arrays `val` and `wt` of size `n` representing values and weights of `n` items, and an integer `W` (knapsack capacity), find the **maximum value** achievable by selecting a subset of items whose total weight does not exceed `W`.

Each item can be picked **at most once** (0-1 property).

---

## Example

```
val = [1, 2, 3],  wt = [4, 5, 1],  W = 4

Item 0: value=1, weight=4
Item 1: value=2, weight=5
Item 2: value=3, weight=1
```

```
Option A: pick item 0           → weight=4, value=1
Option B: pick item 2           → weight=1, value=3
Option C: pick item 0 + item 2  → weight=5 > W, invalid
Option D: pick item 1           → weight=5 > W, invalid

Best: pick item 2  →  value = 3
```

**Output: 3**

---

## The Core Insight

State: `f(idx, W)` = maximum value achievable using items `0..idx` with remaining capacity `W`.

For each item there are two choices — pick or skip:

```
f(idx, W) = max(
    f(idx-1, W),                          ← skip item idx
    val[idx] + f(idx-1, W - wt[idx])      ← pick item idx, if W >= wt[idx]
)
```

**After picking, move to `idx-1`** — this is what makes it 0-1 (each item used at most once). Unbounded knapsack stays at `idx` after picking to allow reuse.

**Answer:** `f(n-1, W)`

---

## Base Case

The tabulation uses **1-based indexing** — `i` runs from `1` to `n`, where `i` represents the number of items considered so far.

`i = 0` means **no items have been considered yet**. The entire `i = 0` row is `0` by initialisation:

```
dp[0][j] = 0  for all j   ← 0 items available → maximum value is always 0
```

This is the implicit base case — no explicit code needed. When the loop starts at `i = 1`, every `dp[i][j]` looks back at `dp[i-1][j]` (the skip path), and at `i = 1` that lookback lands on this all-zero row, which correctly represents having no items to choose from.

---

## Why `INT_MIN` for `pick` (not `0`)

`pick` is initialized to `INT_MIN` as a sentinel meaning "this choice is unavailable" (item too heavy). Using `0` would incorrectly suggest picking nothing has value 0 — conflating "can't pick" with "picked and got nothing". `max(pick, not_pick)` then naturally selects `not_pick` when the item doesn't fit.

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<int>& wt, vector<int>& val, vector<vector<int>>& dp, int idx, int W) {
    if (idx == 0) return (W >= wt[0]) ? val[0] : 0;
    if (dp[idx][W] != -1) return dp[idx][W];

    int not_pick = util(wt, val, dp, idx - 1, W);
    int pick = INT_MIN;
    if (W >= wt[idx])
        pick = val[idx] + util(wt, val, dp, idx - 1, W - wt[idx]);

    return dp[idx][W] = max(pick, not_pick);
}

int knapsack01(vector<int>& wt, vector<int>& val, int n, int W) {
    vector<vector<int>> dp(n, vector<int>(W + 1, -1));
    return util(wt, val, dp, n - 1, W);
}
```

**Time: O(n × W) | Space: O(n × W) + O(n) call stack**

---

## Dry Run — Memoization

```
val = [1, 2, 3],  wt = [4, 5, 1],  W = 4
```

```
f(2, 4)   wt[2]=1, val[2]=3
├── skip:  f(1, 4)
│   │      wt[1]=5 > 4, can't pick
│   ├── skip:  f(0, 4)
│   │          wt[0]=4 <= 4  →  return val[0] = 1
│   └── pick:  unavailable (5 > 4)
│   dp[1][4] = max(INT_MIN, 1) = 1
└── pick:  val[2] + f(1, 3) = 3 + ?
    │      wt[1]=5 > 3, can't pick
    ├── skip:  f(0, 3)
    │          wt[0]=4 > 3  →  return 0
    └── pick:  unavailable
    dp[1][3] = 0
    pick = 3 + 0 = 3

dp[2][4] = max(3, 1) = 3
```

**Answer: 3**

---

## Approach 2 — Tabulation (Bottom-Up)

`dp[i][j]` = max value using the first `i` items with capacity `j`.

**Init:** all zeros — 0 items or 0 capacity gives 0 value.

```cpp
int knapsack01(vector<int>& wt, vector<int>& val, int n, int W) {
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= W; j++) {
            int not_pick = dp[i - 1][j];
            int pick = INT_MIN;
            if (j >= wt[i - 1])
                pick = val[i - 1] + dp[i - 1][j - wt[i - 1]];
            dp[i][j] = max(pick, not_pick);
        }
    }
    return dp[n][W];
}
```

**Time: O(n × W) | Space: O(n × W)**

---

## Table Fill

```
val = [1, 2, 3],  wt = [4, 5, 1],  W = 4

        j=0  j=1  j=2  j=3  j=4
i=0:     0    0    0    0    0    ← no items
i=1:     0    0    0    0    1    ← item 0: wt=4, val=1
i=2:     0    0    0    0    1    ← item 1: wt=5 > all j, no change
i=3:     0    3    3    3    4    ← item 2: wt=1, val=3

return dp[3][4] = 4
```

Wait — at `i=3, j=4`: `not_pick = dp[2][4] = 1`, `pick = val[2] + dp[2][3] = 3 + 1 = 4` → `dp[3][4] = 4`.

This picks item 2 (val=3, wt=1) and item 0 (val=1, wt=4)... but wt=1+4=5 > W=4.

Let me recheck: `pick = val[2] + dp[2][j - wt[2]] = 3 + dp[2][4-1] = 3 + dp[2][3]`.
`dp[2][3]` = max value using items 0,1 with capacity 3. Item 0 weighs 4 > 3, item 1 weighs 5 > 3 → `dp[2][3] = 0`.

So `pick = 3 + 0 = 3`, `not_pick = dp[2][4] = 1` → `dp[3][4] = 3`. ✓

```
        j=0  j=1  j=2  j=3  j=4
i=0:     0    0    0    0    0
i=1:     0    0    0    0    1    ← wt=4, val=1: fits only at j=4
i=2:     0    0    0    0    1    ← wt=5: too heavy for all j<=4
i=3:     0    3    3    3    3    ← wt=1, val=3: fits everywhere

return dp[3][4] = 3  ✓
```

---

## 0-1 vs Unbounded Knapsack

| | 0-1 Knapsack | Unbounded Knapsack |
|---|---|---|
| Same item twice? | No | Yes |
| Pick branch | `f(idx-1, W - wt[idx])` | `f(idx, W - wt[idx])` |
| After picking | Move to `idx-1` | Stay at `idx` |

The only code difference is `idx-1` vs `idx` in the pick branch.

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n × W) | O(n × W) + O(n) stack |
| Tabulation  | O(n × W) | O(n × W) |
