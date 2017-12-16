#include <iostream>
#include <vector>

using namespace std;


int knapsack(const vector<int>& weights, const vector<int>& values, int capacity) {
	
	// init state space
	// base case 0,0 => 0 included
	vector<vector<int>> dp(capacity + 1, vector<int>(weights.size() + 1, 0));
	
	// recursive case
	for (int i = 1; i <= capacity; ++ i) {
		for (int j = 1; j <= (int)weights.size(); ++ j) {
			// case 1: if able to pick the weight
			if (weights[j] <= i) {
				// two choices, pick or not pick, get max
				dp[i][j] = max(dp[i][j-1], values[j] + dp[i-weights[j]][j]);
			} else {
				// cant pick, simple inherit the previous value
				dp[i][j] = dp[i][j-1];
			}
		}
	}
	
	return dp[capacity][weights.size()];
}

int lps(const string& str) {
	
	// init state space
	vector<vector<int>> dp(str.length(), vector<int>(str.length(), 0));
		
	// base case f(i,i) == 1
	for (size_t i = 0; i < str.length(); ++ i) {
		dp[i][i] = 1;
	}
		
	// recursive case
	for (size_t i = 0; i < str.length()-1; ++ i) {
		for (size_t j = i; j < str.length(); ++j) {
			// case 1: i == j => 1
			if (i == j) {
				dp[i][j] = 1;
			}
			// case 2: j == i + 1 && A[i] == A[j]
			else if (j == i + 1 && str[i] == str[j]) {
				dp[i][j] = 2;
			}
			// case 3: A[i] != A[j]
			else if (str[i] != str[j]) {
				dp[i][j] = max(dp[i+1][j], dp[i][j-1]);
			}
			// case 4: A[i] == A[j]
			else if (str[i] == str[j]){
				dp[i][j] = 2 + dp[i-1][j-1];
			}
		}
	}
	
	return dp[0][str.length()-1];
	
}

int main() {
	
//	vector<int> weights = {10, 20, 30};
//	vector<int> values = {60, 100, 120};
//	int capacity = 50;
//		
//	cout << knapsack(weights, values, capacity);

	string str = "GEEKSFORGEEKS";
	cout << lps(str) << endl;
	
}