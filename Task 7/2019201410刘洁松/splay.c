#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "splay.h"

int min(int operant_1, int operant_2) {
    if(operant_1 < operant_2)
        return operant_1;
    else
        return operant_2;
}

int max(int operant_1, int operant_2) {
    if(operant_1 > operant_2)
        return operant_1;
    else
        return operant_2;
}

int count(node *operant) {
    return operant -> cnt;
}

void maintain(node *operant) {

    operant -> size = operant -> cnt;

    if(operant -> left != NULL) {
        operant -> size += operant -> left -> size;
    }
    if(operant -> right != NULL) {
        operant -> size += operant -> right -> size;
    }
}

bool is_root(node *operant) {
    if(operant -> pre == NULL)
        return true;
    else
        return false;
}

bool left_node(node *operant) {
    if(operant == operant -> pre -> left)
        return true;
    else
        return false;
}


void rotate(node *operant) {
	node *father = operant -> pre;
	// node *grand_father = father -> pre;
	int Is_left_node = left_node(operant);

	if(father -> pre != NULL) {
		node *grand_father = father -> pre;
		if(left_node(father))
			grand_father -> left = operant;
		else 
			grand_father -> right = operant;
	}

	operant -> pre = father -> pre;
	father -> pre = operant;

	//fprintf(stderr, "left side %d\n", left_node(operant));
	if(Is_left_node) {
		if(operant -> right != NULL)
			operant -> right -> pre = father;
		father -> left = operant -> right;
		operant -> right = father;
	}
	else {
		if(operant -> left != NULL)
			operant -> left -> pre = father;
		father -> right = operant -> left;
		operant -> left = father;
	}
	maintain(father);
	maintain(operant);
}

void splay(node *operant) {
	while(!is_root(operant)) {
		if(!is_root(operant -> pre) 
				&& left_node(operant) == left_node(operant -> pre) ) {
			rotate(operant -> pre);

		}

		//fprintf(stderr, "splaying enter ? now = %d\n", operant -> value);
		rotate(operant);
		//printf("%d rotate successfully!\n", operant -> value);
	}
	Head = operant;
}


void Insert(int operant, node *current, node *father) {
    if(current == NULL) {

        current = (node *)malloc( sizeof(node) );
        current -> value = operant;
        //printf("%d\n", operant);
        current -> cnt = current -> size = 1;
        current -> left = current -> right = NULL;
        current -> pre = father;
        
        if(father != NULL) {
            int value = father -> value;
            if(operant < value)
                father -> left = current;
            else
                father -> right = current;

        }
        // fprintf(stderr, "left side %d\n", left_node(current));
        maintain(current);
        // fprintf(stderr, "now= %d\n", current -> value);
        splay(current);
        // fprintf(stderr,"*");
        //printf("Insert %d completed!\n", operant);
        return;
    }


    int value = current -> value;
    if(operant == value) {
        (current -> cnt) ++;
        maintain(current);
        splay(current);
        //printf("Insert %d completed!\n", operant);
        return;
    }

    if(operant < value) {
        Insert(operant, current -> left, current);
    }

    if(operant > value) {
    //    fprintf(stderr," operant = %d value = %d\n", operant, value);
        Insert(operant, current -> right, current);
    }
}

void delete_node(node *current) {
    if(current -> left == NULL && current -> right == NULL) {
        current = NULL;
        Head = NULL;
    }
    else if(current -> left == NULL || current -> right == NULL) {
        if(current -> left == NULL) {
            current -> right -> pre = NULL;
            Head = current -> right;
        }
        else {
            current -> left -> pre = NULL;
            Head = current -> left;
        }
    }
    else {
        node *next = current -> right;
        while(next -> left != NULL)
            next = next -> left;
        current -> right -> pre = NULL;
        current -> left -> pre = next;
        next -> left = current -> left;
        splay(current -> left);
    }

    free(current);
}

void delete(int operant, node *current) {
    int value = current -> value;
    if(operant == value) {
        (current -> cnt) --;
        splay(current);
        if(current -> cnt == 0)
            delete_node(current);
        //printf("delete %d completed!\n", operant);
        return;
    }
    else if(operant < value) {
        delete(operant, current -> left);
    }
    else
        delete(operant, current -> right);
}


int find_kth(int operant, node *current) {
    int total_left = 0;
    if(current -> left != NULL)
        total_left = current -> left -> size;
    if(total_left < operant && total_left + count(current) >= operant)
        return current -> value;
    else if(total_left >= operant)
        return find_kth(operant, current -> left);
    else
        return find_kth(operant - total_left - count(current), current -> right);
}

int find_total_min(int operant, node *current) {
    if(current == NULL) return 0;

    int value = current -> value;
    if(value >= operant) {
        return find_total_min(operant, current -> left);
    }
    else {
        int total_left = 0;
        if(current -> left) total_left = current -> left -> size;
        //printf("total = %d\n", count(current));
        return total_left + count(current) + find_total_min(operant, current -> right);
    }
}

int find_min(int operant, node *current) {
    if(current == NULL) return -1;
    
    int value = current -> value;
    if(value < operant) {
        return max(value, find_min(operant, current -> right));
    }
    else{
        return find_min(operant, current -> left);
    }
}

int find_max(int operant, node *current) {
    if(current == NULL) return -1;

    int value = current -> value;
    if(value > operant) {
        int max_left = find_max(operant, current -> left);
        if(max_left != -1) value = min(value, max_left);
        return value;
    }
    else
        return find_max(operant, current -> right);
}
