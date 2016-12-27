#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "myVarible.h"

VaribleList *varibles = CreateVaribleList();

FoursHead *Lrparser(WordsHead *pHead){
	Word *pCurrentWord =pHead;
	GetNext(pCurrentWord);
	int tempIndex = 1;
	FoursHead *pFoursHead =CreateFour(NULL,NULL,NULL,NULL,0,-1);
	pFoursHead->next = pFoursHead;
	pFoursHead->prev = pFoursHead;
	bool res = Program(pCurrentWord,pFoursHead);
	if (res){
		if (pCurrentWord->type == 0){
			Argu *end = CreateArgu("#", NULL,false);
			Emit(end, NULL, NULL, NULL, pFoursHead, -1);
			printf("Success!\n");
		}
		else
		{
			printf("缺少结束标志'#'.\nFail.\n"); 
			return false;
		}
	}
	else{
		printf("Fail!\n");
	}
	return pFoursHead;
}

void GetNext(Word *pWord){
	*(pWord) = *(pWord->next);
}
bool Program(Word *pWord, FoursHead *pHead){
	if (pWord->type == 1){
		GetNext(pWord);
		if (pWord->type == 26){
			GetNext(pWord);
			if (pWord->type == 27){
				GetNext(pWord);
				return StatementBlock(pWord,pHead);
			}
			else
			{
				printf("main函数定义错误.\n");
				return false;
			}
		}
		else{
			printf("main函数定义错误.\n");
			return false;
		}
	}
	else{
		printf("缺少程序入口main函数.\n");
		return false;
	}
}
bool StatementBlock(Word *pWord, FoursHead *pHead){
	if (pWord->type == 28){
		GetNext(pWord);
		if (StatementLine(pWord,pHead)){
			if (pWord->type == 29){
				GetNext(pWord);
				return true;
			}
			else{
				printf("语句块缺少'}'.\n");
				return false;
			}
		}
		else{
			return false;
		}
	}
	else{
		printf("语句块缺少 '{'.\n");
		return false;
	}
}
bool StatementLine(Word *pWord, FoursHead *pHead){
	if (Statement(pWord,pHead)){
		if (pWord->type==31){
			GetNext(pWord);
		}
		else
		{
			printf("语句后缺少';'\n");
			return false;
		}
	}
	else{
		return false;
	}
	while (pWord->type!=29){	
		if (Statement(pWord,pHead)){
			if (pWord->type == 31){
				GetNext(pWord);
				continue;
			}
			else{
				printf("语句后缺少';'.\n");
				return  false;
			}

		}
		else{
			return false;
		}
	}
	return true;
}
bool Statement(Word *pWord, FoursHead *pHead){
	if (pWord->type == 10){
		return Assignment(pWord, pHead);
	}
	else if (pWord->type == 6){
		return Conditionment(pWord, pHead);
	}
	else if (pWord->type){
		return Loopment(pWord, pHead);
	}
	printf("未知语句类型.\n");
	return false;
}
bool Assignment(Word *pWord, FoursHead *pHead){
	if (pWord->type ==10){
		Argu *result = NULL;
		Varible *var = FindVarible(varibles,pWord->value);
		if (var){
			result = var->argu;
		}
		else{
			result = CreateArgu(pWord->value, NULL,false);
			InsertVarible(varibles,result);
		}
		GetNext(pWord);
		if (pWord->type == 21){
			GetNext(pWord);
			Argu *argu1=Expression(pWord,pHead);
			if (argu1){
				Emit(result, argu1, "=", NULL, pHead,-1);
				return true;
			}
			else
			{
				return false;
			}
		}
		else{
			printf("赋值表达式缺少'='");
			return false;
		}
	}
	else{
		if (pWord->type == 30){
			printf("语句不得为空.\n");

		}
		else if (pWord->type==21)
		{
			printf("赋值表达式左边不得为空.\n");
		}
		else if (pWord->type==29){
			printf("至少要有一条语句.\n");
		}
		else
		{
			printf("赋值表达式左边必须为标识符.\n");
		}
		return false;
	}
}
bool Conditionment(Word *pWord,FoursHead *pHead){
	GetNext(pWord);
	Four *trueEnter = Condition(pWord, pHead);
	if (trueEnter == NULL){
		return NULL;
	}
	Four *falseEnter = Emit(NULL, NULL, NULL, NULL, pHead, -1);
	trueEnter->goIndex = trueEnter->index + 2;
	if (StatementBlock(pWord, pHead)){
		falseEnter->goIndex =pHead->prev->index + 1;
		if (pWord->type == 7){
			Four *trueOut=Emit(NULL, NULL, NULL, NULL, pHead, -1);
			falseEnter->goIndex = pHead->prev->index + 1;
			GetNext(pWord);
			if (StatementBlock(pWord, pHead)){
				trueOut->goIndex = pHead->prev->index + 1;
				return true;
			}
			else{
				return false;
			}
		}
		else{
			return true;
		}
	}
	else{
		return false;
	}
}

