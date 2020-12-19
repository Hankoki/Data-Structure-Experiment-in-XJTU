//程序名称:基于串的模式匹配算法
//开发环境:VS2019
//最后修改:2020/12/19
//存在问题:未初始化，个别的单字母变量，分配空间时没有考虑分配失败的情况，释放的时候未判空

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//_____________定义________________________
#define OK 0
#define OVERFLOW -1
#define NOT_FOUND -2
#define ADD 1

typedef struct
{
	char* ch;
	int length;
}HString;


//____________函数声明_____________________
int StrAssign(HString& T, char* chars);
int Strlength(HString S);
int StrCompare(char S, char T);
int ClearString(HString& S);
void get_next(HString T, int next[]);
int Index_KMP(HString S, HString T);


//_____________主函数_____________________
int main(int argc, char* argv[])
{

//____________前期判断_______________________

	if (argc != 3)
	{
		printf("ERROR_01");
		exit(0);
	}//参数错误
	if (strlen(argv[1]) < strlen(argv[2]))
	{
		printf("%d", -1);
		exit(NOT_FOUND + 1);
	}//模式串比主串长必定无法匹配



	HString NO1_str, NO2_str;
	StrAssign(NO1_str, argv[1]);
	StrAssign(NO2_str, argv[2]);
	printf("%d", (Index_KMP(NO1_str, NO2_str))+1);
	ClearString(NO1_str);//释放NO1_str内存
	ClearString(NO2_str);//释放NO2_str内存
	return 0;
}


//___________函数______________________
int StrAssign(HString& T, char* chars)//分配字符串空间
{
	int j = 0;
	int i=0;
	char *CHARS;//接受chars
	if (T.ch==NULL) free(T.ch); 
	for (i = 0, CHARS = chars; *CHARS; ++i, ++CHARS);
	if (!i)
	{	
		return(0);
	}
	else
		if (!(T.ch = (char*)malloc((i+ADD) * sizeof(char))))//需要分配比数组大小大1的内存，否则在释放的时候会出现 
			exit(OVERFLOW);									//CRT detected that the application wrote to memory after 
	for (i = 0; i <= strlen(chars); i++)                    //end of heap buffer这个错误，其发生的原因是写入了超过分配内存的长度的数组
	{														//在释放内存的时候便会报错，认为是\0占用一个空间，但是在分配内存的时候没有
		T.ch[i] = chars[i];                                 //计入\0的大小导致分配的空间比写入的字符串小1
		T.length = i;
	}

	return OK;

}

int Strlength(HString S)
{
	return S.length;
}

int StrCompare(char S, char T)//比较字符串
{
	int i = 0;//若S大于T,返回值大于0，若S等于T，返回值等于0，若S小于T，返回值小于0
	if (S != T) return S - T;
	return 0;
}

int ClearString(HString& S)//清除字符串，释放空间
{
	if (S.ch)
	{
		free(S.ch);
		S.ch = NULL;
	}
	S.length = 0;
	return OK;
}

void get_next(HString T, int next[])//获取next数组
{
	int i = 0; int j = -1;
	next[0] = -1;
	while (i <= T.length)
	{
		if (j == -1 || T.ch[i] == T.ch[j])
		{
			++i;
			++j;
			next[i] = j;
		}
		else
		{
			j = next[j];//回退
		}
	}
}
int Index_KMP(HString S, HString T)//KMP算法
{

	int MAXLENGTH=0;
	MAXLENGTH = strlen(S.ch);
	int* next;
	next = (int*)malloc(sizeof(int) * MAXLENGTH);//动态分配next的大小，防止模式串长度超过设定值而越界
	
	int k=0;
	for (k = 0; k < MAXLENGTH; k++)//初始化next
	{
		next[k] = 0;
	}

	int i = 0;
	int j = 0;
	get_next(T, next);
	while (i < S.length && j < T.length)
	{
		if (j == -1 || StrCompare(S.ch[i], T.ch[j]) == 0)
		{
			++i; ++j;
		}
		else j = next[j];//回退
	}
	if (j >= T.length) return (i - T.length);
	else
	{
		return NOT_FOUND;
	}
	free(next);//释放next空间
}

	