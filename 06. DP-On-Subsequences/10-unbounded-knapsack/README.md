# Unbounded Knapsack

## Problem

Given two arrays `val` and `wt` of size `n` representing values and weights of `n` items, and an integer `W` (knapsack capacity), find the **maximum value** achievable such that total weight does not exceed `W`.

Each item can be picked **any number of times** (unbounded supply).

---

## Example

```
val = [5, 11, 13],  wt = [2, 4, 6],  W = 10

Item 0: value=5,  weight=2
Item 1: value=11, weight=4
Item 2: value=13, weight=6
```

```
Pick item 1 twice + item 0 once → 11 + 11 + 5 = 27,  weight = 4+4+2 = 10  ✓
```

**Output: 27**

---

## The Core Insight

State: `f(idx, W)` = maximum value using items `1..idx` with remaining capacity `W`.

For each item two choices — pick or skip:

```
f(idx, W) = max(
    f(idx-1, W),                            ← skip item idx, move to idx-1
    val[idx-1] + f(idx, W - wt[idx-1])      ← pick item idx, STAY at idx
)
```

**Staying at `idx` after picking** is the only difference from 0-1 Knapsack. It allows the same item to be picked again on the next recursive call.

**Base case:** `idx == 0` → no items left, return `0`.

**Answer:** `f(n, W)`

---

## Recursion Tree

```
val = [5, 11, 13],  wt = [2, 4, 6],  W = 10
```

```
f(3, 10)   item={13,wt=6}
├── skip:  f(2, 10)
│   │      item={11,wt=4}
│   ├── skip:  f(1, 10)
│   │   │      item={5,wt=2}
│   │   ├── skip:  f(0, 10) → 0
│   │   └── pick:  5 + f(1, 8)
│   │              └── pick: 5 + f(1, 6)
│   │                         └── pick: 5 + f(1, 4)
│   │                                    └── pick: 5 + f(1, 2)
│   │                                               └── pick: 5 + f(1, 0) → 0
│   │                                               = 5
│   │                                    = 10
│   │                         = 15
│   │              = 20
│   │   f(1, 10) = max(0, 5+20) = 25
│   │
│   └── pick:  11 + f(2, 6)
│              │   item={11,wt=4}
│              ├── skip: f(1, 6) → 15   (MEMOIZED)
│              └── pick: 11 + f(2, 2)
│                         ├── skip: f(1, 2) → 5  (MEMOIZED)
│                         └── pick: 11+f(2,-2) invalid
│                         f(2,2) = max(5, -) = 5
│              f(2, 6) = max(15, 11+5) = 16
│   f(2, 10) = max(25, 11+16) = 27
│
└── pick:  13 + f(3, 4)
    │      item={13,wt=6}
    ├── skip: f(2, 4)
    │   ├── skip: f(1,4) → 10  (MEMOIZED)
    │   └── pick: 11 + f(2,0) → 11
    │   f(2,4) = max(10, 11) = 11
    └── pick: 13+f(3,-2) invalid
    f(3, 4) = max(11, -) = 11
    pick = 13 + 11 = 24

f(3, 10) = max(27, 24) = 27
```

**Answer: 27** — pick item 1 (wt=4) twice + item 0 (wt=2) once → `11+11+5=27`

---

## 0-1 Knapsack vs Unbounded Knapsack

| | 0-1 Knapsack | Unbounded Knapsack |
|---|---|---|
| Item reuse | No | Yes |
| Pick branch recurse to | `f(idx-1, W-wt[idx-1])` | `f(idx, W-wt[idx-1])` |
| After picking | Move to `idx-1` | Stay at `idx` |
| Tabulation pick | `dp[i-1][j-wt[i-1]]` | `dp[i][j-wt[i-1]]` |

**One character difference** — `i-1` vs `i` in the pick branch.

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<int>& wt, vector<int>& val, vector<vector<int>>& dp, int idx, int W) {
    if (idx == 0) return 0;
    if (dp[idx][W] != -1) return dp[idx][W];

    int not_pick = util(wt, val, dp, idx - 1, W);
    int pick = INT_MIN;
    if (W >= wt[idx - 1])
        pick = val[idx - 1] + util(wt, val, dp, idx, W - wt[idx - 1]);

    return dp[idx][W] = max(not_pick, pick);
}

int unboundedKnapsack(vector<int>& wt, vector<int>& val, int n, int W) {
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, -1));
    return util(wt, val, dp, n, W);
}
```

**Time: O(n × W) | Space: O(n × W) + O(W) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int unboundedKnapsack(vector<int>& wt, vector<int>& val, int n, int W) {
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= W; j++) {
            int not_pick = dp[i - 1][j];
            int pick = INT_MIN;
            if (j >= wt[i - 1])
                pick = val[i - 1] + dp[i][j - wt[i - 1]];  // same row i
            dp[i][j] = max(not_pick, pick);
        }
    }
    return dp[n][W];
}
```

**Time: O(n × W) | Space: O(n × W)**

---

## Table Fill

```
val = [5, 11, 13],  wt = [2, 4, 6],  W = 10

        j=0  j=1  j=2  j=3  j=4  j=5  j=6  j=7  j=8  j=9  j=10
i=0:     0    0    0    0    0    0    0    0    0    0    0    ← no items
i=1:     0    0    5    5   10   10   15   15   20   20   25   ← item 0: wt=2,val=5
i=2:     0    0    5    5   11   11   16   16   22   22   27   ← item 1: wt=4,val=11
i=3:     0    0    5    5   11   11   16   16   22   22   27   ← item 2: wt=6,val=13

return dp[3][10] = 27
```

Key cells at `i=2`:
```
j=4:  not_pick=dp[1][4]=10,  pick=11+dp[2][0]=11  → max=11   {item1}
j=6:  not_pick=dp[1][6]=15,  pick=11+dp[2][2]=16  → max=16   {item0,item1}
j=8:  not_pick=dp[1][8]=20,  pick=11+dp[2][4]=22  → max=22   {item1,item1}
j=10: not_pick=dp[1][10]=25, pick=11+dp[2][6]=27  → max=27   {item0,item1,item1}
```

---

## Base Case

The table uses **1-based indexing** — `i=0` means no items considered. With no items, no value can be gained regardless of capacity:

```
dp[0][j] = 0  for all j   ← implicit from zero-initialisation
```

The loop starts at `i=1`, and the `not_pick` path (`dp[i-1][j]`) at `i=1` reads from this all-zero row — correctly representing "no items available gives zero value."

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n × W) | O(n × W) + O(W) stack |
| Tabulation  | O(n × W) | O(n × W) |
