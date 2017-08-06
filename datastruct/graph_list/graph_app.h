#pragma once
#include "graph_list.h"
#include "queue_ds.h"
using namespace DS;
/*	主要实现图的应用	*/

typedef struct GraphContentCompont
{
	PQueue q;
	GraphContentCompont* next;
}GraphContentCompont, *PGraphContentCompont;

void InitGraphContentCompont(PGraphContentCompont& pgcc);
void FreeGraphContentCompont(PGraphContentCompont& pgcc);

/*	判断两个顶点是否连通，可用于解决多维迷宫路径问题	*/
bool IsConnect(PAdjList& pal, const VertexData src, const VertexData dst);

/*	无向图应用：连通分量
*	利用深度优先或广度优先遍历，生成连通分量，统计连通图数量，
*	如果生成了多个连通分量，说明此图非连通图。	*/
/*	另：有向图连通的概念叫强连通图，即顶点X，Y可互相连通到达，
*	这里没有实现生成强连通分量的方法	*/
void MakeConnectComponents(PAdjList& pal, PGraphContentCompont pgcc);

void PrintGraphContentCompont(PGraphContentCompont& pgcc);

/*  获取单个顶点的连通分量		*/
void MakeConnectComponentForVertex(PAdjList& pal, int index_src, PQueue& q);

/*	生成树：连通网（带权）中，各边的代价之和最小的生成树称为
*	最小代价生成树，也称最小生成树。生成最小生成树有两个经典的算法：
*	Prim算法和Kruskal算法。	*/

/*  Prim算法思想：加点不构成回路。
*	1. 任选一个顶点作为初始生长点，构成生长点集合B，其他点构成非生长点集合U。
*	2. 在生长点集合选一个点b，在非生长点集合选一个点u，使得(u,b)边代价最小，
*		将此边放入生成树边集合TE，同时将u点移动到生长点集合。	
*	3. 重复2，重复进行n-1次直到非生长点集合为空。
*	4. 此时TE集合中含有n-1条边，TE边集合和顶点集合构成了最小生成树。	*/
/*  算法实现中可以优化的地方：
*	1. 生长点和非生长点可以共用数组空间
*	2. 生长点集合中已存在的点，到非生长点集合中每个点的距离不用每次比较得出，
*		可以记录每个非生长点到已生长点集合的哪个点权值最小，权值是多少，当选出最新生长点时，
		只要比较每个非生长点到最新生长点的权值，然后依次更新上面的每个最小值即可。
*	3. 本例实现参考的是国防科大的课程。生长点、非生长点、最小权值保存在一个结构里，
		然后定义一个以这个结构为元素的数组，作为生长点、非生长的、最小权值公用集合。
*	4. 使用邻接矩阵查权值比较快，边数较多也适合使用邻接矩阵，这里用邻接表举例。 */

typedef struct MST_Edge
{
	int grow_vertex;	//生长点
	int vertex;			//非生长点
	int weight;			//权
}MST_Edge, *PMST_Edge;

void MinSpanningTree_Prim(PAdjList& pal, int init_grow, PMST_Edge& pmste);
void PrintMST_Edge(PAdjList& pal, PMST_Edge& pmste, int size);
void CopyMST_Edge(MST_Edge& esrc, MST_Edge& edst);

/*  Kruskal算法思想：加边不构成回路。
*	1. 将n个顶点看成n个集合
*	2. 按权值由小到大的顺序选择边，选边应满足此边不会和之前所有选边构成回路，将该边放到生成树边的集合TE中。
*		同时将该边的两个顶点所在的顶点集合合并
*	3. 重复2，直到所有的顶点都在同一个顶点集合内
*	4. 此时TE集合中含有n-1条边，TE边集合和顶点集合构成了最小生成树。
*	5. 这个算法的关键点:判断新的选边不会和原来选边构成回路。
*		因为选边过程其实是选两个点构成子树，再选两个点构成子树 或与 原来的树构成更大的子树。。。
*		这样就会形成有些顶点合集逐渐增加，有些顶点合集被清空，这个过程转化成了顶点集合的合并过程
*		这里的解决思想是判断最新选边的两个顶点有没有同时存在于某个顶点集合中，如果同时存在于某个顶点集合，
*		则说明这个选边在某个子树上构成了回路，反之则没有构成回路	*/
/*  算法实现中可以优化的地方：
*	1. 所有边排序后放在一个数组便于按权值从小到大选择，生成树边可以共用这个数组
*	2. 为了实现顶点集合和查找顶点所在集合方便，顶点元素数据结构包含其所在集合的索引，这个集合的下一个顶点的指针。
		顶点元素保存在一个数组中，便于索引查找。
		集合元素的数据结构包含顶点元素数量，第一个顶点元素指针（链表）。为了方便顶点快速定位其所在集合，
		集合元素也保存在数组中。
*/
typedef struct VertexItemKruskal
{
	int nset;
	VertexItemKruskal *next;
}VertexItemKruskal, *PVertexItemKruskal;

typedef struct SetItemKruskal
{
	int count;
	PVertexItemKruskal vertexhead;
}SetItemKruskal, *PSetItemKruskal;

void CopyetItemKruskal(SetItemKruskal& src, SetItemKruskal& dst);
int UnionSetItemKruskal(PSetItemKruskal& pset, int l, int r);
void MinSpanningTree_Kruskal(PAdjList& pal, PMST_Edge& pmste);
void TestMinSpanningTree_Prim(PAdjList& pamUN);
void TestGraphApp();


