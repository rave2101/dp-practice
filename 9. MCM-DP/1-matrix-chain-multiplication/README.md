# Matrix Chain Multiplication

## Problem

Given an array `nums` of size `N`, where `nums[i-1] × nums[i]` is the dimension of the `i`-th matrix (1-indexed), find the **minimum number of scalar multiplications** needed to multiply the entire chain of `N-1` matrices.

---

## Examples

```
nums = [10, 20, 30, 40, 30]
Matrices: M1(10×20), M2(20×30), M3(30×40), M4(40×30)

One ordering: ((M1·M2)·M3)·M4
  M1·M2 → 10×20×30 = 6000
  result(10×30)·M3 → 10×30×40 = 12000
  result(10×40)·M4 → 10×40×30 = 12000
  Total = 30000

Optimal ordering: M1·(M2·(M3·M4))
  M3·M4 → 30×40×30 = 36000
  M2·result(30×30) → 20×30×30 = 18000
  M1·result(20×30) → 10×20×30 = 6000
  Total = 60000  ← worse

Best split gives Output: 26000
```

```
nums = [1, 2, 3, 4]
Matrices: M1(1×2), M2(2×3), M3(3×4)

Output: 18
```

---

## The Core Insight

The problem is: **where do we place the last parenthesisation?** If we have matrices `M_i … M_j`, and we decide the last split is at position `k`, then:

- Left subchain `M_i … M_k` is fully multiplied first — cost `util(i, k)`
- Right subchain `M_{k+1} … M_j` is fully multiplied first — cost `util(k+1, j)`
- Merging the two results costs `nums[i-1] × nums[k] × nums[j]`

  - Left result has dimensions `nums[i-1] × nums[k]`
  - Right result has dimensions `nums[k] × nums[j]`
  - Multiplying them: `nums[i-1] × nums[k] × nums[j]` operations

We try every valid split point `k` from `i` to `j-1` and take the minimum.

**State:** `dp[i][j]` = minimum multiplications to compute the product of matrices `i` through `j`.

```
dp[i][j] = min over k in [i, j-1] of:
    nums[i-1] * nums[k] * nums[j]  +  dp[i][k]  +  dp[k+1][j]
```

**Answer:** `dp[1][N-1]` where `N = nums.size()`.

---

## Base Case

A single matrix needs no multiplication:

```
dp[i][i] = 0  for all i
```

---

## Why `nums[i-1] * nums[k] * nums[j]`?

The dimensions encoding is: matrix `i` has shape `nums[i-1] × nums[i]`.

When you split at `k`:
- Left result shape: `nums[i-1]` rows × `nums[k]` cols
- Right result shape: `nums[k]` rows × `nums[j]` cols
- Multiplying an `(a × b)` matrix by a `(b × c)` matrix costs `a * b * c` operations

So the merge cost is `nums[i-1] * nums[k] * nums[j]`.

---

## Recursion Tree

For `nums = [1, 2, 3, 4]` — matrices M1(1×2), M2(2×3), M3(3×4):

```
util(1, 3)
├── k=1: nums[0]*nums[1]*nums[3] + util(1,1) + util(2,3)
│         = 1*2*4 + 0 + util(2,3)
│                        └── k=2: nums[1]*nums[2]*nums[3] + util(2,2) + util(3,3)
│                                 = 2*3*4 + 0 + 0 = 24
│         → 8 + 0 + 24 = 32
│
└── k=2: nums[0]*nums[2]*nums[3] + util(1,2) + util(3,3)
          = 1*3*4 + util(1,2) + 0
                    └── k=1: nums[0]*nums[1]*nums[2] + util(1,1) + util(2,2)
                              = 1*2*3 + 0 + 0 = 6
          → 12 + 6 + 0 = 18

min(32, 18) = 18  ✓
```

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(int i, int j, vector<int>& nums, vector<vector<int>>& dp) {
    if (i == j) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    int mini = INT_MAX;
    for (int k = i; k <= j - 1; k++) {
        int steps = (nums[i-1] * nums[k] * nums[j]) + util(i, k, nums, dp) + util(k+1, j, nums, dp);
        mini = min(mini, steps);
    }
    return dp[i][j] = mini;
}

int matrixMultiplication(vector<int>& nums) {
    int SIZE = nums.size();
    vector<vector<int>> dp(SIZE, vector<int>(SIZE, -1));
    return util(1, SIZE - 1, nums, dp);
}
```

**Time: O(N³) | Space: O(N²) + O(N) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

The key challenge: `dp[i][j]` depends on `dp[i][k]` and `dp[k+1][j]` where `k < j` and `k+1 > i`. This means we need all subproblems of **smaller chain length** to be ready before solving larger ones.

Iterating `i` from right to left (high to low) and `j` from `i+1` forward ensures `dp[k+1][j]` (starting index `k+1 > i`) is already filled when we need it.

```cpp
int matrixMultiplication(vector<int>& nums) {
    int SIZE = nums.size();
    vector<vector<int>> dp(SIZE, vector<int>(SIZE, 0));
    for (int i = SIZE - 1; i >= 1; i--) {
        for (int j = i + 1; j < SIZE; j++) {
            int mini = INT_MAX;
            for (int k = i; k <= j - 1; k++) {
                int steps = (nums[i-1] * nums[k] * nums[j]) + dp[i][k] + dp[k+1][j];
                mini = min(mini, steps);
            }
            dp[i][j] = mini;
        }
    }
    return dp[1][SIZE - 1];
}
```

**Time: O(N³) | Space: O(N²)**

---

## Table Fill

```
nums = [1, 2, 3, 4]   (SIZE=4, matrices indexed 1..3)

dp is SIZE×SIZE; valid entries are dp[i][j] with 1 ≤ i < j ≤ 3.

Base: dp[i][i] = 0 (initialised to 0)

i=3: no j > 3 in range → skip

i=2:
  j=3: k=2: nums[1]*nums[2]*nums[3] + dp[2][2] + dp[3][3]
             = 2*3*4 + 0 + 0 = 24
        dp[2][3] = 24

i=1:
  j=2: k=1: nums[0]*nums[1]*nums[2] + dp[1][1] + dp[2][2]
             = 1*2*3 + 0 + 0 = 6
        dp[1][2] = 6

  j=3: k=1: nums[0]*nums[1]*nums[3] + dp[1][1] + dp[2][3]
             = 1*2*4 + 0 + 24 = 32
        k=2: nums[0]*nums[2]*nums[3] + dp[1][2] + dp[3][3]
             = 1*3*4 + 6 + 0 = 18
        dp[1][3] = min(32, 18) = 18

return dp[1][3] = 18  ✓
```

---

## Complexity Summary

| Approach     | Time   | Space             |
|---|---|---|
| Memoization  | O(N³)  | O(N²) + O(N) stack |
| Tabulation   | O(N³)  | O(N²)             |
