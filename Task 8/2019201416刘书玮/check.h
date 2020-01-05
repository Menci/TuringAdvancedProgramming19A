#define K 4
struct huffman_node{
	struct huffman_node *left, *right;
	int count;
	int ord;
	int code, len;
};
struct pair{
	int first;
	int second;
};
struct tree{
	int left,right;
	char bas[K];
};
extern int deinput(char *, int);
extern int input(char *, int, int);
extern struct pair debracket(char*, struct tree*, struct pair);
extern struct pair huffman_dfs(char*, char*, struct huffman_node*, struct pair);
extern struct pair huffman_decode(char *, struct huffman_node *, struct pair);
extern int tree_dfs(char*, struct tree*, int, int);
extern void deoutput(char*, int);
