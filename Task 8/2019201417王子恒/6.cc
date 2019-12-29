#define K 8
#define N 240000

#include "tree.h"

#include <cstdio>
#include <string>
#include <fstream>

int main() {
	build(6);
	validate();

	std::ifstream fin("6.bmp", std::ios_base::binary);

	for (size_t i = 0; i < N; i++) {
		fin.read(nodes[i].data, K);
	}

	FILE *fp = fopen("zipped.bin", "wb");
	fwrite(nodes.data(), sizeof(Node), nodes.size(), fp);
}
