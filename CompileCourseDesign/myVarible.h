#include "lrparser.h"

struct Varible{
	Argu *argu;
	Varible *prev;
	Varible *next;
};
typedef Varible VaribleList;
Varible *CreateVarible(Argu *argu, Varible *prev, Varible *next);
VaribleList *CreateVaribleList();
void InsertVarible(VaribleList *varibles, Argu *argu);
Varible *FindVarible(VaribleList *varibles,char *key);