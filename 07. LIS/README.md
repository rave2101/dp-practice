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
| `longest_string_chain.cpp` | LIS variant — predecessor check | O(n²·L) | O(n) |
| `longest_bitonic_sequence.cpp` | Two LIS passes (forward + backward) | O(n²) | O(n) |
| `number_of_lis.cpp` | LIS + count[] to track distinct ways | O(n²) | O(n) |

---

## Number of Longest Increasing Subsequences — Problem

**Difficulty:** Medium

Given an array, return the **count** of distinct longest increasing subsequences.

```
Input:  nums = [1, 3, 5, 4, 7]
Output: 2
LIS:    [1, 3, 5, 7]  and  [1, 3, 4, 7]
```

### Why `dp[]` Alone Is Not Enough

`dp[i]` tells you the **length** of the LIS ending at `i`, but not **how many** distinct LIS of that length exist. Multiple paths through different indices can produce the same length.

### The `count[]` Array

`count[i]` = number of distinct LIS of length `dp[i]` ending at index `i`.

When `nums[j] < nums[i]`, two cases:

| Condition | Action | Meaning |
|---|---|---|
| `dp[j]+1 > dp[i]` | `dp[i]=dp[j]+1`, `count[i]=count[j]` | New longer length — inherit all ways from j |
| `dp[j]+1 == dp[i]` | `count[i]+=count[j]` | Another path to same length — accumulate ways |

Final answer: sum `count[i]` for all `i` where `dp[i] == maxLen`.

### Walkthrough: `[1, 3, 5, 4, 7]`

```
i=4 (7): j=2(5): dp[2]+1=4 > dp[4]=1 → dp[4]=4, count[4]=count[2]=1
         j=3(4): dp[3]+1=4 == dp[4]=4 → count[4]+=count[3] → count[4]=2

dp    = [1, 2, 3, 3, 4]
count = [1, 1, 1, 1, 2]

maxLen=4, dp[4]==4 → result = count[4] = 2  ✓
```

---

## Longest Bitonic Sequence — Problem

**Difficulty:** Medium

Given an array, find the length of the longest **bitonic subsequence** — one that first strictly increases then strictly decreases. A purely increasing or purely decreasing sequence also qualifies.

```
Input:  arr = [1, 11, 2, 10, 4, 5, 2, 1]
Output: 7
Chain:  1 → 2 → 10 → 5 → 2 → 1  (increases then decreases)
```

### Approach — Two LIS Passes

Treat every index `i` as the **peak** of the bitonic sequence.
The bitonic sequence through `i` has two parts:

```
[ increasing part ] → i ← [ decreasing part ]
```

Run LIS twice:
- **`dp1[i]`** = LIS length ending **at** `i` (left → right)
- **`dp2[i]`** = LIS length starting **at** `i` (right → left)

```
Answer = max over all i of: dp1[i] + dp2[i] - 1
```

The `-1` removes the double-count of the peak `i` (it appears in both `dp1` and `dp2`).

### Walkthrough: `[1, 11, 2, 10, 4, 5, 2, 1]`

```
dp1 (LIS ending at i):    [1, 2, 2, 3, 3, 4, 2, 1]
dp2 (LIS starting at i):  [1, 6, 2, 5, 3, 3, 2, 1]

dp1+dp2-1:                [1, 7, 3, 7, 5, 6, 3, 1]

Answer: 7  ✓
```

### Why the backward pass is just LIS in reverse

`dp2[i]` = length of longest decreasing subsequence **starting** at `i`.
A decreasing subsequence going left→right is the same as an increasing subsequence going right→left. So the backward pass runs the exact same LIS loop — just iterating `i` from `SIZE-2` down to `0` and `j` from `SIZE-1` down to `i+1`.

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

## Longest String Chain — Problem

**Difficulty:** Medium

Given an array of words, return the length of the longest chain where each word is a **predecessor** of the next. `wordA` is a predecessor of `wordB` if you can insert exactly one letter anywhere in `wordA` to get `wordB`.

```
Input:  words = ["a", "b", "ba", "bca", "bda", "bdca"]
Output: 4
Chain:  "a" → "ba" → "bda" → "bdca"
```

### Why This Is an LIS Variant

`dp[i]` = length of longest chain ending at `words[i]`. For each `i`, look back at all `j < i` and check if `words[j]` is a predecessor of `words[i]` — identical structure to LIS tabulation with a custom condition instead of `nums[j] < nums[i]`.

### Why Sort by Length (Not Lexicographically)

A predecessor is always **1 character shorter** than its successor. The DP requires all valid predecessors to appear at a lower index. Lexicographic sort breaks this:

```
"ab" < "b"  lexicographically  →  "ab" gets index 0, "b" gets index 1

When processing "b" (i=1): compare("ab", "b") → SIZE2-SIZE1 = -1 ≠ 1 → false
The chain "b" → "ab" is never found  ✗
```

Sorting by length guarantees shorter words always come before longer ones.

### compare(a, b) — Is `a` a predecessor of `b`?

Two-pointer: try to match all characters of `a` inside `b`. On a match, advance both. On a mismatch, **advance only `j`** (skip the inserted character in `b`, keep `a`'s pointer in place).

```
a="ab", b="acb"   (insert 'c' in the middle)

i=0,j=0: 'a'=='a' → i=1, j=1
i=1,j=1: 'b'!='c' → j=2      ← skip 'c' in b only
i=1,j=2: 'b'=='b' → i=2, j=3
i==SIZE1 → true  ✓
```

### Bugs Encountered

**Bug 1 — Advancing both `i` and `j` on mismatch:**
```cpp
if(a[i]!=b[j]) mismatch++;
i++; j++;   // ← wrong: skips one char from a too
```
Only works when the inserted character is at the very end. Fails for mid-string insertions — counts 2 mismatches instead of 1.

**Bug 2 — Using `SIZE2-SIZE1 >= 2` instead of `SIZE2-SIZE1 != 1`:**

`>= 2` fails to reject equal-length words (`diff=0`) and cases where `a` is longer than `b` (negative diff). Use `!= 1` to handle all invalid cases in one check.

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
