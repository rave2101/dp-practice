# Best Time to Buy and Sell Stock IV

## Problem

Given an array `arr` of `n` integers where `arr[i]` is the stock price on day `i`, and an integer `k`.

Find the **maximum profit** using **at most k transactions** in total.

Rules:
- Hold at most **one share** at a time (must sell before buying again)
- Buying and selling on the **same day** is allowed
- If no profit is possible, return `0`

---

## Example

```
arr = [3, 2, 6, 5, 0, 3],  k = 2
       0  1  2  3  4  5    (day index)
```

```
Buy  day 1 (price=2), sell day 2 (price=6) → profit = 4
Buy  day 4 (price=0), sell day 5 (price=3) → profit = 3
Total = 7
```

**Output: 7**

---

## The Core Insight

State: `f(idx, isBuy, rt)` = max profit from day `idx` to the last day, given current state.

- `isBuy=1` → can buy (don't hold stock)
- `isBuy=0` → can sell (hold stock)
- `rt` → remaining transactions (a transaction = one buy + one sell; decrements on **sell**)

```
isBuy=1:  f(idx, 1, rt) = max( -arr[idx] + f(idx+1, 0, rt),   ← buy today
                                 f(idx+1, 1, rt) )               ← skip

isBuy=0:  f(idx, 0, rt) = max(  arr[idx] + f(idx+1, 1, rt-1), ← sell today
                                 f(idx+1, 0, rt) )               ← skip (still holding)
```

**Base cases:**
- `idx == n` → no days left, return `0`
- `rt == 0` → no transactions left, return `0`

**Answer:** `f(0, 1, k)` — start at day 0, can buy, k transactions remaining.

**Key rule:** buy → `isBuy` flips to 0. Sell → `isBuy` flips to 1, `rt` decrements. Skip → state stays the same.

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<int>& arr, vector<vector<vector<int>>>& dp, int SIZE, int idx, int isBuy, int rt) {
    if (idx >= SIZE) return 0;
    if (rt <= 0) return 0;
    if (dp[idx][isBuy][rt] != -1) return dp[idx][isBuy][rt];
    int profit = 0;
    if (isBuy) {
        int buy    = -arr[idx] + util(arr, dp, SIZE, idx+1, 0, rt);  // buy → isBuy=0
        int no_buy = util(arr, dp, SIZE, idx+1, 1, rt);               // skip → isBuy=1
        profit = max(buy, no_buy);
    } else {
        int sell    = arr[idx] + util(arr, dp, SIZE, idx+1, 1, rt-1); // sell → isBuy=1, rt--
        int no_sell = util(arr, dp, SIZE, idx+1, 0, rt);               // skip → isBuy=0
        profit = max(sell, no_sell);
    }
    return dp[idx][isBuy][rt] = profit;
}

int stockBuySell(vector<int> arr, int n, int k) {
    vector<vector<vector<int>>> dp(n, vector<vector<int>>(2, vector<int>(k+1, -1)));
    return util(arr, dp, n, 0, 1, k);
}
```

**Time: O(n × 2 × k) | Space: O(n × 2 × k) + O(n) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

Memoization recurses from `idx=0` toward `idx=n`. So tabulation fills from `idx=n-1` back to `idx=0`.

```cpp
int stockBuySell(vector<int> arr, int n, int k) {
    // dp[n+1][2][k+1], all initialized to 0 (base cases)
    vector<vector<vector<int>>> dp(n+1, vector<vector<int>>(2, vector<int>(k+1, 0)));
    for (int idx = n-1; idx >= 0; idx--) {
        for (int buy = 0; buy <= 1; buy++) {
            for (int rt = k; rt >= 1; rt--) {
                if (buy) {
                    int b  = -arr[idx] + dp[idx+1][0][rt];
                    int nb = dp[idx+1][1][rt];
                    dp[idx][buy][rt] = max(b, nb);
                } else {
                    int sell    = arr[idx] + dp[idx+1][1][rt-1];
                    int no_sell = dp[idx+1][0][rt];
                    dp[idx][buy][rt] = max(sell, no_sell);
                }
            }
        }
    }
    return dp[0][1][k];
}
```

### Table fill for the example

```
arr = [3, 2, 6, 5, 0, 3],  n=6,  k=2

dp[6][*][*] = 0   ← base case (all zeros)

idx=5 (arr[5]=3):
  buy=0, rt=2: sell=3+dp[6][1][1]=3,  no_sell=dp[6][0][2]=0  → dp[5][0][2]=3
  buy=0, rt=1: sell=3+dp[6][1][0]=3,  no_sell=dp[6][0][1]=0  → dp[5][0][1]=3
  buy=1, rt=2: b=-3+dp[6][0][2]=-3,   nb=dp[6][1][2]=0       → dp[5][1][2]=0
  buy=1, rt=1: b=-3+dp[6][0][1]=-3,   nb=dp[6][1][1]=0       → dp[5][1][1]=0

