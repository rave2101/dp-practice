# Minimum Insertions to Make String Palindrome

## Problem

Given a string `s`, find the **minimum number of insertions** needed to make it a palindrome. You can insert a character at any position.

---

## Examples

```
s = "abcaa"

Insert 'b' and 'c' → "abcacba"  (or "abcacba")
Output: 2
```

```
s = "abcd"

Insert 'b', 'c', 'd' → "abcdcba"
Output: 3
```

```
s = "aaa"

Already a palindrome.
Output: 0
```

---

## The Key Insight — Build on LPS

This problem reduces to one formula:

```
minimum insertions = s.size() - LPS(s)
```

Where `LPS(s)` is the Longest Palindromic Subsequence from [Problem 4](../4-longest-palindromic-subsequence/README.md).

---

## Why Does That Formula Work?

Think of it this way:

The **LPS is the palindrome skeleton already hiding inside `s`**. Those characters are already in the right order to form a palindrome — you don't need to insert anything for them.

Every character that is **not** part of the LPS is an "orphan" — it has no mirror partner. To make the string a palindrome, each orphan needs exactly **one** insertion (its mirror).

```
total characters        = N
characters already good = LPS(s)
characters needing help = N - LPS(s)   ← each needs one insertion
```

So minimum insertions = `N - LPS(s)`.

---

## Worked Example — `s = "abcaa"`

**Step 1 — Find the LPS**

```
s = "abcaa"
t = reverse(s) = "aacba"
```

LCS of `s` and `t`:

```
           j=0  j=1  j=2  j=3  j=4  j=5
            ""   "a"  "a"  "c"  "b"  "a"
i=0  ""      0    0    0    0    0    0
i=1  a       0    1    1    1    1    1
i=2  b       0    1    1    1    2    2
i=3  c       0    1    1    2    2    2
i=4  a       0    1    2    2    2    3
i=5  a       0    1    2    2    2    3

LPS = dp[5][5] = 3
```

The LPS is `"aba"` or `"aca"` or `"aaa"` — all length 3.

**Step 2 — Count orphans**

```
s     =  a  b  c  a  a
LPS   =  a  b     a       ("aba" — length 3)
orphan=        c     a    ← 2 characters not in the LPS
```

Each orphan needs one mirror inserted. So minimum insertions = 2.

**Step 3 — Verify**

```
s         = "abcaa"
insertions = insert 'c' after position 3, insert 'b' after position 4
result    = "abcacba"   ← palindrome ✓

formula: s.size() - LPS = 5 - 3 = 2  ✓
```

---

## Second Example — `s = "abcd"`

**LPS:**

```
s = "abcd"
t = "dcba"

LCS("abcd", "dcba"):

       ""  d  c  b  a
""      0  0  0  0  0
a       0  0  0  0  1
b       0  0  0  1  1
c       0  0  1  1  1
d       0  1  1  1  1

LPS = 1
```

No two characters in `"abcd"` match each other in the LPS sense — the best palindromic subsequence is any single character (length 1).

**Orphans:**

```
s     =  a  b  c  d
LPS   =  a               ← just one character is "good"
orphans= b  c  d         ← 3 characters need mirrors
```

**Insertions:**

```
s   = "abcd"
→ insert 'c' → "abccd" → insert 'b' → "abcbcd" → insert 'a' → "abcdcba"

formula: 4 - 1 = 3  ✓
```

---

## The Chain of Reductions

This problem is solved in three layers — each building on the previous:

```
Minimum Insertions
      ↓  reduces to
Longest Palindromic Subsequence   (Problem 4)
      ↓  reduces to
Longest Common Subsequence        (Problem 1)
```

No new DP logic is written at any layer — just one formula on top of LPS, and LPS itself is just LCS of `s` and `reverse(s)`.

---

## Full Solution

```cpp
int lcsUtil(string s, string t, int N, vector<vector<int>>& dp) {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            if (s[i-1] == t[j-1])
                dp[i][j] = max(dp[i][j], 1 + dp[i-1][j-1]);
        }
    }
    return dp[N][N];
}

int lpsUtil(string s) {
    int SIZE = s.size();
    vector<vector<int>> dp(SIZE + 1, vector<int>(SIZE + 1, 0));
    string t = s;
    reverse(t.begin(), t.end());
    return lcsUtil(s, t, SIZE, dp);
}

int minInsertion(string s) {
    return s.size() - lpsUtil(s);
}
```

**Time: O(N²) | Space: O(N²)**

---

## Complexity Summary

| Step | Time | Space |
|---|---|---|
| Reverse string | O(N) | O(N) |
| LCS tabulation | O(N²) | O(N²) |
| Final formula | O(1) | O(1) |
| **Total** | **O(N²)** | **O(N²)** |
