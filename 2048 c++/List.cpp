#include "List.h"



List::List()
{
}


List::~List()
{
}
void List::init() //init list
{
	List::next = NULL;
}

void List::add_after(char *val) //adding new record
{
	List *x = new List;
	strcpy(x->value,val);
	x->next = List::next;
	List::next = x;
}

void List::remove_after() // Deletes after
{
	if (List::next != NULL) {
		void* x = List::next;
		List::next = List::next->next;
		delete x;
	}
}

void all_file_names(const char *file_name)
{
	FILE *file;
	file = fopen("save_names.txt", "a");
	if (file != NULL)
	{
		fprintf(file, "\n%s", file_name);
		fclose(file);
	}
}
int get_existing_saves(List *list)
{
	int n = 0;
	FILE *file;
	file = fopen("save_names.txt", "r");
	char name[120];
	if (file != NULL)
	{
		while (fscanf(file, "%s", name) != EOF)
		{
			n++;
			list->add_after(name);
		}
		fclose(file);
	}
	return n;
}
char* get_nth_val(List *list, int n)
{
	int i = 0;
	list = list->next;
	while (list != NULL)
	{
		//if (strlen(list->value) > 0)
		{
			if (i == n)
			{
				return list->value;
			}
			i++;
		}
		list = list->next;
	}
	return "0";
}