#include <iostream>
#include <cstdio>
#include <ctime>
#include <vector>

using namespace std;

using ll = long long;

// calculate binomial coeff using DP
ll binomial_coeff(ll n, ll m) {
	// init states
	ll states[n+1][m+1];
	// start building states
	for (int m_ = 0; m_ <= m; ++ m_){
		for (int n_ = m_; n_ <= n; ++ n_) {
			if (n_ == m_ || m_ == 0) {
				states[n_][m_] = 1;
			} else {
				states[n_][m_] = states[n_-1][m_-1] + states[n_-1][m_];
			}
		}
	}
	return states[n][m];
}

int main() {
	
	cout << binomial_coeff(20, 2);

}