/** @file BuildORQueryIndex
* @Entry of using this database.
* @author ***
* @date 2013 5 21
* @version 0.x
*
* this file contains the main interface to use this database.
*/
#define _CRT_SECURE_NO_WARNINGS
#include "../HeaderFiles/objects/DoubleVector.h"
#include "../HeaderFiles/metric/Metric.h"
#include "../HeaderFiles/metric/EuclideanDistance.h"
#include "../HeaderFiles/index/Index.h"
#include "../HeaderFiles/index/MVPIndex.h"
#include "../HeaderFiles/index/ListIndex.h"
#include "../HeaderFiles/indexalgorithm/PartitionMethod.h"
#include "../HeaderFiles/indexalgorithm/PivotSelectionMethod.h"
#include "../HeaderFiles/indexalgorithm/FFTPivotSelectionMethod.h"
#include "../HeaderFiles/indexalgorithm/IncrementalSelection.h"
#include "../HeaderFiles/indexalgorithm/BalancePartitionMethod.h"
#include "../HeaderFiles/query/Query.h"
#include "../HeaderFiles/query/RangeQuery.h"

#include "../HeaderFiles/metric/DNAMetric.h"
#include "../HeaderFiles/metric/EditDistance.h"
#include "../HeaderFiles/metric/ImageMetric.h"
#include "../HeaderFiles/metric/MSMSMetric.h"
#include "../HeaderFiles/metric/RNAMetric.h"
#include "../HeaderFiles/metric/PeptideMetric.h"



#if defined(_MSC_VER)
#include "../HeaderFiles/util/getopt_win.h"
#include <tchar.h>
#else _MSC_VER
#include "../HeaderFiles/util/getopt.h"
#define ARG_NONE no_argument
#define ARG_NULL no_argument
#define ARG_REQ required_argument
#define getopt_long_a getopt_long
#define optarg_a optarg
#define option_a option
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>


#define WIN_C
#ifndef WIN_C
#include<sys/times>
#else
#include <Windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")  
#endif

using namespace std;

extern void buildAndSearch(char *&dataFileName,int &numPivot,int setA,int setN,int &singlePivotFanout,int fftscale,char *&pivotSelectionMethod,char *&partitionMethod,int &maxLeafSize,char *&dataType,int initialSize,int finalSize,int stepSize,bool bucket,int &fragmentLength,int dim,double maxR,char *&indexType,int fftOpt,char* queryFileName,int firstQuery,int lastQuery,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName,int runMode,char *indexName,int indexMode);

extern void batchBulkLoad(char *&dataFileName,int &numPivot,int setA,int setN,int &singlePivotFanout,int fftscale,char *&pivotSelectionMethod,char *&partitionMethod,int &maxLeafSize,char *&dataType,int initialSize,int finalSize,int stepSize,bool bucket,int &fragmentLength,int dim,double maxR,char *&indexType,int fftOpt,char *&buildingInformationFile,int runMode,char *indexName);

extern void searchIndex(int searchSize,char* queryFileName,char *dataType,int firstQuery,int lastQuery,int dim,int fragmentLength,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName,int runMode,char *indexName,int indexMode);

template<typename type>
type stringToNumber(const char str[])
{
	istringstream iss(str);

	type data;

	iss>>data;

	return data;
}

void showMemoryInfo(char * resultsFileName)  
{  
	char resultFile[100]="./data/";
	strcat(resultFile,resultsFileName);
	ofstream output(resultFile,ofstream::app);
	HANDLE handle=GetCurrentProcess();  
	PROCESS_MEMORY_COUNTERS pmc;  
	GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));  
	output<<"当前物理内存使用:"<<pmc.WorkingSetSize/1048576.0 <<" MB / 峰值物理内存使用:"<<pmc.PeakWorkingSetSize/1048576.0<<" MB "<<endl; 
	output<<"当前虚拟内存使用："<<pmc.PagefileUsage/1048576.0<<" MB/ 峰值虚拟内存使用："<< pmc.PeakPagefileUsage/1048576.0<<" MB "<<endl;
	output<<"当前内存使用："<<pmc.PagefileUsage/1048576.0 + pmc.WorkingSetSize/1048576.0<<" MB/ 峰值内存使用："<< pmc.PeakPagefileUsage/1048576.0 + pmc.PeakWorkingSetSize/1048576.0 <<" MB "<<endl;
}

