#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bitset/bitset.h"
#include "serialization/serialize.h"
#include "huffman/huffman.h"
#include "node.h"

int main() {
	node_t *tree;
	tree = malloc(sizeof(node_t) * N);
	fread(tree, sizeof(node_t), N, stdin);

    fputs("Serializing.\n", stderr);
    bitset_t bitset = serialize(tree);

    fprintf(stderr, "Serialized to %d bytes.\n", (int)bitset_sizeof(bitset));

    fprintf(stderr, "Writing %d bytes.\n", (int)bitset_sizeof(bitset));
    fwrite(bitset, bitset_sizeof(bitset), 1, stdout);
}
