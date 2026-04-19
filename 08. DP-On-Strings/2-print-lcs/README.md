# Print LCS

## Problem

Given two strings `str1` and `str2`, **print** the actual longest common subsequence — not just its length.

This builds directly on [Problem 1 — Longest Common Subsequence](../1-longest-common-subsequence/README.md). The DP table is filled the same way; the only addition is a backtracking pass to reconstruct the sequence from the table.

---

## Example

```
str1 = "bdefg",  str2 = "bfg"

LCS length : 3
LCS string : "bfg"
```

---

## The Core Idea — Backtracking the DP Table

The filled DP table already encodes every decision made during the forward pass. Reading it backwards from `dp[M][N]` retraces those decisions:

```
At cell (i, j):

  str1[i-1] == str2[j-1]  →  characters matched here
                               prepend str1[i-1] to result
                               move diagonally: i--, j--

  else                     →  no match; move toward the cell that contributed dp[i][j]
                               dp[i-1][j] >= dp[i][j-1]  →  i--   (came from above)
                               dp[i-1][j] <  dp[i][j-1]  →  j--   (came from left)
```

Stop when either index reaches 0 — one string is exhausted, no more common characters possible.

---

## Backtracking Trace

```
str1 = "bdefg"  (M=5)
str2 = "bfg"    (N=3)

DP table (from Problem 1):

       j=0  j=1  j=2  j=3
       ""   "b"  "f"  "g"
i=0 ""  0    0    0    0
i=1 b   0    1    1    1
i=2 d   0    1    1    1
i=3 e   0    1    1    1
i=4 f   0    1    2    2
i=5 g   0    1    2    3

Start at (5,3):

(5,3): str1[4]='g' == str2[2]='g'  →  prepend 'g', move to (4,2)   result = "g"
(4,2): str1[3]='f' == str2[1]='f'  →  prepend 'f', move to (3,1)   result = "fg"
(3,1): str1[2]='e' != str2[0]='b'
       dp[2][1]=1 >= dp[3][0]=0    →  i--, move to (2,1)
(2,1): str1[1]='d' != str2[0]='b'
       dp[1][1]=1 >= dp[2][0]=0    →  i--, move to (1,1)
(1,1): str1[0]='b' == str2[0]='b'  →  prepend 'b', move to (0,0)   result = "bfg"

i=0 → stop

LCS = "bfg"  ✓
```

---

## Why Check `str1[i-1] == str2[j-1]` Directly?

An earlier attempt used this condition instead:

```cpp
// Indirect — unreliable
if (dp[i][j] != dp[i-1][j] && dp[i][j] != dp[i][j-1])
```

The idea: if `dp[i][j]` differs from both its top and left neighbours, a diagonal move must have contributed. But this breaks when a match exists yet `dp[i][j]` happens to equal `dp[i-1][j]`:

```
Suppose dp[i-1][j-1]=1, dp[i-1][j]=2, dp[i][j-1]=1, str1[i-1]==str2[j-1]
→ dp[i][j] = max(2, 1, 1+1) = 2 = dp[i-1][j]

Indirect condition: dp[i][j] != dp[i-1][j]  →  false  →  takes else branch  →  misses the match
Direct check:       str1[i-1] == str2[j-1]  →  true   →  correctly prepends the character
```

The direct character comparison is always correct because it checks the actual reason a diagonal move was valid, not a consequence that can coincidentally be absent.

---

## Bugs to Avoid

### 1 — `to_string(char)` prints the ASCII number

```cpp
result = to_string(str1[i-1]) + result;  // to_string('b') = "98"  ✗
result = str1[i-1] + result;             // char prepended to string  ✓
```

`to_string` is overloaded for numeric types. A `char` is implicitly an integer, so `to_string('b')` gives `"98"` — the ASCII code — not `"b"`.

### 2 — Nested for loops cause double-decrements

```cpp
// Broken
for (; i > 0; i--) {
    for (; j > 0; j--) {
        if (match) {
            i--;   // body decrements i ...
            j--;   // body decrements j ...
            // inner for header also runs j-- → j decremented twice
        } else {
            (dp[i-1][j] >= dp[i][j-1]) ? i-- : j--;
            // chosen dimension decremented twice (body + for header)
        }
        // outer for header also runs i-- → i decremented twice after match
    }
}
```

Every decrement in the body competes with the corresponding loop header. The fix is a plain `while` loop where you control all index movement yourself:

```cpp
// Correct
while (i > 0 && j > 0) {
    if (str1[i-1] == str2[j-1]) {
        result = str1[i-1] + result;
        i--; j--;
    } else {
        (dp[i-1][j] >= dp[i][j-1]) ? i-- : j--;
    }
}
```

---

## Full Solution

```cpp
void printLCS(string str1, string str2, int i, int j, vector<vector<int>>& dp) {
    string result = "";
    while (i > 0 && j > 0) {
        if (str1[i-1] == str2[j-1]) {
            result = str1[i-1] + result;
            i--;
            j--;
        } else {
            (dp[i-1][j] >= dp[i][j-1]) ? i-- : j--;
        }
    }
    cout << "LCS :: " << "\t" << result;
}

string lcs(string str1, string str2) {
    int M = str1.size(), N = str2.size();
    vector<vector<int>> dp(M + 1, vector<int>(N + 1, 0));

    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= N; j++) {
            dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            if (str1[i-1] == str2[j-1])
                dp[i][j] = max(dp[i][j], 1 + dp[i-1][j-1]);
        }
    }

    printLCS(str1, str2, M, N, dp);
    return "";  // length is dp[M][N]
}
```

**Time: O(M × N) | Space: O(M × N)**

The backtracking pass is O(M + N) — at each step at least one index decrements, and both start bounded by M and N.
