#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_list.h"
#include "queue_ds.h"
#include "stack_ds.h"
using namespace DS;
/*	有向无环图（Directed Acyclic Graph）是指一个无环的有向图，简称 DAG。有
	向无环图可用来描述工程或系统的进行过程，如一个工程的施工图、学生课程间
	的制约关系图等。
	这里利用有向无环图主要实现两类算法：1拓扑排序 2最短路径

	1、拓扑排序主要应用在AOV网中。
	拓扑排序根据AOV网中活动的依赖关系，得到了活动执行的先后顺序，保证了活动先后执行的正确性
	用顶点表示活动，用弧表示活动间的优先关系的有向无环图，称为 顶点表示
	活动的网（Activity On Vertex Network），简称为 AOV- 网。
		AOV-网的特性如下：
		(1)若 v i 为 v j 的先行活动，v j 为 v k 的先行活动，则 v i 必为 v k 的先行活动
		(2)AOV-网的拓扑序列不是惟一的

	2、关键路径
	有向图在工程计划和经营管理中有着广泛的应用。通常用有向图来表示工程
	计划时有两种方法：
	（1）用弧表示活动，用有向弧表示活动间的优先关系，即上面所讨论的 AOV网。
	（2）用顶点表示事件(event)，用弧表示活动(activity)，弧的权值表示活动所需要的时间。
		事件就是某件事情开始或结束的时间点
		活动就是某件事情进行的过程
		把这种方法构造的有向无环图叫做边表示活动的网（Activity On Edge Network）AOE-网。
		在 AOE 网中存在惟一的、入度为 0 的顶点，叫做 源点
		存在惟一的、出度为0 的顶点，叫做 汇点。
		从源点到汇点的最长路径的长度即为完成整个工程任务所需的时间，该 路径叫做关键路径。
		关键路径上的活动叫做 关键活动。
		这些活动中的任意一项活动未能按期完成，则整个工程的完成时间就要推迟。
		相反，如果能够加快关键活动的进度，则整个工程可以提前完成

	区分几个重要概念：
	(1) 事件最早发生时间 ee, 可以按拓扑排序递推得到
	(2) 事件最晚发生时间 el, 可以按逆拓扑排序递推得到
	(3) 活动最早开始时间 ae, 边a<j,k>的ae等于事件j的最早发生事件即j.ee
	(4) 活动最晚开始时间 al, 边a<j,k>的al等于事件k的最晚发生事件减去边的权值，即k.el - dut(j,k)
	(5) 活动松弛时间(时间余量) ee, 活动最晚开始时间与最早开始时间之差al-ae。
	(6) 关键活动, 松弛时间为0的活动为关键活动
*/

/*  拓扑排序算法步骤：
	1 选择一个无前驱顶点输出
	2 删除此点及以它为顶点的弧
	3 重复1、2直到不存在无前驱顶点
	4 输出顶点数小于图顶点数说明存在回路，否则输出顶点顺序则为拓扑顺序 */
int TopoSort(PAdjList& pal, PQueue& pq);

/*  拓扑排序根据AOV网中活动的依赖关系，得到了活动执行的先后顺序，保证了活动先后执行的正确性 */
int TopoSortForCriticalPath(PAdjList& pal, PStack& psTopoSort, int*& ee);

typedef struct CriticalPathItem
{
	int src;	//事件开始
	int dst;	//事件结束
	int ae;		//活动最早发生时间
	int al;		//活动最晚发生事件
	int	loose;	//活动松弛时间
}CriticalPathItem, *PCriticalPathItem;

/*  关键路径：根据AOE网的各事件依赖关系和每个活动所用的时间消耗关系，
*	得出了工程全部完成最少所用的时间和影响工期提前和拖后的关键事件。
*	关键路径：从原点到汇点的最长路径=完成工期的最短时间=关键路径长度 */

/*  求关键路径的基本步骤：
	1 在对图中的顶点进行拓扑排序过程中按拓扑序列求出每个事件的最早发生时间ee 
	2 按逆拓扑序列求出每个事件的最晚发生时间el	
	3 求出每个活动的最早开始时间ae和最晚开始时间al 
	4 找出i.ae等于i.al，即松弛时间为0的活动i，即为关键活动
*/
void CriticalPath(PAdjList& pal, PCriticalPathItem& pcpi);
//输出关键路径
void PrintCriticalPathItem(PAdjList& pal, PCriticalPathItem& pcpi);

void TestCriticalPath();
void TestTopoSort();

void TestGraphDAG();