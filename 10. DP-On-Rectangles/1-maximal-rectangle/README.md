# Maximal Rectangle

## Problem

Given a binary matrix of `'0'`s and `'1'`s, find the largest rectangle containing only `'1'`s and return its area.

---

## Example

```
matrix:
1 0 1 0 0
1 0 1 1 1
1 1 1 1 1
1 0 0 1 0

Output: 6
```

---

## Core Idea

Treat each row as the **base of a histogram**. The height at each column = number of consecutive `'1'`s ending at the current row. Then the problem reduces to **Largest Rectangle in Histogram** applied to each row's histogram.

```
Row 0:  [1, 0, 1, 0, 0]   → histogram heights: [1, 0, 1, 0, 0]
Row 1:  [1, 0, 1, 1, 1]   → histogram heights: [2, 0, 2, 1, 1]
Row 2:  [1, 1, 1, 1, 1]   → histogram heights: [3, 1, 3, 2, 2]
Row 3:  [1, 0, 0, 1, 0]   → histogram heights: [4, 0, 0, 3, 0]
```

The largest rectangle in any of those histograms is the answer.

---

## Part 1 — Largest Rectangle in Histogram

### What left[ ] and right[ ] mean

For each bar `i`, we want the widest rectangle where bar `i` is the **shortest bar** (limiting height). So we find:

- `left[i]` — leftmost index the rectangle can start at
- `right[i]` — rightmost index the rectangle can end at

These are bounded by the **nearest shorter bar** on each side.

```
Area for bar i = heights[i] * (right[i] - left[i] + 1)
```

### Why we pop from the stack

The stack tracks candidate "wall" bars. When processing bar `i`:

> **Pop** if stack top is taller or equal to `heights[i]` — a taller bar is not a wall, the rectangle of height `heights[i]` clears it completely.
>
> **Stop** when stack top is shorter than `heights[i]` — that shorter bar IS the wall.

```
heights = [2, 1, 5, 6, 2, 3]

i=4, heights[4]=2, stack=[1(h1), 2(h5), 3(h6)]

  Pop index 3 (h=6): 6 >= 2, not a wall, height-2 rect clears it
  Pop index 2 (h=5): 5 >= 2, not a wall, height-2 rect clears it
  Stop index 1 (h=1): 1 <  2, IS a wall → left[4] = 1+1 = 2
```

Visually for bar 4 (height=2):

```
         _ _ _ _
        |2 2 2 2|     ← height-2 rectangle spans indices 2,3,4,5
  1  | 5| 6|   |  3
  0  1  2  3  4  5
       ↑
     wall (h=1)    left[4]=2, right[4]=5, area = 2*4 = 8
```

### Left pass trace — heights = [2, 1, 5, 6, 2, 3]

| i | h[i] | Stack (top→) | Pop? | left[i] | Stack after |
|---|---|---|---|---|---|
| 0 | 2 | empty | — | 0 | [0] |
| 1 | 1 | [0(h2)] | pop 0 (2≥1), empty | 0 | [1] |
| 2 | 5 | [1(h1)] | stop (1<5) | 2 | [1,2] |
| 3 | 6 | [1,2(h5)] | stop (5<6) | 3 | [1,2,3] |
| 4 | 2 | [1,2,3(h6)] | pop 3(6≥2), pop 2(5≥2), stop 1(1<2) | 2 | [1,4] |
| 5 | 3 | [1,4(h2)] | stop (2<3) | 5 | [1,4,5] |

`left = [0, 0, 2, 3, 2, 5]`

**Why i=1 pops bar 0 (h=2):** Bar 0 is taller than bar 1 (height 1). A height-1 rectangle is not blocked by a taller bar — it passes right under it. So bar 0 is useless as a wall and gets popped. Stack is empty → `left[1] = 0` (spans entire array).

```
 _ _ _ _ _ _
|1 1 1 1 1 1|    ← height-1 rect spans all 6 bars, area = 6
  0  1  2  3  4  5
```

### Right pass trace — heights = [2, 1, 5, 6, 2, 3]

Same logic, scanning right to left.

| i | h[i] | Stack (top→) | Pop? | right[i] | Stack after |
|---|---|---|---|---|---|
| 5 | 3 | empty | — | 5 | [5] |
| 4 | 2 | [5(h3)] | pop 5 (3≥2), empty | 5 | [4] |
| 3 | 6 | [4(h2)] | stop (2<6) | 3 | [4,3] |
| 2 | 5 | [4,3(h6)] | pop 3 (6≥5), stop 4 (2<5) | 3 | [4,2] |
| 1 | 1 | [4,2(h5)] | pop 2(5≥1), pop 4(2≥1), empty | 5 | [1] |
| 0 | 2 | [1(h1)] | stop (1<2) | 0 | [1,0] |

`right = [0, 5, 3, 3, 5, 5]`

### Final areas

```
heights = [2,  1,  5,  6,  2,  3]
left    = [0,  0,  2,  3,  2,  5]
right   = [0,  5,  3,  3,  5,  5]
```

| i | h | left | right | width | area |
|---|---|---|---|---|---|
| 0 | 2 | 0 | 0 | 1 | 2 |
| 1 | 1 | 0 | 5 | 6 | **6** |
| 2 | 5 | 2 | 3 | 2 | **10** ← max |
| 3 | 6 | 3 | 3 | 1 | 6 |
| 4 | 2 | 2 | 5 | 4 | **8** |
| 5 | 3 | 5 | 5 | 1 | 3 |

**Answer: 10**

---

## Part 2 — Extending to Maximal Rectangle

### Building the histogram row by row

For each cell `(i, j)`:
- If `matrix[i][j] == '1'`: `dp[j] = dp[j] + 1` (extend the streak)
- If `matrix[i][j] == '0'`: `dp[j] = 0` (streak broken, reset)

```
matrix:
1 0 1 0 0
1 0 1 1 1
1 1 1 1 1
1 0 0 1 0

Row 0: dp = [1, 0, 1, 0, 0]   largestRect = 1
Row 1: dp = [2, 0, 2, 1, 1]   largestRect = 3
Row 2: dp = [3, 1, 3, 2, 2]   largestRect = 6  ← answer
Row 3: dp = [4, 0, 0, 3, 0]   largestRect = 4
```

Row 2 histogram `[3,1,3,2,2]` gives the answer of 6.

### Bug encountered — wrong dp update

```cpp
// Wrong: when matrix[i][j]='0', adds 0 instead of resetting
dp[j] = dp[j] + (matrix[i][j] - '0');

// Fix: explicitly reset on '0'
dp[j] = (matrix[i][j] == '0') ? 0 : dp[j] + 1;
```

Without the reset, a column that hits a `'0'` keeps its old height, making the histogram think there's a tall bar when there isn't — producing wrong areas.

---

## Complexity

| Part | Time | Space |
|---|---|---|
| Largest Rectangle in Histogram | O(N) | O(N) |
| Maximal Rectangle | O(R × C) | O(C) |
