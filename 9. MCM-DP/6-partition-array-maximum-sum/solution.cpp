#include <bits/stdc++.h>
using namespace std;

// Approach 1 — Memoization (Top-Down)
int memoization(vector<int>& nums, int k, int i, vector<int>& dp, int SIZE) {
    if (i == SIZE) return 0;
    if (dp[i] != -1) return dp[i];
    int len = 0, maxi = INT_MIN, result = INT_MIN;
    for (int j = i; j < min(i + k, SIZE); j++) {
        maxi = max(maxi, nums[j]);
        len++;
        int current = (maxi * len) + memoization(nums, k, j + 1, dp, SIZE);
        result = max(result, current);
    }
    return dp[i] = result;
}

// Approach 2 — Tabulation (Bottom-Up)
int tabulation(vector<int>& nums, int k) {
    int SIZE = nums.size();
    vector<int> dp(SIZE + 1, 0);
    for (int i = SIZE - 1; i >= 0; i--) {
        int len = 0, maxi = INT_MIN, result = INT_MIN;
        for (int j = i; j < min(i + k, SIZE); j++) {
            maxi = max(maxi, nums[j]);
            len++;
            int current = (maxi * len) + dp[j + 1];
            result = max(result, current);
        }
        dp[i] = result;
    }
    return dp[0];
}

int maximumSubarray(vector<int>& arr, int k) {
    int SIZE = arr.size();
    vector<int> dp(SIZE + 1, -1);
    // return memoization(arr, k, 0, dp, SIZE);
    return tabulation(arr, k);
}
