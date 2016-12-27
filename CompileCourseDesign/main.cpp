#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "lrparser.h"
int main(){

	char* test[] = {
		"main(){x=1;if x<2 { z=1;} else{z=2;};}#",
		"main(){x=1;if x<1{z=1;} else { if x==123e123 {z=2;};};}#",
		"main(){x=1;if x>10{ do {x=x-1;} while x>0;};}#",
		"main(){x=1;do { if x>1{ x=x-1;} else {x=x+1;}; } while x>10; }#",
		"main(){x=1;if x>10 { do{ if x==1 {x=1;};} while x>0;};}#",
		"main(){if x<2 {do{a=a+1;if a>1 {b=2;c=3;}else{a=1;};}while a>1;} else {if x<2 { z1=1; do{f=g+1;}while f>2;  }else {a=2;};}; }#",
		"end",
	};
	int i = 0;
	while (strcmp(test[i], "end")){
		WordsHead* pHead = Lex(test[i]);
		puts("语法语义分析开始:");
		FoursHead *pFoursHead=Lrparser(pHead);
		printf("四元组结果为:\n");
		PrintFours(pFoursHead);
		i += 1;
	}
	
	getchar();
	return 0;
}