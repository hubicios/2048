#pragma once
#ifndef LIST_H
#define LIST_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
class List
{
public:
	//int value;
	char value[120]="";
	class List *next;
	void init();
	void add_after(char *val);
	void remove_after();
	List();
	~List();
};
void all_file_names(const char *file_name);
int get_existing_saves(List *list);
char *get_nth_val(List *list, int n);
#endif /* LIST_H */

