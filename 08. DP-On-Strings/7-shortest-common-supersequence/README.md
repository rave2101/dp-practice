# Shortest Common Supersequence

## Problem

Given two strings `str1` and `str2`, find the **shortest string that contains both `str1` and `str2` as subsequences**.

---

## Examples

```
str1 = "mno",  str2 = "nop"

Output: "mnop"

"mnop" contains "mno" as a subsequence (first three characters)
"mnop" contains "nop" as a subsequence (last three characters)
```

```
str1 = "abac",  str2 = "cab"

Output: "cabac"  (length 5)
```

---

## The Key Insight — LCS Characters Appear Once

The naive approach would be to concatenate both strings: `str1 + str2`. That always contains both as subsequences, but it's too long because the common characters are duplicated.

The **LCS characters are already shared** — they satisfy both strings simultaneously. So in the SCS, each LCS character appears exactly once. Every non-LCS character from both strings must appear separately.

```
SCS length = M + N - LCS
```

But this problem asks for the **actual string**, not just the length — so we backtrack through the DP table to reconstruct it.

---

## How Backtracking Works

The backtracking logic is a small but important variation on [Print LCS](../2-print-lcs/README.md):

| Situation | Print LCS | Print SCS |
|---|---|---|
| `str1[i-1] == str2[j-1]` (match) | add the character once, move diagonally | add the character once, move diagonally |
| move up (`i--`) | skip — don't add | **add `str1[i-1]`** — it must be in SCS |
| move left (`j--`) | skip — don't add | **add `str2[j-1]`** — it must be in SCS |
| leftovers after loop | nothing | **add all remaining characters** from whichever string isn't exhausted |

In LCS you only collect the matched characters. In SCS you collect **everything** — matched characters once, unmatched characters from whichever string they came from.

---

## Worked Example — `str1 = "mno"`, `str2 = "nop"`

**Step 1 — Fill the LCS table**

```
str1 = "mno"  (M=3)
str2 = "nop"  (N=3)

           j=0  j=1  j=2  j=3
            ""   "n"  "o"  "p"
i=0  ""      0    0    0    0
i=1  m       0    0    0    0
i=2  n       0    1    1    1
i=3  o       0    1    2    2

LCS = dp[3][3] = 2  ("no")
```

**Step 2 — Backtrack from (3, 3)**

```
(i=3, j=3): str1[2]='o',  str2[2]='p'  → no match
            dp[i-1][j]=dp[2][3]=1
            dp[i][j-1]=dp[3][2]=2
            dp[i][j-1] > dp[i-1][j]  →  add str2[2]='p',  j--
            result = "p"

(i=3, j=2): str1[2]='o',  str2[1]='o'  → match!
            add 'o', i--, j--
            result = "op"

(i=2, j=1): str1[1]='n',  str2[0]='n'  → match!
            add 'n', i--, j--
            result = "nop"

j=0 → exit while loop
```

**Step 3 — Drain leftover**

```
i=1 still > 0 → str1[0]='m' remains
add 'm'
result = "mnop"
```

**Verify:**
```
"mnop" contains "mno"?  m-n-o  ✓  (positions 0,1,2)
"mnop" contains "nop"?  n-o-p  ✓  (positions 1,2,3)
Length = 4 = M + N - LCS = 3 + 3 - 2 = 4  ✓
```

---

## Why the Leftover Loops Are Essential

The while loop exits as soon as **either** index hits 0. But the other string may still have characters that haven't been added yet — and every character of both strings must appear in the SCS.

```
str1 = "abc",  str2 = "bc"
LCS = "bc" (length 2)

Backtrack:
(3,2): 'c'=='c' → add 'c', move to (2,1)   result = "c"
(2,1): 'b'=='b' → add 'b', move to (1,0)   result = "bc"

j=0 → exit while loop
i=1 still remaining → str1[0]='a' not yet added

Without leftover loop: "bc"   ✗  ("abc" is not a subsequence of "bc")
With leftover loop:    "abc"  ✓
```

---

## SCS vs Print LCS — Side by Side

```cpp
// Print LCS — only collect matched characters
if (str1[i-1] == str2[j-1]) {
    result = str1[i-1] + result;   // add
    i--; j--;
} else if (dp[i-1][j] >= dp[i][j-1]) {
    i--;                            // skip, don't add
} else {
    j--;                            // skip, don't add
}
// no leftover loops needed — LCS only cares about matches

// Print SCS — collect everything
if (str1[i-1] == str2[j-1]) {
    result = str1[i-1] + result;   // add (shared character, include once)
    i--; j--;
} else if (dp[i-1][j] >= dp[i][j-1]) {
    result = str1[i-1] + result;   // add (str1's unique character)
    i--;
} else {
    result = str2[j-1] + result;   // add (str2's unique character)
    j--;
}
while (i > 0) { result = str1[i-1] + result; i--; }  // drain str1
while (j > 0) { result = str2[j-1] + result; j--; }  // drain str2
```

---

## Full Solution

```cpp
int lcsUtil(string s, string t, int M, int N, vector<vector<int>>& dp) {
    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= N; j++) {
            dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            if (s[i-1] == t[j-1])
                dp[i][j] = max(dp[i][j], 1 + dp[i-1][j-1]);
        }
    }
    return dp[M][N];
}

string shortestCommonSupersequence(string str1, string str2) {
    int M = str1.size(), N = str2.size();
    string result = "";
    vector<vector<int>> dp(M + 1, vector<int>(N + 1, 0));
    lcsUtil(str1, str2, M, N, dp);

    int i = M, j = N;
    while (i > 0 && j > 0) {
        if (str1[i-1] == str2[j-1]) {
            result = str1[i-1] + result;   // LCS character — include once
            i--; j--;
        } else if (dp[i-1][j] >= dp[i][j-1]) {
            result = str1[i-1] + result;   // str1 character not in LCS
            i--;
        } else {
            result = str2[j-1] + result;   // str2 character not in LCS
            j--;
        }
    }
    while (i > 0) { result = str1[i-1] + result; i--; }  // leftover from str1
    while (j > 0) { result = str2[j-1] + result; j--; }  // leftover from str2
    return result;
}
```

**Time: O(M × N) | Space: O(M × N)**
