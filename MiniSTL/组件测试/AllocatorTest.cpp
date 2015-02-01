/*
** AllocatorTest
** Created by Rayn on 2015/01/31
** ≤‚ ‘ø’º‰≈‰÷√∆˜
*/
#include <vector>
#include <iostream>
#include "../Allocator.h"
using namespace std;

int main() {
	vector<int, rayn::allocator<int>> vec{1, 2, 3, 4, 5};

	for (int i = 6; i < 10; ++i) {
		vec.push_back(i);
	}
	for (auto cur : vec) {
		cout << cur << endl;
	}
	return 0;
}