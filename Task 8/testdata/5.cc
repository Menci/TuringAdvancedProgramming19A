#define K 8
#define N 240000

#include "tree.h"

#include <cstdio>
#include <cmath>
#include <string>
#include <fstream>
#include <random>

int main() {
	build(5);
	validate();

	std::mt19937 rng(233);
    std::normal_distribution<> nd{13, 3.2};
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < K; j++) {
            int x;
            do x = round(nd(rng)); while (x > 25 || x < 0);
            nodes[i].data[j] = 'A' + x;
        }
    }

	fwrite(nodes.data(), sizeof(Node), nodes.size(), stdout);
}
