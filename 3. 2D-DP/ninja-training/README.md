# Ninja's Training

## Problem

A ninja has `n` days of training. Each day he can do one of 3 activities, each with a merit point `matrix[day][activity]`.  
He **cannot do the same activity on two consecutive days**.

Return the **maximum merit points** he can earn.

---

## Example

```
Day:       0    1    2
Activity 0: 10   40   70
Activity 1: 20   50   80
Activity 2: 30   60   90
```

```
Day 0: pick activity 2 → 30
Day 1: can't pick 2, pick activity 1 → 50
Day 2: can't pick 1, pick activity 0 → 70   total = 150

Day 0: pick activity 2 → 30
Day 1: can't pick 2, pick activity 1 → 50
Day 2: can't pick 1, pick activity 2 → 90   total = 170  ← best
```

**Output: 170**

---

## The Core Insight

State: `dp[day][last]` = max merit from day `0` to `day`, given that activity `last` was done on day `day`.

At each day, try all activities except the one done the previous day:

```
dp[day][last] = max over i != last:
    matrix[day][i] + dp[day-1][i]
```

The extra column `last = 3` is used as a sentinel for the first call — "no activity was done before day 0", so all 3 are available.

### Building the dp table for the example

```
         last=0  last=1  last=2  last=3
day=0:     50      40      30      60     ← max of the other two on day 0
day=1:    110     100      90     110     ← matrix[1][j] + best dp[0][k], k≠j
day=2:    170     160     160     170
```

Start with `last=3` → answer = `dp[2][3]` = **170**

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

### 2. Base case not cached

```cpp
if (day == 0) {
    ...
    return maxi;              // never writes to dp[0][last]
}
// Fix:
    return dp[0][last] = maxi;
```

Without caching, every call that reaches day 0 recomputes instead of using the stored value.

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
