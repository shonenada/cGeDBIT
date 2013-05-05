/*
Date: 02/03/2011 - Ludvik Jerabek - Initial Release
Version: 1.0
Comment: Sample Application using Getopt for Microsoft C\C++
License: LGPL

Revisions:

02/03/2011 - Ludvik Jerabek - Initial Release

**DISCLAIMER**
THIS MATERIAL IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING, BUT Not LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE, OR NON-INFRINGEMENT. SOME JURISDICTIONS DO NOT ALLOW THE
EXCLUSION OF IMPLIED WARRANTIES, SO THE ABOVE EXCLUSION MAY NOT
APPLY TO YOU. IN NO EVENT WILL I BE LIABLE TO ANY PARTY FOR ANY
DIRECT, INDIRECT, SPECIAL OR OTHER CONSEQUENTIAL DAMAGES FOR ANY
USE OF THIS MATERIAL INCLUDING, WITHOUT LIMITATION, ANY LOST
PROFITS, BUSINESS INTERRUPTION, LOSS OF PROGRAMS OR OTHER DATA ON
YOUR INFORMATION HANDLING SYSTEM OR OTHERWISE, EVEN If WE ARE
EXPRESSLY ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
*/

#include "../HeaderFiles/objects/DoubleVector.h"
#include "../HeaderFiles/metric/Metric.h"
#include "../HeaderFiles/metric/EuclideanDistance.h"
#include "../HeaderFiles/index/index.h"
#include "../HeaderFiles/index/MVPIndex.h"
#include "../HeaderFiles/index/ListIndex.h"
#include "../HeaderFiles/indexalgorithm/PartitionMethod.h"
#include "../HeaderFiles/indexalgorithm/PivotSelectionMethod.h"
#include "../HeaderFiles/indexalgorithm/FFTPivotSelectionMethod.h"
#include "../HeaderFiles/indexalgorithm/BalancePartitionMethod.h"
#include "../HeaderFiles/query/Query.h"
#include "../HeaderFiles/query/RangeQuery.h"
#include "../HeaderFiles/util/getopt.h"


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


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

	CIndex *mvpIndex=new CMVPIndex(rawData,metric,psm,pm,numPivot,singlePivotFanout,maxLeafSize);

	mvpIndex->bulkLoad(rawData);

	

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

	double d[5]={50};
	int dim=1;
	CDoubleVector *cd=new CDoubleVector(d,dim);

	double radius=15;

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
	}


}


template<typename type>
type stringToNumber(const char str[])
{
	istringstream iss(str);

	type data;

	iss>>data;

	return data;
}


