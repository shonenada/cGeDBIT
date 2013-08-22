#include "../../HeaderFiles/objects/DoubleVector.h"
//#include  "../../HeaderFiles/objects/TandemSpectra.h"
#include "../../HeaderFiles/metric/Metric.h"
#include "../../HeaderFiles/metric/EuclideanDistance.h"
#include "../../HeaderFiles/index/Index.h"
#include "../../HeaderFiles/index/MVPIndex.h"
#include "../../HeaderFiles/index/ListIndex.h"
#include "../../HeaderFiles/indexalgorithm/PartitionMethod.h"
#include "../../HeaderFiles/indexalgorithm/PivotSelectionMethod.h"
#include "../../HeaderFiles/indexalgorithm/PCAOnFFT.h"
#include "../../HeaderFiles/indexalgorithm/FFTPivotSelectionMethod.h"
#include "../../HeaderFiles/indexalgorithm/IncrementalSelection.h"
#include "../../HeaderFiles/indexalgorithm/BalancePartitionMethod.h"
#include "../../HeaderFiles/query/Query.h"
#include "../../HeaderFiles/query/RangeQuery.h"

#include "../../HeaderFiles/metric/DNAMetric.h"
#include "../../HeaderFiles//metric/CountedMetric.h"
#include "../../HeaderFiles/metric/EditDistance.h"
#include "../../HeaderFiles/metric/ImageMetric.h"
//#include "../../HeaderFiles/metric/MSMSMetric.h"
#include "../../HeaderFiles/metric/RNAMetric.h"
#include "../../HeaderFiles/metric/PeptideMetric.h"
#include "../../HeaderFiles/query/SeachTask.h"

#include "../../HeaderFiles/objects/Image.h"
#include "../../HeaderFiles/objects/DNA.h"
#include "../../HeaderFiles/objects/Peptide.h"
#include "../../HeaderFiles/objects/StringObject.h"
#include "../../HeaderFiles/util/MemMonitor.h"


#include <sstream>
#define WIN_C
#ifndef WIN_C
#include<sys/times>
#else
#include <Windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")  
#endif
/**
* @This is the main entry point for building a VP Index. It can build more than one index by one time
* 
* The basic commandline options are: 
* @param dataFileName raw data file name. 
* @param numPivot number of pivots in an index node.
* @param setA the value of A in the incremental pivot selection.
* @param setN the value of N in the incremental pivot selection.
* @param singlePivotFanout fanout of a pivot.
* @param fftscale use for pcaonfft pivot selection method.
* @param pivotSelectionMethod the pivot selection selection method: random, fft, center, pcaonfft, pca.
* @param partitionMethod data partition method: balanced, clusteringkmeans, clusteringboundary.
* @param maxLeafSize maximum number of children in leaf nodes.
* @param dataType data type, one of "protein", "dna", "vector", "image", "msms","string".
* @param initialSize size of smallest database.
* @param finalSize size of largest database.
* @param stepSize step size of databases.
* @param bucket whether bucketing will be used, 1: use.
* @param fragmentLength fragment length, only meaningful for sequences.
* @param dim dimension of vector data to load or number of features to be loaded.
* @param maxR maximum radius for partition.
* @param indexType [list,mvptree],the type of the index built later.
* @param fftOpt 0:use old version FFT, 1: use new version FFT but don't choose the third point got from FFT as the second pivot, 2:use new version FFT and choose the third point got from FFT as the second pivot. Default 0.  
*
*/

