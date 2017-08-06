#pragma once
#include "graph_list.h"
#include "queue_ds.h"
using namespace DS;
/*	��Ҫʵ��ͼ��Ӧ��	*/

typedef struct GraphContentCompont
{
	PQueue q;
	GraphContentCompont* next;
}GraphContentCompont, *PGraphContentCompont;

void InitGraphContentCompont(PGraphContentCompont& pgcc);
void FreeGraphContentCompont(PGraphContentCompont& pgcc);

/*	�ж����������Ƿ���ͨ�������ڽ����ά�Թ�·������	*/
bool IsConnect(PAdjList& pal, const VertexData src, const VertexData dst);

/*	����ͼӦ�ã���ͨ����
*	����������Ȼ������ȱ�����������ͨ������ͳ����ͨͼ������
*	��������˶����ͨ������˵����ͼ����ͨͼ��	*/
/*	������ͼ��ͨ�ĸ����ǿ��ͨͼ��������X��Y�ɻ�����ͨ���
*	����û��ʵ������ǿ��ͨ�����ķ���	*/
void MakeConnectComponents(PAdjList& pal, PGraphContentCompont pgcc);

void PrintGraphContentCompont(PGraphContentCompont& pgcc);

/*  ��ȡ�����������ͨ����		*/
void MakeConnectComponentForVertex(PAdjList& pal, int index_src, PQueue& q);

/*	����������ͨ������Ȩ���У����ߵĴ���֮����С����������Ϊ
*	��С������������Ҳ����С��������������С������������������㷨��
*	Prim�㷨��Kruskal�㷨��	*/

/*  Prim�㷨˼�룺�ӵ㲻���ɻ�·��
*	1. ��ѡһ��������Ϊ��ʼ�����㣬���������㼯��B�������㹹�ɷ������㼯��U��
*	2. �������㼯��ѡһ����b���ڷ������㼯��ѡһ����u��ʹ��(u,b)�ߴ�����С��
*		���˱߷����������߼���TE��ͬʱ��u���ƶ��������㼯�ϡ�	
*	3. �ظ�2���ظ�����n-1��ֱ���������㼯��Ϊ�ա�
*	4. ��ʱTE�����к���n-1���ߣ�TE�߼��ϺͶ��㼯�Ϲ�������С��������	*/
/*  �㷨ʵ���п����Ż��ĵط���
*	1. ������ͷ���������Թ�������ռ�
*	2. �����㼯�����Ѵ��ڵĵ㣬���������㼯����ÿ����ľ��벻��ÿ�αȽϵó���
*		���Լ�¼ÿ���������㵽�������㼯�ϵ��ĸ���Ȩֵ��С��Ȩֵ�Ƕ��٣���ѡ������������ʱ��
		ֻҪ�Ƚ�ÿ���������㵽�����������Ȩֵ��Ȼ�����θ��������ÿ����Сֵ���ɡ�
*	3. ����ʵ�ֲο����ǹ����ƴ�Ŀγ̡������㡢�������㡢��СȨֵ������һ���ṹ�
		Ȼ����һ��������ṹΪԪ�ص����飬��Ϊ�����㡢�������ġ���СȨֵ���ü��ϡ�
*	4. ʹ���ڽӾ����Ȩֵ�ȽϿ죬�����϶�Ҳ�ʺ�ʹ���ڽӾ����������ڽӱ������ */

typedef struct MST_Edge
{
	int grow_vertex;	//������
	int vertex;			//��������
	int weight;			//Ȩ
}MST_Edge, *PMST_Edge;

void MinSpanningTree_Prim(PAdjList& pal, int init_grow, PMST_Edge& pmste);
void PrintMST_Edge(PAdjList& pal, PMST_Edge& pmste, int size);
void CopyMST_Edge(MST_Edge& esrc, MST_Edge& edst);

/*  Kruskal�㷨˼�룺�ӱ߲����ɻ�·��
*	1. ��n�����㿴��n������
*	2. ��Ȩֵ��С�����˳��ѡ��ߣ�ѡ��Ӧ����˱߲����֮ǰ����ѡ�߹��ɻ�·�����ñ߷ŵ��������ߵļ���TE�С�
*		ͬʱ���ñߵ������������ڵĶ��㼯�Ϻϲ�
*	3. �ظ�2��ֱ�����еĶ��㶼��ͬһ�����㼯����
*	4. ��ʱTE�����к���n-1���ߣ�TE�߼��ϺͶ��㼯�Ϲ�������С��������
*	5. ����㷨�Ĺؼ���:�ж��µ�ѡ�߲����ԭ��ѡ�߹��ɻ�·��
*		��Ϊѡ�߹�����ʵ��ѡ�����㹹����������ѡ�����㹹������ ���� ԭ���������ɸ��������������
*		�����ͻ��γ���Щ����ϼ������ӣ���Щ����ϼ�����գ��������ת�����˶��㼯�ϵĺϲ�����
*		����Ľ��˼�����ж�����ѡ�ߵ�����������û��ͬʱ������ĳ�����㼯���У����ͬʱ������ĳ�����㼯�ϣ�
*		��˵�����ѡ����ĳ�������Ϲ����˻�·����֮��û�й��ɻ�·	*/
/*  �㷨ʵ���п����Ż��ĵط���
*	1. ���б���������һ��������ڰ�Ȩֵ��С����ѡ���������߿��Թ����������
*	2. Ϊ��ʵ�ֶ��㼯�ϺͲ��Ҷ������ڼ��Ϸ��㣬����Ԫ�����ݽṹ���������ڼ��ϵ�������������ϵ���һ�������ָ�롣
		����Ԫ�ر�����һ�������У������������ҡ�
		����Ԫ�ص����ݽṹ��������Ԫ����������һ������Ԫ��ָ�루������Ϊ�˷��㶥����ٶ�λ�����ڼ��ϣ�
		����Ԫ��Ҳ�����������С�
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


