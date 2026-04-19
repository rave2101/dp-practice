# Min Cost to Cut a Stick

## Problem

Given a stick of length `n` and an array `cuts` of positions where cuts must be made, find the **minimum total cost** to make all cuts. The cost of a single cut is the **length of the stick being cut** at that moment.

The order you make cuts matters — cutting a long piece is more expensive than cutting a short one.

---

## Examples

```
n = 7, cuts = [1, 3, 4, 5]

One order: cut at 1, 3, 4, 5 (left to right)
  cut 1 on stick [0,7] → cost 7
  cut 3 on stick [1,7] → cost 6
  cut 4 on stick [3,7] → cost 4
  cut 5 on stick [4,7] → cost 3
  Total = 20

Optimal order: cut at 3, then 1, then 4, then 5 (or similar)
  cut 3 on [0,7]  → cost 7
  cut 1 on [0,3]  → cost 3
  cut 4 on [3,7]  → cost 4
  cut 5 on [4,7]  → cost 3
  Total = 17 ... try another

Output: 16
```

```
n = 9, cuts = [5, 6, 1, 4, 2]

Output: 22
```

---

## Key Observation — Think in Reverse

Forward thinking: "which cut do I make first on this segment?"
is the same as asking:
"which cut is the **last** one made inside this segment?"

The last cut on a segment `[cuts[i-1], cuts[j+1]]` costs exactly `cuts[j+1] - cuts[i-1]` — the full length of that segment — regardless of which cut it is. The two sub-segments created are solved independently.

This reverse thinking is what makes the recurrence clean.

---

## Index Encoding

Add `0` and `n` as boundaries, then sort. Matrix `i` now refers to the cut at `cuts[i]`.

```
n=7, cuts=[1,3,4,5]  →  sorted with boundaries:

index:  0   1   2   3   4   5
cuts: [ 0,  1,  3,  4,  5,  7 ]
        ↑                   ↑
     left boundary      right boundary
```

Valid cut indices: `1` to `SIZE-2` (everything except the two boundaries).

**`dp[i][j]`** = minimum cost to make all cuts at indices `i` through `j`.

The **stick segment** being worked on when solving `dp[i][j]` is bounded by `cuts[i-1]` on the left and `cuts[j+1]` on the right — its neighbours in the sorted array.

---

## What does `i == j` mean?

`i == j` means **exactly one cut remains** in the current segment.

For `dp[2][2]` — only the cut at index 2 (position 3):

```
index:  0   1   2   3   4   5
cuts: [ 0,  1,  3,  4,  5,  7 ]
                ↑
         only this cut left
```

Segment = `cuts[i-1]` to `cuts[j+1]` = `cuts[1]` to `cuts[3]` = `1` to `4`.  
Cost = `4 - 1 = 3`. No sub-cuts remain, so both children return 0.

---

## Recurrence

For each possible "last cut" position `k` in `[i, j]`:

```
dp[i][j] = min over k in [i, j] of:
    (cuts[j+1] - cuts[i-1])  +  dp[i][k-1]  +  dp[k+1][j]
     ↑ cost of this cut           ↑ left half    ↑ right half
```

- `cuts[j+1] - cuts[i-1]` — length of the current segment (fixed for this `dp[i][j]`, regardless of `k`)
- `dp[i][k-1]` — min cost to handle all cuts to the left of `k`
- `dp[k+1][j]` — min cost to handle all cuts to the right of `k`

**Base case:** `i > j` → `0` (no cuts to make)

**Answer:** `dp[1][SIZE-2]`

---

## Why `j` must start at `i` in tabulation (not `i+1`)

`dp[i][i]` represents making a single cut — it equals `cuts[i+1] - cuts[i-1]`, not `0`.

Multi-cut cells like `dp[i][j]` read `dp[i][k-1]` and `dp[k+1][j]`, which can equal `dp[x][x]` for some `x`. If `j` starts at `i+1`, all `dp[x][x]` stay `0` (never filled), silently undercounting costs everywhere.

**Concrete example:** `n=7, cuts=[3]` — one cut, answer should be `7`.

```
After sorting: cuts = [0, 3, 7],  SIZE = 3
Need: dp[1][1]

With j = i+1:  inner loop condition j <= SIZE-2=1, starts at j=i+1=2 → never runs
               dp[1][1] = 0  ✗

With j = i:    j=1 satisfies j<=1 → runs
               k=1: cost = cuts[2]-cuts[0] + 0 + 0 = 7-0 = 7
               dp[1][1] = 7  ✓
```

---

## Approach 1 — Memoization (Top-Down)

```cpp
int util(int i, int j, vector<int>& cuts, vector<vector<int>>& dp) {
    if (i > j) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    int mini = INT_MAX;
    for (int k = i; k <= j; k++) {
        int current_cost = (cuts[j+1] - cuts[i-1]) + util(i, k-1, cuts, dp) + util(k+1, j, cuts, dp);
        mini = min(mini, current_cost);
    }
    return dp[i][j] = mini;
}

int minCost(int n, vector<int>& cuts) {
    cuts.push_back(0);
    cuts.push_back(n);
    sort(cuts.begin(), cuts.end());
    int SIZE = cuts.size();
    vector<vector<int>> dp(SIZE, vector<int>(SIZE, -1));
    return util(1, SIZE - 2, cuts, dp);
}
```

