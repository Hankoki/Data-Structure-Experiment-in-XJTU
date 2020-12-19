//程序名称:基于串的模式匹配算法
//开发环境:VS2019
//最后修改:2020/12/19
//存在问题:仍然有魔鬼数字与未初始化的指针，未释放空间


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_NUM 100//最大数字个数为100
#define inf 2000000000 //最大值为
#define MAXSIZE 123
#define ERROR -1
typedef struct {
	unsigned int weight;//权值 
	unsigned int parent, lchild, rchild;//父节点，孩子结点的权值 
}HTNode, * HuffmanTree;
typedef char** HuffmanCode;//二维字符数组 

void Select(HuffmanTree& HT, int x,int* s1x,int *s2x) {//选出无父结点，并且权值最小的两个结点，赋值给s1，s2 
	int i=1, min1 = inf, min2 = inf;
	for (i = 1; i <= x; i++) {//找最小 
		if (HT[i].weight < min1 && HT[i].parent == 0) { min1 = HT[i].weight; *s1x = i; }
	}
	for (i = 1; i <= x; i++) {//找次小 
		if (HT[i].weight < min2 && i != *s1x && HT[i].parent == 0) {
			min2 = HT[i].weight; *s2x = i;
		}
	}
}


void HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC, int* w, int n) {//根据输入的结点的权值和个数来构建赫夫曼树 //创建数和编码的地址
	int s1 = 0;
	int s2 = 0;
	if (n <= 1)
	{
		printf("ERROR_03");
		exit(ERROR);
	};
	int m = 2 * n - 1;//n个叶子，有2*n-1个结点 
	int i=0;
	int* s1x=&s1;
	int* s2x=&s2;
	HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));//0号单元未用 开辟空间
	HuffmanTree p;
	for (p = HT + 1, i = 1; i <= n; i++, p++, w++) {//叶子结点赋值 
		p->weight = *w; p->parent = 0; p->lchild = 0; p->rchild = 0;
	}
	for (; i <= m; i++, p++) {//非叶子结点初始化 
		p->weight = 0; p->parent = 0; p->lchild = 0; p->rchild = 0;
	}
	for (i = n + 1; i <= m; i++) {
		Select(HT, i - 1,s1x,s2x);//选出最小的两个无父节点的结点 
		HT[*s1x].parent = i; HT[*s2x].parent = i;
		HT[i].lchild = *s1x; HT[i].rchild = *s2x;
		HT[i].weight = HT[*s1x].weight + HT[*s2x].weight;
	}

	//----------下面是将每个结点的赫夫曼编码存入二维字符数组

	HC = (HuffmanCode)malloc((n + 1) * sizeof(char*));//申请一段以HC为首地址的内存，可以看成二维字符数组 ，这里先申请了第一维 
	char* cd = (char*)malloc(n * sizeof(char));//申请一段临时工作空间 
	cd[n - 1] = '\0';//编码结束符 
	for (i = 1; i <= n; i++) {
		int start = n - 1, c, f;
		for (c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent) {//从叶子到根逆向求编码 
			if (HT[f].lchild == c)cd[--start] = '0';//如果当前结点是父节点的左孩子，则存一个1 
			else cd[--start] = '1';//反之 
		}
		HC[i] = (char*)malloc((n - start) * sizeof(char));//申请第二维 
		strcpy(HC[i], &cd[start]);//将编码从工作空间存入赫夫曼编码表中 
	}
	free(cd); //释放临时空间 
}

int InitHuffmanTree(int* w,char* string, int num, int* list)//寻找最小值和次小值结点
{
	int middle = 0;
	num = 0;
	int l = 0;
	char temp='\0';
	int asc[MAXSIZE] = { 0 };//a-z字符对应ASCII码表小写字母97-122，大写字母是 65-90,空格为32，小写大写相差32 
	for (int i = 0, length = strlen(string); i < length; ++i)
	{
		asc[int(string[i])]++;//对应的ASCII码下标加1,同一个字母对应的下标都相同，进行计数 
	}
	
	//for (int i = 0; i < MAXSIZE; i++)
	//{
	//	printf("%d ", asc[i]);
	//}
	
	for (int j = 0; j < MAXSIZE; ++j)
	{
		if (asc[j] != 0)
		{
			w[l] = asc[j];//以空格为先，大写字母次之，小写字母再次之的排列方法放置权重
			
			list[l] = j;
			l++;
			middle++;
		}
	}
	num = middle;

	return num;
	}

void translate(HuffmanCode& HC, char* string, int num,int* list,int* list_t)//解码哈夫曼树
{
	int i = 1, j = 0,compare_y=1,search=0,count=0,compare_x=0,spot=0;
	
	for (i = compare_y; i <= num && string[search] != '\0'; i++)
	{
		for (j = compare_x; j <= strlen(string); j++)
		{
			if (string[search] == '\0' && HC[i][j] != '\0')//终止于判断中途则出错
			{
				printf("ERROR_03");
				exit(ERROR);
			}
			if (HC[i][j] == '\0' && HC[i][j] == string[search])
			{
				list_t[count] = list[i - 1];
				break;
			}
		
			if (HC[i][j] == string[search])//匹配的情况
			{
				search++;
				continue;
				
			}
			else if (HC[i][j] != string[search] && HC[i][j] != '\0'&&i!=num+1)//匹配的中途出错，但尚未完成查询
			{
				compare_x = 0;
				search = spot;
				break;
			}
			else if (compare_y == num && j == strlen(string) && HC[i][j] != string[search] && HC[i][j] != '\0')
			{
				printf("ERROR_03");
				exit(ERROR);
			}
			if (HC[i][j] == '\0'&&HC[i][j-1]==string[search-1])//与一个字母匹配
			{
				list_t[count] = list[i-1];
				count++;
				i = 0;
				j = 0;
				spot = search;
				break;
			}
		}
	}
}

int main(int argc, char* argv[]) 
{
	if (argc != 3)
	{
		printf("ERROR_01");///参数错误
		exit(ERROR);
	}
	if (strlen(argv[1]) < 20)
	{
		printf("ERROR_02");//编码失败
		exit(ERROR);
	}
	int list_t[MAX_NUM] = { 0 };
	int list[MAX_NUM] = {0};
	char HFlist[MAX_NUM] = {'\0'};
	HuffmanTree HT;//自定义结构体
	HuffmanCode HC;
	int w[MAX_NUM];
	int i=1,j=0;
	int num=0;
	num = 1;
	num=InitHuffmanTree(w,argv[1], num,list);

	HuffmanCoding(HT, HC, w, num);
	for (i = 0; i < strlen(argv[1]); i++)//逐渐匹配字母，输出编码
	{
		for (j = 1; j <= MAX_NUM; j++)
		{
			if (int(argv[1][i]) == list[j-1])
			{
				strcat(HFlist, HC[j]);
			}
		}
	}
	translate(HC, argv[2], num, list, list_t);
	int k = 0;
	if (list_t[k] == 0)//译码完全未匹配
	{
		printf("ERROR_03");
		exit(ERROR);
	}
	printf("%s ", HFlist);
	while (list_t[k] != 0)
	{
		printf("%c", list_t[k]);
		k++;
	}
	free(HC);
	free(HT);
	return 0;
}
//实例4 ERROR_03出错