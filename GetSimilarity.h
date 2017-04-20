#pragma once
#include<iostream> 
#include<fstream>
#include<locale> 
#include<string>
#include<unordered_map>
#include<vector> 
#include<math.h> 
#include<time.h>
using namespace std;

class GetSimilarity {
public:
	GetSimilarity(bool tm = false, string inputname = "199801_clear.txt",string outname="answer.txt"): timer(tm) {
		winfile.imbue(locale("chs"));
		winfile.open(inputname);
		outfile.open(outname);
	}
	~GetSimilarity() {
		for (auto ptr : tf)
			delete ptr;
	}
	GetSimilarity(GetSimilarity &) = delete;
	GetSimilarity &operator=(const GetSimilarity &) = delete;
	void operator()();
private:
	unordered_map<wstring, double> idf;
	vector<unordered_map<wstring, double>*> tf;
	vector<string> textname;
	wifstream winfile;
	ofstream outfile;
	bool timer;
	void readToMap();
	void culculateSim();
};