/**
* @This is the main entry point for building a MVP Index or do search in the index built before. It can build more than one index structure
* 
* The basic commandline options are:
* 
* @param runO [build,search] option that will be done. Input "build" to build a index and input "search" if you want to do search in the index build before.
* @param n raw data file name. 
* @param v number of pivots in an index node.
* @param seta the value of A in the incremental pivot selection.
* @param setn the value of N in the incremental pivot selection.
* @param f fanout of a pivot.
* @param fftscale use for pcaonfft pivot selection method.
* @param psm the pivot selection method: "random", "fft", "center", "pcaonfft", "pca","incremental".
* @param dpm data partition method: "balanced", "clusteringkmeans", "clusteringboundary","excludedmiddle","cght","ght".
* @param m maximum number of objects in leaf nodes.
* @param t data type, one of "peptide", "dna", "vector", "image", "msms","string".
* @param init size of smallest database.
* @param F size of largest database.
* @param s step size of databases.
* @param b whether bucketing will be used, 1: use.
* @param frag fragment length, only meaningful for sequences.
* @param dim dimension of vector data to load or number of features to be loaded.
* @param r maximum radius for partition.
* @param e [ListIndex,MVPIndex],the type of the index.
* @param fftopt 0:use old version FFT, 1: use new version FFT but don't choose the third point got from FFT as the second pivot, 2:use new version FFT and choose the third point got from FFT as the second pivot. Default 0. 
* @param indexN name of file storing the index structure built before. 
* @param q query file name. 
* @param fq offset of first query to be used in the query file, start from 0, inclusive, default 0. 
* @param l offset of last query to be used in the query file, exclusive, default 1. 
* @param a maximum search radius, default 10. 
* @param i minimum search radius, default 0. 
* @param y step size for search radii, default 1. 
* @param ver 1 if search results are to be verified against a linear scan and 0 otherwise, default 0. 
* @param res name of the file to store the result information of the search operation. 
*
*/

