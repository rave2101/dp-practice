#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int maxArea = 0, SIZE = heights.size();
        stack<int> S;
        vector<int> left(SIZE, 0);
        vector<int> right(SIZE, 0);

        for (int i = 0; i < SIZE; i++) {
            while (!S.empty() && heights[i] <= heights[S.top()]) S.pop();
            if (S.empty()) left[i] = 0;
            else left[i] = S.top() + 1;
            S.push(i);
        }

        while (!S.empty()) S.pop();

        for (int i = SIZE - 1; i >= 0; i--) {
            while (!S.empty() && heights[i] <= heights[S.top()]) S.pop();
            if (S.empty()) right[i] = SIZE - 1;
            else right[i] = S.top() - 1;
            S.push(i);
        }

        for (int i = 0; i < SIZE; i++) {
            int currentArea = (right[i] - left[i] + 1) * heights[i];
            maxArea = max(maxArea, currentArea);
        }
        return maxArea;
    }

    int maximalRectangle(vector<vector<char>>& matrix) {
        int R = matrix.size();
        int C = matrix[0].size();
        int maxArea = 0;
        vector<int> dp(C, 0);

        for (int i = 0; i < C; i++) dp[i] = matrix[0][i] - '0';
        maxArea = max(maxArea, largestRectangleArea(dp));

        for (int i = 1; i < R; i++) {
            for (int j = 0; j < C; j++) {
                dp[j] = (matrix[i][j] == '0') ? 0 : dp[j] + 1;
            }
            maxArea = max(maxArea, largestRectangleArea(dp));
        }
        return maxArea;
    }
};
