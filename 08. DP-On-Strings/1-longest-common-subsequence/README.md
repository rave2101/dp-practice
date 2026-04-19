# Longest Common Subsequence

## Problem

Given two strings `str1` and `str2`, find the length of their **longest common subsequence**.

A **subsequence** is a sequence that appears in the same relative order but not necessarily contiguous. A **common subsequence** is a subsequence that is common to both strings.

---

## Examples

```
str1 = "bdefg",  str2 = "bfg"

Common subsequences: "b", "f", "g", "bf", "bg", "fg", "bfg"
Longest: "bfg"
Output: 3
```

```
str1 = "abcde",  str2 = "ace"

Common subsequences: "a", "c", "e", "ac", "ae", "ce", "ace"
Longest: "ace"
Output: 3
```

```
str1 = "abc",  str2 = "abc"

Output: 3  (the strings themselves are common)
```

```
str1 = "abc",  str2 = "def"

Output: 0  (no characters in common)
```

---

## The Core Insight

At each pair of indices `(i, j)` — representing the first `i` characters of `str1` and first `j` characters of `str2` — we ask:

- **Characters match** (`str1[i-1] == str2[j-1]`): we must have found a common character, so take it and move both pointers back — `1 + LCS(i-1, j-1)`.
- **Characters don't match**: skip one character from either string and take the better result — `max(LCS(i-1, j), LCS(i, j-1))`.

State: `dp[i][j]` = length of LCS of `str1[0..i-1]` and `str2[0..j-1]`.

```
if str1[i-1] == str2[j-1]:
    dp[i][j] = 1 + dp[i-1][j-1]
else:
    dp[i][j] = max(dp[i-1][j], dp[i][j-1])
```

**Answer:** `dp[M][N]`

---

## Base Case

Using **1-based indexing** — `i` runs from `1` to `M`, `j` from `1` to `N`.

`i = 0` means `str1` is exhausted; `j = 0` means `str2` is exhausted. In either case, no common characters remain:

```
dp[0][j] = 0  for all j   ← str1 is empty
dp[i][0] = 0  for all i   ← str2 is empty
```

Both are implicit from zero-initialisation.

---

## Recursion Tree

For `str1 = "bde"`, `str2 = "be"` (M=3, N=2):

```
lcs("bde", "be") = util(3, 2)
```

Each node is `util(idx1, idx2)` — the LCS length for `str1[0..idx1-1]` and `str2[0..idx2-1]`.

```
                        util(3,2)
                       [str1[2]='e' == str2[1]='e' → match]
                        1 + util(2,1)
                             |
                    [str1[1]='d' != str2[0]='b' → no match]
                         max(  ,  )
                        /          \
                util(1,1)         util(2,0)
          [str1[0]='b'==str2[0]='b' → match]   [j=0 → return 0]
              1 + util(0,0)
                    |
                [i=0 → return 0]
                → util(1,1) = 1

        → util(2,1) = max(1, 0) = 1
    → util(3,2) = 1 + 1 = 2

Answer: 2  ("be" is the LCS)
```

**Why both branches at a mismatch?**

When `str1[i-1] != str2[j-1]`, we don't know which character is "blocking" the match — maybe dropping from `str1` helps, maybe dropping from `str2` does. We try both and keep the longer result.

**Why `1 + dp[i-1][j-1]` on a match (not `dp[i-1][j]` or `dp[i][j-1]`)?**

When the characters match, both are consumed together — skipping one without the other would risk double-counting or losing the pairing. Moving diagonally (`i-1, j-1`) is the only move that correctly records "we used this pair."

**Overlapping subproblems (why memoization helps):**

```
                        util(3,2)
                       /          \
               util(2,2)         util(3,1)
              /    \             /    \
        util(1,2) util(2,1)  util(2,1) util(3,0)
                      ↑           ↑
                  computed twice without memoization
```

`util(2,1)` appears in multiple branches. Memoization stores its result after the first computation, so subsequent calls return in O(1).

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(string str1, string str2, int idx1, int idx2, vector<vector<int>>& dp) {
    if (idx1 <= 0 || idx2 <= 0) return 0;
    if (dp[idx1][idx2] != -1) return dp[idx1][idx2];

    int not_pick = max(util(str1, str2, idx1 - 1, idx2, dp),
                       util(str1, str2, idx1, idx2 - 1, dp));
    int pick = INT_MIN;
    if (str1[idx1 - 1] == str2[idx2 - 1])
        pick = 1 + util(str1, str2, idx1 - 1, idx2 - 1, dp);

    return dp[idx1][idx2] = max(not_pick, pick);
}

int lcs(string str1, string str2) {
    int M = str1.size(), N = str2.size();
    vector<vector<int>> dp(M + 1, vector<int>(N + 1, -1));
    return util(str1, str2, M, N, dp);
}
```

**Time: O(M × N) | Space: O(M × N) + O(M + N) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int lcs(string str1, string str2) {
    int M = str1.size(), N = str2.size();
    vector<vector<int>> dp(M + 1, vector<int>(N + 1, 0));

    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= N; j++) {
            dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            if (str1[i - 1] == str2[j - 1])
                dp[i][j] = max(dp[i][j], 1 + dp[i - 1][j - 1]);
        }
    }
    return dp[M][N];
}
```

**Time: O(M × N) | Space: O(M × N)**

---

## Table Fill

```
str1 = "bdefg"  (M=5)
str2 = "bfg"    (N=3)

       j=0  j=1  j=2  j=3
       ""   "b"  "f"  "g"
i=0 ""  0    0    0    0
i=1 b   0    1    1    1    ← str1[0]='b' matches str2[0]='b' at (1,1): 1+dp[0][0]=1
i=2 d   0    1    1    1    ← 'd' matches nothing in "bfg"
i=3 e   0    1    1    1    ← 'e' matches nothing in "bfg"
i=4 f   0    1    2    2    ← str1[3]='f' matches str2[1]='f' at (4,2): 1+dp[3][1]=2
i=5 g   0    1    2    3    ← str1[4]='g' matches str2[2]='g' at (5,3): 1+dp[4][2]=3

return dp[5][3] = 3  ✓  (LCS = "bfg")
```

Key cell `(i=4, j=2)`: `str1[3]='f'` matches `str2[1]='f'` → `1 + dp[3][1] = 1 + 1 = 2` ✓  
Key cell `(i=5, j=3)`: `str1[4]='g'` matches `str2[2]='g'` → `1 + dp[4][2] = 1 + 2 = 3` ✓

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Memoization | O(M × N) | O(M × N) + O(M+N) stack |
| Tabulation  | O(M × N) | O(M × N) |
