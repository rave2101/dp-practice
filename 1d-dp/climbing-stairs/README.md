# Climbing Stairs

## Problem

You are climbing a staircase with `n` steps. Each time you can climb either 1 or 2 steps. Return the number of distinct ways to reach the top.

```
n = 1 → 1    (1)
n = 2 → 2    (1+1, 2)
n = 3 → 3    (1+1+1, 1+2, 2+1)
n = 4 → 5    (1+1+1+1, 1+1+2, 1+2+1, 2+1+1, 2+2)
```

---

## Why dp[i] = dp[i-1] + dp[i-2]

To reach stair `i`, your last move was either:
- A **1-step** from stair `i-1` — there are `dp[i-1]` ways to reach `i-1`, each extendable by one step
- A **2-step** from stair `i-2` — there are `dp[i-2]` ways to reach `i-2`, each extendable by two steps

These two groups are **mutually exclusive** (a path has exactly one last move) and **exhaustive** (no other last move is possible). So every path to `i` belongs to exactly one group.

```
dp[4] = dp[3] + dp[2] = 3 + 2 = 5

dp[3] group (last move is 3 → 4):     dp[2] group (last move is 2 → 4):
  1+1+1  +1  =  1+1+1+1                 1+1  +2  =  1+1+2
  1+2    +1  =  1+2+1                   2    +2  =  2+2
  2+1    +1  =  2+1+1
```

The recurrence is identical to Fibonacci, shifted by one position.

---

## Approach 1 — Memoization (Top-Down)

Start from `n` and recurse down. Cache results so each subproblem is solved only once.

```cpp
class Solution {
public:
    int climbStairs(int n) {
        vector<int> cache(n + 1, -1);
        return solve(n, cache);
    }

    int solve(int n, vector<int>& cache) {
        if (n <= 2) return n;
        if (cache[n] != -1) return cache[n];
        cache[n] = solve(n - 1, cache) + solve(n - 2, cache);
        return cache[n];
    }
};
```

### How the calls look for `n = 5`

```
→ solve(5)
  → solve(4)
    → solve(3)
      → solve(2) = 2 ✓           [cache[2] = 2]
      → solve(1) = 1 ✓
    ← solve(3) = 3               [cache[3] = 3]
    → solve(2) ← CACHE HIT, returns 2 instantly
  ← solve(4) = 5                 [cache[4] = 5]
  → solve(3) ← CACHE HIT, returns 3 instantly
← solve(5) = 8                   [cache[5] = 8]
```

### Cache state at each step

```
Initial:        cache = [-1, -1, -1, -1, -1, -1]
After solve(2): cache = [-1, -1,  2, -1, -1, -1]
After solve(3): cache = [-1, -1,  2,  3, -1, -1]
After solve(4): cache = [-1, -1,  2,  3,  5, -1]
After solve(5): cache = [-1, -1,  2,  3,  5,  8]
```

**Time: O(n) | Space: O(n) cache + O(n) call stack**

---

## Approach 2 — Space-Optimized Tabulation (Bottom-Up)

Since `dp[i]` only needs the two previous values, there is no need to store the full array. Use two variables that slide forward.

```cpp
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 3) return n;
        int second_last = 1, last = 2, current = 0;
        for (int idx = 3; idx <= n; idx++) {
            current = second_last + last;
            second_last = last;
            last = current;
        }
        return current;
    }
};
```

### How the variables shift for `n = 5`

```
Start:   second_last = 1 (ways for n=1), last = 2 (ways for n=2)

idx=3:   current = 1 + 2 = 3    →  second_last = 2, last = 3
idx=4:   current = 2 + 3 = 5    →  second_last = 3, last = 5
idx=5:   current = 3 + 5 = 8    →  second_last = 5, last = 8

return current = 8
```

At each step, the window slides one position forward — `second_last` becomes the old `last`, and `last` becomes the new `current`.

**Time: O(n) | Space: O(1)**

---

## Memoization vs Space-Optimized Tabulation

| | Memoization | Space-Optimized Tabulation |
|---|---|---|
| Direction | `n` → down to base cases | base cases → up to `n` |
| Mechanism | Recursion + cache | Iterative loop + 2 variables |
| Space | O(n) cache + O(n) stack | O(1) |
| Call stack overhead | Yes | No |
| Stack overflow risk | Yes (large n) | No |

---

## Generalizing: k steps allowed

If you can take up to `k` steps at a time, the recurrence becomes:

```
dp[i] = dp[i-1] + dp[i-2] + ... + dp[i-k]
```

Each term accounts for paths whose last move was a jump of that size.

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n) | O(n) |
| Space-Optimized Tabulation | O(n) | O(1) |
