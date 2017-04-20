#include "GetSimilarity.h"

void GetSimilarity::readToMap() {
	unordered_map<wstring, double> *p = nullptr;
	if(timer == true)
		cout <<"Reading file and count words started at:" << clock() << "ms" << endl;
	int cwords = 0;
	int cfiles = 0;
	wstring wstr;
	while (getline(winfile, wstr)) {
		if (wstr.size() == 0)
			continue;
		auto p1 = wstr.begin(), p2 = p1;
		if (wstr[16] == '0'&&wstr[17] == '0'&&wstr[18] == '1') {//�����16,17,18���ַ�Ϊ001,���ʾ�¿�ʼһƪ����
			if (tf.size() != 0) {
				for (auto &it : **tf.rbegin())
					it.second /= cwords;//ǰһ��map�е�ÿ��value���������е��ܴ���.
				cwords = 0;
			}
			textname.push_back(string(wstr.begin(), wstr.begin() + 15));//��¼���±���.
			p = new unordered_map<wstring, double>;//Ϊÿƪ���´���һ���µ�map.
			tf.push_back(p);//ָ�����tf vector��.
		}
		while (1) {
			while (*(p1++) != ' ');
			p2 = ++p1;
			if (p1 == wstr.end())
				break;
			while (*(++p2) != ' ');
			p2--;
			if (*p2 != 'r'&&*p2 != 'p'&&*p2 != 'c'&&*p2 != 'u' \
				&&*p2 != 'y'&&*p2 != 'e'&&*p2 != 'o'&&*p2 != 'w'&&*p2 != 'q') {//ȥ����Ч��.
				while (*(--p2) != '/');
				if (p->insert({ wstring(p1, p2),1 }).second == 0)//���Բ��뵽tf��map��,���ʧ��,
					p->find(wstring(p1, p2))->second++;//�����key�Դ���,��value++.
				else//�������ɹ�,���ʾ�ùؼ�������ƪ�����е�һ�γ���,
					if (idf.insert({ wstring(p1, p2),1 }).second == 0)//��ʱ���Բ��뵽idf map��.����ó���ʧ��,
						idf.find(wstring(p1, p2))->second++;//���ʾ��һ�ؼ����������������г��ֹ�,value++����.
				cwords++;
				p1 = p2;
			}
		}
	}
	auto it = *tf.rbegin();
	for (auto it2 = it->begin(); it2 != it->end(); it2++)
		it2->second /= cwords;
	cwords = 0;
	cfiles = tf.size();
	for (auto &i : idf) {
		i.second = cfiles / i.second;
		i.second = log(i.second);
	}
	for (auto &i : tf)
		for (auto &j : *i)
			j.second *= idf.find(j.first)->second;
}

void GetSimilarity::culculateSim() {
	if (timer == true)
		cout << "Culculate similarity and output file started at:" << clock() << "ms" << endl;
	double InnerProduct = 0;
	double l1 = 0, l2 = 0;
	double answer;
	auto itname = textname.begin();
	for (auto it1 = tf.begin(); it1 != tf.end(); it1++) {
		outfile << *itname << ":";
		itname++;
		for (auto it2 = tf.begin(); it2 != it1 + 1; it2++) {
			for (auto &word : **it1) {
				if ((**it2).find(word.first) != (**it2).end())
					InnerProduct += word.second * (**it2).find(word.first)->second;//����ÿ��ƪ����Ȩ�ص��ڻ�.
				l1 += word.second * word.second;//����ÿ��Ȩ�ص�ƽ��.
			}
			for (auto &word : **it2)
				l2 += word.second*word.second;//����ÿ��Ȩ�ص�ƽ��.
			answer = InnerProduct / (sqrt(l1) * sqrt(l2));//������ض�.
			InnerProduct = 0;
			l1 = 0;
			l2 = 0;
			outfile << answer << ' ';//�����ض�.
		}
		outfile << endl;
	}
	if (timer == true)
		cout << "Finish at:" << clock() << "ms" << endl;
}

void GetSimilarity::operator()() {
	readToMap();
	culculateSim();
}