/**
* This is the main entry point for building a VP Index. It can build either one one {@link VPIndex}, 
* or a series of {@link VPIndex}es.
* 
* The basic commandline options are:
* 
* -i [data file name]
* -psm [the pivot selection selection method: random, fft, center, pcaonfft, pca]
* -np [number of pivots in an index node]
* -pm [data partition method: balanced, clusteringkmeans, clusteringboundary]
* -spf [fanout of a pivot]
* -m [maximum number of children in leaf nodes]
* -pl [path length, default 0]
* -t [data type, one of "protein", "dna", "vector", "image", "msms"]
* -g [debug level]
* -frag [fragment length, only meaningful for sequences]
* -dim [dimension of vector data to load]
* -b [whether bucketing will be used, 1: use]
* -r maximum radius for partition
* -seta the value of A in the incremental pivot selection
* -setn the value of N in the incremental pivot selection
* -o the prefix of the output index file
* -fftscale use for pcaonfft pivot selection method
* For build multiple databases, use the following options:
* 
* -i [size of smallest database]
* -a [size of largest database]
* -s [step size of databases]
* 
* For using multiple regression algorithms, use the following options:
* 
* -sa [select algorithm]
*  forward for forward selection, backward for backward selection, enumerate for enumerate selection
* -ym [the method to calculate y array]
*  standard for standard deviation method, average for average method
* -tkind [test kind]
*  ftest for F-test, rss for comparing rss
* The {@link Metric} is hardcoded for each data type.
* @author Rui Mao, Willard
* @version 2007.07.12
*/
int main(int argc, char** argv)
{
	static int verbose_flag;
	int c;
	char *dataFileName=0;
	char *mvpIndexFileName=0;
	char *pivotSelectionMethod=0;
	char *partitionMethod=0;
	char *dataType=0;

	int setA = 0;
    int setN = 0;
	int frag = 0;
	int initialSize = 0;
	int finalSize = 0;	
	int stepSize = 0;	
	int numPivot=0;
	int singlePivotFanout=0;
	int maxLeafSize=0;
	int maxPathLength=0;
	int dim=0;

	if(argc<=0)
	{
		dataFileName="./data/dataFile.txt";
		dataFileName="./data/indexFile.txt";
		pivotSelectionMethod="FFT";
		partitionMethod="BALANCE";
		dataType="vector";
		setA = 10000;
		setN = 50;
		frag = 6;
		initialSize = 100000;
		finalSize = 1000000;	
		stepSize = 1000000;	
		numPivot=3;
		singlePivotFanout=3;
		maxLeafSize=100;
		maxPathLength=7;
		dim=2;
	}

	while (1)
	{		
		static struct option_a long_options[] =
		{
			{"verbose", ARG_NONE, &verbose_flag, 1},
			{"brief",   ARG_NONE, &verbose_flag, 0},

			{"dataFileName",    ARG_REQ, 0 , 'n'},
			{"psm",    ARG_REQ, 0 , 'p'},
			{"np",    ARG_REQ, 0 , 'v'},
			{"pm",    ARG_REQ, 0 , 'M'},
			{"spf",    ARG_REQ, 0 , 'f'},
			{"mls",    ARG_REQ, 0 , 'm'},
			{"pl",    ARG_REQ, 0 , 'x'},
			{"dataType",    ARG_REQ, 0 , 't'},
			{"frag",    ARG_REQ, 0 , 'F'},
			{"dim",    ARG_REQ, 0 , 'd'},
			{"seta",    ARG_REQ, 0 , 'A'},
			{"setn",    ARG_REQ, 0 , 'N'},
			{"mvpIndexFileName",    ARG_REQ, 0 , 'o'},
			{"initialSize",    ARG_REQ, 0 , 'i'},
			{"finalSize",    ARG_REQ, 0 , 'a'},
			{"stepSize",    ARG_REQ, 0 , 's'},

			{ ARG_NULL , ARG_NULL , ARG_NULL , ARG_NULL }
		};
		
		int option_index = 0;
		c = getopt_long_a(argc, argv, ("n:p:v:M:S:m:x:t:f:d:A:N:o:i:a:s:"), long_options, &option_index);
		
		// Check for end of operation or error
		if (c == -1)
			break;
		
		// Handle options
		switch (c)
		{
		case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				break;
			printf (("option %s"), long_options[option_index].name);
			if (optarg_a)
				printf ((" with arg %s"), optarg_a);
			printf (("\n"));
			break;
			
		case ('n'):	
			dataFileName = new char[strlen(optarg_a)+1];
			strcpy(dataFileName,optarg_a);
			printf (("option -dataFileName with value `%s'\n"), dataFileName);
			break;

		case ('p'):	
			pivotSelectionMethod = new char[strlen(optarg_a)+1];
			strcpy(pivotSelectionMethod,optarg_a);
			printf (("option -pivotSelectionMethod with value `%s'\n"), pivotSelectionMethod);
			break;

		case ('v'):	
			numPivot=stringToNumber<int>(optarg_a);
			printf (("option -numPivot with value `%d'\n"), numPivot);
			break;

		case ('M'):	
			partitionMethod = new char[strlen(optarg_a)+1];
			strcpy(partitionMethod,optarg_a);
			printf (("option -partitionMethod with value `%s'\n"), partitionMethod);
			break;

		case ('f'):	
			singlePivotFanout=stringToNumber<int>(optarg_a);
			printf (("option -singlePivotFanout with value `%d'\n"), singlePivotFanout);
			break;

		case ('m'):	
			maxLeafSize=stringToNumber<int>(optarg_a);
			printf (("option -maxLeafSize with value `%d'\n"), maxLeafSize);
			break;

		case ('x'):	
			maxPathLength=stringToNumber<int>(optarg_a);
			printf (("option -maxPathLength with value `%d'\n"), maxPathLength);
			break;

		case ('t'):	
			dataType = new char[strlen(optarg_a)+1];
			strcpy(dataType,optarg_a);
			printf (("option -dataType with value `%s'\n"), dataType);
			break;

		case ('F'):	
			frag = stringToNumber<int>(optarg_a);
			printf (("option -frag with value `%d'\n"), frag);
			break;

		case ('d'):	
			dim=stringToNumber<int>(optarg_a);
			printf (("option -dim with value `%d'\n"), dim);
			break;

		case ('A'):	
			setA = stringToNumber<int>(optarg_a);
			printf (("option -setA with value `%d'\n"), setA);
			break;

		case ('N'):	
			setN = stringToNumber<int>(optarg_a);
			printf (("option -setN with value `%d'\n"), setN);
			break;

		case ('o'):	
			mvpIndexFileName = new char[strlen(optarg_a)+1];
			strcpy(mvpIndexFileName,optarg_a);
			printf (("option -mvpIndexFileName with value `%s'\n"), mvpIndexFileName);
			break;

		case ('i'):	
			initialSize = stringToNumber<int>(optarg_a);
			printf (("option -initialSize with value `%d'\n"), initialSize);
			break;

		case ('a'):	
			finalSize = stringToNumber<int>(optarg_a);
			printf (("option -finalSize with value `%d'\n"), finalSize);
			break;

		case ('s'):	
			stepSize = stringToNumber<int>(optarg_a);
			printf (("option -stepSize with value `%d'\n"), stepSize);
			break;



		case '?':
			/* getopt_long already printed an error message. */
			break;

		default:
			abort();
		}
	}

	if (verbose_flag)
		printf (("verbose flag is set\n"));


	if (optind < argc)
	{
		printf (("non-option ARGV-elements: "));
		while (optind < argc) printf (("%s "), argv[optind++]);
		printf (("\n"));
	}

	batchBulkLoad(dataFileName,mvpIndexFileName,pivotSelectionMethod,partitionMethod,numPivot,singlePivotFanout,maxLeafSize,maxPathLength);

	system("pause");

	return 0;
}
