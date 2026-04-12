# Longest Common Substring

## Problem

Given two strings `str1` and `str2`, find the length of their **longest common substring**.

A **substring** is a contiguous sequence of characters within a string. Unlike a subsequence, you cannot skip characters — every character in the substring must appear consecutively in both strings.

---

## Examples

```
str1 = "abcde",  str2 = "abxde"

Common substrings: "a", "b", "ab", "d", "e", "de"
Longest: "ab" or "de"  (both length 2)
Output: 2
```

```
str1 = "abcde",  str2 = "ace"

Common substrings: "a", "c", "e"
Longest: "a" or "c" or "e"  (all length 1)
Output: 1
```

```
str1 = "hello",  str2 = "hello"

Output: 5
```

---

## Subsequence vs Substring — The One Key Difference

Both problems use the same DP table structure. The only difference is what happens on a **mismatch**:

| | Match | Mismatch |
|---|---|---|
| **Subsequence** | `1 + dp[i-1][j-1]` | `max(dp[i-1][j], dp[i][j-1])` |
| **Substring** | `1 + dp[i-1][j-1]` | `0` |

**Why `max(...)` for subsequence?**
A subsequence can skip characters. A mismatch just means "skip one character from one string and carry forward whatever you had" — the best you've built so far is not lost.

**Why `0` for substring?**
A substring must be contiguous. A mismatch physically snaps the current run. There is no common substring ending at this pair of characters, so the answer for this cell is `0`.

---

## What `dp[i][j]` Means

This is the most important thing to understand — and the source of the most common confusion.

```
dp[i][j] = length of the longest common substring
            ending EXACTLY at str1[i-1] and str2[j-1]
```

It does **not** mean "the best answer found anywhere so far." It is a very specific, local question: does a common substring land right here?

This is why:
- `dp[3][3] = 0` on a mismatch does **not** erase the `"ab"` you found at `dp[2][2]`
- That answer was already captured in `result` when it was computed
- `dp[3][3] = 0` simply says: "no common substring ends at this pair of characters"

The answer is the **maximum over the entire table**, not `dp[M][N]`, because the longest substring can end at any cell — not necessarily the last one.

---

## Detailed Table Fill

```
str1 = "abcde"  (M=5)
str2 = "abxde"  (N=5)
```

```
         j=0  j=1  j=2  j=3  j=4  j=5
          ""   "a"  "b"  "x"  "d"  "e"
i=0  ""    0    0    0    0    0    0
i=1  a     0    1    0    0    0    0
i=2  b     0    0    2    0    0    0
i=3  c     0    0    0    0    0    0
i=4  d     0    0    0    0    1    0
i=5  e     0    0    0    0    0    2
```

Step by step:

```
(1,1): str1[0]='a' == str2[0]='a'  →  dp[1][1] = 1+dp[0][0] = 1       result=1
(1,2): str1[0]='a' != str2[1]='b'  →  dp[1][2] = 0
(2,1): str1[1]='b' != str2[0]='a'  →  dp[2][1] = 0
(2,2): str1[1]='b' == str2[1]='b'  →  dp[2][2] = 1+dp[1][1] = 2       result=2
(2,3): str1[1]='b' != str2[2]='x'  →  dp[2][3] = 0
(3,3): str1[2]='c' != str2[2]='x'  →  dp[3][3] = 0   ← run snapped here
(4,4): str1[3]='d' == str2[3]='d'  →  dp[4][4] = 1+dp[3][3] = 1+0 = 1
       ↑ fresh start because dp[3][3]=0, not continuing from the "ab" run
(5,5): str1[4]='e' == str2[4]='e'  →  dp[5][5] = 1+dp[4][4] = 2       result=2

return result = 2   ✓  ("ab" and "de" are both length 2)
```

Notice `(4,4)`: `dp[4][4] = 1 + dp[3][3] = 1 + 0 = 1` — the `"de"` run starts fresh from 1, not from 2. The `"ab"` run is already safely stored in `result`; the zero at `dp[3][3]` did not erase it.

---

## Why the Answer is `max` over the Table, not `dp[M][N]`

`dp[M][N]` only tells you about substrings ending at the very last character of both strings. In the example above, `dp[5][5] = 2`, which happens to be correct — but only by coincidence.

Consider:
```
str1 = "abcz"
str2 = "abcw"

dp[4][4]: str1[3]='z' != str2[3]='w' → 0
```

`dp[M][N] = 0`, but the answer is `3` ("abc"). Without tracking `result = max(result, dp[i][j])` as you fill, you would return 0 and miss it entirely.

---

## Full Solution

```cpp
int longestCommonSubstr(string str1, string str2) {
    int M = str1.size(), N = str2.size();
    int result = 0;
    vector<vector<int>> dp(M + 1, vector<int>(N + 1, 0));

    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= N; j++) {
            if (str1[i-1] == str2[j-1]) {
                dp[i][j] = 1 + dp[i-1][j-1];
                result = max(result, dp[i][j]);
            } else {
                dp[i][j] = 0;
            }
        }
    }
    return result;
}
```

**Time: O(M × N) | Space: O(M × N)**

---

## Complexity Summary

| Problem | Mismatch | Answer at | Time | Space |
|---|---|---|---|---|
| Longest Common Subsequence | `max(dp[i-1][j], dp[i][j-1])` | `dp[M][N]` | O(M×N) | O(M×N) |
| Longest Common Substring | `0` | `max` over table | O(M×N) | O(M×N) |
