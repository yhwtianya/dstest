#include "pq_complheap.h"
#include <time.h>
#include <iostream>
#include <fstream>

void testPQ_ComlHeap()
{
	//用于问题重现
	//reappear_auto();
	//reappear_auto2();
	//reappear_manual();

	PQ_ComplHeap<int> pq_ch;
	srand((unsigned)time(NULL));
	int maxkey = 99;
	int minkey = 1;
	int count = 15;
	int key = 0;
	vector<int> record;
	//ofstream outfile("outfile.txt");
	//ostream& os = outfile;
	ostream& os = cout;

	os << "随机插入节点 :\n";
	for (int i = 0; i < count; i++)
	{
		key = (rand() % (maxkey - minkey + 1)) + minkey;
		//key = count -i;
		os << "--------------\n\n";
		os << "插入：" << key << endl << "-" << endl << flush;
		pq_ch.pushPQ(key);
		record.push_back(key);
		pq_ch.printPQ(os);
	}
	saveDump(record);

	while (!pq_ch.empty())
	{
		os << "--------------\n\n";
		key = pq_ch.popPQ();
		os << "删除：" << key << endl << "-" << endl << flush;
		pq_ch.printPQ(os);
	}

	record.clear();
	int* A = new int[count];
	for (int i = 0; i < count; i++)
	{
		key = (rand() % (maxkey - minkey + 1)) + minkey;
		A[i] = key;
		record.push_back(key);
	}
	saveDump(record);

	os << "\n批量构造PQ：" << endl << "-" << endl;
	pq_ch.creatPQ(A, count);
	pq_ch.printPQ(os);
	pq_ch.heapSort();
	os << "\nPQ堆排序：" << endl << "-" << endl;
	pq_ch.printOrder(os);
	reverse(pq_ch.begin(), pq_ch.end());
	os << "\n翻转：" << endl << "-" << endl;
	pq_ch.printOrder(os);
	os << "\n堆化：" << endl << "-" << endl;
	pq_ch.heapify();
	pq_ch.printPQ(os);
	os << "\nPQ堆排序：" << endl << "-" << endl;
	pq_ch.heapSort();
	pq_ch.printOrder(os);

	return;
}

void reappear_auto()
{
	//return;
	cout << "\n-------进入问题重现模式 自动模式-------\n" << endl;
	ifstream in("dumptest.txt");
	int buf_size = 512;
	char *buf = new char[buf_size];
	memset(buf, 0, buf_size);
	in.getline(buf, buf_size);
	in.close();

	vector<int> record;
	char* p = buf;
	while ('\0' != *p)
	{
		while (*p == ',' || *p == ' ')
		{
			++p;
		}
		record.push_back(atoi(p));

		++p;
		while ('\0' != *p && *p != ',')
		{
			++p;
		}
	}

	int key = 0;
	ostream& os = cout;
	PQ_ComplHeap<int> pq_ch;
	for (size_t i = 0; i < record.size(); i++)
	{
		key = record[i];
		cout << "插入：" << record[i] << endl << '_' << endl;
		pq_ch.pushPQ(key);
		pq_ch.printPQ();
		cout << "---------\n";
	}

	while (!pq_ch.empty())
	{
		os << "--------------\n\n";
		key = pq_ch.popPQ();
		os << "删除：" << key << endl << "-" << endl << flush;
		pq_ch.printPQ(os);
	}

	exit(0);
}

void reappear_auto2()
{
	//return;
	cout << "\n-------进入问题重现模式 自动模式-------\n" << endl;
	ifstream in("dumptest.txt");
	int buf_size = 512;
	char *buf = new char[buf_size];
	memset(buf, 0, buf_size);
	in.getline(buf, buf_size);
	in.close();

	vector<int> record;
	char* p = buf;
	while ('\0' != *p)
	{
		while (*p == ',' || *p == ' ')
		{
			++p;
		}
		record.push_back(atoi(p));

		++p;
		while ('\0' != *p && *p != ',')
		{
			++p;
		}
	}

	int count = record.size();
	int* A = new int[count];

	int key = 0;
	ostream& os = cout;
	PQ_ComplHeap<int> pq_ch;
	for (size_t i = 0; i < record.size(); i++)
	{
		key = record[i];
		A[i] = key;
	}

	os << "\n批量构造PQ：" << endl << "-" << endl;
	pq_ch.creatPQ(A, count);
	pq_ch.printPQ(os);
	pq_ch.heapSort();
	os << "\nPQ堆排序：" << endl << "-" << endl;
	pq_ch.printOrder(os);

	exit(0);
}

void reappear_manual()
{
	//return;
	cout << "\n-------进入问题重现模式 手动模式-------\n" << endl;
	vector<int> record = { 56,46,73,97,66 };
	PQ_ComplHeap<int> pq_ch;
	for (size_t i = 0; i < record.size(); i++)
	{
		int key = record[i];
		cout << "插入：" << record[i] << endl << '_' << endl;
		pq_ch.pushPQ(key);
		//pTree->printInOrder(os);
		pq_ch.printPQ();
		cout << "---------\n";
	}

	exit(0);
}

template<class T>
void printVector(const vector<T>& record, ostream& os/*=cout*/)
{
	os << "\nitems: {";
	for (int i = 0; i < record.size(); ++i)
	{
		os << record[i];
		if (i + 1 < record.size())
			os << ", ";
		else
			os << "}";
	}
	os << endl;
}

template<class T>
void saveDump(const vector<T>& record)
{
	ofstream out("dumptest.txt");
	for (size_t i = 0; i < record.size(); i++)
	{
		out << record[i];
		if (i + 1 != record.size())
			out << ", ";
	}
	out << endl;
	out.close();
}