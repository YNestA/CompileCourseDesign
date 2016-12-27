#include <stdio.h>
#include "lex.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <cmath>

char* keywords[] = { "main", "int", "float", "double", "char", "if", "else", "do", "while", KEYWORD_END };

WordsHead* Lex(char * string){
	int length = strlen(string),
		index = 0;
	WordsHead *pHead = (WordsHead*)malloc(sizeof(Word));
	Word *pCurrentNode = pHead, *pTempNode, *pPrevWord = NULL;
	while (index < length){
		pTempNode = Scaner(string, &index, pPrevWord);
		if (pTempNode != NULL){
			pCurrentNode->next = pTempNode;
			pTempNode->prev = pCurrentNode;
			pCurrentNode = pTempNode;
			pPrevWord = pCurrentNode;
			//打印结果
			WordType type = pCurrentNode->type;
			char *value = (pCurrentNode->value == NULL) ? " " : pCurrentNode->value;
			if (pCurrentNode->type == 20){
				printf("(%d,%g)\n", type, pCurrentNode->num);
			}
			else{
				printf("(%d,%s)\n", type, value);
			}
		}
	}
	return pHead;
}
Word* ReturnWord(WordType type, char * Value, double num){
	Word* pWord = (Word*)malloc(sizeof(Word));
	pWord->type = type;
	pWord->value = Value;
	pWord->num = num;
	pWord->prev = NULL;
	pWord->next = NULL;
	return pWord;
}
Word* Scaner(char * string, int * pIndex, Word * pPrevWord){
	char ch;
	char *token = (char*)malloc(sizeof(char));
	token[0] = '\0';
	Getch(&ch, string, pIndex);
	Getbc(&ch, string, pIndex);
	if (Letter(ch)){
		while (Letter(ch) || Digit(ch)){
			Concat(ch, token);
			Getch(&ch, string, pIndex);
		}
		Retract(pIndex);
		WordType num = Reserve(token);
		return ReturnWord(num, token, NULL);
	}
	else if ((ch == '+') || (ch == '-')){
		Concat(ch, token);
		Getch(&ch, string, pIndex);
		if (Digit(ch) && ((pPrevWord == NULL) || pPrevWord->type<10 || (pPrevWord->type >= 21 && pPrevWord->type != 27))){
			Word *res = DealDigit(&ch, string, token, pIndex);
			if (res == NULL) return NULL;
			/*
			if (res->num == NULL){
				printf("数值溢出: %s\n", token);
				return NULL;
			}
			*/
			return res;
		}
		else{
			Retract(pIndex);
			WordType num = (token[0] == '+') ? 22 : 23;
			char *temp = (token[0] == '+') ? "+" : "-";
			return ReturnWord(num, temp, NULL);
		}
	}
	else if (Digit(ch)){
		Word *res = DealDigit(&ch, string, token, pIndex);
		if (res == NULL) return NULL;
		/*
		if (res->num == NULL){
			printf("数值溢出: %s\n", token);
			return NULL;
		}
		*/
		return res;;
	}
	else{
		switch (ch)
		{
		case '=':
			Getch(&ch, string, pIndex);
			if (ch == '='){
				return ReturnWord(36, "==", NULL);
			}
			else{
				Retract(pIndex);
				return ReturnWord(21, "=", NULL);
			}
			break;
		case '*':return ReturnWord(24, "*", NULL); break;
		case '/':
			Getch(&ch, string, pIndex);
			if (ch != '*'){
				Retract(pIndex);
				return ReturnWord(25, "/", NULL); break;
			}
			else{
				while (true){
					Getch(&ch, string, pIndex);
					while (ch != '*'){
						Getch(&ch, string, pIndex);
						if (ch == NULL){
							return NULL;
						}
					}
					Getch(&ch, string, pIndex);
					if (ch == '/'){
						return NULL;
					}
				}
			}
			break;
		case '(':return ReturnWord(26, "(", NULL); break;
		case ')':return ReturnWord(27, ")", NULL); break;
		case '{':return ReturnWord(28, "{", NULL); break;
		case '}':return ReturnWord(29, "}", NULL); break;
		case ',':return ReturnWord(30, ",", NULL); break;
		case ';':return ReturnWord(31, ";", NULL); break;
		case '>':
			Getch(&ch, string, pIndex);
			if (ch == '='){
				return ReturnWord(33, ">=", NULL);
			}
			else{
				Retract(pIndex);
				return ReturnWord(32, ">", NULL);
			}
			break;
		case '<':
			Getch(&ch, string, pIndex);
			if (ch == '='){
				return ReturnWord(35, "<=", NULL);
			}
			else{
				Retract(pIndex);
				return ReturnWord(34, "<", NULL);
			}
			break;
		case '!':
			Getch(&ch, string, pIndex);
			if (ch == '='){
				return ReturnWord(37, "!=", NULL);
			}
			else{
				Retract(pIndex);
				printf("不能识别的字符:%c\n", '!');
				return NULL;
			}
			break;
		case '#':return ReturnWord(0, "#", NULL); break;
		default:
			printf("不能识别的字符: %c\n", ch);
			break;
		}
	}
	return NULL;
}
void Getch(char * pCh, char * string, int * pIndex){
	if (*pIndex == strlen(string) - 1){
		*pCh = NULL;
	}
	*pCh = *(string + *pIndex);
	*pIndex += 1;
}
void Getbc(char * pCh, char * string, int * pIndex){
	while (*pCh == ' ' || *pCh == '\t' || *pCh == '\n'){
		Getch(pCh, string, pIndex);
	}
}
void Concat(char ch, char * token){
	int newSize = strlen(token) + 2;
	char *pNewToken = (char *)realloc(token, newSize*sizeof(char));
	if (!pNewToken){
		// Error
	}
	token = pNewToken;
	*(token + newSize - 2) = ch;
	*(token + newSize - 1) = '\0';
}
bool Letter(char ch){
	return ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'));
}
bool Digit(char ch){
	return (ch >= '0') && (ch <= '9');
}
WordType Reserve(char * token){
	int index = 0;
	while (strcmp(KEYWORD_END, keywords[index])){
		if (!strcmp(token, keywords[index])){
			return index + 1;
		}
		else{
			index += 1;
		}
	}
	return IDENTIFIER_NUM;
}
void Retract(int * pIndex){
	*pIndex -= 1;
}
double Dtb(char * token){
	int tokenLength = strlen(token);
	double res;
	int	zf = 1, ezf = 1;
	bool e = false;
	int num[3] = { 0, NULL, NULL },
		index = 0;

	for (int i = 0; i < tokenLength; i++){
		if ((i == 0) && (token[0] == '+')){
			continue;
		}
		if ((i == 0) && (token[0] == '-')){
			zf = -1;
			continue;
		}
		if (token[i] == 'e'){
			e = true;
			num[2] = 0;
			for (int j = i + 1; j < tokenLength; j++){
				if ((j == (i + 1)) && token[j] == '+'){
					continue;
				}
				if ((j == (i + 1)) && token[j] == '-'){
					ezf = -1;
					continue;
				}
				num[2] = num[2] * 10 + token[j] - '0';
			}
			break;
		}
		if (token[i] == '.'){
			index = 1;
			num[index] = 0;
		}
		else{
			num[index] = num[index] * 10 + token[i] - '0';
		}
	}
	res = (double)zf*num[0];
	if (num[1] != NULL){
		double temp = (double)num[1];
		while (temp > 1){
			temp /= 10;
		}
		res += temp;
	}

	if (num[2] != NULL){
		double temp = num[2];
		res *= pow((double)10, temp*ezf);
	}
	return res;
}

