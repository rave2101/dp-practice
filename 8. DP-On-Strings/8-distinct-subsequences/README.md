# Distinct Subsequences

## Problem

Given two strings `s` and `t`, find the **number of distinct ways** to form `t` as a subsequence of `s`.

A subsequence is formed by deleting some or no characters from `s` without changing the relative order of the remaining characters. Two ways are distinct if they use different index positions from `s`, even if they produce the same characters.

---

## Examples

```
s = "rabbbit",  t = "rabbit"

Output: 3

The three ways (bracket = chosen indices from s):
s = [r][a][b]b[b][i][t]   → "rabbit"
s = [r][a]b[b][b][i][t]   → "rabbit"
s = [r][a]bb[b][i][t] ... wait — there are three b's, pick any two

r  a  b  b  b  i  t
↑  ↑  ↑     ↑  ↑  ↑   → way 1
↑  ↑     ↑  ↑  ↑  ↑   → way 2
↑  ↑  ↑  ↑     ↑  ↑   → way 3 (no — need to match "rabbit" which has 2 b's)

Correct: three b's in s, need to choose 2 → C(3,2) = 3 ways
```

```
s = "eeeme",  t = "ee"

Output: 6

Four e's in "eeeme", choose any 2: C(4,2) = 6
```

```
s = "abc",  t = "abc"

Output: 1   (only one way — take all three in order)
```

---

## The Core Insight

State: `dp[i][j]` = number of distinct ways to form `t[0..j-1]` from `s[0..i-1]`.

At each pair `(i, j)`:

- **Characters match** (`s[i-1] == t[j-1]`): two independent choices —
  - **Use** `s[i-1]` to match `t[j-1]` → now need to form `t[0..j-2]` from `s[0..i-2]` → `dp[i-1][j-1]`
  - **Skip** `s[i-1]` and look for `t[j-1]` further left → `dp[i-1][j]`
  - Total: `dp[i][j] = dp[i-1][j-1] + dp[i-1][j]`

- **Characters don't match**: `s[i-1]` is useless for matching `t[j-1]`, skip it →
  - `dp[i][j] = dp[i-1][j]`

---

## Base Cases

```
dp[i][0] = 1  for all i
```
An empty `t` can always be formed from any prefix of `s` in exactly 1 way — select nothing.

```
dp[0][j] = 0  for j > 0
```
An empty `s` cannot form any non-empty `t`. Implicit from zero-initialisation.

---

## Detailed Table Fill

```
s = "rabbbit"  (M=7)
t = "rabbit"   (N=6)
```

```
           j=0  j=1  j=2  j=3  j=4  j=5  j=6
            ""   "r"  "a"  "b"  "b"  "i"  "t"
i=0  ""      1    0    0    0    0    0    0
i=1  r       1    1    0    0    0    0    0
i=2  a       1    1    1    0    0    0    0
i=3  b       1    1    1    1    0    0    0
i=4  b       1    1    1    2    1    0    0
i=5  b       1    1    1    3    3    0    0
i=6  i       1    1    1    3    3    3    0
i=7  t       1    1    1    3    3    3    3

return dp[7][6] = 3  ✓
```

Key cells:

```
(3,3): s[2]='b' == t[2]='b'  →  dp[2][2] + dp[2][3] = 1 + 0 = 1
(4,3): s[3]='b' == t[2]='b'  →  dp[3][2] + dp[3][3] = 1 + 1 = 2
(4,4): s[3]='b' == t[3]='b'  →  dp[3][3] + dp[3][4] = 1 + 0 = 1
(5,3): s[4]='b' == t[2]='b'  →  dp[4][2] + dp[4][3] = 1 + 2 = 3
(5,4): s[4]='b' == t[3]='b'  →  dp[4][3] + dp[4][4] = 2 + 1 = 3
```

The three `b`s in `s` bubbling up through the table — each new `b` combines with all previously counted ways.

---

## Lessons Learned

### 1 — `% MOD` must be applied at storage, not at return

**Wrong:**
```cpp
dp[i][j] = util(s,t,i-1,j-1,dp) + util(s,t,i-1,j,dp);  // stored without mod
return dp[i][j] % MOD;                                    // mod only on return
```

When a future call reads `dp[i][j]` from the cache, it gets the raw unmodded value and uses it in another addition — overflow creeps back in through the cache.

**Correct:**
```cpp
dp[i][j] = (util(s,t,i-1,j-1,dp) + util(s,t,i-1,j,dp)) % MOD;  // mod at storage
return dp[i][j];                                                    // return already-modded value
```

Every cached value must be bounded. The rule: **mod at the point of addition, store the modded result.**

### 2 — `long long` is still needed even with MOD

Each `dp` value is at most `10^9 + 6`. Adding two of them before `% MOD` gives up to `~2 × 10^9`, which overflows `int` (max `~2.1 × 10^9`) at the boundary. `long long` holds the intermediate sum safely before the mod is applied.

```cpp
// Both values up to (10^9 + 6):
(dp[i-1][j-1] + dp[i-1][j])   // can reach ~2 × 10^9 — safe in long long, risky in int
% MOD
```

### 3 — Using `int` as return type is fine

Even though the dp table is `long long`, the final answer after `% MOD` always fits in `int`. The `long long` table is internal — the return type can stay `int`.

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(string s, string t, int i, int j, vector<vector<long long>>& dp) {
    const int MOD = 1e9 + 7;
    if (j <= 0) return 1;
    if (i <= 0) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    if (s[i-1] == t[j-1])
        dp[i][j] = (util(s, t, i-1, j-1, dp) + util(s, t, i-1, j, dp)) % MOD;
    else
        dp[i][j] = util(s, t, i-1, j, dp);
    return dp[i][j];
}

int distinctSubsequences(string s, string t) {
    int M = s.size(), N = t.size();
    vector<vector<long long>> dp(M + 1, vector<long long>(N + 1, -1));
    return util(s, t, M, N, dp);
}
```

**Time: O(M × N) | Space: O(M × N) + O(M + N) call stack**

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int distinctSubsequences(string s, string t) {
    int M = s.size(), N = t.size();
    const int MOD = 1e9 + 7;
    vector<vector<long long>> dp(M + 1, vector<long long>(N + 1, 0));
    for (int i = 0; i <= M; i++) dp[i][0] = 1;
    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= N; j++) {
            if (s[i-1] == t[j-1])
                dp[i][j] = (dp[i-1][j-1] + dp[i-1][j]) % MOD;
            else
                dp[i][j] = dp[i-1][j];
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
| Memoization | O(M × N) | O(M × N) + O(M+N) stack |
| Tabulation  | O(M × N) | O(M × N) |
