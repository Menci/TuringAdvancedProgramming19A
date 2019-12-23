#include "serialize.h"

#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../bitset/bitset.h"
#include "../huffman/huffman.h"
#include "../node.h"

void _serialize_subtree(node_t *root,
						int i,
                        bitstream_t bitstream,
                        char **p_data) {
    if (root[i].left_child != -1) {
        bitstream_write(bitstream, 1);
        _serialize_subtree(root, root[i].left_child, bitstream, p_data);
    } else {
        bitstream_write(bitstream, 0);
    }

	memcpy(*p_data, root[i].data, K);
	*p_data += K;

    if (root[i].right_child != -1) {
        bitstream_write(bitstream, 1);
        _serialize_subtree(root, root[i].right_child, bitstream, p_data);
    } else {
        bitstream_write(bitstream, 0);
    }
}

bitset_t serialize(node_t *tree) {
    size_t tree_size = N;

    // It's not easy to calculate huffman encoded data's size.
    const size_t ENOUGH_SIZE = 1024UL * 1024 * 10 * 8 * 10;
    bitset_t bitset = bitset_create(ENOUGH_SIZE);
    bitstream_t bitstream = bitstream_create(bitset, 0);
    
    bitstream_write_data(bitstream, &tree_size, sizeof(size_t));

    char *array = malloc(K * tree_size), *p_data = array;

    // Use DFS sequence to serialize the tree structure.
    // 0: No such child, 1: Enter a child. After boths childs return to parent.
    _serialize_subtree(tree, 0, bitstream, &p_data);

    void *encoded;
    size_t encoded_length;
    huffman_encode(array, K * tree_size, &encoded, &encoded_length);
    bitstream_write_data(bitstream, &encoded_length, sizeof(size_t));
    bitstream_write_data(bitstream, encoded, encoded_length);

    bitstream_close(bitstream);
    bitstream_destroy(bitstream);

    free(encoded);
    free(array);

    return bitset;
}
