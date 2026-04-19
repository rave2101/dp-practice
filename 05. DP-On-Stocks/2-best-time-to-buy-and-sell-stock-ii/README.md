# Best Time to Buy and Sell Stock II

## Problem

Given an array `arr` of `n` integers where `arr[i]` is the stock price on day `i`.

Find the **maximum profit** from buying and selling **any number of times**.

Rules:
- Hold at most **one share** at a time (must sell before buying again)
- Buying and selling on the **same day** is allowed
- If no profit is possible, return `0`

---

## Example

```
arr = [2, 4, 1, 5]
       0  1  2  3   (day index)
```

```
Buy  day 0 (price=2), sell day 1 (price=4) → profit = 2
Buy  day 2 (price=1), sell day 3 (price=5) → profit = 4
Total = 6
```

**Output: 6**

---

## The Core Insight

State: `f(idx, isBuy)` = max profit from day `idx` to the last day, given current state.

- `isBuy=1` → currently can buy (don't hold stock)
- `isBuy=0` → currently can sell (hold stock)

```
isBuy=1:  f(idx, 1) = max( -arr[idx] + f(idx+1, 0),   ← buy today
                             f(idx+1, 1) )               ← skip

isBuy=0:  f(idx, 0) = max(  arr[idx] + f(idx+1, 1),   ← sell today
                             f(idx+1, 0) )               ← skip
```

**Base case:** `idx == n` → no days left, return `0`.

**Answer:** `f(0, 1)` — start at day 0, initially can buy.

**Key rule:** after buying → state flips to 0. After selling → state flips to 1. After skipping → state stays the same.

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<int>& arr, vector<vector<int>>& dp, int isBuy, int idx, int SIZE) {
    if (idx == SIZE) return 0;
    if (dp[idx][isBuy] != -1) return dp[idx][isBuy];
    int profit = 0;
    if (isBuy) {
        int take     = -arr[idx] + util(arr, dp, 0, idx+1, SIZE);  // buy → isBuy=0
        int not_take = util(arr, dp, 1, idx+1, SIZE);               // skip → isBuy=1
        profit = max(take, not_take);
    } else {
        int sell     = arr[idx] + util(arr, dp, 1, idx+1, SIZE);   // sell → isBuy=1
        int not_sell = util(arr, dp, 0, idx+1, SIZE);               // skip → isBuy=0
        profit = max(sell, not_sell);
    }
    return dp[idx][isBuy] = profit;
}

int stockBuySell(vector<int> arr, int n) {
    vector<vector<int>> dp(n, vector<int>(2, -1));
    return util(arr, dp, 1, 0, n);
}
```

**Time: O(n×2) | Space: O(n×2) + O(n) call stack**

---

## Recursion Tree Walkthrough

```
arr = [2, 4, 1, 5],  n=4
```

Entry call: `util(isBuy=1, idx=0)`

```
util(1, 0)   arr[0]=2
├── take: -2 + util(0, 1)
│   │   arr[1]=4
│   ├── sell: 4 + util(1, 2)
│   │   │   arr[2]=1
│   │   ├── take: -1 + util(0, 3)
│   │   │   │   arr[3]=5
│   │   │   ├── sell:     5 + util(1,4)=0  →  5
│   │   │   └── not_sell: util(0,4)=0      →  0
│   │   │   dp[3][0] = max(5, 0) = 5
│   │   │
│   │   └── not_take: util(1, 3)
│   │       │   arr[3]=5
│   │       ├── take:     -5 + util(0,4)=0  →  -5
│   │       └── not_take: util(1,4)=0       →   0
│   │       dp[3][1] = max(-5, 0) = 0
│   │
│   │   util(1,2): take=-1+dp[3][0]=-1+5=4, not_take=dp[3][1]=0
│   │   dp[2][1] = max(4, 0) = 4
│   │
│   └── not_sell: util(0, 2)
│       │   arr[2]=1
│       ├── sell:     1 + util(1,3) = 1+0 = 1    ← dp[3][1] MEMOIZED
│       └── not_sell: util(0,3) = 5               ← dp[3][0] MEMOIZED
│       dp[2][0] = max(1, 5) = 5
│
│   util(0,1): sell=4+dp[2][1]=4+4=8, not_sell=dp[2][0]=5
│   dp[1][0] = max(8, 5) = 8
│
└── not_take: util(1, 1)
    │   arr[1]=4
    ├── take:     -4 + util(0,2) = -4+5 = 1    ← dp[2][0] MEMOIZED
    └── not_take: util(1,2) = 4                ← dp[2][1] MEMOIZED
    dp[1][1] = max(1, 4) = 4