bool Loopment(Word *pWord, FoursHead *pHead){
	GetNext(pWord);
	int loopIndxe = pHead->prev->index + 1;
	bool loop = StatementBlock(pWord, pHead);
	if (!loop){
		return false;
	}
	if (pWord->type == 9){
		GetNext(pWord);
		Four* trueOut = Condition(pWord, pHead);
		trueOut->goIndex = loopIndxe;
		return true;
	}
	else{
		printf("缺少'while'.\n");
		return false;
	}
}

Argu* Expression(Word *pWord, FoursHead *pHead){
	char * op = NULL;
	Argu *pArgu1 = Item(pWord,pHead);
	Argu *result = pArgu1;
	if (pArgu1){
		while (pWord->type == 22 || pWord->type == 23){
			if (pWord->type == 22){
				op = "+";
			}
			else{
				op = "-";
			}
			GetNext(pWord);
			Argu *pArgu2 = Item(pWord, pHead);
			if (pArgu2){
				result = CreateArgu(NewTemp(),NULL,false);
				Emit(result, pArgu1, op, pArgu2, pHead,-1);
				pArgu1 = result;
				continue;
			}
			else
			{
				return NULL;
			}
		}
		return result;
	}
	else{
		return NULL;
	}
}
Argu *Item(Word *pWord, FoursHead *pHead){
	char * op=NULL;
	Argu *pArgu1= Factor(pWord, pHead);
	Argu *result = pArgu1;
	if (pArgu1){
		while (pWord->type==24||pWord->type==25)
		{
			if (pWord->type == 24){
				op = "*";
			}
			else{
				op = "/";
			}
			GetNext(pWord);
			Argu *pArgu2 = Factor(pWord,pHead);
			if (pArgu2){
				result = CreateArgu(NewTemp(),NULL,false);
				Emit(result, pArgu1,op ,pArgu2,pHead,-1);
				pArgu1 = result;
				continue;
			}
			else
			{
				return NULL;
			}
		}
		return result;
	}
	else{
		return NULL;
	}
}