idx=4 (arr[4]=0):
  buy=0, rt=2: sell=0+dp[5][1][1]=0,  no_sell=dp[5][0][2]=3  → dp[4][0][2]=3
  buy=0, rt=1: sell=0+dp[5][1][0]=0,  no_sell=dp[5][0][1]=3  → dp[4][0][1]=3
  buy=1, rt=2: b=-0+dp[5][0][2]=3,    nb=dp[5][1][2]=0       → dp[4][1][2]=3
  buy=1, rt=1: b=-0+dp[5][0][1]=3,    nb=dp[5][1][1]=0       → dp[4][1][1]=3

idx=3 (arr[3]=5):
  buy=0, rt=2: sell=5+dp[4][1][1]=8,  no_sell=dp[4][0][2]=3  → dp[3][0][2]=8
  buy=0, rt=1: sell=5+dp[4][1][0]=5,  no_sell=dp[4][0][1]=3  → dp[3][0][1]=5
  buy=1, rt=2: b=-5+dp[4][0][2]=-2,   nb=dp[4][1][2]=3       → dp[3][1][2]=3
  buy=1, rt=1: b=-5+dp[4][0][1]=-2,   nb=dp[4][1][1]=3       → dp[3][1][1]=3

idx=2 (arr[2]=6):
  buy=0, rt=2: sell=6+dp[3][1][1]=9,  no_sell=dp[3][0][2]=8  → dp[2][0][2]=9
  buy=0, rt=1: sell=6+dp[3][1][0]=6,  no_sell=dp[3][0][1]=5  → dp[2][0][1]=6
  buy=1, rt=2: b=-6+dp[3][0][2]=2,    nb=dp[3][1][2]=3       → dp[2][1][2]=3
  buy=1, rt=1: b=-6+dp[3][0][1]=-1,   nb=dp[3][1][1]=3       → dp[2][1][1]=3

idx=1 (arr[1]=2):
  buy=0, rt=2: sell=2+dp[2][1][1]=5,  no_sell=dp[2][0][2]=9  → dp[1][0][2]=9
  buy=0, rt=1: sell=2+dp[2][1][0]=2,  no_sell=dp[2][0][1]=6  → dp[1][0][1]=6
  buy=1, rt=2: b=-2+dp[2][0][2]=7,    nb=dp[2][1][2]=3       → dp[1][1][2]=7
  buy=1, rt=1: b=-2+dp[2][0][1]=4,    nb=dp[2][1][1]=3       → dp[1][1][1]=4

idx=0 (arr[0]=3):
  buy=0, rt=2: sell=3+dp[1][1][1]=7,  no_sell=dp[1][0][2]=9  → dp[0][0][2]=9
  buy=0, rt=1: sell=3+dp[1][1][0]=3,  no_sell=dp[1][0][1]=6  → dp[0][0][1]=6
  buy=1, rt=2: b=-3+dp[1][0][2]=6,    nb=dp[1][1][2]=7       → dp[0][1][2]=7
  buy=1, rt=1: b=-3+dp[1][0][1]=3,    nb=dp[1][1][1]=4       → dp[0][1][1]=4

return dp[0][1][2] = 7
```

**Time: O(n × 2 × k) | Space: O(n × 2 × k)**

---

## Difference from Stock III

| | Stock III | Stock IV |
|---|---|---|
| Transactions | At most 2 | At most k |
| DP size | `[n][2][3]` | `[n][2][k+1]` |
| Change | Hardcoded `2` | Parameterized `k` |

Stock IV is a direct generalization — replace the hardcoded `2` with `k` and the logic is identical.

---

## Same-Day Buy and Sell

The problem states that buying and selling on the same day is permitted. However, **the DP never needs to handle this explicitly**.

After buying at `idx`, the recursion always moves to `idx+1` before a sell is possible — the DP never evaluates buy and sell at the same index in the same call.

More importantly, same-day buy+sell always nets **zero profit**:

```
buy  day i: -arr[i]
sell day i: +arr[i]
net = 0
```

No optimal solution would ever choose this, so the DP naturally skips it by always preferring the `no_buy` or `no_sell` path when profit is 0. The constraint is mentioned only to clarify that no enforced gap exists between buy and sell days — it has no effect on the logic.

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n × 2 × k) | O(n × 2 × k) + O(n) stack |
| Tabulation | O(n × 2 × k) | O(n × 2 × k) |
