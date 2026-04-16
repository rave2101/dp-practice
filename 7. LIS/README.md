# Longest Increasing Subsequence (LIS)

Problems based on finding the longest increasing subsequence pattern, including variants like Longest Divisible Subset, Longest String Chain, and Longest Bitonic Subsequence.

---

## Solutions

| File | Approach | Time | Space |
|---|---|---|---|
| `memoization.cpp` | Top-Down DP | O(n²) | O(n²) |
| `tabulation.cpp` | Bottom-Up DP | O(n²) | O(n) |
| `nlogn.cpp` | Binary Search (Patience Sorting) | O(n log n) | O(n) |

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