Argu *Factor(Word *pWord, FoursHead *pHead){
	if (pWord->type == 10){
		Varible *var = FindVarible(varibles,pWord->value);
		if (var){
			GetNext(pWord);
			return var->argu;
		}
		else
		{	
			Argu *result = CreateArgu(pWord->value, NULL,false);
			InsertVarible(varibles, result);
			GetNext(pWord);
			return result;
			/*
			printf("变量未定义先使用:%s.\n",pWord->value);
			return NULL;
			*/

		}
		//Argu *pArgu = CreateArgu(pWord->value, NULL);
		//GetNext(pWord);
		//return pArgu;
	}
	if (pWord->type == 20){
		Argu *pArgu = CreateArgu(pWord->value, pWord->num,true);
		GetNext(pWord);
		return pArgu;
	}
	if (pWord->type == 26){
		GetNext(pWord);
		Argu *pArgu = Expression(pWord, pHead);
		if (pArgu){
			if (pWord->type == 27){
				GetNext(pWord);
				return pArgu;
			}
			else{
				printf("缺少')'.\n");
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		printf("表达式右值因子错误.\n");
		return NULL;
	}
}
Four *Emit(Argu *result,Argu *pArgu1,char *op,Argu *pArgu2,FoursHead *pHead,int goIndex){
	Four *last = pHead->prev;
	Four *newFour = CreateFour(result,pArgu1,op,pArgu2,last->index+1,goIndex);
	last->next = newFour;
	newFour->prev = last;
	newFour->next = pHead;
	pHead->prev = newFour;
	return newFour;
}
Argu *CreateArgu(char *key, double num,bool ifDigit){
	Argu* pArgu = (Argu *)malloc(sizeof(Argu));
	pArgu->key = key;
	pArgu->num = num;
	pArgu->ifDigit = ifDigit;
	return pArgu;
}
Four *CreateFour(Argu* result, Argu *pArgu1, char *op, Argu *pArgu2,int index,int goIndex){
	Four* pFour = (Four *)malloc(sizeof(Four));
	pFour->result = result;
	pFour->argu1 = pArgu1;
	pFour->op = op;
	pFour->argu2 = pArgu2;
	pFour->next = NULL;
	pFour->prev = NULL;
	pFour->index = index;
	pFour->goIndex = goIndex;
	return pFour;
}

char* NewTemp(){
	static int index = 1;
	char* temp = (char*)malloc(sizeof(char)* 32);
	_itoa(index, temp+1,10);
	*temp = 'T';
	index += 1;
	return temp;
}
Four *Condition(Word *pWord,FoursHead *pHead){
	Argu *left = Expression(pWord, pHead);
	if (left == NULL){
		return NULL;
	}
	WordType relation = RelationOp(pWord, pHead);
	Argu *right = Expression(pWord, pHead);
	if (right == NULL){
		return NULL;
	}
	switch (relation)
	{
	case 32:
		return Emit(NULL, left, ">", right, pHead, -1);
	case 33:
		return Emit(NULL, left, ">=", right, pHead, -1);
	case 34:
		return Emit(NULL, left, "<", right, pHead, -1);
	case 35:
		return Emit(NULL, left, "<=", right, pHead, -1);
	case 36:
		return Emit(NULL, left, "==", right, pHead, -1);
	case 37:
		return Emit(NULL, left, "!=", right, pHead, -1);
	default:
		return NULL;
		break;
	}
}
WordType RelationOp(Word *pWord,FoursHead *pHead){
	WordType res;
	switch (pWord->type)
	{
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
		res = pWord->type;
		GetNext(pWord);
		return res;
		break;
	default:
		return NULL;
		break;
	}
}
void PrintFours(FoursHead *pHead){
	Four *pCurrent = pHead->next;
	while (pCurrent != pHead){
		if (pCurrent->goIndex != -1){

			if (pCurrent->op != NULL){
				if (pCurrent->argu1->ifDigit &&pCurrent->argu2->ifDigit){
					printf("%3d: if %g %s %g goto %d\n", pCurrent->index, pCurrent->argu1->num, pCurrent->op, pCurrent->argu2->num, pCurrent->goIndex);
				}
				else if (pCurrent->argu1->ifDigit){
					printf("%3d: if %g %s %s goto %d\n", pCurrent->index, pCurrent->argu1->num, pCurrent->op, pCurrent->argu2->key, pCurrent->goIndex);
				}
				else if(pCurrent->argu2->ifDigit){
					printf("%3d: if %s %s %g goto %d\n", pCurrent->index, pCurrent->argu1->key, pCurrent->op, pCurrent->argu2->num, pCurrent->goIndex);
				}
				else{
					printf("%3d: if %s %s %s goto %d\n", pCurrent->index, pCurrent->argu1->key, pCurrent->op, pCurrent->argu2->key, pCurrent->goIndex);
				}
			}
			else{
				printf("%3d: goto %2d\n",pCurrent->index, pCurrent->goIndex);
			}
		}
		else{
			if (!strcmp(pCurrent->result->key, "#")){
				printf("%3d: #\n", pCurrent->index);
			}
			else if (!strcmp("=", pCurrent->op)){
				if (pCurrent->result->ifDigit &&pCurrent->argu1->ifDigit){
					printf("%3d: %g %s %g \n", pCurrent->index, pCurrent->result->num, pCurrent->op, pCurrent->argu1->num);
				}
				else if (pCurrent->result->ifDigit){
					printf("%3d: %g %s %s \n", pCurrent->index, pCurrent->result->num, pCurrent->op, pCurrent->argu1->key);
				}
				else if(pCurrent->argu1->ifDigit){
					printf("%3d: %s %s %g \n", pCurrent->index, pCurrent->result->key, pCurrent->op, pCurrent->argu1->num);
				}
				else{
					printf("%3d: %s %s %s \n", pCurrent->index, pCurrent->result->key, pCurrent->op, pCurrent->argu1->key);
				}
			}
			else{
				if (pCurrent->argu1->ifDigit &&pCurrent->argu2->ifDigit){
					printf("%3d: %s = %g %s %g \n", pCurrent->index, pCurrent->result->key, pCurrent->argu1->num, pCurrent->op, pCurrent->argu2->num);
				}
				else if (pCurrent->argu1->ifDigit){
					printf("%3d: %s = %g %s %s \n", pCurrent->index, pCurrent->result->key, pCurrent->argu1->num, pCurrent->op, pCurrent->argu2->key);
				}
				else if(pCurrent->argu2->ifDigit){
					printf("%3d: %s = %s %s %g \n", pCurrent->index, pCurrent->result->key, pCurrent->argu1->key, pCurrent->op, pCurrent->argu2->num);
				}
				else{
					printf("%3d: %s = %s %s %s \n", pCurrent->index, pCurrent->result->key, pCurrent->argu1->key, pCurrent->op, pCurrent->argu2->key);
				}
			}
		}
		pCurrent = pCurrent->next;
	}
};