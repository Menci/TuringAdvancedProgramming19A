#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct _dll_node{
	struct _dll_node *_next, *_pren;
	int _value;
};

struct _dll_node *_head = NULL, *_tail = NULL;

struct _dll_node* _dll_build(int _ned)
{
	//build a list beforehand
	//only call once
	//get starting address of list
	
	struct _dll_node *_new = malloc(sizeof(struct _dll_node));
	
	if(_head == NULL) _head = _tail = _new;
	
	(*_new)._pren  = _tail;
	(*_new)._next  = _head;
	(*_tail)._next = _new;
	(*_head)._pren = _new;
	(*_new)._value = _ned;
	_tail = _new;
	
	return _head;
}

struct _dll_node* _dll_find(struct _dll_node *_start, int _count)
{
	//count several times in list from start
	//get final address
	
	struct _dll_node *_pla = _start;
	while(_count)
	{
		_count --;
		_pla = (*_pla)._next;
	}
	
	return _pla;
}

struct _dll_node* _reverse_dll_find(struct _dll_node *_start, int _count)
{
	//count several times in list from start
	//get final address
	
	struct _dll_node *_pla = _start;
	
	while(_count)
	{
		_count --;
		_pla = (*_pla)._pren;
	}
	
	return _pla;
}

void _dll_delete(struct _dll_node *_pla)
{
	// delete the appointed node in list
	// get next address
	
	if(_head == _pla) _head = (*_pla)._next;
	if(_tail == _pla) _tail = (*_pla)._pren;
	
	(*((*_pla)._next))._pren = (*_pla)._pren;
	(*((*_pla)._pren))._next = (*_pla)._next;
	
	return;
}

int _dll_getv(struct _dll_node *_pla)
{
	//get the value saved in appointed address
	//I don't know how to extern a struct !!!
	
	return (*_pla)._value;
}
