# Count Subsets with Sum K

## Problem

Given an array `arr` of `n` integers and an integer `K`, count the number of subsets whose elements sum to exactly `K`. Return the result modulo `(10^9 + 7)`.

---

## Examples

```
arr = [2, 3, 5, 16, 8, 10],  K = 10  →  3
Subsets: [2, 8], [10], [2, 3, 5]

arr = [1, 2, 3, 4, 5],  K = 5  →  3
Subsets: [5], [2, 3], [1, 4]
```

---

## The Core Insight

State: `dp[idx][j]` = number of subsets of the first `idx` elements that sum to `j`.

For each element there are two choices — pick or skip:

```
dp[idx][j] = dp[idx-1][j]                        ← skip arr[idx-1]
           + dp[idx-1][j - arr[idx-1]]            ← pick arr[idx-1], if j >= arr[idx-1]
```

This is structurally identical to the Subset Sum problem, with one key difference: instead of storing a boolean (possible or not), we store a **count** (how many ways).

---

## Base Case — Why Only `dp[0][0] = 1`

This is the most important design decision in this problem.

`dp[0][0] = 1` means: **there is exactly one way to form sum 0 using zero elements — the empty subset.**

### Why not `dp[idx][0] = 1` for all `idx` like in Subset Sum?

In the boolean Subset Sum problems, `dp[idx][0] = true` for all `idx` was correct because:
- The empty subset always exists → sum 0 is always *possible*
- Once possible, it stays `true` regardless of new elements

But here the table stores **counts**, not booleans. When `arr[idx-1] == 0`, both picking and skipping it give sum 0 — so the count should **double**, not stay fixed at 1.

```
arr = [0, 0],  K = 0

Valid subsets: {}, {arr[0]}, {arr[1]}, {arr[0], arr[1]}  →  4 ways

With dp[idx][0] = 1 for all idx (wrong):
  dp[1][0] = 1   ← misses the "pick the zero" path
  dp[2][0] = 1   ← answer = 1  ✗

With dp[0][0] = 1 only, j loop from 0 (correct):
  dp[1][0] = dp[0][0] + dp[0][0] = 2   ← skip zero + pick zero
  dp[2][0] = dp[1][0] + dp[1][0] = 4   ← answer = 4  ✓
```

### Why must the inner loop start at `j = 0`?

If the loop starts at `j = 1`, the `j = 0` column is never updated in the transition — it stays hardcoded at whatever was initialized. Starting at `j = 0` lets the transition correctly double `dp[idx][0]` when `arr[idx-1] == 0`.

---

## Comparison: Bool vs Count Base Cases

| | Subset Sum (bool) | Count Subsets (int) |
|---|---|---|
| `dp[idx][0]` | `true` for all `idx` | Only `dp[0][0] = 1` |
| Inner loop starts | `j = 1` | `j = 0` |
| Why | "Possible" stays true once true | Count must grow when zeros are picked |
| Zero in array | No effect on answer | Doubles the count at `j = 0` |

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

int perfectSum(vector<int>& arr, int K) {
    int SIZE = arr.size();
    vector<vector<int>> dp(SIZE, vector<int>(K + 1, -1));
    return util(arr, dp, SIZE - 1, K);
}
```

**Time: O(n × K) | Space: O(n × K) + O(n) call stack**

> Note: the memoization base case `if(target == 0) return 1` implicitly counts the empty subset — it does not need the `j=0` loop fix since it never iterates; it returns directly.

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int perfectSum(vector<int>& arr, int K) {
    int SIZE = arr.size();
    const int MOD = 1e9 + 7;
    vector<vector<int>> dp(SIZE + 1, vector<int>(K + 1, 0));
    dp[0][0] = 1;

    for (int idx = 1; idx <= SIZE; idx++) {
        for (int j = 0; j <= K; j++) {
            dp[idx][j] = dp[idx - 1][j];
            if (j >= arr[idx - 1])
                dp[idx][j] = (dp[idx][j] + dp[idx - 1][j - arr[idx - 1]]) % MOD;
        }
    }
    return dp[SIZE][K];
}
```

**Time: O(n × K) | Space: O(n × K)**

---

## Table Fill

```
arr = [2, 3, 5, 16, 8, 10],  K = 10

        j=0  j=1  j=2  j=3  j=4  j=5  j=6  j=7  j=8  j=9  j=10
i=0:     1    0    0    0    0    0    0    0    0    0    0    ← base
i=1:     1    0    1    0    0    0    0    0    0    0    0    ← arr[0]=2
i=2:     1    0    1    1    0    1    0    0    0    0    0    ← arr[1]=3
i=3:     1    0    1    1    0    2    0    1    1    0    1    ← arr[2]=5
i=4:     1    0    1    1    0    2    0    1    1    0    1    ← arr[3]=16 (too big)
i=5:     1    0    1    1    0    2    0    1    2    0    2    ← arr[4]=8
i=6:     1    0    1    1    0    2    0    1    2    0    3    ← arr[5]=10

return dp[6][10] = 3  ✓
```

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n × K) | O(n × K) + O(n) stack |
| Tabulation  | O(n × K) | O(n × K) |

---

## Edge Cases

| Input | Output | Reason |
|---|---|---|
| `arr=[0,0], K=0` | `4` | `{}`, `{0}`, `{0}`, `{0,0}` |
| `arr=[1,2,3], K=0` | `1` | Only the empty subset |
| `arr=[5], K=5` | `1` | Only `{5}` |
| `arr=[1,1,1], K=2` | `3` | Three ways to pick two 1s |
