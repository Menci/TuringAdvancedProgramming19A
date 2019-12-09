## TASK 7
You need to use GNU make to complie my code:
```sh
	'$ cd "Task 7/2019201422李林豇/"'
	'$ make'
```
And now you have got a dynamic library named "libtreap.so" which provides enables you to use Treap.

I advise you to use GNU make to run the code(unless you know what you are doing):
```sh
	'$ make run'
```



#### There are something you should know if you want to run your own programe:

Treap in libtreap.so supports any data types, but you must design a function "int comp(const void*, const void*)".
The usage of the function "comp" is just like that in qsort.

The libtreap.so provide you some APIs to use Treap:
```c
Treap* Treap_build(cmp_pointer, int): 
```
	first argument is the pointer of your "comp", the second one is lenth of the data types you want the Treap to sort.
	it will build a new Treap with and return the pointer of the Treap.
	(you shouldn't use a Treap which is not built by Treap_build!)
```c
void Treap_insert(Treap*, void*): 
```
	first argument is the pointer of a Treap, the second one is the pointer of the element you want to add into the Treap.
	it will add the element into the Treap.
```c
bool Treap_delete(Treap*, void*):
```
	first argument is the pointer of a Treap, the second one is the pointer of the element you want to remove from the Treap.
	it while remove the element from the Treap.
```c
void* Treap_find_element(Treap*, int):
```
	first argument is the pointer of a Treap, the second one is the rank of the element you want to get.
	it will return the pointer of the element.
```c
int Treap_find_rnk(Treap*, void*):
```
	first argument is the pointer of a Treap, the second one is the pointer of a element.
	it will return the number of the elements in the Treap which is strictly smaller than the input element.
```c
void* Treap_find_pre(Treap*, void*):
```
	first argument is the pointer of a Treap, the second one is the pointer of a element.
	it will return the pointer of the largest element in the Treap which is strictly smaller than the input element. if not found, it will return NULL.
```c
void* Treap_find_nxt(Treap*, void*):
```
	first argument is the pointer of a Treap, the second one is the pointer of a element.
	it will return the pointer of the smallest element in the Treap which is strictly larger than the input element. if not found, it will return NULL.
```c
void* Treap_free(Treap *):
```
	first argument is the pointer of a Treap.
	it will free all the memory that the Treap uses, including the Treap itself.
