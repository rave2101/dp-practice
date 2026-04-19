# Boolean Parenthesization

## Problem

Given a boolean expression `s` consisting of operands (`T`, `F`) and operators (`&`, `|`, `^`) alternating, count the number of ways to parenthesize it so the whole expression evaluates to `True`. Return the answer modulo `10^9 + 7`.

---

## Examples

```
s = "T|F&T^F"

Answer: 4
```

```
s = "T"

Answer: 1   (only one operand, already True)
```

```
s = "F"

Answer: 0
```

---

## Why This Is MCM-Pattern

In Matrix Chain Multiplication you pick every possible split point `k` in a range `[i, j]` and combine the two halves independently. Here the expression `s[i..j]` has operators at indices `i+1, i+3, ...`. You try every operator position `k` as the **last evaluated operator**, compute how many ways each half evaluates to True/False, then combine based on the truth table of `s[k]`. Subproblems `dp[i][k-1]` and `dp[k+1][j]` overlap heavily, so memoization gives a polynomial solution.

---

## State and Recurrence

`dp[i][j][isTrue]` = number of ways `s[i..j]` evaluates to `isTrue`.

For each operator position `k` in `[i+1, j-1]` (step 2):

```
lt = dp[i][k-1][1]    (left half evaluates True)
lf = dp[i][k-1][0]    (left half evaluates False)
rt = dp[k+1][j][1]    (right half evaluates True)
rf = dp[k+1][j][0]    (right half evaluates False)
```

Truth tables for combining:

| Operator | isTrue = 1         | isTrue = 0                   |
|---|---|---|
| `&`      | lt*rt              | lt*rf + lf*rt + lf*rf        |
| `\|`     | lt*rt + lt*rf + lf*rt | lf*rf                     |
| `^`      | lt*rf + lf*rt      | lt*rt + lf*rf                |

**Base cases:**
- `i > j` → `0` (invalid range)
- `i == j` → `s[i] == 'T'` if `isTrue`, else `s[i] == 'F'`

---

## Solution — Memoization (Top-Down)

```cpp
long long MOD = 1e9 + 7;

int util(string& s, int i, int j, int isTrue, vector<vector<vector<int>>>& dp) {
    if (i > j) return 0;
    if (i == j) {
        if (isTrue) return s[i] == 'T';
        else return s[i] == 'F';
    }
    if (dp[i][j][isTrue] != -1) return dp[i][j][isTrue];

    long long ways = 0;
    for (int k = i + 1; k <= j - 1; k += 2) {
        long long lt = util(s, i, k - 1, 1, dp);
        long long lf = util(s, i, k - 1, 0, dp);
        long long rt = util(s, k + 1, j, 1, dp);
        long long rf = util(s, k + 1, j, 0, dp);

        if (s[k] == '&') {
            if (isTrue) ways = (ways + (lt * rt) % MOD) % MOD;
            else        ways = (ways + (lt * rf) % MOD + (lf * rt) % MOD + (lf * rf) % MOD) % MOD;
        } else if (s[k] == '^') {
            if (isTrue) ways = (ways + (lt * rf) % MOD + (lf * rt) % MOD) % MOD;
            else        ways = (ways + (lt * rt) % MOD + (lf * rf) % MOD) % MOD;
        } else if (s[k] == '|') {
            if (isTrue) ways = (ways + (lt * rt) % MOD + (lt * rf) % MOD + (lf * rt) % MOD) % MOD;
            else        ways = (ways + (lf * rf) % MOD) % MOD;
        }
    }
    return dp[i][j][isTrue] = ways;
}

int evaluateExp(string& exp) {
    int SIZE = exp.size();
    vector<vector<vector<int>>> dp(SIZE, vector<vector<int>>(SIZE, vector<int>(2, -1)));
    return util(exp, 0, SIZE - 1, 1, dp);
}
```

**Time: O(N³) | Space: O(N²) dp + O(N) call stack**

---

## Recursion Tree

Using `s = "T|F"` (indices 0,1,2).

```
util(0, 2, isTrue=1)
  k=1, s[1]='|'
    lt = util(0,0,1) = s[0]=='T' = 1
    lf = util(0,0,0) = s[0]=='F' = 0
    rt = util(2,2,1) = s[2]=='T' = 0   (s[2]='F')
    rf = util(2,2,0) = s[2]=='F' = 1

    isTrue=1 for '|': lt*rt + lt*rf + lf*rt
                     = 1*0  + 1*1  + 0*0  = 1

→ dp[0][2][1] = 1  ✓  (only "T|F" itself, which is True)
```

---

## Bugs Encountered and Fixed

### Bug 1 — Integer overflow in multiplications

**Wrong:**
```cpp
int lt = util(...);   // up to ~10^9
int rt = util(...);   // up to ~10^9
ways = (ways + (rt * lt) % MOD) % MOD;
// rt*lt is int*int → overflows 32-bit before % MOD is applied
```

**Fix:**
```cpp
long long lt = util(...);
long long rt = util(...);
// now rt*lt is long long*long long, can hold up to ~10^18
```

Values stored in `dp` are `< MOD ≈ 10^9`, which fits in `int`. But the **product** of two such values is up to `10^18`, requiring `long long` before the `% MOD` brings it back down. The overflow happened silently because `int*int` wraps at `2^31 - 1 ≈ 2.1×10^9`, far below `10^18`.

### Bug 2 — MOD declared as int

**Wrong:**
```cpp
int MOD = 1e9 + 7;
```

`1e9 + 7` as a `double` is `1000000007.0`, which fits in `int` — so no truncation here — but keeping `MOD` as `int` can still cause issues in expressions like `(long long val) % (int MOD)` on some compilers/architectures.

**Fix:**
```cpp
long long MOD = 1e9 + 7;
```

---

## Complexity Summary

| Approach    | Time  | Space                  |
|---|---|---|
| Memoization | O(N³) | O(N²·2) + O(N) stack  |
