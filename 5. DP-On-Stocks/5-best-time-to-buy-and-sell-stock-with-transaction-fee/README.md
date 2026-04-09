# Best Time to Buy and Sell Stock with Transaction Fee

## Problem

Given an array `arr` of `n` integers where `arr[i]` is the stock price on day `i`, and an integer `fee`.

Find the **maximum profit** with **unlimited transactions**, but pay a `fee` on **every sell**.

Rules:
- Hold at most **one share** at a time (must sell before buying again)
- A transaction fee `fee` is deducted each time you **sell**
- If no profit is possible, return `0`

---

## Example

```
arr = [1, 3, 2, 8, 4, 9],  fee = 2
       0  1  2  3  4  5    (day index)
```

```
Buy  day 0 (price=1), sell day 3 (price=8) → profit = 8 - 1 - 2 = 5
Buy  day 4 (price=4), sell day 5 (price=9) → profit = 9 - 4 - 2 = 3
Total = 8
```

**Output: 8**

---

## The Core Insight

This is Stock II (unlimited transactions) with one change: subtract `fee` from profit whenever you sell.

State: `f(idx, isBuy)` = max profit from day `idx` to the last day, given current state.

- `isBuy=1` → can buy (don't hold stock)
- `isBuy=0` → can sell (hold stock)

```
isBuy=1:  f(idx, 1) = max( -arr[idx] + f(idx+1, 0),          ← buy today
                             f(idx+1, 1) )                      ← skip

isBuy=0:  f(idx, 0) = max( (arr[idx] - fee) + f(idx+1, 1),   ← sell today (pay fee)
                             f(idx+1, 0) )                      ← skip (still holding)
```

**Base case:** `idx == n` → no days left, return `0`.

**Answer:** `f(0, 1)` — start at day 0, initially can buy.

**Key rule:** buy → `isBuy` flips to 0. Sell → `isBuy` flips to 1, fee deducted. Skip → state stays the same.

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<int>& arr, vector<vector<int>>& dp, int SIZE, int idx, int isBuy, int fee) {
    if (idx >= SIZE) return 0;
    if (dp[idx][isBuy] != -1) return dp[idx][isBuy];
    int profit = 0;
    if (isBuy) {
        int buy     = -arr[idx] + util(arr, dp, SIZE, idx+1, 0, fee);        // buy → isBuy=0
        int not_buy = util(arr, dp, SIZE, idx+1, 1, fee);                     // skip → isBuy=1
        profit = max(buy, not_buy);
    } else {
        int sell     = (arr[idx] - fee) + util(arr, dp, SIZE, idx+1, 1, fee); // sell → isBuy=1
        int not_sell = util(arr, dp, SIZE, idx+1, 0, fee);                     // skip → isBuy=0
        profit = max(sell, not_sell);
    }
    return dp[idx][isBuy] = profit;
}

int stockBuySell(vector<int> arr, int n, int fee) {
    vector<vector<int>> dp(n, vector<int>(2, -1));
    return util(arr, dp, n, 0, 1, fee);
}
```

**Time: O(n×2) | Space: O(n×2) + O(n) call stack**

---

## Recursion Tree Walkthrough

```
arr = [1, 3, 2, 8, 4, 9],  fee = 2,  n = 6
```

Entry call: `util(idx=0, isBuy=1)`

```
f(0,1)  arr[0]=1
├── buy: -1 + f(1,0)
│   f(1,0)  arr[1]=3
│   ├── sell: (3-2) + f(2,1) = 1 + f(2,1)
│   │   f(2,1)  arr[2]=2
│   │   ├── buy: -2 + f(3,0)
│   │   │   f(3,0)  arr[3]=8
│   │   │   ├── sell: (8-2) + f(4,1) = 6 + f(4,1)
│   │   │   │   f(4,1)  arr[4]=4
│   │   │   │   ├── buy: -4 + f(5,0)
│   │   │   │   │   f(5,0)  arr[5]=9
│   │   │   │   │   ├── sell: (9-2)+f(6,1)=7   → 7
│   │   │   │   │   └── skip: f(6,0)=0          → 0
│   │   │   │   │   dp[5][0] = 7
│   │   │   │   ├── buy: -4+7 = 3
│   │   │   │   └── skip: f(5,1)
│   │   │   │       ├── buy: -9+f(6,0)=-9  → -9
│   │   │   │       └── skip: f(6,1)=0     →  0
│   │   │   │       dp[5][1] = 0
│   │   │   │   dp[4][1] = max(3, 0) = 3
│   │   │   ├── sell: 6+3 = 9
│   │   │   └── skip: f(4,0)
│   │   │       ├── sell: (4-2)+f(5,1)=2+0=2
│   │   │       └── skip: f(5,0)=7
│   │   │       dp[4][0] = 7
│   │   │   dp[3][0] = max(9, 7) = 9  ← MEMOIZED
│   │   ├── buy: -2+9 = 7
│   │   └── skip: f(3,1)
│   │       ├── buy: -8+f(4,0)=-8+7=-1
│   │       └── skip: f(4,1)=3
│   │       dp[3][1] = 3
│   │   dp[2][1] = max(7, 3) = 7  ← MEMOIZED
│   ├── sell: 1+7 = 8
│   └── skip: f(2,0)
│       ├── sell: (2-2)+f(3,1)=0+3=3
│       └── skip: f(3,0)=9
│       dp[2][0] = 9
│   dp[1][0] = max(8, 9) = 9  ← MEMOIZED
├── buy: -1+9 = 8
└── skip: f(1,1)
    ├── buy: -3+f(2,0)=-3+9=6
    └── skip: f(2,1)=7
    dp[1][1] = 7

