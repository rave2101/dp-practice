# Burst Balloons

## Problem

Given `n` balloons with values in `nums`, you can burst any balloon. Bursting balloon `k` (while its neighbours are `left` and `right`) gives you `nums[left] * nums[k] * nums[right]` coins. Find the **maximum coins** you can collect by bursting all balloons.

---

## Examples

```
nums = [3, 1, 5, 8]

One order: burst 1, burst 5, burst 3, burst 8
  burst 1: 3*1*5 = 15   → [3,5,8]
  burst 5: 3*5*8 = 120  → [3,8]
  burst 3: 1*3*8 = 24   → [8]
  burst 8: 1*8*1 = 8    → []
  Total = 167

Output: 167
```

```
nums = [1, 5]

Output: 10
```

---

## Why Forward Thinking Fails

If you ask "which balloon do I burst **first**?" — the neighbours of every remaining balloon change after each burst. Bursting balloon `k` collapses its left and right neighbours together, so the subproblems on the left and right are no longer **independent** — they share a shifting boundary.

```
nums = [3, 1, 5, 8]

Burst 1 (index 1) first: coins = 3*1*5 = 15
Remaining: [3, 5, 8]

The left subarray [3] and right subarray [5, 8] now look independent.
But before this burst, we couldn't split cleanly —
the left and right both depended on balloon 1 being present as a neighbour.
```

Forward splitting creates **dependent** subproblems. The recurrence breaks down.

---

## The Reverse Insight — Last Balloon to Burst

Instead of asking "which balloon do I burst **first**?", ask "which balloon do I burst **last**?" in range `[i, j]`.

If balloon `k` is the **last** one burst in `[i, j]`:
- At the moment `k` is burst, all other balloons in `[i, j]` are already gone
- Its only neighbours are the **boundaries**: `nums[i-1]` on the left and `nums[j+1]` on the right
- Coins from bursting `k` last = `nums[i-1] * nums[k] * nums[j+1]` (fixed, no ambiguity)

Crucially, before `k` is burst:
- Everything in `[i, k-1]` is burst independently, bounded by `nums[i-1]` and `nums[k]`
- Everything in `[k+1, j]` is burst independently, bounded by `nums[k]` and `nums[j+1]`

These two subproblems **do not interfere** — `k` acts as a permanent wall between them until it is the very last one removed. This is what makes the subproblems independent.

```
Range [i, j],  k = last balloon burst:

  [ i-1 | i ... k-1 | k | k+1 ... j | j+1 ]
     ↑                ↑                ↑
  left boundary    the wall        right boundary
  (never burst)   (last burst)    (never burst)

left subproblem:  dp[i][k-1]  bounded by nums[i-1] and nums[k]
right subproblem: dp[k+1][j]  bounded by nums[k] and nums[j+1]
last burst cost:  nums[i-1] * nums[k] * nums[j+1]
```

---

## Index Encoding

Add `1` at both ends as virtual boundary balloons (they are never burst):

```
nums = [3, 1, 5, 8]  →  [1, 3, 1, 5, 8, 1]
index:   0  1  2  3  4  5
```

Valid balloon indices: `1` to `SIZE-2`.

**`dp[i][j]`** = max coins from bursting all balloons at indices `i` through `j`.

---

## Recurrence

```
dp[i][j] = max over k in [i, j] of:
    nums[i-1] * nums[k] * nums[j+1]  +  dp[i][k-1]  +  dp[k+1][j]
    ↑ coins when k is burst last          ↑ left half    ↑ right half
```

**Base case:** `i > j` → `0` (empty range, no balloons to burst)

**Answer:** `dp[1][SIZE-2]`

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(int i, int j, vector<int>& nums, vector<vector<int>>& dp) {
    if (i > j) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    int maxi = INT_MIN;
    for (int k = i; k <= j; k++) {
        int profit = (nums[i-1] * nums[k] * nums[j+1]) + util(i, k-1, nums, dp) + util(k+1, j, nums, dp);
        maxi = max(maxi, profit);
    }
    return dp[i][j] = maxi;
}

