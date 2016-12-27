#include "lex.h"

struct Argu{
	char* key;
	double num;
	bool ifDigit;
};

struct Four
{
	Argu *result;
	Argu *argu1;
	char *op;
	Argu *argu2;
	int index;
	Four *next;
	Four *prev;
	int goIndex;
};
typedef Four FoursHead;

Four *Emit(Argu* result,Argu *pArgu1,char *op,Argu *pArgu2,FoursHead *pHead,int goIndex);
char* NewTemp();
Four *CreateFour(Argu *result,Argu *pArgu1,char *op,Argu *pArgu2,int index,int goIndex);
Argu *CreateArgu(char *key, double num,bool ifDigit);
FoursHead *Lrparser(WordsHead *pHead);
void GetNext(Word *pWord);
bool Program(Word *pWord, FoursHead *pHead);
bool StatementBlock(Word *pWord, FoursHead *pHead);
bool StatementLine(Word *pWord, FoursHead *pHead);
bool Statement(Word *pWord, FoursHead *pHead);
bool Conditionment(Word *pWord, FoursHead *pHead);
bool Loopment(Word *pWord, FoursHead *pHead);
bool Assignment(Word *pWord, FoursHead *pHead);
Argu* Expression(Word *pWord, FoursHead *pHead);
Argu* Item(Word *pWord, FoursHead *pHead);
Argu* Factor(Word *pWord, FoursHead *pHead);
Four *Condition(Word *pWord,FoursHead *pHead);
WordType RelationOp(Word *pWord, FoursHead *pHead);
void PrintFours(FoursHead *pHead);