util(1,0): take=-2+dp[1][0]=-2+8=6, not_take=dp[1][1]=4
dp[0][1] = max(6, 4) = 6
```

**Answer: 6**

### dp table after all calls

```
        isBuy=0  isBuy=1
idx=0:     8        6     ← answer is dp[0][1] = 6
idx=1:     8        4
idx=2:     5        4
idx=3:     5        0
```

Fill order (base first, root last):
```
dp[3][0]=5 → dp[3][1]=0 → dp[2][1]=4 → dp[2][0]=5 → dp[1][0]=8 → dp[1][1]=4 → dp[0][1]=6
```

---

## Approach 2 — Tabulation (Bottom-Up)

Memoization recurses from `idx=0` toward `idx=n` (base). So tabulation fills from `idx=n-1` back to `idx=0`.

```cpp
int stockBuySell(vector<int> arr, int n) {
    vector<vector<int>> dp(n+1, vector<int>(2, 0));  // dp[n] = base case = 0
    for (int i = n-1; i >= 0; i--) {
        for (int buy = 0; buy <= 1; buy++) {
            if (buy) {
                int take     = -arr[i] + dp[i+1][0];
                int not_take = dp[i+1][1];
                dp[i][buy] = max(take, not_take);
            } else {
                int sell     = arr[i] + dp[i+1][1];
                int not_sell = dp[i+1][0];
                dp[i][buy] = max(sell, not_sell);
            }
        }
    }
    return dp[0][1];
}
```

### Table fill for the example

```
arr = [2, 4, 1, 5]

dp[4] = [0, 0]   ← base case

i=3 (arr[3]=5):
  buy=0: sell=5+dp[4][1]=5,  not_sell=dp[4][0]=0  → dp[3][0]=5
  buy=1: take=-5+dp[4][0]=-5, not_take=dp[4][1]=0 → dp[3][1]=0

i=2 (arr[2]=1):
  buy=0: sell=1+dp[3][1]=1,  not_sell=dp[3][0]=5  → dp[2][0]=5
  buy=1: take=-1+dp[3][0]=4, not_take=dp[3][1]=0  → dp[2][1]=4

i=1 (arr[1]=4):
  buy=0: sell=4+dp[2][1]=8,  not_sell=dp[2][0]=5  → dp[1][0]=8
  buy=1: take=-4+dp[2][0]=1, not_take=dp[2][1]=4  → dp[1][1]=4

i=0 (arr[0]=2):
  buy=0: sell=2+dp[1][1]=6,  not_sell=dp[1][0]=8  → dp[0][0]=8
  buy=1: take=-2+dp[1][0]=6, not_take=dp[1][1]=4  → dp[0][1]=6

return dp[0][1] = 6
```

**Time: O(n×2) | Space: O(n×2)**

---

## Difference from Stock I

| | Stock I | Stock II |
|---|---|---|
| Transactions | At most 1 | Unlimited |
| Approach | Greedy (one pass) | DP (state tracks buy/sell) |
| State needed | Just track min price | Track whether holding or not |

Stock I only needs to find one best buy-sell pair. Stock II must decide each day whether to buy, sell, or hold — decisions interact across days, which is why DP is needed.

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
| Memoization | O(n) | O(n) + O(n) stack |
| Tabulation | O(n) | O(n) |
