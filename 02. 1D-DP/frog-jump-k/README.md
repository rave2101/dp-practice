# Frog Jump — K Distances

## Problem

A frog starts at step `0` and wants to reach step `n-1`. Each step has a height `heights[i]`.

To jump from step `i` to step `j`, the frog spends `abs(heights[i] - heights[j])` energy.  
The frog can jump **1 to k steps** forward at a time.

Return the **minimum total energy** to reach the last step.

---

## Example

```
Index:   0    1    2    3    4
Height: [10, 20,  30,  10,  10]   k = 3
```

From step 0, the frog can reach steps 1, 2, or 3 in one jump.

```
0→1→2→3→4:  |10-20|+|20-30|+|30-10|+|10-10| = 10+10+20+0 = 40
0→1→4:      |10-20|+|20-10|                  = 10+10      = 20  ← not valid (jump of 3 from 1→4)
0→1→2→4:    |10-20|+|20-30|+|30-10|          = 10+10+20   = 40
0→3→4:      |10-10|+|10-10|                  = 0+0        = 0   ← wait idx 0→3 is jump of 3 ✓
            then 3→4 is jump of 1 ✓
```

**Output: 0**  
(Jump from index 0 directly to index 3, cost 0. Then index 3 to index 4, cost 0.)

---

## The Core Insight

At every step `idx`, the frog could have arrived from any of the `k` previous steps.

```
dp[idx] = min over j in [1..k]:
    dp[idx - j] + abs(heights[idx] - heights[idx - j])
```

`dp[i]` = minimum energy to reach step `i`.

### Building the dp table for the example (k=3)

```
dp[0] = 0
dp[1] = dp[0] + |20-10|                              = 10
dp[2] = min(dp[1]+|30-20|, dp[0]+|30-10|)            = min(20, 20) = 20
dp[3] = min(dp[2]+|10-30|, dp[1]+|10-20|, dp[0]+|10-10|) = min(40,20,0) = 0
dp[4] = min(dp[3]+|10-10|, dp[2]+|10-30|, dp[1]+|10-20|) = min(0,40,20) = 0
```

---

## Approach 1 — Memoization (Top-Down)

Ask: *"what is the minimum energy to reach step idx?"*  
Recurse down, try all k jumps, cache the result.

```cpp
int solve(vector<int>& heights, int k, vector<int>& dp, int idx) {
    if (idx == 0) return 0;
    if (idx == 1) return abs(heights[1] - heights[0]);
    if (dp[idx] != INT_MAX) return dp[idx];   // cache hit

    for (int j = 1; j <= k; j++) {
        if (idx - j >= 0) {
            int cost = solve(heights, k, dp, idx - j) + abs(heights[idx - j] - heights[idx]);
            dp[idx] = min(dp[idx], cost);
        }
    }
    return dp[idx];
}
```

### Why the cache check is safe

When `solve(idx)` runs for the first time, it tries **all** j from 1 to k and sets `dp[idx]` to the true minimum before returning. So by the time `dp[idx] != INT_MAX`, the value is already globally optimal — not a partial result.

```
solve(4)
  → solve(3)
      → solve(2) → fully explored → dp[2] = 20
      → solve(1) → base case     → 10
      → solve(0) → base case     → 0
    ← dp[3] = min(20+20, 10+10, 0+0) = 0
  → solve(3) ← CACHE HIT, returns 0 instantly
  → solve(2) ← CACHE HIT, returns 20 instantly
  → solve(1) ← base case, returns 10
← dp[4] = 0
```

**Time: O(n·k) | Space: O(n) cache + O(n) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

Start from the base cases, fill the table forward.

```cpp
int frogJumpTab(vector<int>& heights, int k) {
    int N = heights.size();
    vector<int> dp(N, INT_MAX);
    dp[0] = 0;
    dp[1] = abs(heights[1] - heights[0]);

    for (int idx = 2; idx < N; idx++) {
        for (int j = idx - 1; j >= max(idx - k, 0); j--) {
            dp[idx] = min(dp[idx], dp[j] + abs(heights[idx] - heights[j]));
        }
    }
    return dp[N - 1];
}
```

Note: instead of iterating jump distance `j` from 1..k, here we iterate the source position `j` from `idx-1` down to `idx-k`. Same idea, different loop variable.

### Table walkthrough (k=3)

```
        j=idx-1   j=idx-2   j=idx-3   dp[idx]
idx=2:   10+10     0+20        —        20
idx=3:   20+20     10+10     0+0         0
idx=4:    0+0      20+20     10+10       0
```

**Time: O(n·k) | Space: O(n)**

---

## Approach 3 — Space-Optimized Tabulation (Circular Buffer)

`dp[idx]` only depends on the last `k` values, so we can use an array of size k, indexed by `i % k`.

```cpp
int frogJumpSpaceOpt(vector<int>& heights, int k) {
    int N = heights.size();
    vector<int> dp(k, INT_MAX);
    dp[0] = 0;
    if (k > 1) dp[1 % k] = abs(heights[1] - heights[0]);

    for (int idx = 2; idx < N; idx++) {
        int curr = INT_MAX;
        for (int j = idx - 1; j >= max(idx - k, 0); j--) {
            if (dp[j % k] != INT_MAX)
                curr = min(curr, dp[j % k] + abs(heights[idx] - heights[j]));
        }
        dp[idx % k] = curr;
    }
    return dp[(N - 1) % k];
}
```

### The key subtlety

Slot `idx % k` holds the value for `dp[idx - k]` (written k iterations ago).  
When computing `dp[idx]`, we still need `dp[idx - k]` — it's one of the k sources.  
So we **must** compute into `curr` first, then write to `dp[idx % k]`.

```
k=3, idx=3:  slot 3%3=0 currently holds dp[0]=0  ← still need it!
             compute curr = min(..., dp[0]+...)
             THEN write dp[0] = curr              ← safe now
```

**Time: O(n·k) | Space: O(k)**

---

## Common Bugs

### 1. Wrong index in cost calculation

```cpp
// WRONG — j is the jump distance, not the source position
abs(heights[j] - heights[idx])

// CORRECT
abs(heights[idx - j] - heights[idx])
```

### 2. Missing memoization check

Without `if (dp[idx] != INT_MAX) return dp[idx];`, the dp array is written but never read back. Every call recomputes from scratch — O(k^n) instead of O(n·k).

### 3. Using INT_MAX as initial value with arithmetic

`INT_MAX + anything` overflows in C++. It's safe here because base cases always return real values before any addition happens — but it's a fragile pattern. `1e9` or `INT_MAX/2` as the sentinel is safer in general.

---

## Memoization vs Tabulation vs Space-Optimized

| | Memoization | Tabulation | Space-Optimized |
|---|---|---|---|
| Direction | n-1 → base cases | base cases → n-1 | base cases → n-1 |
| Mechanism | Recursion + cache | Iterative + array | Iterative + circular buffer |
| Space | O(n) + O(n) stack | O(n) | **O(k)** |
| Stack overflow risk | Yes (large n) | No | No |

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n·k) | O(n) |
| Tabulation | O(n·k) | O(n) |
| Space-Optimized | O(n·k) | O(k) |
