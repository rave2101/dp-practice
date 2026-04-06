# House Robber

## Problem

A robber wants to rob houses along a street. Each house has some money `money[i]`.  
Adjacent houses have an alarm — robbing two adjacent houses triggers it.

Return the **maximum money** the robber can steal without triggering the alarm.

---

## Example

```
Index:  0   1   2   3   4
Money: [2,  7,  9,  3,  1]
```

```
Rob 0, 2, 4: 2 + 9 + 1 = 12
Rob 1, 3:    7 + 3     = 10
Rob 0, 3:    2 + 3     = 5
Rob 1, 4:    7 + 1     = 8
```

**Output: 12**

---

## The Core Insight

At every house `i`, you have two choices: rob it or skip it.

- **Rob:** take `money[i]` + best from `i-2` (can't touch `i-1`)
- **Skip:** take best from `i-1`

```
dp[i] = max(money[i] + dp[i-2], dp[i-1])
```

### Building the dp table for the example

```
dp[0] = 2
dp[1] = max(7, 2)                   = 7
dp[2] = max(9 + 2, 7)               = 11
dp[3] = max(3 + 7, 11)              = 11
dp[4] = max(1 + 11, 11)             = 12
```

---

## Approach 1 — Memoization (Top-Down)

Ask: *"what is the max I can rob from houses 0..idx?"*  
Recurse from the last house, cache results.

```cpp
int solve(vector<int>& money, vector<int>& dp, int idx) {
    if (idx < 0) return 0;
    if (dp[idx] != -1) return dp[idx];
    dp[idx] = max(money[idx] + solve(money, dp, idx - 2),
                               solve(money, dp, idx - 1));
    return dp[idx];
}
```

**Time: O(n) | Space: O(n) cache + O(n) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

Fill the dp table forward from the base case.

```cpp
dp[0] = money[0];
for (int i = 1; i < n; i++)
    dp[i] = max(money[i] + (i >= 2 ? dp[i - 2] : 0), dp[i - 1]);
return dp[n - 1];
```

**Time: O(n) | Space: O(n)**

---

## Approach 3 — Space-Optimized

`dp[i]` only depends on the previous two values — no array needed.

```cpp
int prev2 = 0, prev1 = 0;
for (int x : money) {
    int curr = max(x + prev2, prev1);
    prev2 = prev1;
    prev1 = curr;
}
return prev1;
```

**Time: O(n) | Space: O(1)**

---

---

# House Robber II (Circular)

## Problem

Same as above, but the houses are arranged in a **circle** — the first and last house are adjacent.

**Output: same idea, but you can never rob both house `0` and house `n-1`.**

---

## Example

```
Index:  0   1   2   3
Money: [2,  3,  2,  1]
```

House 0 and house 3 are adjacent.

```
Rob 0, 2: 2 + 2 = 4   (can't rob 3 since 0 is taken)
Rob 1, 3: 3 + 1 = 4
Rob 0:    2
Rob 1:    3
```

**Output: 4**

---

## The Core Insight

Since houses 0 and n-1 are adjacent, you can never rob both.  
Split into two independent linear subproblems and take the max:

1. Rob houses `[0, n-2]` — exclude the last house
2. Rob houses `[1, n-1]` — exclude the first house

```
answer = max(linearRob(0, n-2), linearRob(1, n-1))
```

---

## Approach 1 — Memoization (Top-Down)

Two separate memo arrays — the two ranges must not share cache since they have different `start` boundaries.

```cpp
int rob(vector<int>& money, int start, vector<int>& dp, int idx) {
    if (idx < start) return 0;
    if (dp[idx] != -1) return dp[idx];
    dp[idx] = max(money[idx] + rob(money, start, dp, idx - 2),
                               rob(money, start, dp, idx - 1));
    return dp[idx];
}

int houseRobberCircular(vector<int>& money) {
    int n = money.size();
    if (n == 1) return money[0];
    vector<int> dp1(n, -1), dp2(n, -1);
    return max(rob(money, 0, dp1, n - 2),
               rob(money, 1, dp2, n - 1));
}
```

**Time: O(n) | Space: O(n)**

---

## Approach 2 — Space-Optimized

Reuse the linear space-optimized helper with a start/end range.

```cpp
int rob(vector<int>& money, int start, int end) {
    int prev2 = 0, prev1 = 0;
    for (int i = start; i <= end; i++) {
        int curr = max(money[i] + prev2, prev1);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

int houseRobberCircular(vector<int>& money) {
    int n = money.size();
    if (n == 1) return money[0];
    return max(rob(money, 0, n - 2), rob(money, 1, n - 1));
}
```

**Time: O(n) | Space: O(1)**

---

## Common Bugs

### 1. Returning `current` instead of `last`

```cpp
// WRONG — current is never set if the loop doesn't run
int second_last = money[start], last = max(money[start], money[start+1]);
for (int idx = start+2; idx <= end; idx++) { ... }
return current;   // 0 for 2-element ranges!

// CORRECT
return last;
```

When the range has exactly 2 elements, the loop body never executes. `last` holds the correct base-case answer; `current` is still 0.

### 2. Sharing memo arrays across both subproblems

```cpp
// WRONG — dp1 used for both calls
return max(rob(money, 0, dp1, n-2), rob(money, 1, dp1, n-1));

// CORRECT — separate arrays
vector<int> dp1(n, -1), dp2(n, -1);
return max(rob(money, 0, dp1, n-2), rob(money, 1, dp2, n-1));
```

The two ranges have different `start` values. `dp[1]` means different things in each subproblem — sharing the cache produces wrong answers.

### 3. Missing SIZE == 1 edge case (circular version)

```cpp
// rob(money, 0, dp1, -1) and rob(money, 1, dp2, 0) both return 0
// max(0, 0) = 0 — wrong for money = [5]

if (n == 1) return money[0];  // must guard before calling rob
```

---

## Complexity Summary

### Linear House Robber

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n) | O(n) |
| Tabulation | O(n) | O(n) |
| Space-Optimized | O(n) | O(1) |

### Circular House Robber

| Approach | Time | Space |
|---|---|---|
| Memoization | O(n) | O(n) |
| Space-Optimized | O(n) | O(1) |
