# Edit Distance

## Problem

Given two strings `start` and `target`, find the **minimum number of operations** to convert `start` into `target`.

Three operations are allowed, each costing 1:
- **Insert** — add any character at any position
- **Delete** — remove any character from any position
- **Replace** — change any character to another character

---

## Examples

```
start = "planet",  target = "plan"

Delete 'e' → "plant"... wait
Delete 'e' → "plaent" — no, delete from position 5 → "plant"... 

Step by step:
"planet" → delete 'e' → "plant" ... hmm

Actually:
"planet" → delete 'e' (index 4) → "plant" — no

p-l-a-n-e-t
Delete 'e' → "plant" ... p-l-a-n-t (5 chars)
Delete 't' → "plan"

Output: 2
```

```
start = "horse",  target = "ros"

"horse" → replace 'h' with 'r' → "rorse"
        → delete  'r' (index 2) → "rose"
        → delete  'e'           → "ros"

Output: 3
```

```
start = "abc",  target = "abc"

Output: 0   (already equal)
```

---

## The Core Insight

State: `dp[i][j]` = minimum operations to convert `start[0..i-1]` into `target[0..j-1]`.

At each pair `(i, j)`:

**Characters match** (`start[i-1] == target[j-1]`):
No operation needed — both characters are already equal. Just look at the previous subproblem:
```
dp[i][j] = dp[i-1][j-1]
```

**Characters don't match**: try all three operations, take the cheapest:
```
dp[i][j] = 1 + min(
    dp[i-1][j],      ← Delete
    dp[i][j-1],      ← Insert
    dp[i-1][j-1]     ← Replace
)
```

---

## Understanding the Three Operations

The naming feels backwards at first — here is exactly why each index moves the way it does.

### Replace — `dp[i-1][j-1]`

Replace `start[i-1]` with `target[j-1]`. Both characters are now consumed — move both indices back.

```
start  = "cat",  target = "cot"   (i=2, j=2)
start[1]='a',  target[1]='o'  → no match

Replace 'a' with 'o':
"cat" → "cot"   ← one operation
Now solve dp[1][1]: convert "c" into "c"  ← both shrink

i went from 2 to 1  (start char consumed)
j went from 2 to 1  (target char consumed)
```

### Delete — `dp[i-1][j]`

Delete `start[i-1]`. Start shrinks by one. Target is **unchanged** — you didn't satisfy any target character, so `j` stays.

```
start  = "cats",  target = "cat"   (i=4, j=3)
start[3]='s',  target[2]='t'  → no match

Delete 's' from start:
"cats" → "cat"   ← one operation
Now solve dp[3][3]: convert "cat" into "cat"

i went from 4 to 3  (start char consumed)
j stayed at 3       (target unchanged)
```

### Insert — `dp[i][j-1]`

Insert `target[j-1]` into start. That target character is now satisfied — `j` moves back. Start is **unchanged** — you didn't consume any start character, so `i` stays.

```
start  = "cat",  target = "cats"   (i=3, j=4)
start[2]='t',  target[3]='s'  → no match

Insert 's' at end of start:
"cat" → "cats"   ← one operation, target[3]='s' is matched
Now solve dp[3][3]: convert "cat" into "cat"

i stayed at 3       (start unchanged)
j went from 4 to 3  (target char satisfied)
```

### One-line summary

| Operation | What reduces | Index movement |
|---|---|---|
| Replace | both last chars consumed | `i-1`, `j-1` |
| Delete `start[i-1]` | start shrinks | `i-1`, j stays |
| Insert `target[j-1]` | target shrinks (satisfied) | i stays, `j-1` |

You are not literally performing the operation and inspecting the result. You are asking: **"If I do this operation, which subproblem am I left with?"** The indices tell you the answer.

---

## Base Cases

```
dp[i][0] = i  for all i
```
To convert `start[0..i-1]` into an empty target — delete all `i` characters. Cost = `i`.

```
dp[0][j] = j  for all j
```
To convert an empty start into `target[0..j-1]` — insert all `j` characters. Cost = `j`.

---

## Detailed Table Fill

```
start  = "planet"  (M=6)
target = "plan"    (N=4)
```

```
           j=0  j=1  j=2  j=3  j=4
            ""   "p"  "l"  "a"  "n"
i=0  ""      0    1    2    3    4
i=1  p       1    0    1    2    3
i=2  l       2    1    0    1    2
i=3  a       3    2    1    0    1
i=4  n       4    3    2    1    0
i=5  e       5    4    3    2    1
i=6  t       6    5    4    3    2

return dp[6][4] = 2  ✓
```

Key cells:

```
(1,1): start[0]='p' == target[0]='p'  →  dp[0][0] = 0
(2,2): start[1]='l' == target[1]='l'  →  dp[1][1] = 0
(3,3): start[2]='a' == target[2]='a'  →  dp[2][2] = 0
(4,4): start[3]='n' == target[3]='n'  →  dp[3][3] = 0
(5,4): start[4]='e' != target[3]='n'
       1 + min(dp[4][4], dp[5][3], dp[4][3])
       = 1 + min(0, 2, 1) = 1   ← delete 'e'
(6,4): start[5]='t' != target[3]='n'
       1 + min(dp[5][4], dp[6][3], dp[5][3])
       = 1 + min(1, 3, 2) = 2   ← delete 't'
```

The match diagonal `(1,1) → (2,2) → (3,3) → (4,4)` all cost 0 — `"plan"` is already a prefix of `"planet"`. The two deletions at `(5,4)` and `(6,4)` handle `'e'` and `'t'`.

---

## Second Example — `start = "horse"`, `target = "ros"`

```
           j=0  j=1  j=2  j=3
            ""   "r"  "o"  "s"
i=0  ""      0    1    2    3
i=1  h       1    1    2    3
i=2  o       2    2    1    2
i=3  r       3    2    2    2
i=4  s       4    3    3    2
i=5  e       5    4    4    3

return dp[5][3] = 3  ✓
```

Key path:
```
(1,1): 'h' != 'r' → 1 + min(dp[0][1], dp[1][0], dp[0][0]) = 1+min(1,1,0) = 1  ← replace
(2,2): 'o' == 'o' → dp[1][1] = 1
(3,1): 'r' == 'r' → dp[2][0] = 2
(5,3): 'e' != 's' → 1 + min(dp[4][3], dp[5][2], dp[4][2]) = 1+min(2,4,3) = 3
```

Operations: replace `'h'→'r'`, delete `'r'` (second), delete `'e'` → 3 ops ✓

---

## Full Solution

```cpp
int editDistance(string start, string target) {
    int M = start.size(), N = target.size();
    vector<vector<int>> dp(M + 1, vector<int>(N + 1, 0));

    for (int i = 0; i <= M; i++) dp[i][0] = i;   // delete all chars of start
    for (int j = 0; j <= N; j++) dp[0][j] = j;   // insert all chars of target

    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= N; j++) {
            if (start[i-1] == target[j-1]) {
                dp[i][j] = dp[i-1][j-1];                    // no operation needed
            } else {
                dp[i][j] = 1 + min({dp[i-1][j],             // delete
                                    dp[i][j-1],              // insert
                                    dp[i-1][j-1]});          // replace
            }
        }
    }
    return dp[M][N];
}
```

**Time: O(M × N) | Space: O(M × N)**

---

## Complexity Summary

| Approach | Time | Space |
|---|---|---|
| Tabulation | O(M × N) | O(M × N) |
