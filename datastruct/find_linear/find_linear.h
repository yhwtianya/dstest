#pragma once
/*  �������Ա�Ĳ��ҷ� */

/*	���Ա���ҷ����洢�ṹ������˳�������� */
/*  ���ҵ�����ָ�꣺ƽ�����ҳ���ASL	
*	ASL = P1C1+P2C2+...+PnCn
*	PiΪ���ҵ�i��Ԫ�صĸ��ۣ�CiΪ�ҵ���i��Ԫ�صıȽϴ��� */

/*  ˳����ҷ�
*	˳����ҷ���ASLΪ(n+1)/2	*/
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
	//r[0]����������Ԫ
	RecordType r[LIST_SIZE + 1];
	int length;
}RecordList;

//˳����ҷ��������ü����ڣ��Ӻ���ǰ
int SeqSearch(RecordList l, KeyType k);

//˳����ҷ������ü����ڣ��Ӻ���ǰ
//�������ü����ڣ��ı����б�ֵ
int SeqSearch_Post(RecordList l, KeyType k);

/*  �۰���ҷ���Ҳ�ж��ֲ��ҷ�,Ҫ��
*	1 ����˳��洢
*	2 �ؼ�����������*/
int BinSearch(RecordList l, KeyType k);

/*  �ֿ���ҷ�
*	��n�Ƚϴ�ʱ�����Բ��÷ֿ���ҷ���Ҫ��
	1 �ȳ��ֿ飬���һ����Բ��� 
	2 ��������
	3 �������
	4 �ֿ鹹��������
	5 ������Բ���˳����۰���ҹؼ���K��������ؼ��ֱȽϣ�ȷ���ò��Ҽ�¼���ڿ� 
	6 ����˳����� */
//�ֿ�����ݲ�ʵ��

void TestFindLinear();