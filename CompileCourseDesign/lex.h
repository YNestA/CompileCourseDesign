#define KEYWORD_END "Keyword End"
#define  IDENTIFIER_NUM 10
#define ZERO 0.000000000000

extern char* keywords[];
typedef int WordType;
struct Word{
	WordType type;
	char * value;
	double num;
	Word* prev;
	Word* next;
};
typedef Word  WordsHead;

WordsHead* Lex(char * string);
Word* ReturnWord(WordType type, char * Value, double num);
Word* Scaner(char * string, int * pIndex, Word * pPrevWord);
void Getch(char * pCh, char * string, int * pIndex);
void Getbc(char * pCh, char * string, int * pIndex);
void Concat(char ch, char * token);
bool Letter(char ch);
bool Digit(char ch);
WordType Reserve(char * token);
void Retract(int * pIndex);
double Dtb(char * token);
Word* DealDigit(char *pCh, char * string, char *token, int * pIndex);
Word* DealE(char* pCh, char * string, char *token, int *pIndex);
//char* NumT2(double num);
char* NumT2(char* token);
