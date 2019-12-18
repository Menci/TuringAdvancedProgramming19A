#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <stdbool.h>

typedef struct NODE {
	int Cnt;
	int Sum;
	int Val;
	struct NODE * Pre;
	struct NODE * Left;
	struct NODE * Right;
} node;

int max (int x, int y);
int min (int x, int y);

node * Head;

bool Isroot (node * Root);

bool Isline (node * Root);

void Update (node * Root);

void Rotate (node * Root) ;

void Insert (int x, node * Root, node * Last) ;

void Delete (int x, node * Root) ;

int Kth_number (node * Root, int K);

int Lessthanx (node * Root, int x);

int Find_pre (node * Root, int x, int Ans);

int Find_next (node * Root, int x, int Ans);

void Free (node * Root);
