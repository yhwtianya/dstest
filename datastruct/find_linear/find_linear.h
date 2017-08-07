#pragma once
/*  基于线性表的查找法 */

/*	线性表查找法，存储结构可以是顺序表或链表 */
/*  查找的性能指标：平均查找长度ASL	
*	ASL = P1C1+P2C2+...+PnCn
*	Pi为查找第i个元素的概论，Ci为找到第i个元素的比较次数 */

/*  顺序查找法
*	顺序查找法的ASL为(n+1)/2	*/
#define LIST_SIZE		20
typedef int OtherType;
typedef int KeyType;
typedef struct
{
	KeyType key;
	OtherType oter_data;
}RecordType;

typedef struct
{
	//r[0]用作工作单元
	RecordType r[LIST_SIZE + 1];
	int length;
}RecordList;

//顺序查找法，不设置监视哨，从后往前
int SeqSearch(RecordList l, KeyType k);

//顺序查找法，设置监视哨，从后往前
//由于设置监视哨，改变了列表值
int SeqSearch_Post(RecordList l, KeyType k);

/*  折半查找法：也叫二分查找法,要求
*	1 采用顺序存储
*	2 关键字有序排列*/
int BinSearch(RecordList l, KeyType k);

/*  分块查找法
*	当n比较大时，可以采用分块查找法，要求：
	1 等长分块，最后一块可以不满 
	2 块内无序
	3 块间有序
	4 分块构造索引表
	5 定块可以采用顺序或折半查找关键字K与索引表关键字比较，确定该查找记录所在块 
	6 块内顺序查找 */
//分块查找暂不实现

void TestFindLinear();