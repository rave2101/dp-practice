# Palindrome Partitioning II

## Problem

Given a string `s`, partition it so that every substring is a palindrome. Return the **minimum number of cuts** needed.

---

## Examples

```
s = "aab"

Partitions: ["a","a","b"]  →  1 cut
Output: 1
```

```
s = "a"

Output: 0  (already a palindrome, no cuts needed)
```

```
s = "ab"

Output: 1  (["a","b"])
```

---

## Why This Is MCM-Pattern

In Matrix Chain Multiplication you try every split point `k` in a range `[i, j]` and combine the cost of both halves. Here you do the same: at each starting index `i`, you try every ending index `j`. If `s[i..j]` is a palindrome, you "cut" after `j` and recurse on the remaining suffix `[j+1..end]`. The subproblems are **overlapping** (many starting indices reuse the same suffix result), which is exactly why memoization helps.

---

## Recurrence

Define `util(i)` = minimum number of **partitions** (palindromic substrings) for `s[i..SIZE-1]`.

```
util(i) = min over j in [i, SIZE-1], where s[i..j] is a palindrome, of:
              1 + util(j+1)

Base case: util(SIZE) = 0  (empty suffix, zero partitions needed)
```

**Answer:** `util(0) - 1`

---

## Why `- 1` at the End

`util` counts **partitions** (substrings), not cuts. For any partitioning into `k` substrings, you need exactly `k - 1` cuts.

```
"aab"  →  ["a", "a", "b"]  →  3 partitions, 2 cuts
"aba"  →  ["aba"]           →  1 partition,  0 cuts
```

So `minCut = util(0) - 1`.

If `util` returned cuts directly, the recurrence would be `util(i) = min(util(j+1) + 1)` but the base case becomes `util(SIZE) = -1` (so that the last partition adds `1 - 1 = 0` cuts). The `- 1` offset is cleaner to handle at the call site rather than burying it in the base case.

---

## Approach 1 — Memoization (Top-Down)

```cpp
bool isPalindrome(int i, int j, string& s) {
    while (i <= j) {
        if (s[i] != s[j]) return false;
        i++; j--;
    }
    return true;
}

int util(int i, int SIZE, string& s, vector<int>& dp) {
    if (i == SIZE) return 0;
    if (dp[i] != -1) return dp[i];
    int minCost = INT_MAX;
    for (int j = i; j < SIZE; j++) {
        if (isPalindrome(i, j, s)) {
            int cost = 1 + util(j + 1, SIZE, s, dp);
            minCost = min(cost, minCost);
        }
    }
    return dp[i] = minCost;
}

int minCut(string s) {
    int SIZE = s.size();
    vector<int> dp(SIZE, -1);
    return util(0, SIZE, s, dp) - 1;
}
```

**Time: O(N³) | Space: O(N) dp + O(N) call stack**

---

## Recursion Tree

Using `s = "aab"` (SIZE = 3).

```
util(0)   s[0..?]
├── j=0: s[0..0]="a" ✓  →  1 + util(1)
│                               ├── j=1: s[1..1]="a" ✓  →  1 + util(2)
│                               │                               ├── j=2: s[2..2]="b" ✓  →  1 + util(3) = 1+0 = 1
│                               │                               → util(2) = 1
│                               │         cost = 1+1 = 2
│                               │
│                               └── j=2: s[1..2]="ab" ✗ (not palindrome, skip)
│                               → util(1) = 2
│         cost = 1+2 = 3
│
├── j=1: s[0..1]="aa" ✓  →  1 + util(2) = 1+1 = 2  [util(2) cached]
│         cost = 2
│
└── j=2: s[0..2]="aab" ✗ (not palindrome, skip)

→ util(0) = min(3, 2) = 2
→ minCut  = util(0) - 1 = 1  ✓
```

**Overlapping subproblems — why memoization helps:**

`util(2)` is computed under `j=1` of `util(1)` and reused when `util(0)` tries `j=1`. Without memoization it recomputes from scratch every time. With memoization it is stored after the first call.

```
util(2) = 1  computed once under util(1), reused under util(0) via j=1
```

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
bool isPalindrome(int i, int j, string& s) {
    while (i <= j) {
        if (s[i] != s[j]) return false;
        i++; j--;
    }
    return true;
}

int minCut(string s) {
    int SIZE = s.size();
    vector<int> dp(SIZE + 1, 0);   // dp[SIZE] = 0 (base case)
    for (int i = SIZE - 1; i >= 0; i--) {
        int minCost = INT_MAX;
        for (int j = i; j < SIZE; j++) {
            if (isPalindrome(i, j, s)) {
                int cost = 1 + dp[j + 1];
                minCost = min(cost, minCost);
            }
        }
        dp[i] = minCost;
    }
    return dp[0] - 1;
}
```

**Why `dp` has size `SIZE + 1`:**

When `j = SIZE - 1` (last character), the recurrence reads `dp[j + 1] = dp[SIZE]`. That cell must exist and equal `0` (empty suffix). Allocating `SIZE + 1` entries and initializing all to `0` handles this without a bounds check.

**Iteration order:** `i` goes right-to-left so `dp[j+1]` (a larger index) is already filled before `dp[i]` needs it.

**Time: O(N³) | Space: O(N)**

---

## Table Fill

```
s = "aab",  SIZE = 3,  dp = [_, _, _, 0]  (index 3 is base case)

i=2: j=2: s[2..2]="b" ✓  cost = 1 + dp[3] = 1+0 = 1  →  dp[2] = 1

i=1: j=1: s[1..1]="a" ✓  cost = 1 + dp[2] = 1+1 = 2
     j=2: s[1..2]="ab" ✗
     → dp[1] = 2

i=0: j=0: s[0..0]="a" ✓  cost = 1 + dp[1] = 1+2 = 3
     j=1: s[0..1]="aa" ✓  cost = 1 + dp[2] = 1+1 = 2
     j=2: s[0..2]="aab" ✗
     → dp[0] = 2

return dp[0] - 1 = 1  ✓
```

---

## Complexity Summary

| Approach    | Time  | Space           |
|---|---|---|
| Memoization | O(N³) | O(N) + O(N) stack |
| Tabulation  | O(N³) | O(N)            |
