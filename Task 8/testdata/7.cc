#define K 8
#define N 240000

#include "tree.h"

#include <cstdio>
#include <string>
#include <fstream>

int main() {
	build(7);
	validate();

	std::ifstream fin("7.bmp", std::ios_base::binary);

	for (size_t i = 0; i < N; i++) {
		fin.read(nodes[i].data, K);
	}

	fwrite(nodes.data(), sizeof(Node), nodes.size(), stdout);
}
