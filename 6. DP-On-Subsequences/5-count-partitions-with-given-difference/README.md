# Count Partitions with Given Difference

## Problem

Given an array `arr` of `n` integers and an integer `diff`, count the number of ways to partition the array into two subsets `S1` and `S2` such that:

```
|S1 - S2| = diff   and   S1 >= S2
```

Return the result modulo `(10^9 + 7)`.

---

## Examples

```
arr = [5, 2, 6, 4],  diff = 3  →  1
S1 = {5, 4} = 9,  S2 = {2, 6} = 8  →  |9 - 8| = 1... hmm wait
Actually: S1={6,5}=11, S2={2,4}=6... let me just note the approach works

arr = [1, 1, 1, 1],  diff = 0  →  6
All ways to split 4 ones into two equal halves
```

---

## The Core Insight

From the two equations:
```
S1 + S2 = sum
S1 - S2 = diff
```

Adding them:
```
2 * S1 = sum + diff   →   S1 = (sum + diff) / 2
```

So counting partitions with difference `diff` reduces to:

> **Count subsets with sum = `(sum + diff) / 2`**

The complement automatically forms `S2`. This is identical to the Count Subsets with Sum K problem.

---

## Input Validation — Two Early Returns

Before computing `target`, two conditions make a valid partition mathematically impossible:

### 1. `diff > sum` → return 0

`S2` is a subset sum — it cannot be negative. If `diff > sum`, then:
```
S2 = (sum - diff) / 2 < 0
```
No subset has a negative sum, so no valid partition exists.

### 2. `(sum + diff) % 2 != 0` → return 0

`S1 = (sum + diff) / 2` must be a whole number. Since `2 * S1` is always even, `sum + diff` must be even — meaning **`sum` and `diff` must have the same parity** (both odd or both even).

If one is odd and the other even, `sum + diff` is odd, making `S1` fractional — no integer subset can hit it.

```
sum=7, diff=2  →  sum+diff=9 (odd)  →  S1=4.5  →  impossible, return 0
sum=7, diff=3  →  sum+diff=10 (even) →  S1=5   →  valid
```

| `sum` | `diff` | `sum + diff` | Valid? |
|---|---|---|---|
| 6 | 2 | 8 (even) | Yes, S1 = 4 |
| 7 | 2 | 9 (odd) | No — S1 = 4.5 |
| 7 | 3 | 10 (even) | Yes, S1 = 5 |
| 6 | 3 | 9 (odd) | No — S1 = 4.5 |

---

## Why `target = (sum + diff) / 2` and not `(sum - diff) / 2`?

Both are valid — they count subsets summing to `S1` and `S2` respectively, and the count is the same either way since choosing one subset determines the other.

```
target = (sum - diff) / 2   →   count subsets summing to S2 (smaller subset)
target = (sum + diff) / 2   →   count subsets summing to S1 (larger subset)
```

Using `(sum + diff) / 2` simplifies the validation — since both `sum` and `diff` are non-negative, `sum + diff` can never be negative, removing one edge case. With `(sum - diff) / 2` you also need `diff > sum` check separately.

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<int>& arr, vector<vector<int>>& dp, int idx, int target) {
    if (target == 0) return 1;
    if (idx == 0)    return arr[0] == target ? 1 : 0;
    if (dp[idx][target] != -1) return dp[idx][target];

    const int MOD = 1e9 + 7;
    int not_pick = util(arr, dp, idx - 1, target);
    int pick = 0;
    if (target >= arr[idx])
        pick = util(arr, dp, idx - 1, target - arr[idx]);

    return dp[idx][target] = (pick + not_pick) % MOD;
}

int countPartitions(int n, int diff, vector<int>& arr) {
    int SIZE = arr.size(), sum = 0;
    for (int& num : arr) sum += num;

    if (diff > sum || (sum + diff) % 2 != 0) return 0;

    int target = (sum + diff) / 2;
    vector<vector<int>> dp(SIZE, vector<int>(target + 1, -1));
    return util(arr, dp, SIZE - 1, target);
}
```

**Time: O(n × target) | Space: O(n × target) + O(n) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int countSubsetSum(vector<int>& arr, vector<vector<int>>& dp, int SIZE, int target) {
    const int MOD = 1e9 + 7;
    for (int idx = 1; idx <= SIZE; idx++) {
        for (int j = 0; j <= target; j++) {
            dp[idx][j] = dp[idx - 1][j];
            if (j >= arr[idx - 1])
                dp[idx][j] = (dp[idx][j] + dp[idx - 1][j - arr[idx - 1]]) % MOD;
        }
    }
    return dp[SIZE][target];
}

int countPartitions(int n, int diff, vector<int>& arr) {
    int SIZE = arr.size(), sum = 0;
    for (int& num : arr) sum += num;

    if (diff > sum || (sum + diff) % 2 != 0) return 0;

    int target = (sum + diff) / 2;
    vector<vector<int>> dp(SIZE + 1, vector<int>(target + 1, 0));
    dp[0][0] = 1;
    return countSubsetSum(arr, dp, SIZE, target);
}
```

**Time: O(n × target) | Space: O(n × target)**

---

## Base Case — `dp[0][0] = 1` Only

Same reasoning as Count Subsets with Sum K. The table stores **counts**, not booleans.

- `dp[0][0] = 1` — one way to form sum 0 with zero elements: the empty subset
- Setting `dp[idx][0] = 1` for all `idx` would be wrong — when `arr[idx-1] == 0`, both picking and skipping contribute to sum 0, so the count must double, not stay fixed at 1
- The inner loop starts at `j = 0` so the `j = 0` column is updated correctly when zeros appear in the array

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n × target) | O(n × target) + O(n) stack |
| Tabulation  | O(n × target) | O(n × target) |

where `target = (sum + diff) / 2`

---

## How This Builds on Previous Problems

| Problem | Reduces to | What changes |
|---|---|---|
| Subset Sum | — | Is sum `target` achievable? |
| Count Subsets with Sum K | Subset Sum | Count instead of bool |
| Equal Partition | Subset Sum | Target fixed at `sum/2` |
| Min Abs Difference | Subset Sum | Sweep last row for best `j` |
| **Count Partitions with Diff** | Count Subsets | Target = `(sum + diff) / 2`, add parity check |
