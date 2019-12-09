#include <stdio.h>
#include <stdlib.h>
#include "data.h"

void* newNode(size_t size);

struct Data *newData(int id) {
	struct Data *data=newNode(sizeof(struct Data));
	data->id=id;
	return data;
}