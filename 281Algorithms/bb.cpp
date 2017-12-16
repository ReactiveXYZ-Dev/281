#include <iostream>
#include <vector>

using namespace std;

struct NQueensSolver {
	
	NQueensSolver(int n) {
		this->n = n;
		// resize stuff
		this->rows.resize(n);
		this->cols.resize(n, true);
		this->left_diags.resize(n * 2 - 1, true);
		this->right_diags.resize(n * 2 - 1, true);
	}
	
	void solve() {
		this->put_queen(0);
	}

private:
	int n;
	
	// cols and rows indicator
	vector<int> rows;
	// diagonals indicator
	vector<bool> cols;
	vector<bool> left_diags;
	vector<bool> right_diags;
	
	void put_queen(int row) {
		// check each column
		for (int col = 0; col < this->n; ++ col) {
			if (this->cols[col] && this->left_diags[row + col] && this->right_diags[row - col + this->n - 1]) {
				this->rows[row] = col;
				this->cols[col] = false;
				this->left_diags[row + col] = false;
				this->right_diags[row - col + this->n - 1] = false;
				
				// check end
				if (row == this->n - 1) {
					cout << "Found One solution" << endl;
					return;
				} else {
					this->put_queen(row + 1);
				}
				// backtrack
				this->cols[col] = true;
				this->left_diags[row + col] = true;
				this->right_diags[row - col + this->n -1] = true;
			}
		}
	}
	
};


int main(int argc, char *argv[]) {
	
	NQueensSolver solver(20);
	solver.solve();
	
}