void batchBulkLoad(char *&dataFileName,int &numPivot,int setA,int setN,int &singlePivotFanout,int fftscale,char *&pivotSelectionMethod,char *&partitionMethod,int &maxLeafSize,char *&dataType,int initialSize,int finalSize,int stepSize,bool bucket,int &fragmentLength,int dim,double maxR,char *&indexType,int fftOpt,char *&buildingInformationFile,int runMode,char *indexName)
{

	vector<shared_ptr<CIndexObject> > *rawData=0;
	CMetric *metric=0;
	CPivotSelectionMethod *psm=0;
	CPartitionMethod *pm=0;
	CIndex *index =0;
	int indexNum = (finalSize - initialSize) / stepSize + 1;

	double *buildTimes=new double[indexNum];
	double *loadDataTimes=new double[indexNum];
	int *distanceCalculatedTimes=new int[indexNum];

	if(!strcmp(dataType,"vector")) 
	{
		metric = new CEuclideanDistance;
	}
	else if(!strcmp(dataType,"spectra")) 
	{
		//metric = new CMSMSMetric;
	}
	else if(!strcmp(dataType,"string")) 
	{
		metric = new CEditDistance;
	}
	else if(!strcmp(dataType,"rna")) 
	{
		metric = new CRNAMetric ;
	}
	else if(!strcmp(dataType,"image")) 
	{
		metric = new CImageMetric ;	
	}
	else if(!strcmp(dataType,"dna")) 
	{
		metric = new CDNAMetric ;	
	}
	else if(!strcmp(dataType,"peptide"))
	{
		metric = new CPeptideMetric;
	}

#ifdef WIN_C
	FILETIME kernalStartTime,kernalEndTime;
	FILETIME userStartTime,userEndTime;
	FILETIME ftDummy;
	PROCESS_MEMORY_COUNTERS pmc;  
#endif
	char newDataFileName[100]="./data/";
	strcat(newDataFileName,dataFileName);

	char newIndexName[100]="./data/";
	strcat(newIndexName,indexName);

	char resultFileName[200]="./data/";
	strcat(resultFileName,buildingInformationFile);
	ofstream output(resultFileName,ofstream::app);

	stringstream newSize;

	int distanceFunctionCounterBeforSearch=0,distanceFunctionCounterAfterSearch=0;
	for (int size = initialSize, i = 0; (size <= finalSize) & (i < indexNum); size += stepSize, i++)
	{

		if(!strcmp(dataType,"vector")) 
		{
			CObjectFactory factory();
			CObjectFactory::objectRegister("vector",CDoubleVector::getConstructor());
			GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
			rawData=CDoubleVector::loadData(newDataFileName,size,dim);
			GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
			loadDataTimes[i]=((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;
		}
		else if(!strcmp(dataType,"spectra")) 
		{
			//rawData = CTandemSpectra::loadData(newDataFileName,size);
		}
		else if(!strcmp(dataType,"string")) 
		{
			CObjectFactory factory();
			CObjectFactory::objectRegister("string",CStringObject::getConstructor());
			GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
			rawData = CStringObject::loadData(newDataFileName,size);
			GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
			loadDataTimes[i]=((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;

		}
		else if(!strcmp(dataType,"rna")) 
		{
			rawData = CRNA::loadData(newDataFileName, finalSize,fragmentLength);
		}
		else if(!strcmp(dataType,"image")) 
		{
			CObjectFactory factory();
			CObjectFactory::objectRegister("image",CImage::getConstructor());
			GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
			rawData = CImage::loadData(newDataFileName ,size, dim);
			GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
			loadDataTimes[i]=((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;
		}
		else if(!strcmp(dataType,"dna")) 
		{
			CObjectFactory factory();
			CObjectFactory::objectRegister("dna",CDNA::getConstructor());
			GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
			rawData = CDNA::loadData(newDataFileName,finalSize,fragmentLength);
			GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
			loadDataTimes[i]=((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;
		}
		else if(!strcmp(dataType,"peptide"))
		{
			CObjectFactory factory();
			CObjectFactory::objectRegister("peptide",CPeptide::getConstructor());
			GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
			rawData = CPeptide::loadData(newDataFileName,finalSize,fragmentLength);
			GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
			loadDataTimes[i]=((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;
		}

		
		GetProcessMemoryInfo(GetCurrentProcess(),&pmc,sizeof(pmc));
		output<<"=================Loading Data Used Time And Memory================="<<endl;
		output<<"loadDataTime: "<<loadDataTimes[i]<<" s"<<endl;
		output<<"WorkingSetSize: "<<pmc.WorkingSetSize/1048576.0 <<" MB / PeakWorkingSetSize: "<<pmc.PeakWorkingSetSize/1048576.0<<" MB "<<endl; 
		output<<"PagefileUsage: "<<pmc.PagefileUsage/1048576.0<<" MB/ PeakPagefileUsage: "<< pmc.PeakPagefileUsage/1048576.0<<" MB "<<endl;
		output<<"MemoryUsage: "<<pmc.PagefileUsage/1048576.0 + pmc.WorkingSetSize/1048576.0<<" MB/ PeakMemoryUsage: "<< pmc.PeakPagefileUsage/1048576.0 + pmc.PeakWorkingSetSize/1048576.0 <<" MB "<<endl;
		output<<"==================================================================="<<endl<<endl;

		//pivotSelectionMethod
		if(!strcmp(pivotSelectionMethod,"fft"))
		{
			psm=new CFFTPivotSelectionMethod;
		}
		else if(!strcmp(pivotSelectionMethod,"incremental"))
		{
			psm = new CIncrementalSelection(setA,setN) ;
		}
		else if(!strcmp(pivotSelectionMethod,"pcaonfft"))
		{
			psm = new CPCAOnFFT(fftscale);
		}
		else
		{
			cout<<"invalid pivot selection method"<<endl;
		}


		// partitionMethod
		if(!strcmp(partitionMethod,"balanced"))
		{
			pm=new CBalancePartitionMethod;
		}

		CountedMetric *cmetric=new CountedMetric(metric);
		// indexType
		if (!strcmp(indexType,"MVPIndex"))
		{
			index=new CMVPIndex(*rawData,cmetric,psm,pm,numPivot,singlePivotFanout,maxLeafSize);
		}
		else if(!strcmp(indexType,"ListIndex"))
		{
			index=new CListIndex(cmetric);
		}

		index->setFileName(newIndexName);
		index->setDataType(dataType);

		distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();
#ifdef WIN_C
		GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
		index->bulkLoad(*rawData,runMode);
		GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
		buildTimes[i]=((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;
#else
		struct tms tmp;
		clock_t begin = times(&tmp);
		index->bulkLoad(*rawData,runMode);
		clock_t end = times(&tmp);
		buildTimes[i]=(double) (end.tms_cutime-begin.tms_cutime)/CLOCKS_PER_SEC
#endif
			distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();
		distanceCalculatedTimes[i]=distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch;


		GetProcessMemoryInfo(GetCurrentProcess(),&pmc,sizeof(pmc));
		output<<"=================Building index Used Time And Memory================="<<endl;
		output<<"datasize: "<<size<<endl;
		output<<"distCalculateTimes: "<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<endl;
		output<<"buildTime: "<<buildTimes[i]<<" s"<<endl;
		output<<"WorkingSetSize: "<<pmc.WorkingSetSize/1048576.0 <<" MB / PeakWorkingSetSize: "<<pmc.PeakWorkingSetSize/1048576.0<<" MB "<<endl; 
		output<<"PagefileUsage: "<<pmc.PagefileUsage/1048576.0<<" MB/ PeakPagefileUsage: "<< pmc.PeakPagefileUsage/1048576.0<<" MB "<<endl;
		output<<"MemoryUsage: "<<pmc.PagefileUsage/1048576.0 + pmc.WorkingSetSize/1048576.0<<" MB/ PeakMemoryUsage: "<< pmc.PeakPagefileUsage/1048576.0 + pmc.PeakWorkingSetSize/1048576.0 <<" MB "<<endl;
		output<<"==================================================================="<<endl<<endl;

	}
}

/**
* @This is a utility class to query a VPIndex. It runs a set of query on the given index and compute the average
* performance.
* 
* main function to evaluate the query performance of an Index The
* eveluation is done by run a set of query on the Index, and compute the
* average performance statistics, such as number of distance calculation,
* number of index node visited I/O, and search time. The command line
* interface to bulkload one Index, or a series of Indexes for
* scalability study.
* 
* @param indexFileName name of the file storing the index structure built before.
* @param queryFileName query file name. 
* @param firstQuery offset of first query to be used in the query file, start from 0, inclusive, default 0. 
* @param lastQuery offset of last query to be used in the query file, exclusive, default 1. 
* @param minRadius maximum search radius, default 10. 
* @param maxRadius minimum search radius, default 0. 
* @param step step size for search radii, default 1. 
* @param verify 1 if search results are to be verified against a linear scan and 0 otherwise, default 0. 
* @param resultsFileName name of the file to store the result information of the search operation. 
* @param dim dimension of vector data to load or number of features to be loaded.
* @param fragmentLength fragment length, only meaningful for sequences.
* 
*/
void searchIndex(int searchSize,char* queryFileName,char *dataType,int firstQuery,int lastQuery,int dim,int fragmentLength,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName,int runMode,char *indexName,int indexMode)
{
	char fileName[100]="./data/";
	strcat(fileName,indexName);

	CIndex *index=0;
	CMetric *metric=0;

	vector<shared_ptr<CIndexObject> > linerDataList;

	char newQueryFile[100]="./data/";
	strcat(newQueryFile,queryFileName);
	vector<shared_ptr<CIndexObject> > *queryList;
	if(strcmp(dataType,"vector")==0)
	{
		CObjectFactory factory();

		CObjectFactory::objectRegister("vector",CDoubleVector::getConstructor());
		metric = new CEuclideanDistance;
		queryList=CDoubleVector::loadData(newQueryFile,lastQuery,dim);
	}
	else if(!strcmp(dataType,"spectra")) 
	{
		//metric = new CMSMSMetric;
		//queryList=CTandemSpectra::loadData(newQueryFile,lastQuery);
	}
	else if(!strcmp(dataType,"string")) 
	{
		CObjectFactory factory();
		CObjectFactory::objectRegister("string",CStringObject::getConstructor());
		metric = new CEditDistance;
		queryList = CStringObject::loadData( newQueryFile,lastQuery);
	}
	else if(!strcmp(dataType,"rna")) 
	{
		metric = new CRNAMetric ;
		queryList = CRNA::loadData(newQueryFile, lastQuery,fragmentLength);
	}
	else if(!strcmp(dataType,"image")) 
	{
		CObjectFactory factory();
		CObjectFactory::objectRegister("image",CImage::getConstructor());
		metric = new CImageMetric ;	
		queryList = CImage::loadData(newQueryFile ,lastQuery, dim);
	}
	else if(!strcmp(dataType,"dna")) 
	{
		CObjectFactory factory();
		CObjectFactory::objectRegister("dna",CDNA::getConstructor());
		metric = new CDNAMetric ;	
		queryList = CDNA::loadData( newQueryFile,lastQuery,fragmentLength);
	}
	else if(!strcmp(dataType,"peptide"))
	{
		CObjectFactory factory();
		CObjectFactory::objectRegister("peptide",CPeptide::getConstructor());
		metric = new CPeptideMetric;
		queryList=CPeptide::loadData(newQueryFile,lastQuery,fragmentLength);
	}

	CountedMetric *cmetric=new CountedMetric(metric);
#ifdef WIN_C
	FILETIME kernalStartTime,kernalEndTime;
	FILETIME userStartTime,userEndTime;
	FILETIME ftDummy;
#endif

	char resultFile[200]="./data/";
	strcat(resultFile,resultsFileName);
	ofstream output(resultFile,ofstream::app);
	output<<"\nresults of search in the given index"<<"\n";

	int numRun=ceil((maxRadius-minRadius)/step)+1;

	int distanceFunctionCounterBeforSearch=0,distanceFunctionCounterAfterSearch=0;
	double *mvpqueryTime=new double[numRun];
	double *linerqueryTime=new double[numRun];
	double *maxQueryTime=new double[numRun];
	double *minQueryTime=new double[numRun];
	double *mvpdistanceCalculatedTimes=new double[numRun];
	double *linerdistanceCalculatedTimes=new double[numRun];
	double *maxDistanceCalculatedTimes=new double[numRun];
	double *minDistanceCalculatedTimes=new double[numRun];

	double *avgpeakWorkingSetSize=new double[numRun];
	double *peakWorkingSetSize=new double[numRun];

	double *peekmemoryUseSize=new double[numRun];
	double *avgpagefileusage=new double[numRun];

	double *peakpagefileusage=new double[numRun];
	double *avgpeekmemoryUseSize=new double[numRun];

	double *averageMVPSearchCount=new double[numRun];
	double *averageLinerSearchCount=new double[numRun];

	output<<"datasize   "<<"searchradius   "<<"distcalculation   "<<"searchtime   "<<"searchresults   "<<"avgpeakworkingsetsize   "<<"peakworkingsetsize   "<<"avgpagefileusage    "<<"peakpagefileusage   "<<"avgpeekmemoryUseSize   "<<"peekmemoryUseSize   "<<endl;

	for(int i=0;i<numRun;i++)
	{
		mvpqueryTime[i]=0;
		linerqueryTime[i]=0;
		maxQueryTime[i]=DBL_MIN;
		minQueryTime[i]=DBL_MAX;
		mvpdistanceCalculatedTimes[i]=0;
		linerdistanceCalculatedTimes[i]=0;
		maxDistanceCalculatedTimes[i]=DBL_MIN;
		minDistanceCalculatedTimes[i]=DBL_MAX;
		averageMVPSearchCount[i] = 0.0;
		averageLinerSearchCount[i] = 0.0;

		peakWorkingSetSize[i] = 0.0;
		avgpeakWorkingSetSize[i] = 0.0;

		peekmemoryUseSize[i] = 0.0;
		avgpeekmemoryUseSize[i] = 0.0;

		peakpagefileusage[i] = 0.0;
		avgpagefileusage[i] = 0.0;



		double searchRadius=minRadius+step*i;
		int count=1;
		int startQuery=(firstQuery<0) ? 0:firstQuery;
		int endQuery=(lastQuery>queryList->size()) ? queryList->size():lastQuery;

		vector<shared_ptr<CIndexObject> > newQueryList;
		vector<shared_ptr<CIndexObject> >::iterator ite=queryList->begin();
		newQueryList.insert(newQueryList.begin(),ite+startQuery,ite+endQuery);
		ite=newQueryList.begin();

		for(;ite!=newQueryList.end();ite++)
		{
			CMemMonitor::reset();
			CRangeQuery q(minRadius+step*i,*ite);
			double t = 0.0;
			vector<shared_ptr<CIndexObject> >* mvptreeSearchResults;
			distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(cmetric)->getCounter();
#ifdef WIN_C
			GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
			if (indexMode == 0)
			{
				shared_ptr<CMVPIndex>  mvpIndex(new CMVPIndex());
				mvpIndex->setFileName(fileName);
				mvpIndex->setDataType(dataType);
				mvpIndex->setMetric(cmetric);
				mvptreeSearchResults = mvpIndex->search(&q);
				CMemMonitor::updateMem();
				averageMVPSearchCount[i] += mvptreeSearchResults->size();
			}
			else
			{
				shared_ptr< CSeachTask > searchTask (new CSeachTask(fileName,dataType,*cmetric));
				mvptreeSearchResults=searchTask->search(q);
				CMemMonitor::updateMem();
				averageMVPSearchCount[i] += mvptreeSearchResults->size();
			}
			CMemMonitor::updateMem();
			/*vector<CIndexObject*> mvptreeSearchResults=index->search(&q);*/
			GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
			t=((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;
#else
			struct tms tmp;
			clock_t begin = times(&tmp);
			CSeachTask *searchTask = new CSeachTask(fileName,dataType,*metric);
			vector<CIndexObject*> mvptreeSearchResults=searchTask->search(q);
			clock_t end = times(&tmp);
			t = (double) (end.tms_cutime-begin.tms_cutime)/CLOCKS_PER_SEC ;
#endif
			distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(cmetric)->getCounter();
			mvpqueryTime[i] +=t;
			if(maxQueryTime[i]<t)
				maxQueryTime[i]=t;
			if(minQueryTime[i]>t)
				minQueryTime[i]=t;
			int d=distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch;

			mvpdistanceCalculatedTimes[i]+=d;
			if(maxDistanceCalculatedTimes[i]<d)
				maxDistanceCalculatedTimes[i]=d;
			if(minDistanceCalculatedTimes[i]>d)
				minDistanceCalculatedTimes[i]=d;
			//output<<"mvptree search results size:"<<mvptreeSearchResults.size()<<"\ndistance calculation function called:"<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<" times"<<"\ntime elapsed during search:"<<(userEndTime.dwLowDateTime - userStartTime.dwLowDateTime)/10000000.00<<endl;


			if (peakWorkingSetSize[i] < CMemMonitor::maxWorkingSet)
				peakWorkingSetSize[i] = CMemMonitor::maxWorkingSet;
			avgpeakWorkingSetSize[i] += CMemMonitor::maxWorkingSet/1048576.0;

			if (peekmemoryUseSize[i] < CMemMonitor::maxMem)
				peekmemoryUseSize[i] = CMemMonitor::maxMem;
			avgpeekmemoryUseSize[i] += CMemMonitor::maxMem/1048576.0;

			if (peakpagefileusage[i] < CMemMonitor::maxPageUsing)
				peakpagefileusage[i] = CMemMonitor::maxPageUsing;
			avgpagefileusage[i] += CMemMonitor::maxPageUsing/1048576.0;

			count++;


			//ExitThread(statisticMemory);
			//vector<CIndexObject*> listdataobject=index->getAllDataPoints();


			if(verify)
			{
				vector<shared_ptr<CIndexObject> >* linerSearchResults;
				CListIndex* listIndex = new CListIndex(cmetric);
				linerDataList = index->getAllDataPoints();
				listIndex->bulkLoad(linerDataList,runMode);
				double t= 0.0;
				distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(listIndex->getMetric())->getCounter();
#ifdef WIN_C
				GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
				linerSearchResults=listIndex->search(&q);
				GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
				t = ((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;
#else
				struct tms tmp;
				clock_t begin = times(&tmp);
				linerSearchResults=listIndex->search(&q);
				clock_t end = times(&tmp);
				t = (double) (end.tms_cutime-begin.tms_cutime)/CLOCKS_PER_SEC ;
#endif
				distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(listIndex->getMetric())->getCounter();
				int dist = distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch;
				linerdistanceCalculatedTimes[i]+=dist;

				//output<<"liner search results size:"<<linerSearchResults.size()<<"\ndistance calculation function called:"<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<" times"<<"\ntime elapsed during search is:"<<(userEndTime.dwLowDateTime - userStartTime.dwLowDateTime)/10000000.00<<endl;

				averageLinerSearchCount[i] += linerSearchResults->size();

				linerqueryTime[i] +=t;

			}
		}
		mvpqueryTime[i]/=count-1;
		linerqueryTime[i]/=count-1;
		mvpdistanceCalculatedTimes[i]/=count-1;
		linerdistanceCalculatedTimes[i]/=count-1;
		averageMVPSearchCount[i] /= count-1;
		averageLinerSearchCount[i] /= count-1;
		avgpagefileusage[i] /= count-1;
		avgpeekmemoryUseSize[i] /= count-1;
		avgpeakWorkingSetSize[i] /= count-1;
		peakWorkingSetSize[i] = peakWorkingSetSize[i]/1048576.0;
		peekmemoryUseSize[i] = peekmemoryUseSize[i]/1048576.0;
		peakpagefileusage[i] = peakpagefileusage[i]/1048576.0;

		//output<<"\naverage search results: "<<averageMVPSearchCount[i]<<"\naverage time elapsed during search in previously built mvptree:"<<mvpqueryTime[i]<<"  maximum time elapsed during search:"<<maxQueryTime[i]<<"   minimum time elapsed during search:"<<minQueryTime[i]<<"\n";
		output<<searchSize<< "      " << searchRadius <<"      "<< mvpdistanceCalculatedTimes[i] <<"      "<<mvpqueryTime[i]<<"      "<<averageMVPSearchCount[i]<<"      "<<avgpeakWorkingSetSize[i]<<"      "<<peakWorkingSetSize[i]<<"      "<<avgpagefileusage[i]<<"      "<<peakpagefileusage[i]<<"      "<<avgpeekmemoryUseSize[i]<<"      "<<peekmemoryUseSize[i]<<endl;
	}
	delete[] mvpqueryTime; 
	delete[] linerqueryTime; 
	delete[] maxQueryTime; 
	delete[] minQueryTime; 
	delete[] mvpdistanceCalculatedTimes; 
	delete[] linerdistanceCalculatedTimes; 
	delete[] maxDistanceCalculatedTimes; 
	delete[] minDistanceCalculatedTimes; 
	delete[] averageMVPSearchCount; 
	delete[] averageLinerSearchCount; 
	delete(queryList);

}

/**@function to build a index first and then do search operation with the given query objects
*
* @param dataFileName raw data file name. 
* @param numPivot number of pivots in an index node.
* @param setA the value of A in the incremental pivot selection.
* @param setN the value of N in the incremental pivot selection.
* @param singlePivotFanout fanout of a pivot.
* @param fftscale use for pcaonfft pivot selection method.
* @param pivotSelectionMethod the pivot selection selection method: random, fft, center, pcaonfft, pca.
* @param partitionMethod data partition method: balanced, clusteringkmeans, clusteringboundary.
* @param maxLeafSize maximum number of children in leaf nodes.
* @param dataType data type, one of "protein", "dna", "vector", "image", "msms","string".
* @param initialSize size of smallest database.
* @param finalSize size of largest database.
* @param stepSize step size of databases.
* @param bucket whether bucketing will be used, 1: use.
* @param fragmentLength fragment length, only meaningful for sequences.
* @param dim dimension of vector data to load or number of features to be loaded.
* @param maxR maximum radius for partition.
* @param indexType [list,mvptree],the type of the index built later.
* @param fftOpt 0:use old version FFT, 1: use new version FFT but don't choose the third point got from FFT as the second pivot, 2:use new version FFT and choose the third point got from FFT as the second pivot. Default 0.
* @param indexFileName name of the file storing the index structure built before.
* @param queryFileName query file name. 
* @param firstQuery offset of first query to be used in the query file, start from 0, inclusive, default 0. 
* @param lastQuery offset of last query to be used in the query file, exclusive, default 1. 
* @param minRadius maximum search radius, default 10. 
* @param maxRadius minimum search radius, default 0. 
* @param step step size for search radii, default 1. 
* @param verify 1 if search results are to be verified against a linear scan and 0 otherwise, default 0. 
* @param resultsFileName name of the file to store the result information of the search operation. 
*
*/
void buildAndSearch(char *&dataFileName,int &numPivot,int setA,int setN,int &singlePivotFanout,int fftscale,char *&pivotSelectionMethod,char *&partitionMethod,int &maxLeafSize,char *&dataType,int initialSize,int finalSize,int stepSize,bool bucket,int &fragmentLength,int dim,double maxR,char *&indexType,int fftOpt,char* queryFileName,int firstQuery,int lastQuery,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName,int runMode,char *indexName,int indexMode)
{

	vector< shared_ptr<CIndexObject> > *rawData=0;
	CMetric *metric=0;
	CPivotSelectionMethod *psm=0;
	CPartitionMethod *pm=0;
	CIndex *index =0;
	int indexNum = (finalSize - initialSize) / stepSize + 1;

	char newDataFileName[100]="./data/";
	strcat(newDataFileName,dataFileName);

	char newQueryFile[50]="./data/";
	strcat(newQueryFile,queryFileName);

	char newIndexName[100]="./data/";
	strcat(newIndexName,indexName);

	vector< shared_ptr<CIndexObject> > *queryList;

	stringstream newSize;
#ifdef WIN_C
	FILETIME kernalStartTime,kernalEndTime;
	FILETIME userStartTime,userEndTime;
	FILETIME ftDummy;
#endif

	for (int size = initialSize, i = 0; (size <= finalSize) & (i < indexNum); size += stepSize, i++)
	{
		if(!strcmp(dataType,"vector")) 
		{
			CObjectFactory factory();
			CObjectFactory::objectRegister("vector",CDoubleVector::getConstructor());
			rawData=CDoubleVector::loadData(newDataFileName,size,dim);
			metric = new CEuclideanDistance;
			queryList=CDoubleVector::loadData(newQueryFile,lastQuery,dim);
		}
		else if(!strcmp(dataType,"spectra")) 
		{
			//rawData = CTandemSpectra::loadData(newDataFileName,size);
			//metric = new CMSMSMetric;
			//queryList=CDoubleVector::loadData(newQueryFile,lastQuery,dim);
		}
		else if(!strcmp(dataType,"string")) 
		{
			CObjectFactory factory();
			CObjectFactory::objectRegister("string",CStringObject::getConstructor());
			rawData = CStringObject::loadData(newDataFileName,size);
			metric = new CEditDistance;
			queryList = CStringObject::loadData( newQueryFile,lastQuery);
		}
		else if(!strcmp(dataType,"rna")) 
		{
			rawData = CRNA::loadData(newDataFileName, finalSize,fragmentLength);
			metric = new CRNAMetric ;
			queryList = CRNA::loadData(newQueryFile, lastQuery,fragmentLength);
		}
		else if(!strcmp(dataType,"image")) 
		{
			CObjectFactory factory();
			CObjectFactory::objectRegister("image",CImage::getConstructor());
			rawData = CImage::loadData(newDataFileName ,size, dim);
			metric = new CImageMetric ;
			queryList = CImage::loadData(newQueryFile ,lastQuery, dim);
		}
		else if(!strcmp(dataType,"dna")) 
		{
			CObjectFactory factory();
			CObjectFactory::objectRegister("dna",CDNA::getConstructor());
			rawData = CDNA::loadData(newDataFileName,finalSize,fragmentLength);
			metric = new CDNAMetric ;	
			queryList = CDNA::loadData( newQueryFile,lastQuery,fragmentLength);
		}
		else if(!strcmp(dataType,"peptide"))
		{
			CObjectFactory factory();
			CObjectFactory::objectRegister("peptide",CPeptide::getConstructor());
			rawData = CPeptide::loadData(newDataFileName,finalSize,fragmentLength);
			metric = new CPeptideMetric;
			queryList=CPeptide::loadData(newQueryFile,lastQuery,fragmentLength);
		}

		//pivotSelectionMethod
		if(!strcmp(pivotSelectionMethod,"fft"))
		{
			psm=new CFFTPivotSelectionMethod;
		}
		else if(!strcmp(pivotSelectionMethod,"incremental"))
		{
			psm = new CIncrementalSelection(setA,setN) ;
		}
		else if(!strcmp(pivotSelectionMethod,"pcaonfft"))
		{
			psm = new CPCAOnFFT(fftscale);
		}
		else
		{
			cout<<"invalid pivot selection method"<<endl;
		}

		// partitionMethod
		if(!strcmp(partitionMethod,"balanced"))
		{
			pm=new CBalancePartitionMethod;
		}

		CountedMetric *cmetric=new CountedMetric(metric);

		// indexType
		if (!strcmp(indexType,"MVPIndex"))
		{
			index=new CMVPIndex(*rawData,cmetric,psm,pm,numPivot,singlePivotFanout,maxLeafSize);
		}
		else if(!strcmp(indexType,"ListIndex"))
		{
			index=new CListIndex(cmetric);
		}

		int distanceFunctionCounterBeforSearch=0,distanceFunctionCounterAfterSearch=0;

		index->setFileName(newIndexName);
		index->setDataType(dataType);

		char resultFile[100]="./data/";
		strcat(resultFile,resultsFileName);
		ofstream output(resultFile,ofstream::app);
		double t = 0.0;
		distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();
#ifdef WIN_C
		GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
		index->bulkLoad(*rawData,runMode);
		GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
		t = ((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime))/10000000.00;
#else
		struct tms tmp;
		clock_t begin = times(&tmp);
		index->bulkLoad(*rawData,runMode);
		clock_t end = times(&tmp);
		t = (double) (end.tms_cutime-begin.tms_cutime)/CLOCKS_PER_SEC;
#endif
		distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();

		output<<"database size:"<<size<<"  distance calculated times:"<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<" time elapsed during building index:"<<t<<'\n';

		if(index==0)
		{
			exit(0);
		}

		output<<"\nresults of search in the given index"<<"\n";

		int numRun=floor((maxRadius-minRadius)/step)+1;


		double *mvpqueryTime=new double[numRun];
		double *linerqueryTime=new double[numRun];
		double *maxQueryTime=new double[numRun];
		double *minQueryTime=new double[numRun];
		double *mvpdistanceCalculatedTimes=new double[numRun];
		double *linerdistanceCalculatedTimes=new double[numRun];
		double *maxDistanceCalculatedTimes=new double[numRun];
		double *minDistanceCalculatedTimes=new double[numRun];
		double *avgmemoryUseSize=new double[numRun];
		double *peekmemoryUseSize=new double[numRun];
		double *avgpagefileusage=new double[numRun];
		double *peakpagefileusage=new double[numRun];

		double *averageMVPSearchCount=new double[numRun];
		double *averageLinerSearchCount=new double[numRun];
		HANDLE handle;
		PROCESS_MEMORY_COUNTERS pmc;  
		output<<"datasize   "<<"searchradius   "<<"mvpdistancecalculation   "<<"mvpsearchtime(s)   "<<"mvpsearchresults   "<<"avgmemoryUseSize(MB)   "<<"peekmemoryUseSize(MB)   "<<"avgpagefileusage(MB)    "<<"peakpagefileusage(MB)"<<endl;
		shared_ptr<CMVPIndex>  mvpIndex(new CMVPIndex());
		mvpIndex->setFileName(newIndexName);
		mvpIndex->setDataType(dataType);
		mvpIndex->setMetric(cmetric);
		vector<shared_ptr<CIndexObject> >* mvptreeSearchResults;

		for(int i=0;i<numRun;i++)
		{
			mvpqueryTime[i]=0;
			linerqueryTime[i]=0;
			maxQueryTime[i]=DBL_MIN;
			minQueryTime[i]=DBL_MAX;
			mvpdistanceCalculatedTimes[i]=0;
			linerdistanceCalculatedTimes[i]=0;
			maxDistanceCalculatedTimes[i]=DBL_MIN;
			minDistanceCalculatedTimes[i]=DBL_MAX;
			averageMVPSearchCount[i] = 0.0;
			averageLinerSearchCount[i] = 0.0;
			avgmemoryUseSize[i] = 0.0;
			peekmemoryUseSize[i] = 0.0;
			avgpagefileusage[i] = 0.0;
			peakpagefileusage[i] = 0.0;
			pmc.WorkingSetSize = 0;
			pmc.PeakWorkingSetSize = 0;
			pmc.PagefileUsage = 0;
			pmc.PeakPagefileUsage = 0;

			//output<<"*****************************************************"<<endl;
			double searchRadius=minRadius+step*i;
			//output<<"search radius is:"<<searchRadius<<endl;

			int count=1;
			double t = 0.0;

			int startQuery=(firstQuery<0) ? 0:firstQuery;
			int endQuery=(lastQuery>queryList->size()) ? queryList->size():lastQuery;

			vector<shared_ptr<CIndexObject> > newQueryList;
			vector<shared_ptr<CIndexObject> >::iterator ite=queryList->begin();
			newQueryList.insert(newQueryList.begin(),ite+startQuery,ite+endQuery);
			ite=newQueryList.begin();

			for(;ite!=newQueryList.end();ite++)
			{
				//output<<"----------------------------------------"<<endl;
				//output<<"q:"<<count<<endl;
				CRangeQuery q(minRadius+step*i,*ite);
				distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();
#ifdef WIN_C
				GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
				/*vector<CIndexObject*> mvptreeSearchResults=index->search(&q);*/
				if (indexMode == 0)
				{
					mvptreeSearchResults = mvpIndex->search(&q);
					averageMVPSearchCount[i] += mvptreeSearchResults->size();
				}
				else
				{
					shared_ptr< CSeachTask > searchTask (new CSeachTask(newIndexName,dataType,*cmetric));
					mvptreeSearchResults=searchTask->search(q);
					averageMVPSearchCount[i] += mvptreeSearchResults->size();
				}

				GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
				t = ((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime))/10000000.00;
#else
				struct tms tmp;
				clock_t begin = times(&tmp);
				if (indexMode == 0)
				{
					mvptreeSearchResults=&(mvpIndex->search(&q));
					averageMVPSearchCount[i] += mvptreeSearchResults->size();
				}
				else if (indexMode == 1)
				{
					CSeachTask *searchTask = new CSeachTask(newIndexName,dataType,*(cmetric));
					mvptreeSearchResults=searchTask->search(q);
					averageMVPSearchCount[i] += mvptreeSearchResults->size();
				}
				clock_t end = times(&tmp);
				t = (double) (end.tms_cutime-begin.tms_cutime)/CLOCKS_PER_SEC;
#endif

				distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();
				mvpqueryTime[i] +=t;
				if(maxQueryTime[i]<t)
					maxQueryTime[i]=t;
				if(minQueryTime[i]>t)
					minQueryTime[i]=t;
				int d=distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch;

				mvpdistanceCalculatedTimes[i]+=d;
				if(maxDistanceCalculatedTimes[i]<d)
					maxDistanceCalculatedTimes[i]=d;
				if(minDistanceCalculatedTimes[i]>d)
					minDistanceCalculatedTimes[i]=d;
				//output<<"mvptree search results size:"<<mvptreeSearchResults.size()<<"\ndistance calculation function called:"<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<" times"<<"\ntime elapsed during search:"<<(userEndTime.dwLowDateTime - userStartTime.dwLowDateTime)/10000000.00<<endl;

				handle=GetCurrentProcess();  
				GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));  
				avgmemoryUseSize[i] += pmc.WorkingSetSize/1048576.0;
				peekmemoryUseSize[i] +=  pmc.PeakWorkingSetSize/1048576.0;
				avgpagefileusage[i] += pmc.PagefileUsage/1048576.0;
				peakpagefileusage[i] +=  pmc.PeakPagefileUsage/1048576.0;



				count++;


				if(verify)
				{
					vector<shared_ptr<CIndexObject> >* linerSearchResults;
					CListIndex* listIndex = new CListIndex(cmetric);
					listIndex->bulkLoad(*rawData,runMode);
					double t= 0.0;
					distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(listIndex->getMetric())->getCounter();
#ifdef WIN_C
					GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
					linerSearchResults=listIndex->search(&q);
					GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
					t = ((userEndTime.dwLowDateTime + kernalEndTime.dwLowDateTime) - (userStartTime.dwLowDateTime + kernalStartTime.dwLowDateTime)) / 10000000.00;
#else
					struct tms tmp;
					clock_t begin = times(&tmp);
					linerSearchResults=listIndex->search(&q);
					clock_t end = times(&tmp);
					t = (double) (end.tms_cutime-begin.tms_cutime)/CLOCKS_PER_SEC ;
#endif
					distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(listIndex->getMetric())->getCounter();
					int dist = distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch;
					linerdistanceCalculatedTimes[i]+=dist;

					averageLinerSearchCount[i] += linerSearchResults->size();
					linerqueryTime[i] +=t;
					delete listIndex;

				}
			}

			mvpqueryTime[i]/=count-1;
			linerqueryTime[i]/=count-1;
			mvpdistanceCalculatedTimes[i]/=count-1;
			linerdistanceCalculatedTimes[i]/=count-1;
			averageMVPSearchCount[i] /= count-1;
			averageLinerSearchCount[i] /= count-1;
			avgmemoryUseSize[i] /= count-1;
			peekmemoryUseSize[i] /= count-1;
			avgpagefileusage[i] /= count-1;
			peakpagefileusage[i] /= count-1;

			output<< initialSize << "      " << searchRadius <<"      "<< mvpdistanceCalculatedTimes[i] <<"      "<<mvpqueryTime[i]<<"      "<<averageMVPSearchCount[i]<<"      "<<avgmemoryUseSize[i]<<"      "<<peekmemoryUseSize[i]<<"      "<<avgpagefileusage[i]<<"      "<<peakpagefileusage[i]<<endl;

		}
	}
}
