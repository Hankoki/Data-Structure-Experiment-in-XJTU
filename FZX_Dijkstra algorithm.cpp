//程序名称:无向图最短路径搜索
//开发环境:VS2019
//最后修改:2020/12/19
//存在问题:INF不要直接输入一个数字，利用INT_MAX可以在所有的平台上作为最大值使用，main函数一般不用void，在别的编译器上可能无法运行


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define INF INT_MAX //int类型的最大值
#define MAX 14
#define LENGTH 13
#define ERROR -1
#define NOT_FOUND -1
#define FOUND 0
#define FALSE 0
#define TRUE 1

// 邻接矩阵
typedef struct Graph
{
    char vexs[MAX];       // 顶点集合
    int vexnum;           // 顶点数
    int edgnum;           // 边数
    int matrix[MAX][MAX]; // 邻接矩阵
}Graph, * PGraph;

// 边的结构体
typedef struct Edge
{
    char start; // 边的起点
    char end;   // 边的终点
    int weight; // 边的权重
}EData;

//char Position[MAX][MAX]={"北门","饮水思源","腾飞塔","图书馆","教学主楼","活动中心","南门","宪梓堂","西迁馆","传送门1","传送门2","传送门3","传送门4"}


Graph* Init_Graph()
{
   
    char vexs[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M' };
    int matrix[][LENGTH] =

    {
        {INF,18 ,INF,INF,INF,INF,INF,INF,INF,INF,INF,22 ,INF},
        {18 ,INF,19 ,INF,INF,INF,INF,INF,INF,27 ,INF,INF,INF},
        {INF,19 ,INF,23 ,INF,INF,INF,INF,INF,INF,INF, 4 ,32 },
        {INF,INF,23 ,INF,15 ,INF,INF,INF,INF, 4 , 4 ,INF,INF},
        {INF,INF,INF,15 ,INF,21 ,INF, 8 ,INF,INF,INF,INF,INF},
        {INF,INF,INF,INF,21 ,INF,30 ,INF,INF,INF,INF,INF,INF},
        {INF,INF,INF,INF,INF,30 ,INF,14 ,20 ,INF,21 ,INF,INF},
        {INF,INF,INF,INF, 8 ,INF,14 ,INF,INF,INF,INF,INF, 4 },
        {INF,INF,INF,INF,INF,INF,20 ,INF,INF,INF,43 ,INF,INF},
        {INF,27 ,INF, 4 ,INF,INF,INF,INF,INF,INF,INF,INF,INF},
        {INF,INF,INF, 4 ,INF,INF,21 ,INF,43 ,INF,INF,INF,INF},
        {22 ,INF, 4 ,INF,INF,INF,INF,INF,INF,INF,INF,INF,INF},
        {INF,INF,32 ,INF,INF,INF,INF, 4 ,INF,INF,INF,INF,INF}
    };
    int vlen = LENGTH;
    int i=0, j=0;
    Graph* pG;

    if ((pG = (Graph*)malloc(sizeof(Graph))) == NULL)//为邻接矩阵
        return NULL;
    memset(pG, 0, sizeof(Graph));//初始化邻接矩阵

    // 初始化"顶点数"
    pG->vexnum = vlen;
    // 初始化"顶点"
    for (i = 0; i < pG->vexnum; i++)
    {
        pG->vexs[i] = vexs[i];
    }
        

    // 初始化"边"
    for (i = 0; i < pG->vexnum; i++)
    {
        for (j = 0; j < pG->vexnum; j++)
        {
            pG->matrix[i][j] = matrix[i][j];
        }
            
    }
        

    // 统计边的数目
    for (i = 0; i < pG->vexnum; i++)
    {
        for (j = 0; j < pG->vexnum; j++)
        {
            if (i != j && pG->matrix[i][j] != INF)
            {
                pG->edgnum++;
            }
        }
    }
    pG->edgnum= pG->edgnum/2;

    return pG;
}



//G -- 图
//vs -- 起始顶点(start vertex)。即计算"顶点vs"到其它顶点的最短路径。
//prev -- 前驱顶点数组。即，prev[i]的值是"顶点vs"到"顶点i"的最短路径所经历的全部顶点中，位于"顶点i"之前的那个顶点。
//dist -- 长度数组。即，dist[i]是"顶点vs"到"顶点i"的最短路径的长度。
void dijkstra(Graph G, int vs, int prev[], int dist[],int end,int route[])//统计图(G)中"顶点vs"到其它各个顶点的最短路径
{
   
    int i=0, j=0, k=0,m=0,n=0,cmp=0,l=0,count=0,aim=0,mid=0,indicator=1;
    int min=0;
    int tmp=0;
    int final[MAX] = {FALSE};      // final[i]=TRUE表示"顶点vs"到"顶点i"的最短路径已成功获取。
    char Position[MAX][MAX] =
    {
        "北门",
        "饮水思源",
        "腾飞塔",
        "图书馆",
        "教学主楼",
        "活动中心",
        "南门",
        "宪梓堂",
        "西迁馆",
        "传送门1",
        "传送门2",
        "传送门3",
        "传送门4"
    };

    
    for (i = 0; i < G.vexnum; i++)// 初始化
    {
        final[i] = FALSE;              // 顶点i的最短路径还没获取到。
        prev[i] = FALSE;              // 顶点i的前驱顶点为0。
        dist[i] = G.matrix[vs][i];// 顶点i的最短路径为"顶点vs"到"顶点i"的权。
    }

    // 对"顶点vs"自身进行初始化
    final[vs] = TRUE;
    dist[vs] = FALSE;

    // 遍历所有顶点；每次找出一个顶点的最短路径
    for (i = 1; i < G.vexnum; i++)
    {
        // 寻找当前最小的路径；
        // 即，在未获取最短路径的顶点中，找到离vs最近的顶点(k)。
        min = INF;
        for (j = 0; j < G.vexnum; j++)
        {
            if (final[j] == FALSE && dist[j] < min)//该点没有被遍历过且距离最小
            {
                min = dist[j];
                k = j;
            }

        }
        // 标记"顶点k"为已经获取到最短路径
        final[k] = TRUE;
        


        // 当已知道"顶点k的最短路径"之后，更新最短路径和前驱顶点
        for (j = 0; j < G.vexnum; j++)
        {

            if (G.matrix[k][j] == INF)
            {
                tmp = INF;

            }
            else
            {
                tmp = min + G.matrix[k][j];

            }

            if (final[j] == FALSE && (tmp < dist[j]))
            {
                dist[j] = tmp;
                prev[j] = k;
            }
        }
    }
   

    // 测试用输出到所有点的距离
    /*printf("起点为(%s): \n", Position[vs]);
    for (i = 0; i < G.vexnum; i++)
        printf("  两点最短距离(%s, %s)=%d\n", Position[vs], Position[i], dist[i]);*/

    printf("%d", dist[end]);//输出所需最短路径


    ////输出最短路径结点,思想是终点必有一个或一个以上的前置结点，且前置结点到起点的距离与终点到起点的距离之差为连接两点的边的权值(验收时取消注释)
    //i = 0;
    //tmp = end;
    //count = 0;
    //route[0] = end;
 
    //    for (; i < LENGTH; i++)//循环所有的连接点
    //    {
    //        for (j = 0; j < LENGTH; j++)//循环所有的最短距离
    //        {
    //            if (dist[tmp] - G.matrix[tmp][i] == dist[j] && G.matrix[tmp][j] != INF)//必须直接相连
    //            {
    //                tmp = j;
    //                //dist[tmp] = dist[tmp] - G.matrix[tmp][i];
    //                route[count] = tmp;

    //                i = 0; count++; j = 0;
    //                break;
    //            }
    //        }


    //    }
   
    //if (vs == 0)
    //    printf("%s ",Position[0]);//起点为北门的解决办法
    //for (i=MAX;i>=0;i--)
    //{
    //    if (route[i] != 0)
    //    {
    //        printf("%s ", Position[route[i]] );//逆序输出
    //    }
    //}
    //printf("%s ", Position[end]);//没有输入

}


int main(int argc, char* argv[])
{
    //先行判断参数数量
    if (argc != 3)
    {
        printf("ERROR_01");
        exit(ERROR);
    }

    int start = 0, end = 0;
    int i = 0;
    int indicate = NOT_FOUND;
    int prev[MAX] = { FALSE };
    int dist[MAX] = { 0 };
    int route[MAX+1] = { 0 };

    Graph* pG = Init_Graph();
    char Position[MAX][MAX] =//结点序列
    {
        "北门",
        "饮水思源",
        "腾飞塔",
        "图书馆",
        "教学主楼",
        "活动中心",
        "南门",
        "宪梓堂",
        "西迁馆",
        "传送门1",
        "传送门2",
        "传送门3",
        "传送门4"
    };
    
    for (i = 0; i < LENGTH; i++)
    {
        if (strcmp(argv[1],Position[i])==0)//如果输入的起点与任意图的结点匹配
        {
            start = i;
            indicate = FOUND;//找到
            break;
        }
    }
    if (indicate != FOUND)//如果输入的起点不在图中
    {
        printf("ERROR_02");//报错
        exit(ERROR);
    }
    indicate = NOT_FOUND;


    for (i = 0; i < LENGTH; i++)
    {
        if (strcmp(argv[2], Position[i]) == 0)//如果输入的终点与任意图的结点匹配
        {
            end = i;
            indicate = FOUND;//找到
            break;
        }
    }
    if (indicate != FOUND)//如果输入的终点不在图中
    {
        printf("ERROR_02");//报错
        exit(ERROR);
    }

    dijkstra(*pG, start, prev, dist,end,route);

    if (pG != NULL)//释放指针
    {
        free(pG);
    }
    return 0;
}