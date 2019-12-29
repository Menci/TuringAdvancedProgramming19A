#define K 4
#define N 300000

#include "tree.h"

#include <cstdio>

unsigned rdtsc() {
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return hi ^ lo;
}

int main() {
	build();
	validate();
	for (size_t i = 0; i < N; i++) *(unsigned *)(&nodes[i].data) = rdtsc();
	FILE *fp = fopen("zipped.bin", "wb");
	fwrite(nodes.data(), sizeof(Node), nodes.size(), fp);
}
