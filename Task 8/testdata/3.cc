#define K 64
#define N 100000

#include "tree.h"

#include <cstdio>
#include <string>
#include <fstream>

int main() {
	build();
	validate();

	std::ifstream fin("/tmp/data.txt");

	for (size_t i = 0; i < N; i++) {
		std::string line;
		std::getline(fin, line);
		for (size_t j = 0; j < K; j++) {
			nodes[i].data[j] = (j >= line.length() ? '\0' : line[j]) ^ (char)(j ^ i);
		}
	}

	fwrite(nodes.data(), sizeof(Node), nodes.size(), stdout);
}
