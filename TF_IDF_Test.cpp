#include"TF_IDF.h"

void TestWordFreq()
{
	TF_IDF tsy;  //��������
	unordered_map<string, double> wfg1;  //�ĵ�1�Ĵ�Ƶ
	unordered_map<string, double> wfg2;  //�ĵ�2�Ĵ�Ƶ
	const char* test1 = "E://test1.txt";
	const char* test2 = "E://test2.txt";

	wfg1 = tsy.getWordFreq(test1);
	wfg2 = tsy.getWordFreq(test2);

	//auto�Զ�ʶ��Ϊ����������unordered_map<string,int>::iterator
	auto iter1 = wfg1.begin();
	cout << "�ĵ�1�Ĵ�Ƶ--------------------------------------" << endl;
	while (iter1 != wfg1.end())  //��ӡ�ĵ�1�Ĵ�Ƶ
	{
		cout << tsy.utf8_gbk(iter1->first) << ":" << iter1->second << endl;;
		++iter1;
	}
	cout << wfg1.size() << endl;

	auto iter2 = wfg2.begin();
	cout << "�ĵ�2�Ĵ�Ƶ--------------------------------------" << endl;
	while (iter2 != wfg2.end())  //��ӡ�ĵ�2�Ĵ�Ƶ
	{
		cout << tsy.utf8_gbk(iter2->first) << ":" << iter2->second << endl;
		++iter2;
	}
	cout << wfg2.size() << endl;
}


void TestSort()  //�������ĵ��Ĵ�Ƶ���н�������
{
	TF_IDF tsy;  //��������
	unordered_map<string, double> wfg1;  //�ĵ�1�Ĵ�Ƶ
	unordered_map<string, double> wfg2;  //�ĵ�2�Ĵ�Ƶ
	const char* test1 = "E://test1.txt";
	const char* test2 = "E://test2.txt";

	wfg1 = tsy.getWordFreq(test1);
	wfg2 = tsy.getWordFreq(test2);

	vector<pair<string, double>> v1 = tsy.sortByValueReverse(wfg1);  //���ĵ�1�õ��Ĵ�Ƶ��������
	vector<pair<string, double>> v2 = tsy.sortByValueReverse(wfg2);  //���ĵ�2�õ��Ĵ�Ƶ��������

	auto iter1 = v1.begin();
	cout << "�ĵ�1�Ĵ�Ƶ��������--------------------------------------" << endl;
	while (iter1 != v1.end())  //��ӡ�õ����ĵ�1�Ĵ�Ƶ����������
	{
		cout << tsy.utf8_gbk(iter1->first) << ":" << iter1->second << endl;
		++iter1;
	}

	auto iter2 = v2.begin();
	cout << "�ĵ�2�Ĵ�Ƶ��������--------------------------------------" << endl;
	while (iter2 != v2.end())  //��ӡ�õ����ĵ�2�Ĵ�Ƶ����������
	{
		cout << tsy.utf8_gbk(iter2->first) << ":" << iter2->second << endl;
		++iter2;
	}
}

void TestOneHot()  //�õ���Ƶ����
{
	TF_IDF tsy;  //��������
	unordered_map<string, double> wfg1;  //�ĵ�1�Ĵ�Ƶ
	unordered_map<string, double> wfg2;  //�ĵ�2�Ĵ�Ƶ
	const char* test1 = "E://test1.txt";
	const char* test2 = "E://test2.txt";

	wfg1 = tsy.getWordFreq(test1);
	wfg2 = tsy.getWordFreq(test2);

	vector<pair<string, double>> v1 = tsy.sortByValueReverse(wfg1);  //���ĵ�1�õ��Ĵ�Ƶ��������
	vector<pair<string, double>> v2 = tsy.sortByValueReverse(wfg2);  //���ĵ�2�õ��Ĵ�Ƶ��������

	unordered_set<string> wset1;  //�������ĵ�1������õ�Ԫ��
	unordered_set<string> wset2;  //�������ĵ�2������ŵ�Ԫ��
	tsy.selectAimWords(v1, wset1);
	tsy.selectAimWords(v2, wset2); //����������������vector�е�Ԫ�ش浽set�У�������Ƶ����


	vector<double> vd1;
	vector<double> vd2;
	vd1 = tsy.getOneHot(wset1, wfg1);  //�õ��ĵ�1�Ĺ����õĴ�Ƶ����
	vd2 = tsy.getOneHot(wset2, wfg2); //�õ��ĵ�2�Ĺ����õĴ�Ƶ���� 

	for (int i = 0; i < vd1.size(); i++)  //��ӡ�ĵ�1�Ĵ�Ƶ����
	{
		cout << "�ĵ�1 �Ĵ�Ƶ����" << i + 1 << ":" << vd1[i] << "\t";
	}
	cout << endl;

	for (int i = 0; i < vd2.size(); i++)  //��ӡ�ĵ�2�Ĵ�Ƶ����
	{
		cout << "�ĵ�2 �Ĵ�Ƶ����" << i + 1 << ":" << vd2[i] << "\t";
	}
	cout << endl;
}

void TestConine()  //���������ĵ����ı����ƶ�
{
	TF_IDF tsy;  //��������
	unordered_map<string, double> wfg1;  //�ĵ�1�Ĵ�Ƶ
	unordered_map<string, double> wfg2;  //�ĵ�2�Ĵ�Ƶ
	const char* test1 = "E://test1.txt";
	const char* test2 = "E://test2.txt";

	wfg1 = tsy.getWordFreq(test1);
	wfg2 = tsy.getWordFreq(test2);

	vector<pair<string, double>> v1 = tsy.sortByValueReverse(wfg1);  //���ĵ�1�õ��Ĵ�Ƶ��������
	vector<pair<string, double>> v2 = tsy.sortByValueReverse(wfg2);  //���ĵ�2�õ��Ĵ�Ƶ��������

	unordered_set<string> wset1;  //�������ĵ�1������õ�Ԫ��
	unordered_set<string> wset2;  //�������ĵ�2������ŵ�Ԫ��
	tsy.selectAimWords(v1, wset1);
	tsy.selectAimWords(v2, wset2); //����������������vector�е�Ԫ�ش浽set�У�������Ƶ����


	vector<double> vd1;
	vector<double> vd2;
	vd1 = tsy.getOneHot(wset1, wfg1);  //�õ��ĵ�1�Ĺ����õĴ�Ƶ����
	vd2 = tsy.getOneHot(wset2, wfg2); //�õ��ĵ�2�Ĺ����õĴ�Ƶ���� 

	double res = tsy.cosine(vd1, vd2);   //���������ı����ı����ƶ�
	cout << "�����ĵ����ı����ƶ�:" << res << endl;
}

int main()
{
	//TestShowWordFreq();    //���Դ�ӡ�������ĵ��Ĵ�Ƶ
	//TestSort();  //�������ĵ��Ĵ�Ƶ���н�������
	//TestOneHot();  //�õ���Ƶ����
	TestConine();  //���������ĵ����ı����ƶ�
	system("pause");
	return 0;
}