char* NumT2(char *token){
	return token;
}
Word* DealDigit(char *pCh, char * string, char *token, int * pIndex){
	double num;
	while (Digit(*pCh)){
		Concat(*pCh, token);
		Getch(pCh, string, pIndex);
	}
	if (*pCh == '.'){
		Concat(*pCh, token);
		Getch(pCh, string, pIndex);
		if (!Digit(*pCh)){
			Retract(pIndex);
			//Error()
			printf("浮点数格式错误: %s \n", token);
			return NULL;
		}
		else{
			Concat(*pCh, token);
			Getch(pCh, string, pIndex);
		}
		while (Digit(*pCh)){
			Concat(*pCh, token);
			Getch(pCh, string, pIndex);
		}
		if (*pCh == 'e'){
			return DealE(pCh, string, token, pIndex);
		}
		else{
			Retract(pIndex);
			double num = Dtb(token);
			return ReturnWord(20, NumT2(token), num);
		}
	}
	else if (*pCh == 'e')
	{
		return DealE(pCh, string, token, pIndex);
	}
	else{
		Retract(pIndex);
		double num = Dtb(token);
		return ReturnWord(20, NumT2(token), num);
	}
}
Word* DealE(char* pCh, char * string, char *token, int *pIndex){
	Concat(*pCh, token);
	Getch(pCh, string, pIndex);
	if ((*pCh != '+') && (*pCh != '-') && !Digit(*pCh)){
		Retract(pIndex);
		//Error();
		printf("指数格式错误: %s\n ", token);
		return NULL;
	}
	else{
		Concat(*pCh, token);
		char temp = *pCh;
		Getch(pCh, string, pIndex);
		if ((temp == '+' || temp == '-') && !Digit(*pCh)){
			Retract(pIndex);
			//Error();
			printf("指数格式错误: %s\n", token);
			return NULL;
		}
	}
	while (Digit(*pCh)){
		Concat(*pCh, token);
		Getch(pCh, string, pIndex);
	}
	Retract(pIndex);
	double num = Dtb(token);
	return ReturnWord(20, NumT2(token), num);
}
