#define K 8
#define N 200000

#include "tree.h"

#include <cstdio>
#include <cstdint>
#include <random>
#include <numeric>

int main() {
	build();
	validate();

	std::mt19937_64 rng{std::random_device()()};
	for (size_t i = 0; i < N; i++)
		*(double *)(&nodes[i].data) = (double)rng() / std::numeric_limits<uint64_t>::max();

	FILE *fp = fopen("zipped.bin", "wb");
	fwrite(nodes.data(), sizeof(Node), nodes.size(), fp);
}
