# Coin Change II — Count Combinations

## Problem

Given an array `coins` of `n` denominations and an integer `amount`, return the **number of distinct combinations** that sum to `amount`. Each coin can be used **any number of times**. Return the result modulo `(10^9 + 7)`.

---

## Example

```
coins = [2, 4, 10],  amount = 10  →  4

{2, 2, 2, 2, 2}  = 10  ✓
{2, 2, 2, 4}     = 10  ✓
{2, 4, 4}        = 10  ✓
{10}             = 10  ✓
```

---

## The Core Insight

State: `dp[i][j]` = number of ways to form amount `j` using the first `i` denominations.

```
dp[i][j] = dp[i-1][j]                      ← skip coin i
          + dp[i][j - coins[i-1]]           ← pick coin i (stay at row i — reuse allowed)
```

Staying at row `i` after picking is what makes this **unbounded** — the same coin can be used again.

---

## Base Case

The table uses **1-based indexing** — `i` runs from `1` to `N`, where `i` is the number of denominations considered so far.

### `dp[i][0] = 1` for all `i`

Amount `0` can always be formed in exactly **one way** — use no coins at all (the empty combination). This holds regardless of how many denominations are available:

```
dp[0][0] = 1   ← 0 denominations, amount 0 → 1 way (empty)
dp[1][0] = 1   ← 1 denomination,  amount 0 → 1 way (still just empty)
dp[2][0] = 1   ← 2 denominations, amount 0 → 1 way (still just empty)
...
```

Unlike the **Count Subsets** problems where only `dp[0][0] = 1` was set (arrays can have zeros that double the count at `j=0`), here **all coin values are positive** — picking any coin increases `j`, so `j=0` can never be reached by picking. The count at `j=0` stays exactly 1 for every row.

### `dp[0][j] = 0` for all `j > 0`

With no denominations available, no positive amount can be formed — stays `0` from initialisation.

---

## Coin Change I vs Coin Change II

| | Coin Change I (Min Coins) | Coin Change II (Count Ways) |
|---|---|---|
| `dp[i][j]` stores | Minimum coins to make `j` | Number of ways to make `j` |
| Operation | `min(not_pick, pick)` | `not_pick + pick` |
| Init value | `INF` (minimising) | `0` (counting) |
| `dp[i][0]` | `0` (0 coins for amount 0) | `1` (1 way — empty combination) |
| Impossible | return `-1` | return `0` (already default) |
| Pick branch | `1 + dp[i][j - coins[i-1]]` | `dp[i][j - coins[i-1]]` |

Both stay at row `i` after picking (unbounded). The only differences are the operation (`min` vs `+`) and initialisation (`INF` vs `0`).

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<int>& coins, vector<vector<int>>& dp, int idx, int amount) {
    if (idx == 0) return amount % coins[0] == 0 ? 1 : 0;
    if (dp[idx][amount] != -1) return dp[idx][amount];

    const int MOD = 1e9 + 7;
    int not_pick = util(coins, dp, idx - 1, amount);
    int pick = 0;
    if (amount >= coins[idx - 1])
        pick = util(coins, dp, idx, amount - coins[idx - 1]);

    return dp[idx][amount] = (pick + not_pick) % MOD;
}

int count(vector<int>& coins, int N, int amount) {
    vector<vector<int>> dp(N + 1, vector<int>(amount + 1, -1));
    return util(coins, dp, N, amount);
}
```

**Base case at `idx == 0`:** only `coins[0]` is available. Amount is formable only if perfectly divisible by `coins[0]` — return `1` if so, `0` otherwise.

**Time: O(N × amount) | Space: O(N × amount) + O(amount) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int count(vector<int>& coins, int N, int amount) {
    const int MOD = 1e9 + 7;
    vector<vector<int>> dp(N + 1, vector<int>(amount + 1, 0));
    for (int i = 0; i <= N; i++) dp[i][0] = 1;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= amount; j++) {
            dp[i][j] = dp[i - 1][j];
            if (j - coins[i - 1] >= 0)
                dp[i][j] = (dp[i][j] + dp[i][j - coins[i - 1]]) % MOD;
        }
    }
    return dp[N][amount];
}
```

**Time: O(N × amount) | Space: O(N × amount)**

---

## Table Fill

```
coins = [2, 4, 10],  amount = 10

         j=0  j=1  j=2  j=3  j=4  j=5  j=6  j=7  j=8  j=9  j=10
i=0:      1    0    0    0    0    0    0    0    0    0    0    ← no coins
i=1:      1    0    1    0    1    0    1    0    1    0    1    ← coin=2
i=2:      1    0    1    0    2    0    2    0    3    0    3    ← coin=4
i=3:      1    0    1    0    2    0    2    0    3    0    4    ← coin=10

return dp[3][10] = 4
```

Key cells:
```
i=2, j=4:  dp[1][4] + dp[2][0] = 1 + 1 = 2   → {2,2} and {4}
i=2, j=10: dp[1][10]+ dp[2][6] = 1 + 2 = 3   → {2,2,2,2,2}, {2,2,2,4}, {2,4,4}
i=3, j=10: dp[2][10]+ dp[3][0] = 3 + 1 = 4   → above 3 + {10}
```

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(N × amount) | O(N × amount) + O(amount) stack |
| Tabulation  | O(N × amount) | O(N × amount) |
