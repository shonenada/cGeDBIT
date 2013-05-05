#include "../HeaderFiles/query/RangeQuery.h"
#include "../HeaderFiles/objects/DoubleVector.h"
#include "../HeaderFiles/metric/Metric.h"
#include "../HeaderFiles/index/Index.h"
#include "../HeaderFiles/index/ListIndex.h"
#include "../HeaderFiles/metric/EuclideanDistance.h"

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <string>



#include <boost/serialization/export.hpp>


BOOST_CLASS_EXPORT_GUID(CListIndex, "CListIndex");
BOOST_CLASS_EXPORT_GUID(CDoubleVector, "CDoubleVector");
BOOST_CLASS_EXPORT_GUID(CEuclideanDistance, "CEuclideanDistance");


void batchBulkLoad(char* inFileName, char* outFileName)
{
	//read the raw data from the data file stored in the external memory
	vector<CIndexObject*>indexdata = CDoubleVector::loadData(inFileName);

	CMetric* m = new CEuclideanDistance;
	CListIndex* index  = new CListIndex(m);
	
	//build an index;
	index->bulkLoad(indexdata);

	//write the index,built above,into hard disk
	ofstream out(outFileName);
	boost::archive::text_oarchive oa(out);	
	index->write(oa);

	out.close();
	out.clear();

	//input the index from the external memory
	CListIndex* indexx=new CListIndex;
	ifstream in(outFileName);
	boost::archive::text_iarchive ia(in);
	indexx->read(ia);
	
	
	//test the index 
	double d[5]={3.3,5.5,2.2,1.1,4.4};
	int dim=5;

	CDoubleVector *cd=new CDoubleVector(d,dim);

	double radius=200;
	CRangeQuery *q=new CRangeQuery(radius,cd);

	//search in the index
	vector<CIndexObject*> ve=indexx->search(q);

	cout<<"the size of result is:"<<ve.size()<<endl;
	for(vector<CIndexObject*>::size_type i=0;i!=ve.size();i++)
	{
		CDoubleVector *cdo=(CDoubleVector*)(ve[i]);

		for(int j=0;j<cdo->getLen();j++)
		{
			if(j==0)
				cout<<"(";
			if(j!=(cdo->getLen()-1))
				cout<<(cdo->getData())[j]<<",";
			else
				cout<<(cdo->getData())[j]<<")"<<endl;
		}
	}
	in.close();
	in.clear();
}



/**
  * -i input file name of data
  * -o output file of the index
  * -t define the type, now the parameter should be "dv" indicates DoubleVector after it
  */
int main(int argc, char* argv[])
{
	char* inFileName = NULL;
	char* outFileName = NULL;
	for (int i = 1; i < argc; i = i + 2)
	{
		if(!strcmp(argv[i], "-i"))
		{
			inFileName = new char[strlen(argv[i+1]) + 1];
			strcpy(inFileName, argv[i+1]);
		}
		else if(!strcmp(argv[i], "-o"))
		{
			outFileName = new char[strlen(argv[i+1]) + 1];
			strcpy(outFileName, argv[i+1]);
		}
	}
	batchBulkLoad(inFileName, outFileName);
	system("pause");

	return 0;
}