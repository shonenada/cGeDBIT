#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "../../HeaderFiles/objects/DNA.h"
#include "../../HeaderFiles/metric/DNAMetric.h"

using namespace std;

int main(){

	clock_t start,finish;
	double totaltime;
	start=clock();

	string filename = "data/arab1.con";
	int maxSize = 100000;
    int fragmentLength = 18;

	ofstream outfile("data/arab1-cpp.result", ios::out);

	cout << "Begin loadding file" << endl;
	vector<shared_ptr<CDNA>> list = CDNA::loadData(filename, maxSize, fragmentLength);
	CDNAMetric metric(&CDNA::EditDistanceMatrix[0][0], DNASYMBOLNUMBER);

	double dist;
	for(int i=0;i<list.size()-1;i++){
		dist = metric.getDistance(list[0].get(), list[i+1].get());
		outfile << dist << ".0" <<endl;
	}

	finish=clock();
	totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
	cout << "Execute Time: " << totaltime << " seconds" << endl;

	system("pause");
	return 1;
}
