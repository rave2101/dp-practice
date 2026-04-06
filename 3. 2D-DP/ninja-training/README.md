# Ninja's Training

## Problem

A ninja has `n` days of training. Each day he can do one of 3 activities:
- `0` — Running
- `1` — Stealth Training
- `2` — Fighting Practice

`matrix[i][j]` = merit points for activity `j` on day `i`.  
He **cannot do the same activity on two consecutive days**.

Return the **maximum merit points** he can earn.

---

## Example

```
             Running  Stealth  Fighting
Day 0:          10      40       70
Day 1:          20      50       80
Day 2:          30      60       90
```

```
Day 0: Fighting  = 70
Day 1: Stealth   = 50  (can't repeat Fighting)
Day 2: Fighting  = 90  (can't repeat Stealth)
Total = 70 + 50 + 90 = 210  ← best
```

**Output: 210**

---

## The Core Insight

State: `dp[day][last]` = max merit from day `0` to `day`, given that activity `last` was done on day `day`.

At each day, try all activities except the one done the previous day:

```
dp[day][last] = max over i != last:
    matrix[day][i] + dp[day-1][i]
```

`last = 3` is a sentinel for the initial call — no activity was done before day 0, so all 3 are available.

### dp table for the example

```
             last=0  last=1  last=2  last=3
day=0:          70      70      40      —     ← max of the other two activities on day 0
day=1:         120     120     120      —     ← matrix[1][j] + best dp[0][k], k≠j
day=2:          —       —       —      210    ← answer
```

Start with `last=3` → answer = `dp[2][3]` = **210**

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(vector<vector<int>>& matrix, int day, int last, vector<vector<int>>& dp) {
    if (dp[day][last] != -1) return dp[day][last];
    if (day == 0) {
        int maxi = 0;
        for (int i = 0; i <= 2; i++)
            if (i != last) maxi = max(maxi, matrix[0][i]);
        return dp[0][last] = maxi;
    }
    int maxi = 0;
    for (int i = 0; i <= 2; i++) {
        if (i != last) {
            int activity = matrix[day][i] + util(matrix, day - 1, i, dp);
            maxi = max(maxi, activity);
        }
    }
    return dp[day][last] = maxi;
}

int ninjaTraining(vector<vector<int>>& matrix) {
    int day = matrix.size();
    vector<vector<int>> dp(day, vector<int>(4, -1));
    return util(matrix, day - 1, 3, dp);
}
```

**Time: O(n × 4 × 3) = O(n) | Space: O(n × 4) + O(n) call stack**

---

## Recursion Stack Walkthrough

```
             Running  Stealth  Fighting
Day 0:          10      40       70
Day 1:          20      50       80
Day 2:          30      60       90
```

Entry call: `util(day=2, last=3)`

```
util(2, 3)                               dp[2][3] = -1, not cached
├── try i=0 (Running): 30 + util(1, 0)
│   │                                    dp[1][0] = -1, not cached
│   ├── try i=1 (Stealth): 50 + util(0, 1)
│   │   │                                day==0 → pick all i != 1
│   │   │                                i=0: maxi = max(0, 10) = 10
│   │   │                                i=2: maxi = max(10, 70) = 70
│   │   └── return dp[0][1] = 70
│   │   activity = 50 + 70 = 120,  maxi = 120
│   │
│   ├── try i=2 (Fighting): 80 + util(0, 2)
│   │   │                                day==0 → pick all i != 2
│   │   │                                i=0: maxi = max(0, 10) = 10
│   │   │                                i=1: maxi = max(10, 40) = 40
│   │   └── return dp[0][2] = 40
│   │   activity = 80 + 40 = 120,  maxi = max(120, 120) = 120
│   │
│   └── return dp[1][0] = 120
│   activity = 30 + 120 = 150,  maxi = 150
│
├── try i=1 (Stealth): 60 + util(1, 1)
│   │                                    dp[1][1] = -1, not cached
│   ├── try i=0 (Running): 20 + util(0, 0)
│   │   │                                day==0 → pick all i != 0
│   │   │                                i=1: maxi = max(0, 40) = 40
│   │   │                                i=2: maxi = max(40, 70) = 70
│   │   └── return dp[0][0] = 70
│   │   activity = 20 + 70 = 90,  maxi = 90
│   │
│   ├── try i=2 (Fighting): 80 + util(0, 2)
│   │   │                                day==0 → base case hit again (recomputed, see bug below)
│   │   │                                i=0: maxi = 10,  i=1: maxi = 40
│   │   └── return 40
│   │   activity = 80 + 40 = 120,  maxi = max(90, 120) = 120
│   │
│   └── return dp[1][1] = 120
│   activity = 60 + 120 = 180,  maxi = max(150, 180) = 180
│
├── try i=2 (Fighting): 90 + util(1, 2)
│   │                                    dp[1][2] = -1, not cached
│   ├── try i=0 (Running): 20 + util(0, 0)
│   │   │                                day==0 → base case hit again (recomputed)
│   │   │                                i=1: maxi = 40,  i=2: maxi = 70
│   │   └── return 70
│   │   activity = 20 + 70 = 90,  maxi = 90
│   │
│   ├── try i=1 (Stealth): 50 + util(0, 1)
│   │   │                                day==0 → base case hit again (recomputed)
│   │   │                                i=0: maxi = 10,  i=2: maxi = 70
│   │   └── return 70
│   │   activity = 50 + 70 = 120,  maxi = max(90, 120) = 120
│   │
│   └── return dp[1][2] = 120
│   activity = 90 + 120 = 210,  maxi = max(180, 210) = 210
│
└── return dp[2][3] = 210
```

### dp table after all calls complete

```
             last=0  last=1  last=2  last=3
