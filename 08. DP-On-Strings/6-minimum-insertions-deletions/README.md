# Minimum Insertions and Deletions to Convert String A to B

## Problem

Given two strings `str1` and `str2`, find the **minimum number of insertions and deletions** required to transform `str1` into `str2`. Operations can happen at any position.

---

## Examples

```
str1 = "kitten",  str2 = "sitting"

Output: 5
```

```
str1 = "heap",  str2 = "pea"

Output: 3
```

---

## The Key Insight — Build on LCS

The **LCS of str1 and str2 is the part that's already correct** — those characters appear in the right relative order in both strings. You don't touch them.

Everything outside the LCS needs work:
- Characters in `str1` that are **not** in the LCS → **delete** them (they don't belong in `str2`)
- Characters in `str2` that are **not** in the LCS → **insert** them (they are missing from `str1`)

```
deletions  = SIZE1 - LCS
insertions = SIZE2 - LCS
total      = (SIZE1 - LCS) + (SIZE2 - LCS)
           = SIZE1 + SIZE2 - 2 * LCS
```

---

## Worked Example — `str1 = "kitten"`, `str2 = "sitting"`

**Step 1 — Find the LCS**

```
str1 = "k i t t e n"   (SIZE1 = 6)
str2 = "s i t t i n g" (SIZE2 = 7)
```

LCS table:

```
           j=0  j=1  j=2  j=3  j=4  j=5  j=6  j=7
            ""   "s"  "i"  "t"  "t"  "i"  "n"  "g"
i=0  ""      0    0    0    0    0    0    0    0
i=1  k       0    0    0    0    0    0    0    0
i=2  i       0    0    1    1    1    1    1    1
i=3  t       0    0    1    2    2    2    2    2
i=4  t       0    0    1    2    3    3    3    3
i=5  e       0    0    1    2    3    3    3    3
i=6  n       0    0    1    2    3    3    4    4

LCS = dp[6][7] = 4
```

The LCS is `"ittn"`:
```
str1: k [i] [t] [t] e [n]     ← brackets = in LCS
str2: s [i] [t] [t] i [n] g   ← brackets = in LCS
```

**Step 2 — Count deletions**

Characters in `str1` NOT in the LCS → must be deleted:
```
str1 = k  i  t  t  e  n
           ↑  ↑  ↑     ↑   ← LCS ("ittn")
       ↑            ↑       ← NOT in LCS → delete "k" and "e"

deletions = SIZE1 - LCS = 6 - 4 = 2
```

**Step 3 — Count insertions**

Characters in `str2` NOT in the LCS → must be inserted into `str1`:
```
str2 = s  i  t  t  i  n  g
          ↑  ↑  ↑     ↑      ← LCS ("ittn")
       ↑            ↑     ↑  ← NOT in LCS → insert "s", "i", "g"

insertions = SIZE2 - LCS = 7 - 4 = 3
```

**Step 4 — Total**

```
total = deletions + insertions = 2 + 3 = 5

formula: SIZE1 + SIZE2 - 2*LCS = 6 + 7 - 2*4 = 13 - 8 = 5  ✓
```

**Verification — the actual transformation:**

```
"kitten"
  → delete 'k'          → "itten"
  → delete 'e'          → "ittn"    ← this is the LCS
  → insert 's' at front → "sittn"
  → insert 'i' at pos 4 → "sittin"
  → insert 'g' at end   → "sitting" ✓
```

---

## Second Example — `str1 = "heap"`, `str2 = "pea"`

**LCS:**

```
str1 = "heap"  (SIZE1 = 4)
str2 = "pea"   (SIZE2 = 3)
```

```
           j=0  j=1  j=2  j=3
            ""   "p"  "e"  "a"
i=0  ""      0    0    0    0
i=1  h       0    0    0    0
i=2  e       0    0    1    1
i=3  a       0    0    1    2
i=4  p       0    1    1    2

LCS = dp[4][3] = 2   ("ea")
```

```
str1 = h [e] [a] p    ← "ea" is the LCS
str2 = p [e] [a]

deletions  = 4 - 2 = 2   (delete 'h' and 'p' from str1)
insertions = 3 - 2 = 1   (insert 'p' into str1)
total      = 3

formula: 4 + 3 - 2*2 = 7 - 4 = 3  ✓
```

**Verification:**
```
"heap"
  → delete 'h'          → "eap"
  → delete 'p'          → "ea"    ← LCS
  → insert 'p' at front → "pea"  ✓
```

---

## Relation to Previous Problems

| Problem | Formula | What LCS Represents |
|---|---|---|
| [Print LCS](../2-print-lcs) | — | Common subsequence itself |
| [Min Insertions — Palindrome](../5-minimum-insertions-palindrome) | `N - LPS` | Palindrome skeleton already in place |
| **Min Insertions + Deletions** | `SIZE1 + SIZE2 - 2*LCS` | The "bridge" — characters already in the right place in both strings |

In all three, LCS is the **safe zone** — the characters you never touch. The work is always done on what falls outside it.

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

int minOperations(string str1, string str2) {
    int SIZE1 = str1.size(), SIZE2 = str2.size();
    vector<vector<int>> dp(SIZE1 + 1, vector<int>(SIZE2 + 1, 0));
    return (SIZE1 + SIZE2) - 2 * lcsUtil(str1, str2, SIZE1, SIZE2, dp);
}
```

**Time: O(M × N) | Space: O(M × N)**
