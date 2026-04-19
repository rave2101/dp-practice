# Target Sum

## Problem

Given an array `nums` of `n` integers and an integer `target`, assign a `+` or `-` sign to each element and evaluate the expression. Return the **number of ways** to achieve the target sum. Return the result modulo `(10^9 + 7)`.

---

## Example

```
nums = [1, 2, 7, 1, 5],  target = 4

+1 + 2 + 7 - 1 - 5 = 4   ✓
-1 + 2 + 7 + 1 - 5 = 4   ✓

Output: 2
```

---

## Connection to Count Partitions with Given Difference

This problem is **identical** to [Count Partitions with Given Difference](../5-count-partitions-with-given-difference/solution.cpp).

Here's why:

Every element gets either a `+` or `-` sign. Group the `+` elements into `S1` and the `-` elements into `S2`:

```
Expression value = S1 - S2 = target
Total array sum  = S1 + S2 = sum
```

Adding both equations:
```
2 * S1 = sum + target   →   S1 = (sum + target) / 2
```

So **counting expressions that evaluate to `target`** = **counting subsets with sum `(sum + target) / 2`**.

This is exactly Count Partitions with difference = `target`, where we count subsets summing to `S1 = (sum + target) / 2`.

---

## Side-by-Side Comparison

| | Count Partitions with Diff | Target Sum |
|---|---|---|
| Input | array, `diff` | array, `target` |
| Framing | Split into S1, S2 where S1−S2 = diff | Assign +/− so expression = target |
| Reduces to | Count subsets summing to `(sum+diff)/2` | Count subsets summing to `(sum+target)/2` |
| Code | Identical | Identical |

The only difference is the variable name (`diff` vs `target`). The underlying logic and code are the same.

---

## Input Validation

Two early returns before computing `T = (sum + target) / 2`:

### 1. `abs(target) > sum` → return 0

`target` can be negative (e.g., all elements get `-` sign). The check must use `abs(target)` — using just `target > sum` misses the case where `target` is a large negative:

```
nums = [1, 2],  target = -10
sum = 3,  target = -10

target > sum  →  -10 > 3  →  false  ← passes incorrectly!
T = (3 + (-10)) / 2 = -3  ← negative vector size → crash

abs(target) > sum  →  10 > 3  →  true  ← correctly returns 0
```

### 2. `(sum + target) % 2 != 0` → return 0

`S1 = (sum + target) / 2` must be a whole number. Since `2 * S1` is always even, `sum + target` must be even — `sum` and `target` must have the same parity.

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<int>& nums, vector<vector<int>>& dp, int idx, int target) {
    if (idx == 0) return target == 0 ? 1 : 0;
    if (dp[idx][target] != -1) return dp[idx][target];

    const int MOD = 1e9 + 7;
    int not_pick = util(nums, dp, idx - 1, target);
    int pick = 0;
    if (target >= nums[idx - 1])
        pick = util(nums, dp, idx - 1, target - nums[idx - 1]);

    return dp[idx][target] = (pick + not_pick) % MOD;
}

int targetSum(int n, int target, vector<int>& nums) {
    int sum = 0;
    for (int& num : nums) sum += num;
    if (abs(target) > sum || (sum + target) % 2 != 0) return 0;
    int T = (sum + target) / 2;
    vector<vector<int>> dp(n + 1, vector<int>(T + 1, -1));
    return util(nums, dp, n, T);
}
```

**Time: O(n × T) | Space: O(n × T) + O(n) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int countSubsetSum(vector<int>& nums, vector<vector<int>>& dp, int SIZE, int target) {
    const int MOD = 1e9 + 7;
    for (int idx = 1; idx <= SIZE; idx++) {
        for (int j = 0; j <= target; j++) {
            dp[idx][j] = dp[idx - 1][j];
            if (j >= nums[idx - 1])
                dp[idx][j] = (dp[idx][j] + dp[idx - 1][j - nums[idx - 1]]) % MOD;
        }
    }
    return dp[SIZE][target];
}

int targetSum(int n, int target, vector<int>& nums) {
    int sum = 0;
    for (int& num : nums) sum += num;
    if (abs(target) > sum || (sum + target) % 2 != 0) return 0;
    int T = (sum + target) / 2;
    vector<vector<int>> dp(n + 1, vector<int>(T + 1, 0));
    dp[0][0] = 1;
    return countSubsetSum(nums, dp, n, T);
}
```

**Time: O(n × T) | Space: O(n × T)**

where `T = (sum + target) / 2`

---

## Full Problem Chain

Every problem in this section builds on the same subset sum DP — only the question asked changes:

| Problem | Reduces to | Key formula |
|---|---|---|
| Subset Sum | — | Is sum `K` achievable? |
| Count Subsets with Sum K | Subset Sum | Count ways to reach `K` |
| Equal Partition | Subset Sum | Target = `sum/2` |
| Min Abs Difference | Subset Sum | Sweep last row for best `j` |
| Count Partitions with Diff | Count Subsets | Target = `(sum+diff)/2` |
| **Target Sum** | Count Partitions with Diff | Same math, `+/−` framing |

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n × T) | O(n × T) + O(n) stack |
| Tabulation  | O(n × T) | O(n × T) |
