# Task 8
[Chinese](README.zh.md) | English

The serialization and compression of a binary tree.

Given a binary tree with `K` bytes data on each node, save the tree's form and data to a compressed file and restore the tree from the file.

The structure of tree's node is (using 0-based index):

```cpp
#define K ???

struct Node {
    int left_child, right_child;
    char data[K];
} nodes[]; 
```

Read the nodes' information from the file `input.bin`. You can calculate the count of nodes from the file size and read all nodes at once with `fread`.

Write the compressed data to `zipped.bin` file. We call the ratio of the size of `zippped.bin` to `input.bin` your *compression ratio*.  
You need to read the file again and restore the tree to check your algorithm's correctness.

Limits: the count of nodes `>= 100000`, the size of data on each node `>= 4`.

# Grading
* **Code Style**: Perfect, Great, Good, Bad.
* **Correctness**: Yes, No.
* **Algorithm**

# Deadline
Your Pull Request should be created before December 16, 2019 23:00 (CST).
