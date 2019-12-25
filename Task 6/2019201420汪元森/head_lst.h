typedef struct Linked_List_Node{
	struct Linked_List_Node *pre , *nxt ;
	int id ;
} Node ;

typedef struct Linked_List{
	Node **st , *nd ;
	int cnt , siz ;
	int *Lpos , *Rpos ;
} LST ;
