# Longest Increasing Subsequence (LIS)

Problems based on finding the longest increasing subsequence pattern, including variants like Longest Divisible Subset, Longest String Chain, and Longest Bitonic Subsequence.

---

## Solutions

| File | Approach | Time | Space |
|---|---|---|---|
| `memoization.cpp` | Top-Down DP | O(n²) | O(n²) |
| `tabulation.cpp` | Bottom-Up DP | O(n²) | O(n) |
| `nlogn.cpp` | Binary Search (Patience Sorting) | O(n log n) | O(n) |
| `largest_divisible_subset.cpp` | LIS variant — divisibility condition | O(n²) | O(n) |

---

## Largest Divisible Subset — Problem

**Difficulty:** Medium

Given an array of positive integers, find the largest subset such that every pair `(a, b)` satisfies `a % b == 0` or `b % a == 0`.

```
Input:  nums = [3, 5, 10, 20]
Output: [5, 10, 20]

Explanation: 10 % 5 == 0  and  20 % 10 == 0  ✓
```

### Why This Is an LIS Variant

After **sorting** the array:
- For any pair where `j < i`, we have `nums[j] <= nums[i]`, so we only need to check `nums[i] % nums[j] == 0` (never the reverse).
- Divisibility is **transitive** on a sorted array: if `c % b == 0` and `b % a == 0`, then `c % a == 0` automatically. So we don't need to check every pair — just extend from the last element.

This makes it identical in structure to LIS tabulation, with one change:

| LIS | Largest Divisible Subset |
|---|---|
| `nums[i] > nums[j]` | `nums[i] % nums[j] == 0` |

### Walkthrough: `[3, 5, 10, 20]`

```
i=1 (5):  5%3 ≠ 0                  → dp[1]=1
i=2 (10): 10%3 ≠ 0, 10%5==0        → dp[2]=2, parent[2]=1
i=3 (20): 20%3 ≠ 0, 20%5==0 → dp=2
                    20%10==0 → dp=3 → dp[3]=3, parent[3]=2

dp     = [1, 1, 2, 3]
parent = [0, 1, 1, 2]
maxIdx = 3

Backtrack: 3 → 2 → 1, parent[1]==1 stop
Collected: [20, 10, 5] → reverse → [5, 10, 20]  ✓
```

---

## Tabulation — How It Works

`dp[i]` = length of LIS **ending at index i**. For every `i`, look back at all `j < i`:

```
if nums[j] < nums[i] && dp[j]+1 > dp[i]:
    dp[i]     = dp[j] + 1
    parent[i] = j
```

Every element starts as its own LIS of length 1 (`dp[i] = 1`, `parent[i] = i`).

---

## Reconstructing the Actual LIS (Backtracking)

After filling `dp[]`, track `maxIdx` — the index where the longest LIS **ends**.
Then follow `parent[]` backwards until you reach an element that is its own parent.

```
nums   = [2, 5, 3, 7]
dp     = [1, 2, 2, 3]
parent = [0, 0, 0, 1]    ← parent[3]=1 means 7 came after 5

Backtrack from maxIdx=3:
  i=3 → push 7, move to parent[3]=1
  i=1 → push 5, move to parent[1]=0
  i=0 → push 2, parent[0]==0 → stop

Collected: [7, 5, 2]  →  reverse  →  [2, 5, 7]  ✓
```

---

## Bug: `maxLen=0` vs `maxLen=1`

The DP loop starts at `i=1`, so **index 0 is never compared against `maxLen`**.

If `maxLen=0`, the first update fires at `i=1`, making index 0 permanently unreachable as `maxIdx`. For a fully decreasing input like `[10,8,6,3]`, every `dp[i]=1` and `maxIdx` wrongly settles at index 1 instead of 0.

```
Input: [10, 8, 6, 3]

maxLen=0 → maxIdx=1 → returns [8]   ✗
maxLen=1 → maxIdx=0 → returns [10]  ✓
```

**Fix:** initialize `maxLen = 1, maxIdx = 0` — treat index 0 as the default answer.

---

## O(n log n) — How It Works

Maintain a `result` array where `result[i]` holds the **smallest possible tail** of any increasing subsequence of length `i+1`.

For each element:
- If it's **larger than the last element** → append it (LIS length grows)
- Otherwise → **replace** the first element ≥ current with `lower_bound`

`result` is always sorted, which is why binary search works.

> **Note:** `result` does not store the actual LIS — only `result.size()` (the length) is meaningful.

---

## What is `lower_bound`?

`lower_bound(begin, end, val)` performs **binary search** on a sorted range and returns an iterator to the **first element that is ≥ val**.

```
result = [2, 3, 7, 101]

lower_bound for 5:
  Check mid=7 → 7 >= 5, go left
  Check mid=3 → 3 < 5,  go right
  → points to 7 (first element >= 5)

result[2] = 5  →  result = [2, 3, 5, 101]
```

### Why replace instead of insert?

Replacing keeps tails **as small as possible**, which gives future elements the best chance of extending the subsequence.

---

## `lower_bound` vs `upper_bound` — Why not `upper_bound`?

LIS requires **strictly increasing** elements (each element must be greater than the previous, equal is not allowed).

```
nums = [1, 3, 3, 5]
```

**Using `lower_bound` (correct):**
```
result = [1, 3]          after processing 1, 3

num = 3 → lower_bound finds first element >= 3 → points to index 1 (the existing 3)
          replace → result = [1, 3]             length stays 2  ✓

num = 5 → append → result = [1, 3, 5]          length = 3  ✓

Answer: 3  →  LIS is [1, 3, 5]  ✓
```

**Using `upper_bound` (wrong):**
```
result = [1, 3]          after processing 1, 3

num = 3 → upper_bound finds first element > 3 → points past the 3, to index 2 (end)
          append → result = [1, 3, 3]          length becomes 3  ✗

num = 5 → append → result = [1, 3, 3, 5]      length = 4  ✗

Answer: 4  →  counts [1, 3, 3, 5] as valid LIS, but 3 == 3 is not strictly increasing  ✗
```

**The key difference when the value already exists in `result`:**

| | `lower_bound` (≥ val) | `upper_bound` (> val) |
|---|---|---|
| val exists in result | lands **on** it → replaces it → length unchanged | lands **past** it → appends/replaces next → length grows wrongly |
| val doesn't exist | both behave the same | both behave the same |

**Rule:**
- Strictly increasing LIS → use `lower_bound`
- Non-decreasing LIS (duplicates allowed) → use `upper_bound`
