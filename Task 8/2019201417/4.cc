#define K 8
#define N 240000

#include "tree.h"

#include <cstdio>
#include <string>
#include <fstream>

int main() {
	build(4);
	validate();

	std::ifstream fin("/tmp/3.xz", std::ios_base::binary);

	for (size_t i = 0; i < N; i++) {
		fin.read(nodes[i].data, K);
	}

	FILE *fp = fopen("zipped.bin", "wb");
	fwrite(nodes.data(), sizeof(Node), nodes.size(), fp);
}
