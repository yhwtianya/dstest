#include "pq_left_heap.h"
#include <time.h>
#include <iostream>
#include <fstream>

void testPQ_ComlHeap()
{
	//用于问题重现
	//reappear_auto();
	//reappear_auto2();
	//reappear_manual();

	PQ_LeftHeap<int> pq_ch;
	srand((unsigned)time(NULL));
	int maxkey = 99;
	int minkey = 1;
	int count = 6;
	int key = 0;
	vector<int> record;
	//ofstream outfile("outfile.txt");
	//ostream& os = outfile;
	ostream& os = cout;

	os << "构造左式堆1 :\n";
	for (int i = 0; i < count; i++)
	{
		key = (rand() % (maxkey - minkey + 1)) + minkey;
		pq_ch.pushPQ(key);
		record.push_back(key);
	}
	pq_ch.printPQ(os);
	os << "\n数量:" << pq_ch.sizePQ() << endl << endl;
	saveDump(record);

	os << "构造左式堆2 :\n";
	PQ_LeftHeap<int> pq_ch2;
	count = 5;
	for (int i = 0; i < count; i++)
	{
		key = (rand() % (maxkey - minkey + 1)) + minkey;
		pq_ch2.pushPQ(key);
		record.push_back(key);
	}
	pq_ch2.printPQ(os);
	os << "\n数量:" << pq_ch2.sizePQ() << endl << endl;
	saveDump(record);

	os << "左式堆2 合并到 左式堆2 :\n";
	pq_ch.mergeLeftHeap(pq_ch2);
	pq_ch.printPQ(os);
	os << "\n数量:" << pq_ch.sizePQ() << endl << endl;


	while (!pq_ch.emptyPQ())
	{
		os << "--------------\n\n";
		key = pq_ch.popPQ();
		os << "删除：" << key << endl << "-" << endl << flush;
		pq_ch.printPQ(os);
	}

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
	PQ_LeftHeap<int> pq_ch;
	for (size_t i = 0; i < record.size(); i++)
	{
		key = record[i];
		cout << "插入：" << record[i] << endl << '_' << endl;
		pq_ch.pushPQ(key);
		pq_ch.printPQ();
		cout << "---------\n";
	}

	while (!pq_ch.emptyPQ())
	{
		os << "--------------\n\n";
		key = pq_ch.popPQ();
		os << "删除：" << key << endl << "-" << endl << flush;
		pq_ch.printPQ(os);
	}

	exit(0);
}

void reappear_manual()
{
	//return;
	cout << "\n-------进入问题重现模式 手动模式-------\n" << endl;
	vector<int> record = { 56,46,73,97,66 };
	PQ_LeftHeap<int> pq_ch;
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