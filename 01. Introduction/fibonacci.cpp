#include <iostream>
#include <vector>
using namespace std;

// Approach 1 — Memoization (Top-Down)
class Solution {
public:
    int fib(int n) {
        vector<int> cache(n + 1, -1);
        return solve(n, cache);
    }

    int solve(int n, vector<int>& cache) {
        if (n <= 1) return n;
        if (cache[n] != -1) return cache[n];
        cache[n] = solve(n - 1, cache) + solve(n - 2, cache);
        return cache[n];
    }
};

// Approach 2 — Tabulation (Bottom-Up)
class Solution2 {
public:
    int fib(int n) {
        if (n <= 1) return n;
        vector<int> dp(n + 1);
        dp[0] = 0;
        dp[1] = 1;
        for (int i = 2; i <= n; i++)
            dp[i] = dp[i - 1] + dp[i - 2];
        return dp[n];
    }
};
