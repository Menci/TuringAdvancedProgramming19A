#include <iostream>
#include <cstdio>
#include <random>
using namespace std;
mt19937 gen(time(0) ^ 20010125);
inline int rnd(int l, int r) {
	uniform_int_distribution<> rng(l, r);
	return rng(gen);
}
inline double rndf(double l, double r) {
	uniform_real_distribution<> rng(l, r);
	return rng(gen);
}
int main() {
	freopen("data.in", "w", stdout);
	int n = 100000;
	printf("%d\n", n);
	// for(int i = 1; i <= n; ++i) cout << rndf(1e-312, 1e-310) << ' ' << rndf(1e-5, 1e-3) << endl;
	for(int i = 1; i <= n; ++i) cout << rndf(0.7e-308, 5.0e-308) << " " << rndf(1.0e-4, 5) << endl;
	// for(int i = 1; i <= n; ++i) cout << rndf(-1e3, 1e8) << ' '<< rndf(-1e8, 1e10) << endl;
	return 0;
}