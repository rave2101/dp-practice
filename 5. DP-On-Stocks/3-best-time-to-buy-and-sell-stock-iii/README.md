# Best Time to Buy and Sell Stock III

## Problem

Given an array `arr` of `n` integers where `arr[i]` is the stock price on day `i`.

Find the **maximum profit** using **at most 2 transactions** in total.

Rules:
- Hold at most **one share** at a time (must sell before buying again)
- Buying and selling on the **same day** is allowed
- If no profit is possible, return `0`

---

## Example

```
arr = [4, 2, 7, 1, 11, 5]
       0  1  2  3   4  5   (day index)
```

```
Buy  day 1 (price=2), sell day 2 (price=7)  → profit = 5
Buy  day 3 (price=1), sell day 4 (price=11) → profit = 10
Total = 15
```

**Output: 15**

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

**Answer:** `f(0, 1, 2)` — start at day 0, can buy, 2 transactions remaining.

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

int stockBuySell(vector<int> arr, int n) {
    vector<vector<vector<int>>> dp(n, vector<vector<int>>(2, vector<int>(3, -1)));
    return util(arr, dp, n, 0, 1, 2);
}
```

**Time: O(n×2×3) | Space: O(n×2×3) + O(n) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

Memoization recurses from `idx=0` toward `idx=n`. So tabulation fills from `idx=n-1` back to `idx=0`.

```cpp
int stockBuySell(vector<int> arr, int n) {
    // dp[n+1][2][3], all initialized to 0 (base cases)
    vector<vector<vector<int>>> dp(n+1, vector<vector<int>>(2, vector<int>(3, 0)));
    for (int idx = n-1; idx >= 0; idx--) {
        for (int buy = 0; buy <= 1; buy++) {
            for (int rt = 2; rt >= 1; rt--) {
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
    return dp[0][1][2];
}
```

### Table fill for the example

```
arr = [4, 2, 7, 1, 11, 5],  n=6

dp[6][*][*] = 0   ← base case (all zeros)

idx=5 (arr[5]=5):
  buy=0, rt=2: sell=5+dp[6][1][1]=5,  no_sell=dp[6][0][2]=0  → dp[5][0][2]=5
  buy=0, rt=1: sell=5+dp[6][1][0]=5,  no_sell=dp[6][0][1]=0  → dp[5][0][1]=5
  buy=1, rt=2: b=-5+dp[6][0][2]=-5,   nb=dp[6][1][2]=0       → dp[5][1][2]=0
  buy=1, rt=1: b=-5+dp[6][0][1]=-5,   nb=dp[6][1][1]=0       → dp[5][1][1]=0

idx=4 (arr[4]=11):
  buy=0, rt=2: sell=11+dp[5][1][1]=11, no_sell=dp[5][0][2]=5  → dp[4][0][2]=11
  buy=0, rt=1: sell=11+dp[5][1][0]=11, no_sell=dp[5][0][1]=5  → dp[4][0][1]=11
  buy=1, rt=2: b=-11+dp[5][0][2]=-6,  nb=dp[5][1][2]=0        → dp[4][1][2]=0
  buy=1, rt=1: b=-11+dp[5][0][1]=-6,  nb=dp[5][1][1]=0        → dp[4][1][1]=0

idx=3 (arr[3]=1):
  buy=0, rt=2: sell=1+dp[4][1][1]=1,  no_sell=dp[4][0][2]=11 → dp[3][0][2]=11
  buy=0, rt=1: sell=1+dp[4][1][0]=1,  no_sell=dp[4][0][1]=11 → dp[3][0][1]=11
  buy=1, rt=2: b=-1+dp[4][0][2]=10,   nb=dp[4][1][2]=0       → dp[3][1][2]=10
  buy=1, rt=1: b=-1+dp[4][0][1]=10,   nb=dp[4][1][1]=0       → dp[3][1][1]=10

idx=2 (arr[2]=7):
  buy=0, rt=2: sell=7+dp[3][1][1]=17, no_sell=dp[3][0][2]=11 → dp[2][0][2]=17
  buy=0, rt=1: sell=7+dp[3][1][0]=7,  no_sell=dp[3][0][1]=11 → dp[2][0][1]=11
  buy=1, rt=2: b=-7+dp[3][0][2]=4,    nb=dp[3][1][2]=10      → dp[2][1][2]=10
  buy=1, rt=1: b=-7+dp[3][0][1]=4,    nb=dp[3][1][1]=10      → dp[2][1][1]=10

idx=1 (arr[1]=2):
  buy=0, rt=2: sell=2+dp[2][1][1]=12, no_sell=dp[2][0][2]=17 → dp[1][0][2]=17
  buy=0, rt=1: sell=2+dp[2][1][0]=2,  no_sell=dp[2][0][1]=11 → dp[1][0][1]=11
  buy=1, rt=2: b=-2+dp[2][0][2]=9,    nb=dp[2][1][2]=10      → dp[1][1][2]=10
  buy=1, rt=1: b=-2+dp[2][0][1]=9,    nb=dp[2][1][1]=10      → dp[1][1][1]=10

idx=0 (arr[0]=4):
  buy=0, rt=2: sell=4+dp[1][1][1]=14, no_sell=dp[1][0][2]=17 → dp[0][0][2]=17
  buy=0, rt=1: sell=4+dp[1][1][0]=4,  no_sell=dp[1][0][1]=11 → dp[0][0][1]=11
  buy=1, rt=2: b=-4+dp[1][0][2]=13,   nb=dp[1][1][2]=10      → dp[0][1][2]=13  ← Hmm...
```

Wait — let me re-trace from idx=1, buy=1, rt=2:
```
b=-2+dp[2][0][2]=-2+17=15,  nb=dp[2][1][2]=10  → dp[1][1][2]=15

idx=0, buy=1, rt=2:
b=-4+dp[1][0][2]=-4+17=13,  nb=dp[1][1][2]=15  → dp[0][1][2]=15
```

**return dp[0][1][2] = 15**

---

**Time: O(n×2×3) | Space: O(n×2×3)**

---

## Why `rt` decrements on sell, not buy

A "transaction" = one complete buy + sell cycle. We track remaining transactions to cap at 2. Decrementing on sell (rather than buy) means the transaction is "consumed" only when completed — consistent with the problem's definition of a transaction.

---

## Difference from Stock II

| | Stock II | Stock III |
|---|---|---|
| Transactions | Unlimited | At most 2 |
| DP dimensions | `[n][2]` | `[n][2][3]` |
| Extra state | None | `remainingTransactions` (0–2) |

Adding the transaction count as a third dimension extends the Stock II DP naturally.

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n×2×3) = O(n) | O(n) + O(n) stack |
| Tabulation | O(n×2×3) = O(n) | O(n) |
