# Partition Array for Maximum Sum

## Problem

Given an array `arr` of `n` integers and an integer `k`, partition the array into subarrays such that:
- Every element belongs to exactly one subarray
- Each subarray has length **at most `k`**
- All elements in a subarray become the **maximum** of that subarray

Find the **maximum possible sum** after partitioning.

---

## Examples

```
k = 3, arr = [1, 20, 13, 4, 4, 1]

Partition: [1,20] | [13,4,4] | [1]
           20*2   +  13*3    + 1*1
         = 40     +  39      + 1    = 80

Output: 80
```

```
k = 2, arr = [1, 4, 1, 5, 7, 3]

Partition: [1,4] | [1,5] | [7,3]
           4*2   +  5*2  + 7*2
         = 8     + 10    + 14   = 32

Output: 32
```

---

## Why This Is MCM-Pattern

In MCM-style problems you pick a **split point** inside a range and combine the cost of the two halves. Here, instead of a 2D range, you work on a 1D suffix: at index `i` you try every possible subarray endpoint `j` in `[i, i+k-1]`. The contribution of the subarray `[i..j]` is `max(arr[i..j]) * (j - i + 1)`, then the problem recurses on the remaining suffix `[j+1..n-1]`. Subproblems overlap because many starting indices share the same suffix result.

---

## State and Recurrence

Define `dp[i]` = maximum sum achievable for `arr[i..n-1]`.

```
dp[i] = max over j in [i, min(i+k, n) - 1] of:
            max(arr[i..j]) * (j - i + 1)  +  dp[j+1]

Base case: dp[n] = 0   (empty suffix, nothing to sum)
```

The inner loop also tracks `len = j - i + 1` incrementally instead of recomputing, and updates the running `maxi` as `j` grows — both O(1) per step.

---

## Approach 1 — Memoization (Top-Down)

```cpp
int memoization(vector<int>& nums, int k, int i, vector<int>& dp, int SIZE) {
    if (i == SIZE) return 0;
    if (dp[i] != -1) return dp[i];
    int len = 0, maxi = INT_MIN, result = INT_MIN;
    for (int j = i; j < min(i + k, SIZE); j++) {
        maxi = max(maxi, nums[j]);
        len++;
        int current = (maxi * len) + memoization(nums, k, j + 1, dp, SIZE);
        result = max(result, current);
    }
    return dp[i] = result;
}
```

**Time: O(N·K) | Space: O(N) dp + O(N) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int tabulation(vector<int>& nums, int k) {
    int SIZE = nums.size();
    vector<int> dp(SIZE + 1, 0);
    for (int i = SIZE - 1; i >= 0; i--) {
        int len = 0, maxi = INT_MIN, result = INT_MIN;
        for (int j = i; j < min(i + k, SIZE); j++) {
            maxi = max(maxi, nums[j]);
            len++;
            int current = (maxi * len) + dp[j + 1];
            result = max(result, current);
        }
        dp[i] = result;
    }
    return dp[0];
}
```

**Why `dp` has size `N + 1`:** When `j = SIZE - 1`, the recurrence reads `dp[j + 1] = dp[SIZE]`. That cell is the base case (empty suffix = 0). Allocating `SIZE + 1` entries and initializing to `0` handles this without a bounds check.

**Iteration order:** `i` goes right-to-left so `dp[j+1]` (a larger index) is always filled before `dp[i]` needs it.

**Time: O(N·K) | Space: O(N)**

---

## Recursion Tree

Using `k = 3`, `arr = [1, 20, 13]` (SIZE = 3).

```
dp[0]
├── j=0: subarray [1],     max=1,  len=1  → 1*1  + dp[1]
│                                           dp[1]
│                                           ├── j=1: [20], max=20, len=1 → 20*1 + dp[2]
│                                           │                               dp[2]
│                                           │                               └── j=2: [13], max=13, len=1 → 13*1 + dp[3] = 13
│                                           │                               dp[2] = 13
│                                           │         20*1 + 13 = 33
│                                           ├── j=2: [20,13], max=20, len=2 → 20*2 + dp[3] = 40
│                                           dp[1] = max(33, 40) = 40
│         1 + 40 = 41
│
├── j=1: subarray [1,20],  max=20, len=2  → 20*2 + dp[2] = 40 + 13 = 53
│
└── j=2: subarray [1,20,13], max=20, len=3 → 20*3 + dp[3] = 60

dp[0] = max(41, 53, 60) = 60
```

---

## Table Fill

```
k=3, arr=[1,20,13,4,4,1], SIZE=6

dp = [_, _, _, _, _, _, 0]   (index 6 is base case)

i=5: j=5: max=1, len=1 → 1*1 + dp[6] = 1         → dp[5] = 1

i=4: j=4: max=4, len=1 → 4*1 + dp[5] = 5
     j=5: max=4, len=2 → 4*2 + dp[6] = 8          → dp[4] = 8

i=3: j=3: max=4, len=1 → 4*1 + dp[4] = 12
     j=4: max=4, len=2 → 4*2 + dp[5] = 9
     j=5: max=4, len=3 → 4*3 + dp[6] = 12         → dp[3] = 12

i=2: j=2: max=13,len=1 → 13*1 + dp[3] = 25
     j=3: max=13,len=2 → 13*2 + dp[4] = 34
     j=4: max=13,len=3 → 13*3 + dp[5] = 40        → dp[2] = 40

i=1: j=1: max=20,len=1 → 20*1 + dp[2] = 60
     j=2: max=20,len=2 → 20*2 + dp[3] = 52
     j=3: max=20,len=3 → 20*3 + dp[4] = 68        → dp[1] = 68   ← [20,13,4] → 20*3=60+dp[4]=68? Wait:
                                                                      20*3 + dp[4] = 60 + 8 = 68 ✓

i=0: j=0: max=1, len=1 → 1*1  + dp[1] = 69
     j=1: max=20,len=2 → 20*2 + dp[2] = 80
     j=2: max=20,len=3 → 20*3 + dp[3] = 72        → dp[0] = 80

return dp[0] = 80  ✓
```

---

## Complexity Summary

| Approach    | Time   | Space              |
|---|---|---|
| Memoization | O(N·K) | O(N) + O(N) stack  |
| Tabulation  | O(N·K) | O(N)               |
