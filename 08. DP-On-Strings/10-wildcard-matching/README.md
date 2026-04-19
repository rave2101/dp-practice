# Wildcard Matching

## Problem

Given a string `str` and a pattern `pat`, determine if the pattern matches the entire string.

The pattern can contain two special characters:
- `?` — matches **exactly one** character
- `*` — matches **zero or more** of any character

---

## Examples

```
str = "aa",  pat = "a"
Output: false   ('a' only matches one character)
```

```
str = "aa",  pat = "*"
Output: true    ('*' matches the entire string)
```

```
str = "cb",  pat = "?a"
Output: false   ('?' matches 'c', but 'a' != 'b')
```

```
str = "adceb",  pat = "*a*b"
Output: true    ('*' matches "", 'a' matches 'a', '*' matches "dce", 'b' matches 'b')
```

```
str = "acdcb",  pat = "a*c?b"
Output: false
```

---

## State

```
dp[i][j] = can pat[0..j-1] match str[0..i-1]?
```

`i` → how far into the string
`j` → how far into the pattern

---

## Base Cases

There are three base cases. Each answers a different question about empty inputs.

---

### Base Case 1 — `dp[0][0] = true`

> Can an empty pattern match an empty string?

Yes — trivially. Nothing matches nothing.

```
dp[0][0] = true
```

---

### Base Case 2 — `dp[0][j]` for `j > 0` (empty string, non-empty pattern)

> Can an empty string be matched by the first `j` characters of the pattern?

`i = 0` means string is empty. `j > 0` means pattern is not empty.

The only character that can match an empty string is `*` — because `*` means zero or more characters, and zero is valid. But it's not enough for just the current character to be `*`. **Every character before it must also be capable of matching empty.**

```cpp
for (int j = 1; j <= N; j++)
    dp[0][j] = (pat[j-1] == '*') && dp[0][j-1];
```

- `pat[j-1] == '*'` — current character must be a star
- `dp[0][j-1]` — everything before it must also match empty (the permission slip from the past)

Both must be true. One real character anywhere breaks the entire chain — even if more stars follow after it.

**Trace on `pat = "***a*"`:**

```
dp[0][0] = true    ← given

dp[0][1]: pat[0]='*' && dp[0][0]=true   → true    ("*"    matches "")
dp[0][2]: pat[1]='*' && dp[0][1]=true   → true    ("**"   matches "")
dp[0][3]: pat[2]='*' && dp[0][2]=true   → true    ("***"  matches "")
dp[0][4]: pat[3]='a' && dp[0][3]=true   → FALSE   ("***a" cannot match "" — 'a' needs a real char)
dp[0][5]: pat[4]='*' && dp[0][4]=false  → FALSE   (chain already broken — star can't undo 'a')
```

Once `'a'` breaks the chain, everything after stays false — even more stars cannot undo a real character that still needs something to match.

**Trace on `pat = "***"`:**

```
dp[0][1]: '*' && true  → true
dp[0][2]: '*' && true  → true
dp[0][3]: '*' && true  → true   ("***" matches "")
```

---

### Base Case 3 — `dp[i][0] = false` for `i > 0` (non-empty string, empty pattern)

> Can a non-empty string match an empty pattern?

No. A non-empty string always has characters that need to be matched. An empty pattern has nothing to match them with.

```
dp[i][0] = false   for all i > 0
```

This is already handled by the initialisation `vector<bool>(N+1, false)` — no explicit loop needed.

---

### All Three Base Cases Together

| Cell | String | Pattern | Question | Answer |
|---|---|---|---|---|
| `dp[0][0]` | empty | empty | can `""` match `""` ? | always `true` |
| `dp[0][j]` | empty | not empty | can `""` match this pattern? | only if all `*` |
| `dp[i][0]` | not empty | empty | can this string match `""` ? | always `false` |

The filled top row and left column for `pat = "***a*"`:

```
         j=0   j=1   j=2   j=3   j=4   j=5
          ""    *     *     *     a     *
i=0  ""   T     T     T     T     F     F
i=1  s[0] F
i=2  s[1] F
i=3  s[2] F
```

---

## Recurrence

### Exact match or `?` — `dp[i-1][j-1]`

`str[i-1] == pat[j-1]` or `pat[j-1] == '?'`:

Both characters are consumed — move diagonally.

`?` matches exactly one character, no more, no less. It always consumes one from both sides.

```
str = "abc",  pat = "a?c"   (i=2, j=2)
str[1]='b',  pat[1]='?'  → match
dp[2][2] = dp[1][1]
```

---

### `*` — `dp[i-1][j] || dp[i][j-1]`

`pat[j-1] == '*'`:

`*` has two choices — take whichever works:

**`dp[i-1][j]` — `*` matches one character**

Consume one character from `str`. Pattern stays at `*` (it can still match more).

```
str = "aab",  pat = "*b"   (i=2, j=1)
pat[0]='*' — use it to match 'a' at str[1]
dp[2][1] = dp[1][1]   ← str shrinks, pattern stays at '*'
```

**`dp[i][j-1]` — `*` matches zero characters**

Skip the `*` — treat it as matching nothing. Move past it in the pattern.

```
str = "b",  pat = "*b"   (i=1, j=1)
pat[0]='*' — match zero chars, skip it
dp[1][1] = dp[1][0]   ← pattern moves past '*', str unchanged
```

Either path working is enough → `||`

---

### `?` vs `*` — The Key Difference

| Character | Matches | Index movement |
|---|---|---|
| `?` | exactly **one** character — always | diagonal only (`i-1, j-1`) |
| `*` | **zero or more** — flexible | consume str (`i-1, j`) OR skip star (`i, j-1`) |

`?` is rigid — one character, no choice, always diagonal.
`*` is flexible — two options, take whichever leads to a match.

---

## Detailed Table Fill

```
str = "adceb"  (M=5)
pat = "*a*b"   (N=4)
```

**Base cases:**

```
dp[0][0] = true
dp[0][1]: pat[0]='*' && true  → true
dp[0][2]: pat[1]='a' && true  → false
dp[0][3]: pat[2]='*' && false → false
dp[0][4]: pat[3]='b' && false → false
```

**Full table:**

```
           j=0   j=1   j=2   j=3   j=4
            ""    *     a     *     b
i=0  ""      T     T     F     F     F
i=1  a       F     T     T     T     F
i=2  d       F     T     F     T     F
i=3  c       F     T     F     T     F
i=4  e       F     T     F     T     F
i=5  b       F     T     F     T     T

return dp[5][4] = true  ✓
```

Key cells:

```
(1,1): pat[0]='*' → dp[0][1] || dp[1][0] = true || false = true
(1,2): str[0]='a' == pat[1]='a' → dp[0][1] = true
(1,3): pat[2]='*' → dp[0][3] || dp[1][2] = false || true = true
(2,1): pat[0]='*' → dp[1][1] || dp[2][0] = true || false = true
(5,4): str[4]='b' == pat[3]='b' → dp[4][3] = true
```

---

## Full Solution

```cpp
bool wildCard(string str, string pat) {
    int M = str.size(), N = pat.size();
    vector<vector<bool>> dp(M + 1, vector<bool>(N + 1, false));

    dp[0][0] = true;
    for (int j = 1; j <= N; j++)
        dp[0][j] = (pat[j-1] == '*') && dp[0][j-1];

    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= N; j++) {
            if (str[i-1] == pat[j-1] || pat[j-1] == '?')
                dp[i][j] = dp[i-1][j-1];
            else if (pat[j-1] == '*')
                dp[i][j] = dp[i-1][j] || dp[i][j-1];
            else
                dp[i][j] = false;
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
