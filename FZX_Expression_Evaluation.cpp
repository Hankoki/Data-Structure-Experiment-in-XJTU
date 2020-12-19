//程序名:利用栈进行表达式求值
//开发环境:VS2019                       
//最后修改:2020/12/19                             
//存在问题:魔鬼数字，未初始化，未free分配的内存，单字母变量过多







#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10//不加分号
#define OVERFLOW -2
#define ERROR 0

typedef struct //运算数的栈定义
{
	double* base;
	double* top;
	int stacksize;
} SqStack1;

typedef struct//运算符的栈定义
{
	char* base;
	char* top;
	int stacksize;
} SqStack2;

//________________________函数声明________________________
void InitStack_N(SqStack1& S);
void Push_N(SqStack1& S, double e);
double Pop_N(SqStack1& S, double& e);
double GetTop_N(SqStack1& S, double& e);

void InitStack_O(SqStack2& S);
void Push_O(SqStack2& S, char e);
char Pop_O(SqStack2& S, char& e);
char GetTop_O(SqStack2& S, char& e);

int Prior(char c1, char c2);
double Operate(double num_1, char theta, double num_2);
int In(char c);
int Judge(char a);
int bracket(char a, char b);
//_________________________函数声明________________________

//_______________________主函数_______________________________
int main(int argc, char* argv[])//命令行参数的内容在属性里改
{
	if (argc != 2)
	{
		printf("ERROR_01");
		exit(0);
	}
	int i = 0;
	char e = {}, theta = {};
	double a=0, b=1, temp1=0,temp2=0,result=0;
	SqStack2 OPTR;//需要初始化
	SqStack1 OPND;

	InitStack_O(OPTR); Push_O(OPTR, '\0');
	InitStack_N(OPND); 
	GetTop_O(OPTR, e);
	
	char c = argv[1][i];//初始化
	
	for (i = 0; i <= strlen(argv[1]);)
	{
		if (In(c))
		{
			if (Judge(argv[1][i]) == 1 && Judge(argv[1][i+1]) == 1)//判断符号是否重复
			{
				printf("ERROR_02");
				exit(0);
			}
			if (bracket(argv[1][i],argv[1][1+i]))//判断括号的重复
			{
				printf("ERROR_02");
				exit(0);
			}
			switch (Prior(e, c))
			{
			case -1:
				Push_O(OPTR, c); i++; c = argv[1][i]; break;
			case 0:
				Pop_O(OPTR, e); i++; c = argv[1][i]; break;
			case 1:
				if (e == '/' && b == 0)
				{
					printf("ERROR_03");
					exit(0);
				}
				Pop_O(OPTR, e);
				Pop_N(OPND, b);
				Pop_N(OPND, a);
				Push_N(OPND, Operate(a, e, b));
				//printf("%g ", GetTop_N(OPND, a));
			}
			GetTop_O(OPTR, e);
		

		}
//____________________对运算符的判断结束________________________
		else if (c >= '0' && c <= '9')
		{
			temp1 = 0; temp2 = 10;
			while (c >= '0' && c <= '9')
			{
				temp1 = temp1 * 10 + c - '0';
				i++;
				c = argv[1][i];//多位数的解决 
			}
			if (c == '.')
			{
				i++; c = argv[1][i];
				while (c >= '0' && c <= '9')
				{
					temp1 = (c - '0') / temp2 + temp1;
					temp2 = temp2 * 10;
					i++;
					c = argv[1][i];//小数点后面的解决
				}
			}
			Push_N(OPND, temp1);

		}
		else
		{
			printf("ERROR_02");
			exit(0);
		}
//____________________对运算数的判断结束____________________
	}
	GetTop_O(OPTR,e);
	GetTop_N(OPND, result);
	if ((int)result == result)
	{
		printf("%d", (int)result);//判断是整数还是小数
	}
	else
	{
		printf("%.01lf", result);
	}
	getchar();
	return 0;






}
//_______________________主函数_____________________________________


//_____________________函数_________________________________________
void InitStack_N(SqStack1& S)//生成运算数的栈
{
	S.base = (double*)malloc(STACK_INIT_SIZE * sizeof(double));
	if (!S.base)exit(OVERFLOW);
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
}

void InitStack_O(SqStack2& S)//生成运算符的栈
{
	S.base = (char*)malloc(STACK_INIT_SIZE * sizeof(double));
	if (!S.base)exit(OVERFLOW);
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
}

