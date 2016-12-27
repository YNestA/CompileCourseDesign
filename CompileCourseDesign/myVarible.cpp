#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myVarible.h"
Varible *CreateVarible(Argu *argu, Varible *prev, Varible *next){
	Varible *varible = (Varible*)malloc(sizeof(Varible));
	varible->argu = argu;
	varible->prev = prev;
	varible->next = next;
	return varible;
}
VaribleList *CreateVaribleList(){
	VaribleList *varibles = CreateVarible(NULL,NULL,NULL);
	varibles->next = varibles;
	varibles->prev = varibles;
	return varibles;
}
void InsertVarible(VaribleList *varibles, Argu *argu){
	Varible *last = varibles->prev;
	Varible *newVar = CreateVarible(argu,NULL,NULL);
	last->next = newVar;
	newVar->prev = last;
	newVar->next = varibles;
	varibles->prev = newVar;
}
Varible *FindVarible(VaribleList *varibles,char *key){
	Varible *var = varibles->next;
	while (var != varibles){
		if (!strcmp(var->argu->key, key)){
			return var;
		}
		var = var->next;
	}
	return NULL;
}