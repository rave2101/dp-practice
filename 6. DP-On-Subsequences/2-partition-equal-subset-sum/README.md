# Partition Equal Subset Sum

## Problem

Given an array `arr` of `n` integers, return `true` if the array can be partitioned into **two subsets** such that the sum of elements in both subsets is equal, else return `false`.

---

## Examples

```
arr = [1, 10, 21, 10]  →  True
Partition: [1, 10, 10] and [21]  (both sum to 22)

arr = [1, 2, 3, 5]     →  False
No way to split into two equal-sum subsets
```

---

## The Core Insight

If total `sum` is **odd**, equal partition is impossible — return `false` immediately.

If `sum` is even, we need one subset with sum = `sum/2`. The other subset automatically has the same sum.

So this reduces to: **"Does any subset sum to `sum/2`?"** — identical to the Subset Sum problem.

```
target = sum / 2
answer = isSubsetSumPossible(arr, target)
```

---

## Approach 1 — Memoization (Top-Down)

State: `f(idx, target)` = can we form `target` using elements from `arr[0..idx]`?

```
f(idx, target) = f(idx-1, target)                         ← skip arr[idx]
              || f(idx-1, target - arr[idx])               ← pick arr[idx], if target >= arr[idx]
```

**Base cases:**
- `target == 0` → `true` (empty subset)
- `idx == 0`    → `arr[0] == target` (only one element left, pick it or not)

**Answer:** `f(n-1, sum/2)`

```cpp
bool subsetSumPossible(int idx, int target, vector<int>& arr, vector<vector<int>>& dp) {
    if (target == 0) return true;
    if (idx == 0)    return arr[idx] == target;
    if (dp[idx][target] != -1) return dp[idx][target];

    bool not_pick = subsetSumPossible(idx - 1, target, arr, dp);
    bool pick = false;
    if (target >= arr[idx])
        pick = subsetSumPossible(idx - 1, target - arr[idx], arr, dp);

    return dp[idx][target] = pick || not_pick;
}

bool equalPartition(int n, vector<int> arr) {
    int sum = 0;
    for (int& x : arr) sum += x;
    if (sum % 2 != 0) return false;
    vector<vector<int>> dp(n, vector<int>((sum / 2) + 1, -1));
    return subsetSumPossible(n - 1, sum / 2, arr, dp);
}
```

**Time: O(n × sum/2) | Space: O(n × sum/2) + O(n) call stack**

> **Note:** The memoization here uses 0-indexed `idx` directly into `arr`, so `arr[idx]` is correct (unlike the tabulation approach which uses 1-indexed `idx` and needs `arr[idx-1]`).

---

## Dry Run — Memoization

```
arr = [1, 10, 21, 10],  sum = 42,  target = 21
```

```
f(3, 21)   arr[3]=10
├── skip:  f(2, 21)
│   │      arr[2]=21
│   ├── skip: f(1, 21)
│   │   │    arr[1]=10
│   │   ├── skip: f(0, 21) → arr[0]=1 ≠ 21 → false
│   │   └── pick: f(0, 11) → arr[0]=1 ≠ 11 → false   [21-10=11]
│   │   dp[1][21] = false
│   └── pick: f(1, 0) → target==0 → true ✓             [21-21=0]
│   dp[2][21] = true
dp[3][21] = true
```

**Answer: True** — path taken: pick `arr[2]=21` → subset `{21}`, remaining `{1,10,10}` sums to 21.

---

## Approach 2 — Tabulation (Bottom-Up)

`dp[idx][t]` = can we form sum `t` using the first `idx` elements (1-indexed)?

**Init:** `dp[i][0] = true` for all `i` (empty subset always valid).

**Transition:**
```
dp[idx][t] = dp[idx-1][t]                              ← skip arr[idx-1]
           | dp[idx-1][t - arr[idx-1]]                 ← pick arr[idx-1], if t >= arr[idx-1]
```

```cpp
bool isSubsetSumPossible(vector<vector<bool>>& dp, vector<int>& arr, int target, int SIZE) {
    for (int idx = 1; idx <= SIZE; idx++) {
        for (int t = 1; t <= target; t++) {
            dp[idx][t] = dp[idx - 1][t];
            if (t - arr[idx - 1] >= 0)
                dp[idx][t] = dp[idx][t] || dp[idx - 1][t - arr[idx - 1]];
        }
    }
    return dp[SIZE][target];
}

bool equalPartition(int n, vector<int> arr) {
    int sum = 0, target;
    for (int& num : arr) sum += num;
    if (sum % 2 != 0) return false;
    target = sum / 2;
    vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));
    for (int i = 0; i <= n; i++) dp[i][0] = true;
    return isSubsetSumPossible(dp, arr, target, n);
}
```

**Time: O(n × sum/2) | Space: O(n × sum/2)**

---

## Table Fill

```
arr = [1, 10, 21, 10],  target = 21

dp[i][0] = true for all i

        t=0   t=1   t=2  ...  t=10  t=11  ...  t=21
i=0:     T     F     F         F     F           F     ← no elements
i=1:     T     T     F         F     F           F     ← arr[0]=1
i=2:     T     T     F         T     T           F     ← arr[1]=10
i=3:     T     T     F         T     T           T     ← arr[2]=21  ← dp[3][21]=T ✓
i=4:     T     T     F         T     T           T     ← arr[3]=10

return dp[4][21] = True
```

At `i=3, t=21`: `dp[2][21]=F` (skip 21), `dp[2][0]=T` (pick 21, need 0 from first 2) → `True`

---

## Why This Reduces to Subset Sum

Splitting into two equal parts means each part = `total/2`. You don't need to find both subsets explicitly — finding one subset with sum = `total/2` guarantees the remaining elements sum to `total/2` as well.

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n × sum/2) | O(n × sum/2) + O(n) stack |
| Tabulation  | O(n × sum/2) | O(n × sum/2) |

---

## Edge Cases

| Input | Output | Reason |
|---|---|---|
| `sum` is odd | `False` | Can't split odd total equally |
| `arr = [2, 2]` | `True` | Each element forms one subset |
| `arr = [1, 1, 1, 1]` | `True` | Two elements per subset |
| `arr = [1, 5, 11, 5]` | `True` | `{11}` and `{1,5,5}` |
