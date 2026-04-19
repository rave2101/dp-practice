# Coin Change — Minimum Coins

## Problem

Given an array `coins` of distinct denominations and an integer `amount`, return the **minimum number of coins** needed to make up that amount. If it is impossible, return `-1`.

Each coin can be used **any number of times** (unbounded).

---

## Examples

```
coins = [1, 2, 5],  amount = 11  →  3
11 = 5 + 5 + 1

coins = [2],  amount = 3  →  -1
No combination of 2s makes 3
```

---

## The Core Insight

State: `dp[i][j]` = minimum coins needed to form amount `j` using the first `i` coin denominations.

```
dp[i][j] = min(
    dp[i-1][j],                      ← skip coin i (don't use this denomination)
    1 + dp[i][j - coins[i-1]]        ← pick coin i (stay at row i — reuse allowed)
)
```

**Staying at row `i` after picking** is what makes this unbounded — unlike 0-1 Knapsack which moves to `i-1` after picking to prevent reuse.

---

## Base Case

The table uses **1-based indexing** — `i` runs from `1` to `SIZE`, where `i` represents the number of coin denominations considered so far.

`i = 0` means **no coin denominations available**. No coins means no amount can be formed, so:

```
dp[0][j] = INF  for all j > 0   ← impossible (no coins, non-zero amount)
dp[i][0] = 0    for all i        ← amount 0 always needs 0 coins
```

`dp[i][0] = 0` is set explicitly for all rows. The rest of `i = 0` stays `INF` from initialisation — correctly representing that without any coin denomination, no positive amount is reachable.

When `i = 1` looks back at `dp[0][j]` via `not_pick`, it gets `INF`, meaning "ignoring all coins gives infinite cost" — forcing the algorithm to consider picking instead.

---

## Why `INF = 1e9` instead of `INT_MAX`

`INT_MAX = 2,147,483,647`. Adding 1 to it overflows to a large negative number, making `min()` pick it as the "minimum" — a silent wrong answer.

`INF = 1e9` is safe:
- `1 + 1e9 = 1,000,000,001` — well within `int` range, no overflow
- Amount is at most `10^4` on most platforms, so a legitimate answer can never reach `1e9`
- Use `>= INF` to detect impossibility and return `-1`

```
INT_MAX + 1  →  overflow  →  negative  →  wrong min  ✗
1e9     + 1  →  1e9+1     →  still large →  correct   ✓
```

---

## 0-1 Knapsack vs Coin Change (Unbounded)

| | 0-1 Knapsack | Coin Change |
|---|---|---|
| Reuse item? | No | Yes |
| Pick branch | `dp[i-1][j - wt[i-1]]` | `dp[i][j - coins[i-1]]` |
| Optimise for | Maximum value | Minimum coins |
| Impossible sentinel | — | `INF`, return `-1` |

The only structural difference is `i-1` vs `i` in the pick branch.

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<int>& coins, vector<vector<int>>& dp, int idx, int amount) {
    if (idx == 0) return amount == 0 ? 0 : (int)1e9;  // no coins left
    if (dp[idx][amount] != -1) return dp[idx][amount];

    int not_pick = util(coins, dp, idx - 1, amount);
    int pick = (int)1e9;
    if (amount >= coins[idx - 1])
        pick = 1 + util(coins, dp, idx, amount - coins[idx - 1]);

    return dp[idx][amount] = min(pick, not_pick);
}

int MinimumCoins(vector<int>& coins, int amount) {
    int SIZE = coins.size();
    vector<vector<int>> dp(SIZE + 1, vector<int>(amount + 1, -1));
    int ans = util(coins, dp, SIZE, amount);
    return ans >= (int)1e9 ? -1 : ans;
}
```

**Base case at `idx == 0`:** no coin denominations left. If `amount == 0`, we're done — return `0`. Otherwise it's unreachable — return `1e9`.

**Time: O(n × amount) | Space: O(n × amount) + O(amount) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int MinimumCoins(vector<int>& coins, int amount) {
    int SIZE = coins.size();
    int INF = 1e9;
    vector<vector<int>> dp(SIZE + 1, vector<int>(amount + 1, INF));
    for (int i = 0; i <= SIZE; i++) dp[i][0] = 0;

    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= amount; j++) {
            int not_pick = dp[i - 1][j];
            int pick = INF;
            if (j >= coins[i - 1])
                pick = 1 + dp[i][j - coins[i - 1]];
            dp[i][j] = min(pick, not_pick);
        }
    }
    return dp[SIZE][amount] >= INF ? -1 : dp[SIZE][amount];
}
```

**Time: O(n × amount) | Space: O(n × amount)**

---

## Table Fill

```
coins = [1, 2, 5],  amount = 6

INF shown as ∞, dp[i][0] = 0 for all i

        j=0  j=1  j=2  j=3  j=4  j=5  j=6
i=0:     0    ∞    ∞    ∞    ∞    ∞    ∞    ← no coins
i=1:     0    1    2    3    4    5    6    ← coin=1: pick as many as needed
i=2:     0    1    1    2    2    3    3    ← coin=2: reuse reduces count
i=3:     0    1    1    2    2    1    2    ← coin=5: 5+1=6 → 2 coins

return dp[3][6] = 2  ✓  (5 + 1)
```

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n × amount) | O(n × amount) + O(amount) stack |
| Tabulation  | O(n × amount) | O(n × amount) |
