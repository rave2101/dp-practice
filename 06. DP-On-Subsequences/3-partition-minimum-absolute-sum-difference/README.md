# Partition a Set into Two Subsets with Minimum Absolute Sum Difference

## Problem

Given an array `arr` of `n` integers, partition the array into **two subsets** such that the **absolute difference** between their sums is minimized.

---

## Examples

```
arr = [1, 7, 14, 5]  →  1
Partition: [1, 7, 5] (sum=13) and [14] (sum=14)  →  |13 - 14| = 1

arr = [3, 1, 6, 2, 2]  →  0
Partition: [3, 2, 2] (sum=7) and [6, 1] (sum=7)  →  |7 - 7| = 0
```

---

## The Core Insight

Let `sum` = total array sum, and let subset1 have sum `j`. Then subset2 has sum `sum - j`.

```
absolute difference = |j - (sum - j)| = |2j - sum|
```

To minimize this, we want `j` as close to `sum/2` as possible. So:

1. Run **subset sum DP** for all achievable sums up to `sum/2`
2. Sweep from `j = sum/2` down to `0` — the first achievable `j` gives the minimum difference

> This builds directly on the Subset Sum problem. The only addition is the final sweep to find the best `j`.

**Why `target = sum/2`?**

Integer division already floors — it works for both odd and even totals without any special case. For odd `sum` (e.g., `sum=27`), the two subsets can never be perfectly equal, so the closest split is `j=13` and `sum-j=14`, giving difference 1. `sum/2 = 13` covers this exactly.

---

## Approach 1 — Memoization (Top-Down)

Query `f(n-1, j)` for each `j` from `sum/2` down to `0`. Memoization ensures subproblems are computed only once.

```cpp
bool subsetSumPossible(int idx, int target, vector<int>& arr, vector<vector<int>>& dp) {
    if (target == 0) return true;
    if (idx == 0)    return arr[0] == target;
    if (dp[idx][target] != -1) return dp[idx][target];

    bool not_pick = subsetSumPossible(idx - 1, target, arr, dp);
    bool pick = false;
    if (target >= arr[idx])
        pick = subsetSumPossible(idx - 1, target - arr[idx], arr, dp);

    return dp[idx][target] = pick || not_pick;
}

int minDifference(vector<int>& arr, int n) {
    int sum = 0;
    for (int& x : arr) sum += x;

    int target = sum / 2;
    vector<vector<int>> dp(n, vector<int>(target + 1, -1));

    for (int j = target; j >= 0; j--) {
        if (subsetSumPossible(n - 1, j, arr, dp))
            return abs(sum - 2 * j);
    }
    return sum;
}
```

**Time: O(n × sum/2) | Space: O(n × sum/2) + O(n) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

Fill the entire DP table first, then sweep the last row for the best `j`.

```cpp
void fillSubsetSumDP(vector<int>& arr, vector<vector<bool>>& dp, int target, int SIZE) {
    for (int idx = 1; idx <= SIZE; idx++) {
        for (int s = 1; s <= target; s++) {
            dp[idx][s] = dp[idx - 1][s];
            if (s - arr[idx - 1] >= 0)
                dp[idx][s] = dp[idx][s] || dp[idx - 1][s - arr[idx - 1]];
        }
    }
}

int minDifference(vector<int>& arr, int n) {
    int sum = 0;
    for (int& num : arr) sum += num;

    int target = sum / 2;
    vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));
    for (int i = 0; i <= n; i++) dp[i][0] = true;

    fillSubsetSumDP(arr, dp, target, n);

    for (int j = target; j >= 0; j--) {
        if (dp[n][j]) return abs(sum - 2 * j);
    }
    return sum;
}
```

**Time: O(n × sum/2) | Space: O(n × sum/2)**

---

## Dry Run

```
arr = [1, 7, 14, 5],  sum = 27,  target = sum/2 = 13
```

**DP table (last row = dp[4]):**

```
        s=0   s=1   s=2  ...  s=7   s=8  ...  s=12  s=13
i=0:     T     F     F         F     F          F     F
i=1:     T     T     F         F     F          F     F    ← arr[0]=1
i=2:     T     T     F         T     T          F     F    ← arr[1]=7
i=3:     T     T     F         T     T          F     F    ← arr[2]=14 (too big)
i=4:     T     T     F         T     T          T     T    ← arr[3]=5
```

**Sweep dp[4] from j=13 down:**

```
j=13: dp[4][13] = T  →  return |27 - 26| = 1  ✓
```

Subset with sum 13 = {1, 7, 5}, remaining = {14}, |13 - 14| = **1**

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n × sum/2) | O(n × sum/2) + O(n) stack |
| Tabulation  | O(n × sum/2) | O(n × sum/2) |

---

## Comparison with Related Problems

| Problem | Goal | What changes |
|---|---|---|
| Subset Sum | Does any subset sum to `target`? | Fixed target |
| Equal Partition | Can we split into two equal halves? | Target = `sum/2`, answer is bool |
| Min Abs Difference | Minimize `\|s1 - s2\|` | Target = `sum/2`, sweep last row for best `j` |

Each problem reuses the same subset sum DP — only the question asked about the final table changes.

---

## Edge Cases

| Input | Output | Reason |
|---|---|---|
| `arr = [1, 1]` | `0` | Equal split always possible |
| `arr = [1, 2, 3, 5]` | `1` | Best split: {1,5} and {2,3} |
| `arr = [1]` | `1` | One subset empty, diff = arr[0] |
| All elements equal, even count | `0` | Perfect equal split |