int main(int argc, char** argv)
{

	int c;

	int runMode = 0;
	char *dataFileName="uniformvector-20dim-1m.txt";
	int numPivot=2;
	int setA = 10000;
	int setN = 50;
	int singlePivotFanout=3;
	int fftscale=100;
	char *pivotSelectionMethod="fft";
	char *partitionMethod="balanced";
	int maxLeafSize=100;
	char *dataType="vector";
	int initialSize = 20000;
	int finalSize = 20000;	
	int stepSize = 20000;	
	bool bucket = false;
	int fragmentLength=6;
	int dim=20;
	double maxR=1;
	char *indexType="MVPIndex";
	int fftopt=0;

	char *indexName=0;
	char* queryFileName="querydata.txt";
	int firstQuery=0;
	int lastQuery=10;
	double maxRadius= 1.5;
	double minRadius= 0;
	double step=0.25;
	bool verify=false;
	char *resultsFileName="resultsfile.txt";

	char *runOption="buildandsearch";
	int indexMode=1;

	while (1)
	{		
		static struct option_a long_options[] =
		{
			/*{"verbose", ARG_NONE, &verbose_flag, 1},
			{"brief",   ARG_NONE, &verbose_flag, 0},*/

			{"r",    ARG_REQ, 0, 'a'},
			{"runMode",    ARG_REQ, 0, 'A'},
			{"b", ARG_REQ,0,'b'},
			{"indexN", ARG_REQ,0,'B'},
			{"indexM", ARG_REQ,0,'c'},
			{"n",    ARG_REQ, 0 , 'd'},
			{"t",    ARG_REQ, 0 , 'D'},
			{"e",    ARG_REQ, 0 , 'e'},
			{"fftscale", ARG_REQ, 0, 'f'},
			{"F",    ARG_REQ, 0 , 'F'},
			{"frag",    ARG_REQ, 0 , 'g'},
			{"init",    ARG_REQ, 0 , 'i'},
			{"dim",    ARG_REQ, 0 , 'I'},
			{"l",    ARG_REQ, 0 , 'l'},
			{"res",    ARG_REQ, 0 , 'L'},
			{"m",    ARG_REQ, 0 , 'm'},
			{"v",    ARG_REQ, 0 , 'n'},
			{"psm",    ARG_REQ, 0 , 'p'},
			{"dpm",    ARG_REQ, 0 , 'P'},
			{"q",    ARG_REQ, 0 , 'q'},
			{"runO",ARG_REQ, 0,'r'},
			{"fq",  ARG_REQ, 0 , 'R'},
			{"seta",    ARG_REQ, 0 , 's'},
			{"setn",    ARG_REQ, 0 , 'S'},
			{"s",    ARG_REQ, 0 , 't'},
			{"fftopt",     ARG_REQ, 0, 'T'},
			{"i",    ARG_REQ, 0 , 'U'},
			{"ver",    ARG_REQ, 0 , 'v'},
			{"a",    ARG_REQ, 0 , 'x'},
			{"y",    ARG_REQ, 0 , 'y'},
			{"f",    ARG_REQ, 0 , 'z'},

			/*{ ARG_NULL , ARG_NULL , ARG_NULL , ARG_NULL }*/
		};

		int option_index = 0;
		c = getopt_long_a(argc, argv, ("a:A:b:B:c:d:D:e:f:F:g:i:I:l:L:m:n:p:P:q:r:R:s:S:t:T:U:v:x:y:z:"), long_options, &option_index);

		// Check for end of operation or error
		if (c == -1)
			break;

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
		case ('a'):	
			maxR = stringToNumber<double>(optarg_a);
			cout<<"option -maxR with value " << maxR << endl;
			break;
		case ('A'):	
			runMode = stringToNumber<int>(optarg_a);
			cout<<"option -runMOde with value " << runMode << endl;
			break;
		case ('b'):	
			bucket = stringToNumber<bool>(optarg_a);
			cout<<"option -bucket with value " << bucket << endl;
			break;
		case ('B'):	
			indexName = new char[strlen(optarg_a)+1];
			strcpy(indexName,optarg_a);
			cout<<"option -indexN with value " << indexName << endl;
			break;
		case ('c'):	
			indexMode = stringToNumber<int>(optarg_a);
			cout<<"option -indexMode with value " << indexMode << endl;
			break;
		case ('d'):	
			dataFileName = new char[strlen(optarg_a)+1];
			strcpy(dataFileName,optarg_a);
			printf (("option -dataFileName with value `%s'\n"), dataFileName);
			break;
		case ('D'):	
			dataType = new char[strlen(optarg_a)+1];
			strcpy(dataType,optarg_a);
			printf (("option -dataType with value `%s'\n"), dataType);
			break;
		case ('e'):	
			indexType = new char[strlen(optarg_a)+1];
			strcpy(indexType,optarg_a);
			printf (("option -listType with value `%s'\n"), indexType);
			break;
		case ('f'):	
			fftscale = stringToNumber<int>(optarg_a);
			printf (("option -fftscale with value `%d'\n"), fftscale);
			break;
		case ('F'):	
			finalSize = stringToNumber<int>(optarg_a);
			printf (("option -finalSize with value `%d'\n"), finalSize);
			break;
		case ('g'):	
			fragmentLength = stringToNumber<int>(optarg_a);
			printf (("option -fragmentLength with value `%d'\n"), fragmentLength);
			break;
		case ('i'):	
			initialSize = stringToNumber<int>(optarg_a);
			printf (("option -initialSize with value `%d'\n"), initialSize);
			break;
		case ('I'):	
			dim=stringToNumber<int>(optarg_a);
			printf (("option -dim with value `%d'\n"), dim);
			break;
		case ('l'):	
			lastQuery=stringToNumber<int>(optarg_a);
			printf (("option -lastQuery with value `%d'\n"), lastQuery);
			break;
		case ('L'):	
			resultsFileName=new char[strlen(optarg_a)+1];
			strcpy(resultsFileName,optarg_a);
			printf (("option -resultsFileName with value `%s'\n"), resultsFileName);
			break;
		case ('m'):	
			maxLeafSize=stringToNumber<int>(optarg_a);
			printf (("option -maxLeafSize with value `%d'\n"), maxLeafSize);
			break;
		case ('n'):	
			numPivot=stringToNumber<int>(optarg_a);
			printf (("option -numPivot with value `%d'\n"), numPivot);
			break;
		case ('p'):	
			pivotSelectionMethod = new char[strlen(optarg_a)+1];
			strcpy(pivotSelectionMethod,optarg_a);
			printf (("option -pivotSelectionMethod with value `%s'\n"), pivotSelectionMethod);
			break;
		case ('P'):	
			partitionMethod = new char[strlen(optarg_a)+1];
			strcpy(partitionMethod,optarg_a);
			printf (("option -partitionMethod with value `%s'\n"), partitionMethod);
			break;
		case ('q'):	
			queryFileName = new char[strlen(optarg_a)+1];
			strcpy(queryFileName,optarg_a);
			printf (("option -queryFileName with value `%s'\n"), queryFileName);
			break;
		case ('r'):	
			runOption = new char[strlen(optarg_a)+1];
			strcpy(runOption,optarg_a);
			printf (("option -runOption with value `%s'\n"), optarg_a);
			break;
		case ('R'):	
			firstQuery = stringToNumber<int>(optarg_a);
			printf (("option -firstQuery with value `%d'\n"), firstQuery);
			break;
		case ('s'):	
			setA = stringToNumber<int>(optarg_a);
			printf (("option -setA with value `%d'\n"), setA);
			break;
		case ('S'):	
			setN = stringToNumber<int>(optarg_a);
			printf (("option -setN with value `%d'\n"), setN);
			break;
		case ('t'):	
			stepSize = stringToNumber<int>(optarg_a);
			printf (("option -stepSize with value `%d'\n"), stepSize);
			break;
		case ('T'):	
			fftopt = stringToNumber<int>(optarg_a);
			printf (("option -fftopt with value `%d'\n"), fftopt);
			break;
		case ('U'):	
			minRadius = stringToNumber<double>(optarg_a);
			printf (("option -minRadius with value `%f'\n"), minRadius);
			break;
		case ('v'):	
			verify = stringToNumber<bool>(optarg_a);
			printf (("option -verify with value `%d'\n"), verify);
			break;
		case ('x'):	
			maxRadius = stringToNumber<float>(optarg_a);
			printf (("option -maxRadius with value `%f'\n"), maxRadius);
			break;
		case ('y'):	
			step=stringToNumber<double>(optarg_a);
			printf (("option -step with value `%f'\n"), step);
			break;
		case ('z'):	
			singlePivotFanout=stringToNumber<int>(optarg_a);
			printf (("option -singlePivotFanout with value `%d'\n"), singlePivotFanout);
			break;

		case '?':
			/* getopt_long already printed an error message. */
			break;

		default:
			abort();
		}
	}

	if (optind < argc)
	{
		printf (("non-option ARGV-elements: "));
		while (optind < argc) printf (("%s "), argv[optind++]);
		printf (("\n"));
	}
