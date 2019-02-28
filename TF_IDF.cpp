
#include"TF_IDF.h"
#include"utf8_gbk.h"

using namespace std;

TF_IDF::TF_IDF() //���幹�캯��
	:_jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH), _maxWordNumber(10)
{
	getStopWordTable(STOP_WORD_PATH.c_str());
	GetIdfWords(IDF_PATH.c_str());
}

void TF_IDF::getStopWordTable(const char* stopWordFile)  //�õ�ͣ�ôʱ�
{
	ifstream isf(stopWordFile);
	if (!isf.is_open())
	{
		cout << "open file:" << stopWordFile << endl;
		return;
	}

	while (!isf.eof())
	{
		string str;
		getline(isf, str);
		stopWordSet.insert(str);
	}
	isf.close();
}


void TF_IDF::GetIdfWords(const char* IdfFile)   //�õ�IDF 
{
	ifstream isf(IdfFile);
	if (!isf.is_open())
	{
		cout << "open file:" << IdfFile << endl;
		return;
	}

	while (!isf.eof())
	{
		string str;
		double dbData;
		isf >> str >> dbData;
		idfWordMap.insert(pair<string, double>(str, dbData));
	}
	isf.close();
}


unordered_map<string, double> TF_IDF::getWordFreq(const char* filename)  //�õ���Ƶ
{
	ifstream fln(filename);
	if (!fln.is_open())
	{
		cout << "open file:" << filename << "failed";
		return unordered_map<string, double>();
	}

	string line;
	unordered_map<string, int> wf;

	while (!fln.eof())
	{
		getline(fln, line);
		// GBK --> UTF8
		line = gbk_utf8(line);
		vector<string> words;
		// ���ı���ǰ�зִ�
		_jieba.Cut(line, words, true);
		// ͳ�ƴ�Ƶ
		for (const auto& e : words)
		{
			// ȥ��ͣ�ô�
			if (stopWordSet.count(e) > 0)  //count�������set��������ʾͷ���1
				continue;
			else
			{
				// ͳ�ƴ�Ƶ
				if (wf.count(e) > 0)
					wf[e]++;
				else
					wf[e] = 1;
			}
		}
	}

	unordered_map<string, double> res;
	auto wfIter = wf.begin();
	int count = wf.size();
	while (wfIter != wf.end())
	{
		res.insert(pair<string,double>(wfIter->first, (double)(((double)(wfIter->second)) / (double)(count))));
		++wfIter;
	}
	return res;
}

bool cmpReverse(pair<string, double> lp, pair<string, double> rp)  //�ȽϺ���
{
	return lp.second > rp.second;
}

vector<pair<string, double>> TF_IDF::sortByValueReverse(unordered_map<string, double>& wf) //�����������ս������У�
{
	vector<pair<string, double>> wfvector(wf.begin(), wf.end());
	sort(wfvector.begin(), wfvector.end(), cmpReverse);
	return wfvector;
}

void TF_IDF::selectAimWords(vector<pair<string, double>>&wfvec, unordered_set<string>& wset)  //������õ�Ԫ�ط���set��
{
	int len = wfvec.size();
	int sz = len > _maxWordNumber ? _maxWordNumber : len;
	for (int i = 0; i < sz; i++)
	{
		wset.insert(wfvec[i].first);
	}
}

vector<double> TF_IDF::getOneHot(unordered_set<string>& wset, unordered_map<string, double>& wf)  //����Ԫ������
{
	auto wfIter = wf.begin();
	auto idfIter = idfWordMap.begin();
	while(wfIter != wf.end())
	{
		while (idfIter != idfWordMap.end())
		{
			if (wfIter->first == idfIter->first)
				wfIter->second *= idfIter->second;
		}
	}

	//����wset�е�ÿһ����
	vector<double> oneHot;
	for (const auto& e : wset)
	{
		if (wf.count(e))
			oneHot.push_back(wf[e]);
		else
			oneHot.push_back(0);
	}
	return oneHot;
}


double TF_IDF::cosine(vector<double> oneHot1, vector<double> oneHot2)  //���������������ƶ�
{
	double modular1 = 0, modular2 = 0;
	double products = 0;
	assert(oneHot1.size() == oneHot2.size());
	for (int i = 0; i < oneHot1.size(); i++)
	{
		products += oneHot1[i] * oneHot2[2];
	}

	for (int i = 0; i < oneHot1.size(); i++)
	{
		modular1 += pow(oneHot1[i], 2);
	}
	modular1 = pow(modular1, 0.5);

	for (int i = 0; i < oneHot2.size(); i++)
	{
		modular2 += pow(oneHot1[i], 2);
	}
	modular2 = pow(modular2, 0.5);

	return products / (modular2 * modular1);
}


string TF_IDF::gbk_utf8(string str)
{
	//��ȡbuffer��С
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	//GBK-->UTF16
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* utf8char = new char[len];
	//utf16-->utf8
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8char, len, NULL, NULL);
	string out(utf8char);

	if (wstr != NULL)
	{
		delete[] wstr;
		wstr = NULL;
	}
	if (utf8char != NULL)
	{
		delete[] utf8char;
		utf8char = NULL;
	}
	return out;
}

string TF_IDF::utf8_gbk(string str)
{
	//��ȡbuffer��С
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	//utf8-->utf16
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);

	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* gbkchar = new char[len];
	//utf16--->gbk
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, gbkchar, len, NULL, NULL);

	string out(gbkchar);

	if (wstr != NULL)
	{
		delete[] wstr;
		wstr = NULL;
	}

	if (gbkchar != NULL)
	{
		delete[] gbkchar;
		gbkchar = NULL;
	}

	return out;
}

double TF_IDF::GetCosine(const char* txt1, const char* txt2)  //�����к�����װ��һ���ӿڣ��������ĵ����ı����ƶ�
{
	TF_IDF tsy;  //��������
	unordered_map<string, double> wfg1;  //�ĵ�1�Ĵ�Ƶ
	unordered_map<string, double> wfg2;  //�ĵ�2�Ĵ�Ƶ
	const char* test1 = txt1;
	const char* test2 = txt2;

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
	return res;
}