#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int stockBuySell(vector<int> arr, int n) {
        int min_so_far = arr[0];
        int max_profit = 0;
        int SIZE = arr.size();
        for (int i = 1; i < SIZE; i++) {
            min_so_far = min(min_so_far, arr[i]);
            max_profit = max(max_profit, arr[i] - min_so_far);
        }
        return max_profit;
    }
};
