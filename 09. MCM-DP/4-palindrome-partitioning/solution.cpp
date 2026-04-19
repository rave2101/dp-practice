#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
/*
class Solution {
public:
    bool isPalindrome(int i, int j, string& s) {
        while (i <= j) {
            if (s[i] != s[j]) return false;
            i++; j--;
        }
        return true;
    }

    int util(int i, int SIZE, string& s, vector<int>& dp) {
        if (i == SIZE) return 0;
        if (dp[i] != -1) return dp[i];
        int minCost = INT_MAX;
        for (int j = i; j < SIZE; j++) {
            if (isPalindrome(i, j, s)) {
                int cost = 1 + util(j + 1, SIZE, s, dp);
                minCost = min(cost, minCost);
            }
        }
        return dp[i] = minCost;
    }

    int minCut(string s) {
        int SIZE = s.size();
        vector<int> dp(SIZE, -1);
        return util(0, SIZE, s, dp) - 1;
    }
};
*/

// Approach 2 — Tabulation (Bottom-Up)
class Solution {
public:
    bool isPalindrome(int i, int j, string& s) {
        while (i <= j) {
            if (s[i] != s[j]) return false;
            i++; j--;
        }
        return true;
    }

    int minCut(string s) {
        int SIZE = s.size();
        vector<int> dp(SIZE + 1, 0);
        for (int i = SIZE - 1; i >= 0; i--) {
            int minCost = INT_MAX;
            for (int j = i; j < SIZE; j++) {
                if (isPalindrome(i, j, s)) {
                    int cost = 1 + dp[j + 1];
                    minCost = min(cost, minCost);
                }
            }
            dp[i] = minCost;
        }
        return dp[0] - 1;
    }
};
