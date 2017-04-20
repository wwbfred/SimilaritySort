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
		if (wstr[16] == '0'&&wstr[17] == '0'&&wstr[18] == '1') {//如果第16,17,18个字符为001,则表示新开始一篇文章
			if (tf.size() != 0) {
				for (auto &it : **tf.rbegin())
					it.second /= cwords;//前一个map中的每个value除以文章中的总词数.
				cwords = 0;
			}
			textname.push_back(string(wstr.begin(), wstr.begin() + 15));//记录文章标题.
			p = new unordered_map<wstring, double>;//为每篇文章创建一个新的map.
			tf.push_back(p);//指针放在tf vector中.
		}
		while (1) {
			while (*(p1++) != ' ');
			p2 = ++p1;
			if (p1 == wstr.end())
				break;
			while (*(++p2) != ' ');
			p2--;
			if (*p2 != 'r'&&*p2 != 'p'&&*p2 != 'c'&&*p2 != 'u' \
				&&*p2 != 'y'&&*p2 != 'e'&&*p2 != 'o'&&*p2 != 'w'&&*p2 != 'q') {//去除无效词.
				while (*(--p2) != '/');
				if (p->insert({ wstring(p1, p2),1 }).second == 0)//尝试插入到tf的map中,如果失败,
					p->find(wstring(p1, p2))->second++;//则表明key以存在,则value++.
				else//如果插入成功,则表示该关键词在这篇文章中第一次出现,
					if (idf.insert({ wstring(p1, p2),1 }).second == 0)//此时尝试插入到idf map中.如果该尝试失败,
						idf.find(wstring(p1, p2))->second++;//则表示这一关键词曾在其他文章中出现过,value++即可.
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
					InnerProduct += word.second * (**it2).find(word.first)->second;//计算每两篇文章权重的内积.
				l1 += word.second * word.second;//计算每个权重的平方.
			}
			for (auto &word : **it2)
				l2 += word.second*word.second;//计算每个权重的平方.
			answer = InnerProduct / (sqrt(l1) * sqrt(l2));//计算相关度.
			InnerProduct = 0;
			l1 = 0;
			l2 = 0;
			outfile << answer << ' ';//输出相关度.
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