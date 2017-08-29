#include "b_tree.h"
#include <time.h>
#include <iostream>
#include <fstream>

void testBTree()
{
	//������������
	//reappear_auto();
	//reappear_manual();

	srand((unsigned)time(NULL));
	int order = 4;
	BTree<int>* pTree = new BTree<int>(order);

	int maxkey = 99;
	int minkey = 1;
	int count = 40;
	int key = 0;
	vector<int> record;
	//ofstream outfile("outfile.txt");
	//ostream& os = outfile;
	ostream& os = cout;

	os << "�������ڵ� :\n";
	for (int i = 0; i < count; i++)
	{
		key = (rand() % (maxkey - minkey + 1)) + minkey;
		//key = i+1;
		os << "--------------\n\n";
		os << "���룺" << key << endl << "-" << endl << flush;
		pTree->insert(key);
		record.push_back(key);
		//pTree->printInOrder(os);
		pTree->print(os);
		//printVector(record, os);
	}
	//outfile.close();
	//����������е��ļ����������Զ���������
	saveDump(record, order);

	//vector<int>::reverse_iterator it = record.rbegin();
	//for (; it != record.rend(); ++ it)
	vector<int>::iterator it = record.begin();
	for (; it != record.end(); ++it)
	{
		key = /*record[record.size()-1]*/*it;
		os << "--------------\n\n";
		os << "ɾ����" << key << endl << "-" << endl << flush;
		pTree->remove(key);
		pTree->print(os);
	}

	return;
}

void reappear_auto()
{
	//return;
	cout << "\n-------������������ģʽ �Զ�ģʽ-------\n" << endl;
	ifstream in("dump.txt");
	int buf_size = 512;
	char *buf = new char[buf_size];
	memset(buf, 0, buf_size);
	in.getline(buf, buf_size);
	int order = atoi(buf);

	memset(buf, 0, buf_size);
	in.getline(buf, buf_size);
	in.close();
	
	if (order<3)
	{
		cout << "��ȡ����ʧ��" << endl;
		exit(-1);
	}

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

	BTree<int>* pTree = new BTree<int>(order);
	for (size_t i = 0; i < record.size(); i++)
	{
		cout << "���룺" << record[i] << endl << '_' << endl;
		pTree->insert(record[i]);
		pTree->print();
		cout << "---------\n";
	}
	//pTree->print();

	int key;
	ostream& os = cout;
	vector<int>::iterator it = record.begin();
	for (; it != record.end(); ++it)
	//vector<int>::reverse_iterator it = record.rbegin();
	//for (; it != record.rend(); ++it)
	{
		key = /*record[record.size()-1]*/*it;
		os << "--------------\n\n";
		os << "ɾ����" << key << endl << "-" << endl << flush;
		pTree->remove(key);
		pTree->print(os);
		cout << "";
	}

	exit(0);
}

void reappear_manual()
{
	//return;
	cout << "\n-------������������ģʽ �ֶ�ģʽ-------\n" << endl;
	vector<int> record = { 38, 13, 98, 65};
	BTree<int>* pTree = new BTree<int>(4);
	for (size_t i = 0; i < record.size(); i++)
	{
		cout << "���룺" << record[i] << endl << '_' << endl;
		pTree->insert(record[i]);
		pTree->print();
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
void saveDump(const vector<T>& record, int order)
{
	ofstream out("dump.txt");
	out << order << endl;
	for (size_t i = 0; i < record.size(); i++)
	{
		out << record[i];
		if (i + 1 != record.size())
			out << ", ";
	}
	out << endl;
	out.close();
}