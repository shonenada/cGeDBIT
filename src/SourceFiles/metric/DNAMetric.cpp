#include "../../HeaderFiles/metric/DNAMetric.h"

CDNAMetric::CDNAMetric(double* dist, int _distWidth){
	distWidth = _distWidth;
	distance = new double[_distWidth * _distWidth];
	int i, j;
	for(i=0;i<_distWidth;i++){
		for(j=0;j<_distWidth;j++){
			distance[ i * _distWidth + j ] = dist[i * _distWidth + j];
		}
	}
}

double CDNAMetric::getDistance(CIndexObject* one, CIndexObject* two)
{
	CDNA* new_one = (CDNA*) one;
	CDNA* new_two = (CDNA*) two;
	return getDistance(new_one, new_two);
}

double CDNAMetric::getDistance(CDNA* one, CDNA* two)
{
	int i;
	int one_size = one->getSize();
	int two_size = two->getSize();
	if(one_size != two_size){
		cerr << "Stop! Two DNA must have the same length.";
		return -1.0;
	}
	double dist = 0.0;
	vector<int> one_byte = one->getBytes();
	vector<int> two_byte = two->getBytes();
	for(i=0;i<one_size;i++){
		dist = dist + distance[one_byte[i] + distWidth * two_byte[i] ];
	}
	return dist;
}
