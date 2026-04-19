# Subset Sum

## Problem

Given an array `arr` of `n` integers and an integer `target`, determine if there is a **subset** of the array whose elements sum to exactly `target`.

---

## Example

```
arr = [1, 2, 7, 3],  target = 6
```

```
Subset (1, 2, 3) → sum = 6
```

**Output: True**

---

## The Core Insight

For each element, there are two choices: **pick** it or **skip** it.

State: `f(idx, target)` = can we form `target` using elements from `arr[0..idx-1]`?

```
f(idx, target) = f(idx-1, target)                          ← skip arr[idx-1]
              || f(idx-1, target - arr[idx-1])              ← pick arr[idx-1], if target >= arr[idx-1]
```

**Base cases:**
- `target == 0` → always `true` (empty subset sums to 0)
- `idx == 0` and `target != 0` → `false` (no elements left, target not met)

**Answer:** `f(SIZE, target)`

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<int>& arr, vector<vector<int>>& dp, int target, int idx) {
    if (target == 0) return 1;
    if (idx == 0)    return 0;
    if (dp[idx][target] != -1) return dp[idx][target];

    int not_pick = util(arr, dp, target, idx - 1);
    int pick = 0;
    if (target - arr[idx - 1] >= 0)
        pick = util(arr, dp, target - arr[idx - 1], idx - 1);

    return dp[idx][target] = pick || not_pick;
}

bool isSubsetSum(vector<int> arr, int target) {
    int SIZE = arr.size();
    vector<vector<int>> dp(SIZE + 1, vector<int>(target + 1, -1));
    return util(arr, dp, target, SIZE);
}
```

**Time: O(n × target) | Space: O(n × target) + O(n) call stack**

---

## Dry Run — Memoization

```
arr = [1, 2, 7, 3],  target = 6,  SIZE = 4
```

Call tree (only non-memoized paths shown):

```
f(4, 6)
├── skip: f(3, 6)
│   ├── skip: f(2, 6)
│   │   ├── skip: f(1, 6)
│   │   │   ├── skip: f(0, 6) → 0
│   │   │   └── pick: f(0, 5) → 0      [6 - arr[0]=1 = 5]
│   │   │   dp[1][6] = 0
│   │   └── pick: f(1, 4)               [6 - arr[1]=2 = 4]
│   │       ├── skip: f(0, 4) → 0
│   │       └── pick: f(0, 3) → 0      [4 - arr[0]=1 = 3]
│   │       dp[1][4] = 0
│   │   dp[2][6] = 0
│   └── pick: f(2, 3)                   [6 - arr[2]=3 = 3]
│       ├── skip: f(1, 3)
│       │   ├── skip: f(0, 3) → 0
│       │   └── pick: f(0, 2) → 0      [3 - arr[0]=1 = 2]
│       │   dp[1][3] = 0
│       └── pick: f(1, 1)               [3 - arr[1]=2 = 1]
│           ├── skip: f(0, 1) → 0
│           └── pick: f(0, 0) → 1  ✓   [1 - arr[0]=1 = 0]
│           dp[1][1] = 1
│       dp[2][3] = 1
│   dp[3][6] = 1
dp[4][6] = 1
```

**Answer: True** — path taken: pick `arr[2]=3`, pick `arr[1]=2`, pick `arr[0]=1` → `1+2+3=6`

---

## Approach 2 — Tabulation (Bottom-Up)

Memoization recurses from `idx=SIZE` down to `idx=0`. Tabulation fills the table from `idx=1` up to `idx=SIZE`.

```cpp
bool isSubsetSum(vector<int> arr, int target) {
    int SIZE = arr.size();
    vector<vector<bool>> dp(SIZE + 1, vector<bool>(target + 1, false));

    for (int i = 0; i <= SIZE; i++) dp[i][0] = true;  // empty subset

    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= target; j++) {
            dp[i][j] = dp[i - 1][j];                          // skip
            if (j >= arr[i - 1])
                dp[i][j] = dp[i][j] || dp[i - 1][j - arr[i - 1]];  // pick
        }
    }

    return dp[SIZE][target];
}
```

**Time: O(n × target) | Space: O(n × target)**

---

## Table Fill

```
arr = [1, 2, 7, 3],  target = 6

dp[i][0] = true for all i  (empty subset always valid)

        j=0   j=1   j=2   j=3   j=4   j=5   j=6
i=0:     T     F     F     F     F     F     F    ← no elements
i=1:     T     T     F     F     F     F     F    ← arr[0]=1
i=2:     T     T     T     T     F     F     F    ← arr[1]=2
i=3:     T     T     T     T     F     F     F    ← arr[2]=7 (too big for j≤6 except j=7)
i=4:     T     T     T     T     T     T     T    ← arr[3]=3

return dp[4][6] = True
```

At `i=4, j=6`: `dp[3][6]=F` (skip 3), `dp[3][3]=T` (pick 3, need 3 from first 3 elements) → `True`

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n × target) | O(n × target) + O(n) stack |
| Tabulation  | O(n × target) | O(n × target) |

---

## Edge Cases

| Input | Output | Reason |
|---|---|---|
| `target = 0` | `True` | Empty subset always valid |
| `arr = [5], target = 5` | `True` | Single element equals target |
| `arr = [3, 7], target = 6` | `False` | No combination sums to 6 |
| `arr = [1, 2, 3], target = 7` | `False` | Max possible sum is 6 |
