#include <iostream>
#include <vector>
using namespace std;

// Approach 1 — Memoization (Top-Down)
class Solution {
public:
    int climbStairs(int n) {
        vector<int> cache(n + 1, -1);
        return solve(n, cache);
    }

    int solve(int n, vector<int>& cache) {
        if (n <= 2) return n;
        if (cache[n] != -1) return cache[n];
        cache[n] = solve(n - 1, cache) + solve(n - 2, cache);
        return cache[n];
    }
};

// Approach 2 — Space-Optimized Tabulation (Bottom-Up)
class Solution2 {
public:
    int climbStairs(int n) {
        if (n <= 3) return n;
        int second_last = 1, last = 2, current = 0;
        for (int idx = 3; idx <= n; idx++) {
            current = second_last + last;
            second_last = last;
            last = current;
        }
        return current;
    }
};
