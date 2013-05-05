#include "../HeaderFiles/objects/IndexObject.h"
#include "../HeaderFiles/objects/DoubleVector.h"
#include "../HeaderFiles/query/Query.h"
#include "../HeaderFiles/query/RangeQuery.h"
#include "../HeaderFiles/index/Index.h"
#include "../HeaderFiles/index/ListIndex.h"
#include "../HeaderFiles/metric/Metric.h"
#include "../HeaderFiles/metric/EuclideanDistance.h"

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>


using namespace std;


vector<CIndexObject*> Queryi(string _IndexDataFileName, string _QueryDataFileName, double _radius)
{

	vector<CIndexObject*> indexData = CDoubleVector::loadData(_IndexDataFileName);
	vector<CIndexObject*> queryData = CDoubleVector::loadData(_QueryDataFileName);

	CRangeQuery* rangeQuery = new CRangeQuery(_radius,queryData[0]);

	CMetric *m = new CEuclideanDistance;
	CListIndex* listIndex = new CListIndex(m);
	

	listIndex->bulkLoad(indexData);
	
	vector<CIndexObject*> results = listIndex->search(rangeQuery);
	
	cout<<"results size:"<<results.size()<<endl;
	for(vector<CIndexObject*>::size_type ix=0;ix!=results.size();++ix)
	{
		CIndexObject *ob=results[ix];
		CDoubleVector *cd=(CDoubleVector*)ob;
		
		for(int i=0;i<cd->getLen();i++)
		{
			if(i==0)
				cout<<"(";
			if(i!=cd->getLen()-1)
				cout<<(cd->getData())[i]<<",";
			else
				cout<<(cd->getData())[i]<<")";
		}

		cout<<"\n";
	}

	return results;
}

/**
 * -q: 
 * -i:
 * -r:
 */
int main(int argc, char* argv[])
{
	string IndexFileName = "";
	string QueryDataFileName = "";
	double radius = 0;

	//get arguments
	for(int i=1; i<argc; i=i+2)
	{
		if(!strcmp("-q",argv[i]))
		{
			QueryDataFileName += argv[i+1];
		}
		else if(!strcmp("-i",argv[i]))
		{
			IndexFileName += argv[i+1];
		}
		else if(!strcmp("-r",argv[i]))
		{
			radius = atof(argv[i+1]);
		}
	}

	//check arguments
	if(IndexFileName == "")
		cout<<"indexfilename is null"<<endl;
	if(QueryDataFileName == "")
		cout<<"querydatafilename is null"<<endl;

	Queryi(IndexFileName,QueryDataFileName,radius);
}