int maxCoins(vector<int>& nums) {
    nums.insert(nums.begin(), 1);
    nums.push_back(1);
    int SIZE = nums.size();
    vector<vector<int>> dp(SIZE, vector<int>(SIZE, -1));
    return util(1, SIZE - 2, nums, dp);
}
```

**Time: O(N³) | Space: O(N²) + O(N) call stack**

---

## Recursion Tree

Using `nums = [3, 1, 5]` → with boundaries: `[1, 3, 1, 5, 1]`, valid range `[1..3]`.

```
util(1, 3)
├── k=1: nums[0]*nums[1]*nums[4] + util(1,0) + util(2,3)
│        = 1*3*1 + 0 + util(2,3)
│                      │
│                      ├── k=2: nums[1]*nums[2]*nums[4] + util(2,1) + util(3,3)
│                      │        = 3*1*1 + 0 + util(3,3)
│                      │                       └── k=3: 1*5*1 + 0 + 0 = 5  → util(3,3) = 5
│                      │        = 3 + 0 + 5 = 8
│                      │
│                      └── k=3: nums[1]*nums[3]*nums[4] + util(2,2) + util(4,3)
│                               = 3*5*1 + util(2,2) + 0
│                                          └── k=2: 3*1*5 + 0 + 0 = 15  → util(2,2) = 15
│                               = 15 + 15 + 0 = 30
│                      → util(2,3) = max(8, 30) = 30
│        = 3 + 0 + 30 = 33
│
├── k=2: nums[0]*nums[2]*nums[4] + util(1,1) + util(3,3)
│        = 1*1*1 + util(1,1) + 5 [cached]
│                  └── k=1: 1*3*1 + 0 + 0 = 3  → util(1,1) = 3
│        = 1 + 3 + 5 = 9
│
└── k=3: nums[0]*nums[3]*nums[4] + util(1,2) + util(4,3)
         = 1*5*1 + util(1,2) + 0
                   │
                   ├── k=1: nums[0]*nums[1]*nums[3] + util(1,0) + util(2,2)
                   │        = 1*3*5 + 0 + 15 [cached] = 30
                   │
                   └── k=2: nums[0]*nums[2]*nums[3] + util(1,1) + util(3,2)
                            = 1*1*5 + 3 [cached] + 0 = 8
                   → util(1,2) = max(30, 8) = 30
         = 5 + 30 + 0 = 35

→ util(1,3) = max(33, 9, 35) = 35  ✓
```

**Overlapping subproblems — why memoization helps:**

`util(3,3)`, `util(2,2)`, and `util(1,1)` each appear in **multiple branches**. Without memoization they recompute from scratch every time. With memoization the result is stored after the first call and subsequent calls return in O(1).

```
util(3,3) = 5   computed once under k=2 of util(2,3), reused under k=2 of util(1,3)
util(2,2) = 15  computed once under k=3 of util(2,3), reused under k=1 of util(1,2)
util(1,1) = 3   computed once under k=2 of util(1,3), reused under k=2 of util(1,2)
```

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int maxCoins(vector<int>& nums) {
    nums.insert(nums.begin(), 1);
    nums.push_back(1);
    int SIZE = nums.size();
    vector<vector<int>> dp(SIZE, vector<int>(SIZE, 0));  // 0 for empty ranges
    for (int i = SIZE - 2; i >= 1; i--) {
        for (int j = i; j <= SIZE - 2; j++) {
            int maxi = INT_MIN;
            for (int k = i; k <= j; k++) {
                int profit = (nums[i-1] * nums[k] * nums[j+1]) + dp[i][k-1] + dp[k+1][j];
                maxi = max(maxi, profit);
            }
            dp[i][j] = maxi;
        }
    }
    return dp[1][SIZE - 2];
}
```

**Why initialize `dp` to `0` (not `-1`):**

