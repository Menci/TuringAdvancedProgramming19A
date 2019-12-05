#ifndef _LINK_H
#define _LINK_H

struct Link{
	int id;
	struct Link *next;
	struct Link *last;
};

struct Link *clear(struct Link *p);
void print_link(int siz, struct Link *Head);
void add(int key, struct Link *Now);
void del(struct Link *p, struct Link *Head);

#endif
