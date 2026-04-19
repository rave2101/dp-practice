# Frog Jump

## Problem

A frog starts at step `0` and wants to reach step `n-1`. Each step has a height given by `heights[i]`.

To jump from step `i` to step `j`, the frog spends `abs(heights[i] - heights[j])` energy. The frog can jump either **1 step** or **2 steps** forward at a time.

Return the **minimum total energy** to reach the last step.

---

## Example

```
Index:   0    1    2    3    4
Height: [10, 20,  30,  10,  10]
```

All possible paths and their costs:

```
0→1→2→3→4:  |10-20| + |20-30| + |30-10| + |10-10|  = 10+10+20+0 = 40
0→2→4:       |10-30| + |30-10|                       = 20+20      = 40
0→1→3→4:    |10-20| + |20-10| + |10-10|              = 10+10+0    = 20  ← minimum
0→2→3→4:    |10-30| + |30-10| + |10-10|              = 20+20+0    = 40
0→1→2→4:    |10-20| + |20-30| + |30-10|              = 10+10+20   = 40
```

**Output: 20**

---

## The Core Insight

At every step `i`, the frog could only have arrived from step `i-1` or step `i-2`. So:

```
dp[i] = min(
    dp[i-1] + abs(heights[i] - heights[i-1]),   // jumped from 1 step back
    dp[i-2] + abs(heights[i] - heights[i-2])    // jumped from 2 steps back
)
```

`dp[i]` represents the **minimum energy to reach step i**.

### Building the dp table for the example

```
dp[0] = 0                                               (already here)
dp[1] = dp[0] + |20-10|            = 0  + 10  = 10
dp[2] = min(dp[1]+|30-20|, dp[0]+|30-10|) = min(20,20) = 20
dp[3] = min(dp[2]+|10-30|, dp[1]+|10-20|) = min(40,20) = 20
dp[4] = min(dp[3]+|10-10|, dp[2]+|10-30|) = min(20,40) = 20
```

---

## Approach 1 — Memoization (Top-Down)

Ask: *"what is the minimum energy to reach step idx?"*
Recurse down to smaller subproblems, cache results to avoid recomputation.

```cpp
int solve(vector<int>& heights, vector<int>& dp, int idx) {
    if (idx == 0) return 0;
    if (idx == 1) return abs(heights[1] - heights[0]);
    if (dp[idx] != -1) return dp[idx];
    int second_last = abs(heights[idx - 2] - heights[idx]) + solve(heights, dp, idx - 2);
    int last        = abs(heights[idx - 1] - heights[idx]) + solve(heights, dp, idx - 1);
    dp[idx] = min(last, second_last);
    return dp[idx];
}
```

### How the calls look for `heights = [10, 20, 30, 10, 10]`

```
→ solve(4)
  → solve(3)
    → solve(2)
      → solve(1) = 10 ✓             [base case]
      → solve(0) = 0  ✓             [base case]
    ← solve(2) = min(10+10, 0+20) = 20    [dp[2] = 20]
    → solve(1) = 10 ✓               [base case]
  ← solve(3) = min(20+20, 10+10) = 20     [dp[3] = 20]
  → solve(2) ← CACHE HIT, returns 20 instantly
← solve(4) = min(20+0, 20+20) = 20        [dp[4] = 20]
```

**Time: O(n) | Space: O(n) cache + O(n) call stack**

---

## Approach 2 — Space-Optimized Tabulation (Bottom-Up)

Start from the base cases and build forward. Since `dp[i]` only depends on the two previous values, two variables are enough — no array needed.

```cpp
int frogJump(vector<int>& heights) {
    int N = heights.size();
    if (N == 1) return 0;
    if (N == 2) return abs(heights[1] - heights[0]);
    int second_last = 0, last = abs(heights[1] - heights[0]);
    int current;
    for (int idx = 2; idx < N; idx++) {
        current = min(second_last + abs(heights[idx - 2] - heights[idx]),
                      last        + abs(heights[idx - 1] - heights[idx]));
        second_last = last;
        last = current;
    }
    return current;
}
```

### How the variables shift for `heights = [10, 20, 30, 10, 10]`

```
Start:   second_last = 0 (dp[0]), last = 10 (dp[1])

idx=2:   current = min(0+|10-30|, 10+|20-30|) = min(20,20) = 20
         second_last = 10, last = 20

idx=3:   current = min(10+|20-10|, 20+|30-10|) = min(20,40) = 20
         second_last = 20, last = 20

idx=4:   current = min(20+|30-10|, 20+|10-10|) = min(40,20) = 20
         second_last = 20, last = 20

return current = 20
```

At every step, `second_last` = min energy to reach `idx-2`, `last` = min energy to reach `idx-1`.

**Time: O(n) | Space: O(1)**

---

## Edge Cases

| Input | Output | Reason |
|---|---|---|
| `[10]` | `0` | Already at the destination |
| `[10, 30]` | `20` | Only one jump possible: `\|30-10\| = 20` |

---

## Memoization vs Space-Optimized Tabulation

| | Memoization | Space-Optimized Tabulation |
|---|---|---|
| Direction | `n-1` → down to base cases | base cases → up to `n-1` |
| Mechanism | Recursion + cache | Iterative loop + 2 variables |
| Space | O(n) cache + O(n) stack | O(1) |
| Stack overflow risk | Yes (large n) | No |

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n) | O(n) |
| Space-Optimized Tabulation | O(n) | O(1) |
