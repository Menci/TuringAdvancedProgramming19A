#define K 4

struct huffman_node{
	struct huffman_node *left, *right;
	int count;
};
struct tree{
	struct tree *left, *right;
	char bas[K];
};
struct pair{
	int first;
	int second;
};
extern struct huffman_node* huffman_code(struct huffman_node*, char*, char*, int);
extern int huffman_dfs(int, int, int, struct huffman_node*, char*);
extern void input(struct tree *, int, int);
extern void output(char *, int);
extern struct pair bracket_dfs(struct tree*, int, char*, char*, int, int, int);
extern int compress(int, struct huffman_node*, char*, char*, int);
