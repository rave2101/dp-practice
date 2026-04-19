# Maximum Sum of Non-Adjacent Elements

## Problem

Given an array `nums`, find the **maximum sum** of elements such that no two chosen elements are adjacent.

---

## Example

```
Index:  0   1   2   3   4
nums:  [2,  1,  4,  9,  3]
```

```
Pick 0, 2, 4: 2 + 4 + 3  = 9
Pick 0, 3:    2 + 9       = 11
Pick 1, 3:    1 + 9       = 10
Pick 2, 4:    4 + 3       = 7
Pick 0, 2:    2 + 4       = 6
```

**Output: 11**

---

## The Core Insight

At every index `i`, you have two choices: pick it or skip it.

- **Pick:** take `nums[i]` + best sum from `i-2` (can't use `i-1`)
- **Skip:** take best sum from `i-1`

```
dp[i] = max(nums[i] + dp[i-2], dp[i-1])
```

### Building the dp table for the example

```
dp[0] = 2
dp[1] = max(1, 2)               = 2
dp[2] = max(4 + 2, 2)           = 6
dp[3] = max(9 + 2, 6)           = 11
dp[4] = max(3 + 6, 11)          = 11
```

---

## Approach 1 — Memoization (Top-Down)

Ask: *"what is the max sum I can get from indices 0..idx?"*  
Recurse from the last index, cache results.

```cpp
int util(vector<int>& nums, int idx, vector<int>& dp) {
    if (idx < 0) return 0;
    if (dp[idx] != -1) return dp[idx];
    dp[idx] = max(nums[idx] + util(nums, idx - 2, dp),
                              util(nums, idx - 1, dp));
    return dp[idx];
}
```

**Time: O(n) | Space: O(n) cache + O(n) call stack**

---

## Approach 2 — Space-Optimized

`dp[i]` only depends on the previous two values — track them directly.

```cpp
int nonAdjacent(vector<int>& nums) {
    int n = nums.size();
    if (n == 1) return nums[0];
    if (n == 2) return max(nums[0], nums[1]);
    int second_last = nums[0], last = max(nums[0], nums[1]), current = 0;
    for (int idx = 2; idx < n; idx++) {
        current = max(nums[idx] + second_last, last);
        second_last = last;
        last = current;
    }
    return current;
}
```

**Time: O(n) | Space: O(1)**

---

## Relation to House Robber

This is the same problem as **House Robber (linear)**. The only difference is naming — `money[]` vs `nums[]`. The recurrence, approaches, and complexity are identical.

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n) | O(n) |
| Space-Optimized | O(n) | O(1) |
