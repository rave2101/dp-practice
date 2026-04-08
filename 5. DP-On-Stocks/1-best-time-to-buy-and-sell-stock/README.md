# Best Time to Buy and Sell Stock

## Problem

Given an array `arr` of `n` integers where `arr[i]` is the stock price on day `i`.

Find the **maximum profit** from at most **one** buy and one sell.

Rules:
- You must buy **before** you sell
- Buy and sell cannot happen on the **same day**
- If no profit is possible, return `0`

---

## Example

```
arr = [10, 7, 5, 8, 11, 9]
         0  1  2  3   4  5   (day index)
```

```
Buy on day 2 (price = 5), sell on day 4 (price = 11)
Profit = 11 - 5 = 6
```

**Output: 6**

---

## The Core Insight

For each day `i`, the best profit achievable by selling on day `i` is:

```
profit = arr[i] - min(arr[0..i-1])
```

Track the **running minimum** seen so far (cheapest buy price up to today).  
At each price, compute `price - min_so_far` and update the global maximum.

---

## Dry Run

```
arr = [10, 7, 5, 8, 11, 9]

i=0: min_so_far = 10,  max_profit = 0         (init)
i=1: arr[1]=7  < 10  → min_so_far = 7
     profit = 7 - 7  = 0   → max_profit = 0
i=2: arr[2]=5  < 7   → min_so_far = 5
     profit = 5 - 5  = 0   → max_profit = 0
i=3: arr[3]=8  > 5   → min_so_far = 5
     profit = 8 - 5  = 3   → max_profit = 3
i=4: arr[4]=11 > 5   → min_so_far = 5
     profit = 11 - 5 = 6   → max_profit = 6   ← best
i=5: arr[5]=9  > 5   → min_so_far = 5
     profit = 9 - 5  = 4   → max_profit = 6

return 6
```

---

## Solution

```cpp
int stockBuySell(vector<int> arr, int n) {
    int min_so_far = arr[0];
    int max_profit = 0;
    int SIZE = arr.size();
    for (int i = 1; i < SIZE; i++) {
        min_so_far = min(min_so_far, arr[i]);
        max_profit = max(max_profit, arr[i] - min_so_far);
    }
    return max_profit;
}
```

**Time: O(n) | Space: O(1)**

---

## Why not DP?

This problem is solvable greedily in a single pass — no overlapping subproblems or choices to memoize. The more complex stock problems (multiple transactions, cooldown, fee) require DP because decisions interact across days.

---

## Edge Cases

| Input | Output | Reason |
|---|---|---|
| `[5, 4, 3, 2, 1]` | `0` | Prices always fall — never profitable |
| `[1, 2]` | `1` | Buy day 0, sell day 1 |
| `[3, 3, 3]` | `0` | No profit possible (same price) |