void Push_N(SqStack1& S, double e)//插入e为新的运算数栈顶元素
{
	if (S.top - S.base >= S.stacksize)
	{
		S.base = (double*)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(double));
		if (!S.base)exit(OVERFLOW);
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	//printf("%g ", *(S.top-1));
}


void Push_O(SqStack2& S, char e)//插入e为新的运算符栈顶元素
{
	if (S.top - S.base >= S.stacksize)
	{
		S.base = (char*)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(double));
		if (!S.base)exit(OVERFLOW);
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	//printf("%d ", *(S.top-1));
}

double Pop_N(SqStack1& S, double& e)//删除运算符栈S的栈顶元素并且用e返回
{
	if (S.top == S.base) return ERROR;
	e = *--S.top;
	return e;
}


char Pop_O(SqStack2& S, char& e)//删除运算数栈S的栈顶元素并且用e返回
{
	if (S.top == S.base) return ERROR;
	e = *--S.top;
	return e;
}

double GetTop_N(SqStack1& S, double& e)//获得运算数栈的栈顶元素
{
	if (S.top == S.base)return ERROR;
	e = *(S.top - 1);
	return e;
}

char GetTop_O(SqStack2& S, char& e)//获得运算符栈的栈顶元素
{
	if (S.top == S.base)return ERROR;
	e = *(S.top - 1);
	return e;
}

int getIndex(char c)//寻找单个运算符对应的坐标，用于比较优先权
{
	switch (c) {
	case '+': return 0;
	case '-': return 1;
	case '*': return 2;
	case '/': return 3;
	case '(': return 4;
	case ')': return 5;
	case '^': return 6;
	case '[': return 7;
	case ']': return 8;
	case '\0': return 9;
	}
	return -1;
}   

int Prior(char c1, char c2) //寻找运算符的优先级
{
	char location[11][11] = { ">><<<><<>>",
							  ">><<<><<>>",
							  ">>>><><<>>",
							  ">>>><><<>>",
							  "<<<<<=<xxx",
							  ">>>><>><>>",
							  ">>>><>><>>",
							  "<<<<<x<x=x",
							  ">>>>xxxx>>",
							  "<<<<<x<<x="};//运算符优先表
	int c1_index = getIndex(c1);
	int c2_index = getIndex(c2);//找出算符代表的数字
	int priority = location[c1_index][c2_index];
	switch (priority) 
	{
	case '<': return -1; break; // c1<c2
	case '>': return 1;  break;// c1>c2
	case '=': return 0;  break;// c1=c2错误情况
	}
}

double Operate(double num_1, char theta, double num_2)//进行+-*/^的运算
{
	if (theta == '/' && num_2 == 0)
	{
		printf("ERROR_03");
		exit(0);
	}
	switch (theta) 
	{
	case'+':return num_1 + num_2;
	case'-':return num_1 - num_2;
	case '*': return num_1 * num_2;
	case '/': if (num_2 != 0) return num_1 / num_2;
	case '^': return pow(num_1, num_2);
	}
}       

int Judge(char a)//判断可能的两个运算符重复错误
{
	int j=0,k=0,m=0,n=0;
	char b[] = "+-*/^";//遍历所有的运算符
	char c[] = "+-*/^[]().0123456789\0";
	for (j = 0;j < strlen(b); j++)
	{
		if (a == b[j])//如果出现了任何一个运算符，则返回1
		{
			k = 1;
		}
	}

	for (m = 0;m < strlen(c);m++)
	{
		if (a = c[j]);
		n = 1;
	}
	if (n == 0)
	{
		printf("ERROR_02");
		exit(0);
	}
	return k;
}
int In(char c) 
{
	char OPRT_LIST[] = "+-*/^()[]\0";
	int l = 0, indicator;
	for (indicator = 0; l <= strlen(OPRT_LIST); l++)
		if (c == OPRT_LIST[l]) indicator = 1;
	return indicator;
}    //判别是运算符还是数字

int bracket(char a, char b)
{
	int i = 0;
	char c[] = "()[]";
	for (i = 0; i < strlen(c); i++)
	{
		if (a == c[i])
		{
			a = i;
		}
		if (b == c[i])
		{
			b = i;
		}
	}
	if (a == b)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//_____________________________函数____________________________________