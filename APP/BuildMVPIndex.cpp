#include "..\HeaderFiles\objects\DoubleVector.h"
#include "..\HeaderFiles\metric\Metric.h"
#include "..\HeaderFiles\metric\EuclideanDistance.h"
#include "..\HeaderFiles\index\index.h"
#include "..\HeaderFiles\index\MVPIndex.h"
#include "..\HeaderFiles\indexalgorithm\PartitionMethod.h"
#include "..\HeaderFiles\indexalgorithm\PivotSelectionMethod.h"
#include "..\HeaderFiles\indexalgorithm\FFTPivotSelectionMethod.h"
#include "..\HeaderFiles\indexalgorithm\BalancePartitionMethod.h"
#include "..\HeaderFiles\query\Query.h"
#include "..\HeaderFiles\query\RangeQuery.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>

BOOST_CLASS_EXPORT_GUID(CMVPIndex,"CMVPIndex");
BOOST_CLASS_EXPORT_GUID(CIndexNode,"CIndexNode");
BOOST_CLASS_EXPORT_GUID(CMVPInternalNode,"CMVPInternalNode");
BOOST_CLASS_EXPORT_GUID(CMVPLeafNode,"CMVPLeafNode");
BOOST_CLASS_EXPORT_GUID(CPivotSelectionMethod,"CPivotSelectionMethod");
BOOST_CLASS_EXPORT_GUID(CPartitionMethod,"CPartitionMethod");
BOOST_CLASS_EXPORT_GUID(CDoubleVector,"CDoubleVector");
BOOST_CLASS_EXPORT_GUID(CEuclideanDistance,"CEuclideanDistance");
BOOST_CLASS_EXPORT_GUID(CFFTPivotSelectionMethod,"CFFTPivotSelectionMethod");
BOOST_CLASS_EXPORT_GUID(CBalancePartitionMethod,"CBalancePartitionMethod");


using namespace std;



void batchBulkLoad(char *&dataFileName,char *&mvpIndexFileName,char *&pivotSelectionMethod, char *&partitionMethod, int &numPivot, int &singlePivotFanout, int &maxLeafSize, int &maxPathLength)
{
	vector<CIndexObject*> rawData=CDoubleVector::loadData(dataFileName);

	CMetric *metric=new CEuclideanDistance;

	CPivotSelectionMethod *psm=0;
	CPartitionMethod *pm=0;

	if(!strcmp(pivotSelectionMethod,"FFT"))
	{
		psm=new CFFTPivotSelectionMethod;
	}
	else
	{
		cout<<"invalid pivot selection method"<<endl;
	}

	if(!strcmp(partitionMethod,"BALANCE"))
	{
		pm=new CBalancePartitionMethod;
	}

	CIndex *mvpIndex = new CMVPIndex(rawData,metric,psm,pm,numPivot,singlePivotFanout,maxLeafSize,maxPathLength);

	mvpIndex->bulkLoad(rawData);

	cout<<"Building MVP tree over"<<endl;
	

	ofstream ofs(mvpIndexFileName);
	boost::archive::text_oarchive oa(ofs);
	mvpIndex->write(oa);
	
	ofs.close();
	ofs.clear();

	/*CMVPIndex *mvpIndexx=new CMVPIndex;
	
	ifstream ifs(mvpIndexFileName);
	boost::archive::text_iarchive ia(ifs);
	mvpIndex->read(ia);

	ifs.close();
	ifs.clear();*/

	/*double d[5]={3.3,5.5,2.2,1.1,4.4};
	int dim=5;
	CDoubleVector *cd=new CDoubleVector(d,dim);

	double radius=200;

	CRangeQuery *q=new CRangeQuery(radius,cd);

	vector<CIndexObject*> ve=mvpIndex->search(q);
	
	for(vector<CIndexObject*>::iterator it=ve.begin();it!=ve.end();it++)
	{
		CDoubleVector *cd=(CDoubleVector*)(*it);

		for(int j=0;j<cd->getLen();j++)
		{
			if(j==0)
				cout<<"(";
			if(j!=(cd->getLen()-1))
				cout<<cd->getData()[j]<<",";
			else
				cout<<cd->getData()[j]<<")"<<endl;
		}
	}*/


}

template<typename type>
type stringToNumber(const char str[])
{
	istringstream iss(str);
	
	type data;

	iss>>data;
	
	return data;
}

int main(int argc, char *argv[])
{

	char *dataFileName=0;
	char *mvpIndexFileName=0;
	char *pivotSelectionMethod=0;
	char *partitionMethod=0;

	int numPivot=1;
	int singlePivotFanout=2;
	int maxLeafSize=0;
	int maxPathLength=0;

	if(argc<=0)
	{
		dataFileName="./data/dataFile.txt";
		dataFileName="./data/indexFile.txt";
		pivotSelectionMethod="FFT";
		partitionMethod="balance";

		numPivot=1;
		singlePivotFanout=2;
		maxLeafSize=5;
		maxPathLength=7;
	}
	else
	{
		for(int i=1;i<argc;i=i+2)
		{
			if(!strcmp(argv[i],"-i"))
			{
				dataFileName = new char[strlen(argv[i+1]+1)];
				strcpy(dataFileName,argv[i+1]);
			}
			else if(!strcmp(argv[i],"-o"))
			{
				mvpIndexFileName= new char[strlen(argv[i+1]+1)];
				strcpy(mvpIndexFileName,argv[i+1]);
			}
			else if(!strcmp(argv[i],"-psm"))
			{
				pivotSelectionMethod=new char[strlen(argv[i+1]+1)];
				strcpy(pivotSelectionMethod,argv[i+1]);
			}
			else if(!strcmp(argv[i],"-pm"))
			{
				partitionMethod=new char[strlen(argv[i+1]+1)];
				strcpy(partitionMethod,argv[i+1]);
			}
			else if(!strcmp(argv[i],"-np"))
			{
				numPivot=stringToNumber<int>(argv[i+1]);
			}
			else if(!strcmp(argv[i],"-spf"))
			{
				singlePivotFanout=stringToNumber<int>(argv[i+1]);
			}
			else if(!strcmp(argv[i],"-mls"))
			{
				maxLeafSize=stringToNumber<int>(argv[i+1]);
			}
			else if(!strcmp(argv[i],"-pl"))
			{
				maxPathLength=stringToNumber<int>(argv[i+1]);
			}
		}
	}

	batchBulkLoad(dataFileName,mvpIndexFileName,pivotSelectionMethod,partitionMethod,numPivot,singlePivotFanout,maxLeafSize,maxPathLength);

	//system("pause");
	return 0;
}