When `k = i`, `dp[i][k-1] = dp[i][i-1]` — an empty range. When `k = j`, `dp[k+1][j] = dp[j+1][j]` — also empty. These cells are never explicitly filled; they must return `0`. Initializing to `-1` poisons every profit calculation.

**Iteration order:** `i` decreases so `dp[k+1][j]` (larger starting index) is ready; `j` increases so `dp[i][k-1]` (smaller ending index) is ready.

**Time: O(N³) | Space: O(N²)**

---

## Table Fill

```
nums = [3,1,5,8]  →  [1,3,1,5,8,1],  SIZE=6,  valid range [1..4]

i=4: j=4: k=4: nums[3]*nums[4]*nums[5] + 0 + 0 = 5*8*1 = 40   → dp[4][4] = 40

i=3: j=3: k=3: nums[2]*nums[3]*nums[4] + 0 + 0 = 1*5*8 = 40   → dp[3][3] = 40
     j=4: k=3: nums[2]*nums[3]*nums[5] + dp[3][2] + dp[4][4] = 1*5*1 + 0 + 40 = 45
          k=4: nums[2]*nums[4]*nums[5] + dp[3][3] + dp[5][4] = 1*8*1 + 40 + 0 = 48
                                                                → dp[3][4] = 48

i=2: j=2: k=2: nums[1]*nums[2]*nums[3] + 0 + 0 = 3*1*5 = 15   → dp[2][2] = 15
     j=3: k=2: nums[1]*nums[2]*nums[4] + dp[2][1] + dp[3][3] = 3*1*8 + 0 + 40 = 64
          k=3: nums[1]*nums[3]*nums[4] + dp[2][2] + dp[4][3] = 3*5*8 + 15 + 0 = 135
                                                                → dp[2][3] = 135
     j=4: k=2: nums[1]*nums[2]*nums[5] + dp[2][1] + dp[3][4] = 3*1*1 + 0 + 48 = 51
          k=3: nums[1]*nums[3]*nums[5] + dp[2][2] + dp[4][4] = 3*5*1 + 15 + 40 = 70
          k=4: nums[1]*nums[4]*nums[5] + dp[2][3] + dp[5][4] = 3*8*1 + 135 + 0 = 159
                                                                → dp[2][4] = 159

i=1: j=1: k=1: nums[0]*nums[1]*nums[2] + 0 + 0 = 1*3*1 = 3    → dp[1][1] = 3
     j=2: k=1: nums[0]*nums[1]*nums[3] + dp[1][0] + dp[2][2] = 1*3*5 + 0 + 15 = 30
          k=2: nums[0]*nums[2]*nums[3] + dp[1][1] + dp[3][2] = 1*1*5 + 3 + 0  = 8
                                                                → dp[1][2] = 30
     j=3: k=1: nums[0]*nums[1]*nums[4] + 0 + dp[2][3] = 1*3*8 + 0 + 135 = 159
          k=2: nums[0]*nums[2]*nums[4] + dp[1][1] + dp[3][3] = 1*1*8 + 3 + 40 = 51
          k=3: nums[0]*nums[3]*nums[4] + dp[1][2] + dp[4][3] = 1*5*8 + 30 + 0 = 70
                                                                → dp[1][3] = 159
     j=4: k=1: nums[0]*nums[1]*nums[5] + 0 + dp[2][4] = 1*3*1 + 0 + 159 = 162
          k=2: nums[0]*nums[2]*nums[5] + dp[1][1] + dp[3][4] = 1*1*1 + 3 + 48 = 52
          k=3: nums[0]*nums[3]*nums[5] + dp[1][2] + dp[4][4] = 1*5*1 + 30 + 40 = 75
          k=4: nums[0]*nums[4]*nums[5] + dp[1][3] + dp[5][4] = 1*8*1 + 159 + 0 = 167
                                                                → dp[1][4] = 167

return dp[1][4] = 167  ✓
```

---

## Complexity Summary

| Approach    | Time  | Space              |
|---|---|---|
| Memoization | O(N³) | O(N²) + O(N) stack |
| Tabulation  | O(N³) | O(N²)              |
