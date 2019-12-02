struct linkedList {
	struct linkedList *nxt,*pre;
	struct Data *data;
	struct linkedList *(*add)(struct linkedList *pos,struct Data *data);
	struct linkedList *(*delete)(struct linkedList *pos);
};