#ifdef WIN_C
	if(strcmp(runOption,"buildandsearch")==0)
	{
		buildAndSearch(dataFileName,numPivot,setA,setN,singlePivotFanout,fftscale,pivotSelectionMethod,partitionMethod,maxLeafSize,dataType,initialSize,finalSize,stepSize,bucket,fragmentLength,dim,maxR,indexType,fftopt,queryFileName,firstQuery,lastQuery,maxRadius,minRadius,step,verify,resultsFileName,runMode,indexName,indexMode);
		showMemoryInfo(resultsFileName);
	}
	else if(strcmp(runOption,"build")==0)
	{
		batchBulkLoad(dataFileName,numPivot,setA,setN,singlePivotFanout,fftscale,pivotSelectionMethod,partitionMethod,maxLeafSize,dataType,initialSize,finalSize,stepSize,bucket,fragmentLength,dim,maxR,indexType,fftopt,resultsFileName,runMode,indexName);
		/*showMemoryInfo(resultsFileName);*/

	}
	else if(strcmp(runOption,"search")==0)
	{
		searchIndex(initialSize,queryFileName,dataType,firstQuery,lastQuery,dim,fragmentLength,maxRadius,minRadius,step,verify,resultsFileName,runMode,indexName,indexMode);
		showMemoryInfo(resultsFileName);
	}
#else
	char resultFile[100]="./data/";
	strcat(resultFile,resultsFileName);
	ofstream output(resultFile,ofstream::app);
	struct sysinfo s_info;
	if(strcmp(runOption,"buildandsearch")==0)
	{
		buildAndSearch(dataFileName,numPivot,setA,setN,singlePivotFanout,fftscale,pivotSelectionMethod,partitionMethod,maxLeafSize,dataType,initialSize,finalSize,stepSize,bucket,fragmentLength,dim,maxR,indexType,fftopt,queryFileName,firstQuery,lastQuery,maxRadius,minRadius,step,verify,resultsFileName,runMode,indexName);
		output<<"内存使用:"<<s_info. mem_unit/1048576.0 <<" MB"<<endl; 
	}
	else if(strcmp(runOption,"build")==0)
	{
		struct tms tmp;
		clock_t begin = times(&tmp);
		batchBulkLoad(dataFileName,numPivot,setA,setN,singlePivotFanout,fftscale,pivotSelectionMethod,partitionMethod,maxLeafSize,dataType,initialSize,finalSize,stepSize,bucket,fragmentLength,dim,maxR,indexType,fftopt,resultsFileName,runMode,indexName);
		clock_t end = times(&tmp);
		output<<"内存使用:"<<s_info. mem_unit/1048576.0 <<" MB"<<endl; 
		output<<"Total time elapsed during building the index structure is "<< (double) (end.tms_cutime-begin.tms_cutime)/CLOCKS_PER_SEC << " s"<<endl;

	}
	else if(strcmp(runOption,"search")==0)
	{
		searchIndex(initialSize,queryFileName,dataType,firstQuery,lastQuery,dim,fragmentLength,maxRadius,minRadius,step,verify,resultsFileName,runMode,indexName,indexMode);
		output<<"内存使用:"<<s_info. mem_unit/1048576.0 <<" MB"<<endl; 
	}
#endif
	//system("pause");

	return 0;
}