dp[0][1] = max(8, 7) = 8
```

**Answer: 8**

### dp table after all calls

```
        isBuy=0  isBuy=1
idx=0:     -       8     ← answer is dp[0][1] = 8
idx=1:     9       7
idx=2:     9       7
idx=3:     9       3
idx=4:     7       3
idx=5:     7       0
```

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int stockBuySell(vector<int> arr, int n, int fee) {
    vector<vector<int>> dp(n+1, vector<int>(2, 0));  // dp[n] = base case = 0
    for (int idx = n-1; idx >= 0; idx--) {
        for (int buy = 0; buy <= 1; buy++) {
            if (buy) {
                int b  = -arr[idx] + dp[idx+1][0];
                int nb = dp[idx+1][1];
                dp[idx][buy] = max(b, nb);
            } else {
                int sell     = (arr[idx] - fee) + dp[idx+1][1];
                int not_sell = dp[idx+1][0];
                dp[idx][buy] = max(sell, not_sell);
            }
        }
    }
    return dp[0][1];
}
```

### Table fill for the example

```
arr = [1, 3, 2, 8, 4, 9],  fee = 2

dp[6] = [0, 0]   ← base case

idx=5 (arr[5]=9):
  buy=0: sell=(9-2)+dp[6][1]=7,  not_sell=dp[6][0]=0  → dp[5][0]=7
  buy=1: b=-9+dp[6][0]=-9,       nb=dp[6][1]=0         → dp[5][1]=0

idx=4 (arr[4]=4):
  buy=0: sell=(4-2)+dp[5][1]=2,  not_sell=dp[5][0]=7  → dp[4][0]=7
  buy=1: b=-4+dp[5][0]=3,        nb=dp[5][1]=0         → dp[4][1]=3

idx=3 (arr[3]=8):
  buy=0: sell=(8-2)+dp[4][1]=9,  not_sell=dp[4][0]=7  → dp[3][0]=9
  buy=1: b=-8+dp[4][0]=-1,       nb=dp[4][1]=3         → dp[3][1]=3

idx=2 (arr[2]=2):
  buy=0: sell=(2-2)+dp[3][1]=3,  not_sell=dp[3][0]=9  → dp[2][0]=9
  buy=1: b=-2+dp[3][0]=7,        nb=dp[3][1]=3         → dp[2][1]=7

idx=1 (arr[1]=3):
  buy=0: sell=(3-2)+dp[2][1]=8,  not_sell=dp[2][0]=9  → dp[1][0]=9
  buy=1: b=-3+dp[2][0]=6,        nb=dp[2][1]=7         → dp[1][1]=7

idx=0 (arr[0]=1):
  buy=0: sell=(1-2)+dp[1][1]=-1, not_sell=dp[1][0]=9  → dp[0][0]=9
  buy=1: b=-1+dp[1][0]=8,        nb=dp[1][1]=7         → dp[0][1]=8

return dp[0][1] = 8
```

**Time: O(n×2) | Space: O(n×2)**

---

## Difference from Stock II

| | Stock II | Stock with Fee |
|---|---|---|
| Transactions | Unlimited | Unlimited |
| Fee | None | `fee` deducted on each sell |
| Change | `sell = arr[idx] + f(idx+1, 1)` | `sell = (arr[idx] - fee) + f(idx+1, 1)` |

Only a single-term change — the fee is subtracted at the point of selling.

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n) | O(n) + O(n) stack |
| Tabulation | O(n) | O(n) |
