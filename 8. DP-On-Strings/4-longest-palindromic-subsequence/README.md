# Longest Palindromic Subsequence

## Problem

Given a string `s`, find the length of its **longest palindromic subsequence**.

A **palindrome** reads the same forwards and backwards.
A **subsequence** can be derived by deleting some or no characters without changing the order of the remaining characters.

---

## Examples

```
s = "eeeme"

Palindromic subsequences: "e", "ee", "eee", "eeee", "eme", ...
Longest: "eeee" (pick all four e's, skip the 'm')
Output: 4
```

```
s = "bbabcbcab"

Output: 7   ("babcbab")
```

```
s = "abcd"

Output: 1   (no two characters match)
```

---

## The Key Insight — Reduce to LCS

The longest palindromic subsequence of `s` is exactly the **LCS of `s` and its reverse**.

Why? A palindromic subsequence reads the same forwards and backwards. So any palindromic subsequence of `s` is also a subsequence of `reverse(s)`. The longest one common to both is the answer.

```
s = "eeeme"
t = reverse(s) = "emeee"

LCS("eeeme", "emeee") = 4   ("eeee")
```

No new algorithm needed — the entire LCS tabulation from Problem 1 is reused unchanged.

---

## `string t = s` — Value or Reference?

```cpp
string t = s;
reverse(t.begin(), t.end());
```

**`string t = s` is a copy by value.**

`t` is a brand new, independent string with its own memory. Reversing `t` has zero effect on `s`.

```cpp
string s = "eeeme";
string t = s;               // t = "eeeme"  (independent copy)
reverse(t.begin(), t.end()); // t = "emeee"  (only t is changed)
// s is still "eeeme"
```

If you had written `string& t = s` instead (a reference), `t` would be an alias for `s` — reversing `t` would also reverse `s`, corrupting the input before LCS even starts.

```cpp
// What a reference would look like — DO NOT do this here
string& t = s;               // t is just another name for s
reverse(t.begin(), t.end()); // s is now "emeee" — input destroyed!
return lcsUtil(s, t, ...);   // s and t are the same string — always LCS = SIZE
```

| | `string t = s` | `string& t = s` |
|---|---|---|
| Memory | New copy | Same object |
| Modifying `t` affects `s`? | No | Yes |
| Correct here? | Yes | No |

`reverse(t.begin(), t.end())` works **in-place** on whatever string `t` refers to. Since `t` is an independent copy, `s` stays intact and you correctly have two different strings to feed into LCS.

---

## Detailed Table Fill

```
s = "eeeme"
t = "emeee"
```

```
         j=0  j=1  j=2  j=3  j=4  j=5
          ""   "e"  "m"  "e"  "e"  "e"
i=0  ""    0    0    0    0    0    0
i=1  e     0    1    1    1    1    1
i=2  e     0    1    1    2    2    2
i=3  e     0    1    1    2    3    3
i=4  m     0    1    2    2    3    3
i=5  e     0    1    2    3    3    4
```

Key cells:

```
(1,1): s[0]='e' == t[0]='e'  →  1+dp[0][0] = 1
(2,3): s[1]='e' == t[2]='e'  →  1+dp[1][2] = 1+1 = 2
(3,4): s[2]='e' == t[3]='e'  →  1+dp[2][3] = 1+2 = 3
(4,2): s[3]='m' == t[1]='m'  →  1+dp[3][1] = 1+1 = 2
       but max(dp[3][2], dp[4][1]) = max(2,1) = 2 → dp[4][2] = 2
(5,3): s[4]='e' == t[2]='e'  →  1+dp[4][2] = 1+2 = 3
       but max(dp[4][3], dp[5][2]) = max(3,2) = 3 → dp[5][3] = 3
(5,5): s[4]='e' == t[4]='e'  →  1+dp[4][4] = 1+3 = 4

return dp[5][5] = 4  ✓
```

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

int longestPalinSubseq(string s) {
    int SIZE = s.size();
    vector<vector<int>> dp(SIZE + 1, vector<int>(SIZE + 1, 0));
    string t = s;                // copy by value
    reverse(t.begin(), t.end()); // reverse the copy, s untouched
    return lcsUtil(s, t, SIZE, dp);
}
```

**Time: O(N²) | Space: O(N²)**

---

## Why `dp[N][N]` is the Answer Here (Unlike Substring)

The answer is at `dp[N][N]` — not a running max — because this is LCS, not substring. The LCS accumulates the global best at the bottom-right corner. Revisit [Problem 3 — Longest Common Substring](../3-longest-common-substring/README.md) for why substring needs `max` over the whole table instead.

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Reduce to LCS + Tabulation | O(N²) | O(N²) |