The base case `i > j` naturally handles single-cut ranges — when `i == j`, `k` can only be `i`, and `util(i, k-1)` = `util(i, i-1)` returns `0` (since `i > i-1`).

**Time: O(M³) | Space: O(M²) + O(M) call stack**  where M = number of cuts

---

## Approach 2 — Tabulation (Bottom-Up)

```cpp
int minCost(int n, vector<int>& cuts) {
    cuts.push_back(0);
    cuts.push_back(n);
    sort(cuts.begin(), cuts.end());
    int SIZE = cuts.size();
    vector<vector<int>> dp(SIZE, vector<int>(SIZE, 0));
    for (int i = SIZE - 2; i >= 1; i--) {
        for (int j = i; j <= SIZE - 2; j++) {   // j starts at i, not i+1
            int mini = INT_MAX;
            for (int k = i; k <= j; k++) {
                int current_cost = (cuts[j+1] - cuts[i-1]) + dp[i][k-1] + dp[k+1][j];
                mini = min(mini, current_cost);
            }
            dp[i][j] = mini;
        }
    }
    return dp[1][SIZE - 2];
}
```

**Iteration order:** `i` decreases (right to left) so that `dp[k+1][j]` (larger starting index) is already filled when needed.

**Time: O(M³) | Space: O(M²)**

---

## Table Fill

```
n=7, cuts=[1,3,4,5]
sorted: [0, 1, 3, 4, 5, 7],  SIZE=6,  valid range i,j in [1..4]

i=4: j=4: k=4: cuts[5]-cuts[3] + 0 + 0 = 7-4 = 3      → dp[4][4] = 3

i=3: j=3: k=3: cuts[4]-cuts[2] + 0 + 0 = 5-3 = 2      → dp[3][3] = 2
     j=4: k=3: cuts[5]-cuts[2] + dp[3][2] + dp[4][4] = 4 + 0 + 3 = 7
          k=4: cuts[5]-cuts[2] + dp[3][3] + dp[5][4] = 4 + 2 + 0 = 6
                                                        → dp[3][4] = 6

i=2: j=2: k=2: cuts[3]-cuts[1] + 0 + 0 = 4-1 = 3      → dp[2][2] = 3
     j=3: k=2: cuts[4]-cuts[1] + dp[2][1] + dp[3][3] = 4 + 0 + 2 = 6
          k=3: cuts[4]-cuts[1] + dp[2][2] + dp[4][3] = 4 + 3 + 0 = 7
                                                        → dp[2][3] = 6
     j=4: k=2: cuts[5]-cuts[1] + dp[2][1] + dp[3][4] = 6 + 0 + 6 = 12
          k=3: cuts[5]-cuts[1] + dp[2][2] + dp[4][4] = 6 + 3 + 3 = 12
          k=4: cuts[5]-cuts[1] + dp[2][3] + dp[5][4] = 6 + 6 + 0 = 12
                                                        → dp[2][4] = 12

i=1: j=1: k=1: cuts[2]-cuts[0] + 0 + 0 = 3-0 = 3      → dp[1][1] = 3
     j=2: k=1: cuts[3]-cuts[0] + dp[1][0] + dp[2][2] = 4 + 0 + 3 = 7
          k=2: cuts[3]-cuts[0] + dp[1][1] + dp[3][2] = 4 + 3 + 0 = 7
                                                        → dp[1][2] = 7
     j=3: k=1: cuts[4]-cuts[0] + 0 + dp[2][3] = 5 + 0 + 6 = 11
          k=2: cuts[4]-cuts[0] + dp[1][1] + dp[3][3] = 5 + 3 + 2 = 10
          k=3: cuts[4]-cuts[0] + dp[1][2] + dp[4][3] = 5 + 7 + 0 = 12
                                                        → dp[1][3] = 10
     j=4: k=1: cuts[5]-cuts[0] + 0 + dp[2][4] = 7 + 0 + 12 = 19
          k=2: cuts[5]-cuts[0] + dp[1][1] + dp[3][4] = 7 + 3 + 6 = 16
          k=3: cuts[5]-cuts[0] + dp[1][2] + dp[4][4] = 7 + 7 + 3 = 17
          k=4: cuts[5]-cuts[0] + dp[1][3] + dp[5][4] = 7 + 10 + 0 = 17
                                                        → dp[1][4] = 16

return dp[1][4] = 16  ✓
```

---

## Complexity Summary

| Approach    | Time   | Space              |
|---|---|---|
| Memoization | O(M³)  | O(M²) + O(M) stack |
| Tabulation  | O(M³)  | O(M²)              |

M = number of cuts (size of `cuts` array before adding boundaries)
