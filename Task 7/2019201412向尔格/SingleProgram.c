#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <malloc.h>
#define inf 1e9 + 7

typedef struct NODE {
	int Cnt;
	int Sum;
	int Val;
	struct NODE * Pre;
	struct NODE * Left;
	struct NODE * Right;
} node;

node * Head;

int max (int x, int y) {return x > y ? x : y;}
int min (int x, int y) {return x > y ? y : x;}

bool Isroot (node * Root) {
	if (Root -> Pre == NULL) return true;
	return false;
}

bool Isline (node * Root) {
	return (Root -> Pre -> Left == Root);
}

void Update (node * Root) {
	Root -> Sum = Root -> Cnt;
	if (Root -> Left != NULL) Root -> Sum += Root -> Left -> Sum;
	if (Root -> Right != NULL) Root -> Sum += Root -> Right -> Sum;
}

void Rotate (node * Root) {
	node * father;
	father = Root -> Pre;
	// Case 1 : Left;
	if (father -> Left == Root) {
		if (Root -> Right != NULL){
			Root -> Right -> Pre = father;
			father -> Left = Root -> Right;
			Root -> Right = father;
		}
		else {
			Root -> Right = father;
			father -> Left = NULL;
		}
	}
	// Case 2 : Right
	else {
		if (Root -> Left != NULL) {
			Root -> Left -> Pre = father;
			father -> Right = Root -> Left;
			Root -> Left = father;
		}
		else {
			Root -> Left = father;
			father -> Right = NULL;
		}
	}
	//set father;
	Root -> Pre = father -> Pre;
	father -> Pre = Root;
	//set child
	if (Root -> Pre != NULL) {
		if (Root -> Pre -> Left == father) Root -> Pre -> Left = Root;
		else if (Root -> Pre -> Right == father) Root -> Pre -> Right = Root;
	}

	Update (father);
	Update (Root);
}

void Splay (node * Root) {
	while (!Isroot (Root) ) {
		if (!Isroot (Root -> Pre) && Isline (Root) == Isline (Root -> Pre) ) Rotate (Root -> Pre);
		Rotate (Root);
	}
	
	Head = Root;
}

void Insert (int x, node * Root, node * Last) {
	if (Root == NULL) {
		// set root
		Root = (node *) malloc (sizeof (node));
		Root -> Cnt = 1;
		Root -> Val = x;
		Root -> Pre = Last;
		Root -> Left = NULL, Root -> Right = NULL;
		// set relationship
		if (Last != NULL) {
			if (x < Last -> Val) Last -> Left = Root;
			else Last -> Right = Root;
		}
		// update data
		Update (Root); 
		Splay (Root);
		return ;
	}

	if ( (Root -> Val) == x) {
		++ (Root -> Cnt);
		Update (Root);
		Splay (Root);
		return ;
	}
	else if (x < Root -> Val) Insert (x, Root -> Left, Root);
	else Insert (x, Root -> Right, Root);
}

void Delete (int x, node * Root) {
	if (Root -> Val == x) {
		-- (Root -> Cnt);
		Update (Root);
		Splay (Root);
		if (Root -> Cnt == 0) {
			if(Root -> Left == NULL && Root -> Right == NULL) {
				Head = NULL;
			}
			else if (Root -> Left == NULL || Root -> Right == NULL) {
				if (Root -> Left == NULL) {
					Root -> Right -> Pre = NULL;
					Head = Root -> Right;
				}
				else {
					Root -> Left -> Pre = NULL;
					Head = Root -> Left;
				}
			}
			else {
				node * Temp = Root -> Right;
				while (Temp -> Left != NULL) {
					Temp = Temp -> Left;
				}
				Root -> Right -> Pre = NULL;
				Root -> Left -> Pre = Temp;
				Temp -> Left = Root -> Left;
				Splay (Root -> Left);
			}
			free (Root);
		}
		return ;
	}

	else if (x < Root -> Val) Delete (x, Root -> Left);
	else Delete (x, Root -> Right);
}

int Kth_number (node * Root, int K) {
	int Total = 0;
	if (Root -> Left != NULL) {
		if (Root -> Left -> Sum >= K) return Kth_number (Root -> Left, K);
		Total += Root -> Left -> Sum;
	}

	Total += Root -> Cnt;
	if (Total >= K) return Root -> Val;
	return Kth_number (Root -> Right, K - Total);
}

int Lessthanx (node * Root, int x) {
	if (Root == NULL) return 0;
	int Total = 0;
	if (Root -> Val == x) {
		if (Root -> Left != NULL) Total = Root -> Left -> Sum;
		return Total;
	}
	else if (Root -> Val > x) return Lessthanx (Root -> Left, x);
	else {
		Total = Root -> Cnt;
		if (Root -> Left != NULL) Total += Root -> Left -> Sum;
		return Total + Lessthanx (Root -> Right, x);
	}
}

int Find_pre (node * Root, int x, int Ans) {
	if (Root == NULL) return Ans;
	if (Root -> Val < x) {
		int ans = Ans;
		if (Root -> Cnt != 0) ans = max(ans, Root -> Val);
		return Find_pre (Root -> Right, x, ans);
	}
	else return Find_pre (Root -> Left, x, Ans);
}

int Find_next (node * Root, int x, int Ans) {
	if (Root == NULL) return Ans;
	if (Root -> Val > x) {
		int ans = Ans;
		if (Root -> Cnt != 0) ans = min(ans, Root -> Val);
		return Find_next (Root -> Left, x, ans);
	}
	else return Find_next (Root -> Right, x, Ans);
}

void Free (node * Root) {
	if (Root == NULL) return;
	if (Root -> Left != NULL) Free (Root -> Left);
	if (Root -> Right != NULL) Free (Root -> Right);
	free (Root);
}

int main () {
	int n;
	scanf ("%d", &n);
	Head = NULL;

	while (n --) {
		int op, x;
		scanf ("%d%d", &op, &x);
		if (op == 0) Insert (x, Head, NULL);
		else if (op == 1) Delete (x, Head);
		else if (op == 2) printf ("%d\n", Kth_number (Head, x) );
		else if (op == 3) printf ("%d\n", Lessthanx (Head, x) );
		else if (op == 4) printf ("%d\n", Find_pre(Head, x, -1) );
		else if (op == 5) {
			int ans = Find_next (Head, x, inf);
			if (ans == inf) ans = -1;
			printf ("%d\n", ans);
		}
	}
	Free (Head);

	return 0;
}