day=0:          70      70      40      -1    ← set but never read back (see bug below)
day=1:         120     120     120      -1
day=2:          -1      -1      -1     210    ← answer
```

### What got cached vs recomputed

| Call | Result | Cached? | Recomputed? |
|------|--------|---------|-------------|
| util(1, 0) | 120 | dp[1][0] = 120 | never called again |
| util(1, 1) | 120 | dp[1][1] = 120 | never called again |
| util(1, 2) | 120 | dp[1][2] = 120 | never called again |
| util(0, 0) | 70  | dp[0][0] = 70  | **recomputed** (cache not used) |
| util(0, 1) | 70  | dp[0][1] = 70  | **recomputed** (cache not used) |
| util(0, 2) | 40  | dp[0][2] = 40  | **recomputed** (cache not used) |

Day 1 calls are cached correctly. Day 0 calls are always recomputed — because the base case check comes **before** the cache check, so `dp[day][last] != -1` is never reached when `day == 0`.

**Fix — move the cache check to the top:**

```cpp
int util(...) {
    if (dp[day][last] != -1) return dp[day][last];  // ← must be first
    if (day == 0) {
        ...
        return dp[0][last] = maxi;
    }
    ...
}
```

---

## Approach 2 — Tabulation with Space Optimization

Since `dp[day]` only depends on `dp[day-1]`, we only keep the previous row.

```cpp
int ninjaTraining(vector<vector<int>>& matrix) {
    int R = matrix.size(), C = matrix[0].size();
    vector<int> prev = matrix[0];
    for (int i = 1; i < R; i++) {
        vector<int> current(C, 0);
        for (int j = 0; j < C; j++)
            for (int k = 0; k < C; k++)
                if (k != j) current[j] = max(current[j], matrix[i][j] + prev[k]);
        prev = current;
    }
    return *max_element(prev.begin(), prev.end());
}
```

**Time: O(n × 3 × 3) = O(n) | Space: O(C) — no full dp table needed**

---

## Common Bugs

### 1. dp sized `day` instead of `day+1`

```cpp
int day = matrix.size() - 1;
vector<vector<int>> dp(day, vector<int>(4, -1));  // indices 0..day-1
return util(matrix, day, 3, dp);                  // accesses dp[day] → out of bounds!

// Fix: size dp by the full number of days
int day = matrix.size();
vector<vector<int>> dp(day, vector<int>(4, -1));
return util(matrix, day - 1, 3, dp);
```

### 2. Cache check placed after base case

```cpp
if (day == 0) { ... return dp[0][last] = maxi; }  // base case first
if (dp[day][last] != -1) return dp[day][last];     // cache never reached for day==0

// Fix: cache check must come first
if (dp[day][last] != -1) return dp[day][last];
if (day == 0) { ... return dp[0][last] = maxi; }
```

---

## Why 2D DP?

The state needs **two** variables to be fully defined:
- Which **day** we're on
- Which activity was done **last** (to know what's forbidden today)

A 1D array `dp[day]` is not enough — the answer at day `i` differs depending on what was done on day `i-1`.

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n) | O(n) |
| Tabulation (space-optimized) | O(n) | O(C) |
