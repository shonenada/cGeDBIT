#include "../../HeaderFiles/objects/DoubleVector.h"
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
#include "../../HeaderFiles/metric/MSMSMetric.h"
#include "../../HeaderFiles/metric/RNAMetric.h"
#include "../../HeaderFiles/metric/PeptideMetric.h"

#include <sstream>
#include <Windows.h>
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
void batchBulkLoad(char *&dataFileName,int &numPivot,int setA,int setN,int &singlePivotFanout,int fftscale,char *&pivotSelectionMethod,char *&partitionMethod,int &maxLeafSize,char *&dataType,int initialSize,int finalSize,int stepSize,bool bucket,int &fragmentLength,int dim,double maxR,char *&indexType,int fftOpt,char *&buildingInformationFile)
{

	vector<CIndexObject*> *rawData=0;
	CMetric *metric=0;
	CPivotSelectionMethod *psm=0;
	CPartitionMethod *pm=0;
	CIndex *index =0;
	int indexNum = (finalSize - initialSize) / stepSize + 1;
	
    double *buildTimes=new double[indexNum];
    int *distanceCalculatedTimes=new int[indexNum];
    
    if(!strcmp(dataType,"vector")) 
	{
		metric = new CEuclideanDistance;
	}
    else if(!strcmp(dataType,"spectra")) 
	{
		metric = new CMSMSMetric;
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

    FILETIME kernalStartTime,kernalEndTime;
    FILETIME userStartTime,userEndTime;
    FILETIME ftDummy;

    char newDataFileName[100]="./data/";
    strcat(newDataFileName,dataFileName);

    stringstream newSize;
    
    int distanceFunctionCounterBeforSearch=0,distanceFunctionCounterAfterSearch=0;
	for (int size = initialSize, i = 0; (size <= finalSize) & (i < indexNum); size += stepSize, i++)
	{
	    if(!strcmp(dataType,"vector")) 
	    {
		    rawData=CDoubleVector::loadData(newDataFileName,size,dim);
	    }
        else if(!strcmp(dataType,"spectra")) 
	    {
		    rawData = CTandemSpectra::loadData(newDataFileName);
	    }
	    else if(!strcmp(dataType,"string")) 
	    {
		    rawData = CStringObject::loadData(newDataFileName);
	    }
	    else if(!strcmp(dataType,"rna")) 
	    {
		    rawData = CRNA::loadData(newDataFileName, finalSize,fragmentLength);
	    }
	    else if(!strcmp(dataType,"image")) 
        {
		    rawData = CImage::loadData(newDataFileName ,size, dim);
        }
        else if(!strcmp(dataType,"dna")) 
        {
		    rawData = CDNA::loadData(newDataFileName,finalSize,fragmentLength);
	    }
        else if(!strcmp(dataType,"peptide"))
        {
            rawData = CPeptide::loadData(newDataFileName,finalSize,fragmentLength);
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
        
        distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();
        GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);

	    index->bulkLoad(*rawData);
        
        GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
        distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();

        buildTimes[i]=(userEndTime.dwLowDateTime - userStartTime.dwLowDateTime) / 10000000.00;
        distanceCalculatedTimes[i]=distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch;

        newSize<<size;

        char newFileName[100]="./data/";
        strcat(newFileName,indexType);
        strcat(newFileName,"-");
        strcat(newFileName,(newSize.str()).c_str());
        strcat(newFileName,"-GeDBIT.db");

	    ofstream ofs(newFileName);
	    boost::archive::text_oarchive oa(ofs);
	    index->write(oa);
	    ofs.close();
	    ofs.clear();

	}

    char resultFileName[50]="./data/";
    strcat(resultFileName,buildingInformationFile);

    ofstream of(resultFileName,ofstream::app);
    of<<"*database size*  *building time*  *distance calculated times*"<<'\n';
    for(int i=0,size=initialSize;i<indexNum && size<=finalSize;i++,size+=stepSize)
    {
        of<<" "<<size<<"              "<<buildTimes[i]<<"         "<<distanceCalculatedTimes[i]<<"   \n"<<endl;
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
void searchIndex(char* indexFileName,char* queryFileName,char *dataType,int firstQuery,int lastQuery,int dim,int fragmentLength,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName)
{
        char fileName[100]="./data/";
        strcat(fileName,indexFileName);

        CIndex *index=0;
        CMetric *metric=0;

        vector<CIndexObject*> linerDataList;

        ifstream inf(fileName);
        boost::archive::text_iarchive ia(inf);

        ia>>index;
        inf.close();
        inf.clear();

        char newQueryFile[20]="./data/";
        strcat(newQueryFile,queryFileName);

        if(index==0)
        {
            exit(0);
        }
        vector<CIndexObject*> *queryList;
        if(strcmp(dataType,"vector")==0)
        {
            queryList=CDoubleVector::loadData(newQueryFile,lastQuery,dim);
        }
        else if(!strcmp(dataType,"spectra")) 
	    {
		    queryList = CTandemSpectra::loadData(newQueryFile);
	    }
	    else if(!strcmp(dataType,"string")) 
	    {
		    queryList = CStringObject::loadData( newQueryFile);
	    }
	    else if(!strcmp(dataType,"rna")) 
	    {
		    queryList = CRNA::loadData(newQueryFile, lastQuery,fragmentLength);
	    }
	    else if(!strcmp(dataType,"image")) 
	    {
		    queryList = CImage::loadData(newQueryFile ,lastQuery, dim);
	    }
	    else if(!strcmp(dataType,"dna")) 
	    {
		    queryList = CDNA::loadData( newQueryFile,lastQuery,fragmentLength);
	    }

        if(verify)
        {
            linerDataList=index->getAllDataPoints();

            if(!strcmp(dataType,"vector")) 
	        {
		        metric = new CEuclideanDistance;
	        }
	        else if(!strcmp(dataType,"spectra")) 
	        {
		        metric = new CMSMSMetric;
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
        }
        CountedMetric *cmetric=new CountedMetric(metric);

        FILETIME kernalStartTime,kernalEndTime;
        FILETIME userStartTime,userEndTime;
        FILETIME ftDummy;
        
        char resultFile[100]="./data/";
        strcat(resultFile,resultsFileName);
        ofstream output(resultFile,ofstream::app);
        output<<"results of search in the given index"<<"\n";

        int numRun=(maxRadius-minRadius)/step+1;
        
        double *queryTime=new double[numRun];
        double *maxQueryTime=new double[numRun];
        double *minQueryTime=new double[numRun];
        int *distanceCalculatedTimes=new int[numRun];
        int *maxDistanceCalculatedTimes=new int[numRun];
        int *minDistanceCalculatedTimes=new int[numRun];
        
        for(int i=0;i<numRun;i++)
        {
            queryTime[i]=0;
            maxQueryTime[i]=DBL_MIN;
            minQueryTime[i]=DBL_MAX;
            distanceCalculatedTimes[i]=0;
            maxDistanceCalculatedTimes[i]=DBL_MIN;
            minDistanceCalculatedTimes[i]=DBL_MAX;

            output<<"*****************************************************"<<endl;
            double searchRadius=minRadius+step*i;
            output<<"search radius is:"<<searchRadius<<endl;
            
            int count=1;

            int startQuery=(firstQuery<0) ? 0:firstQuery;
            int endQuery=(lastQuery>queryList->size()) ? queryList->size():lastQuery;

            vector<CIndexObject*> newQueryList;
            vector<CIndexObject*>::iterator ite=queryList->begin();
            newQueryList.insert(newQueryList.begin(),ite+startQuery,ite+endQuery);

            ite=newQueryList.begin();
            vector<CIndexObject*> mpvtreeSearchResults;

            int distanceFunctionCounterBeforSearch=0,distanceFunctionCounterAfterSearch=0;
            for(;ite!=newQueryList.end();ite++)
            {
                output<<"----------------------------------------"<<endl;
                output<<"q:"<<count<<endl;
                CRangeQuery q(minRadius+step*i,(CDoubleVector*)(*ite));

                distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();
                GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
                vector<CIndexObject*> mvptreeSearchResults=index->search(&q);
	            GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
                distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();

                double t=(userEndTime.dwLowDateTime - userStartTime.dwLowDateTime) / 10000000.00;
                queryTime[i]=t;
                if(maxQueryTime[i]<t)
                    maxQueryTime[i]=t;
                if(minQueryTime[i]>t)
                    minQueryTime[i]=t;
                int d=distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch;

                distanceCalculatedTimes[i]=d;
                if(maxDistanceCalculatedTimes[i]<d)
                    maxDistanceCalculatedTimes[i]=d;
                if(minDistanceCalculatedTimes[i]>d)
                    minDistanceCalculatedTimes[i]=d;

	            output<<"mvptree search results size:"<<mvptreeSearchResults.size()<<"\ndistance calculation function called:"<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<" times\n"<<"time elapsed during search:"<<(userEndTime.dwLowDateTime - userStartTime.dwLowDateTime) / 10000000.00<<'\n';

                count++;
                if(verify)
                {
                    vector<CIndexObject*> linerSearchResults;

	                CListIndex* listIndex = new CListIndex(cmetric);
                    listIndex->bulkLoad(linerDataList);

                    distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();
                    GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
                    linerSearchResults=listIndex->search(&q);
                    GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
                    distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();

	                output<<"liner search results size:"<<linerSearchResults.size()<<"\ndistance calculation function called:"<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<" times"<<"\nTime elapsed during search:"<<(userEndTime.dwLowDateTime - userStartTime.dwLowDateTime)/10000000.00<<endl;
                }
                output<<"----------------------------------------"<<endl;
            }

            queryTime[i]/=count;
            distanceCalculatedTimes[i]/=count;
            
            output<<"average time elapsed during search in previously built mvptree:"<<queryTime[i]<<"  maximum time elapsed during search:"<<maxQueryTime[i]<<"   minimum time elapsed during search:"<<minQueryTime[i]<<"\n";
            output<<"*****************************************************"<<endl;
        }

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
void buildAndSearch(char *&dataFileName,int &numPivot,int setA,int setN,int &singlePivotFanout,int fftscale,char *&pivotSelectionMethod,char *&partitionMethod,int &maxLeafSize,char *&dataType,int initialSize,int finalSize,int stepSize,bool bucket,int &fragmentLength,int dim,double maxR,char *&indexType,int fftOpt,char* queryFileName,int firstQuery,int lastQuery,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName)
{
    vector<CIndexObject*> *rawData=0;
	CMetric *metric=0;
	CPivotSelectionMethod *psm=0;
	CPartitionMethod *pm=0;
	CIndex *index =0;
	int indexNum = (finalSize - initialSize) / stepSize + 1;
	
    char newDataFileName[100]="./data/";
    strcat(newDataFileName,dataFileName);

    char newQueryFile[50]="./data/";
    strcat(newQueryFile,queryFileName);

    vector<CIndexObject*> *queryList;

    stringstream newSize;

    FILETIME kernalStartTime,kernalEndTime;
    FILETIME userStartTime,userEndTime;
    FILETIME ftDummy;

	for (int size = initialSize, i = 0; (size <= finalSize) & (i < indexNum); size += stepSize, i++)
	{
	    if(!strcmp(dataType,"vector")) 
	    {
		    rawData=CDoubleVector::loadData(newDataFileName,size,dim);
		    metric = new CEuclideanDistance;
            queryList=CDoubleVector::loadData(newQueryFile,lastQuery,dim);
	    }
        else if(!strcmp(dataType,"spectra")) 
	    {
		    rawData = CTandemSpectra::loadData(newDataFileName);
		    metric = new CMSMSMetric;
            queryList=CDoubleVector::loadData(newQueryFile,lastQuery,dim);
	    }
	    else if(!strcmp(dataType,"string")) 
	    {
		    rawData = CStringObject::loadData(newDataFileName);
	        metric = new CEditDistance;
		    queryList = CStringObject::loadData( newQueryFile);
	    }
	    else if(!strcmp(dataType,"rna")) 
	    {
		    rawData = CRNA::loadData(newDataFileName, finalSize,fragmentLength);
	        metric = new CRNAMetric ;
		    queryList = CRNA::loadData(newQueryFile, lastQuery,fragmentLength);
	    }
	    else if(!strcmp(dataType,"image")) 
        {
		    rawData = CImage::loadData(newDataFileName ,size, dim);
	        metric = new CImageMetric ;
		    queryList = CImage::loadData(newQueryFile ,lastQuery, dim);
        }
        else if(!strcmp(dataType,"dna")) 
        {
		    rawData = CDNA::loadData(newDataFileName,finalSize,fragmentLength);
	        metric = new CDNAMetric ;	
		    queryList = CDNA::loadData( newQueryFile,lastQuery,fragmentLength);
	    }
        else if(!strcmp(dataType,"peptide"))
        {
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


        char resultFile[100]="./data/";
        strcat(resultFile,resultsFileName);
        ofstream output(resultFile,ofstream::app);
      
        distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();
        GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
	    index->bulkLoad(*rawData);
        GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
        distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();

        output<<"database size:"<<size<<"  distance calculated times:"<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<" time elapsed during building index:"<<(userEndTime.dwLowDateTime - userStartTime.dwLowDateTime)/10000000.00<<'\n';

        if(index==0)
        {
            exit(0);
        }


        /*char resultFile[100]="./data/";
        strcat(resultFile,resultsFileName);
        ofstream output(resultFile,ofstream::app);*/

        output<<"results of search in the given index"<<"\n";

        int numRun=(maxRadius-minRadius)/step+1;

        double *queryTime=new double[numRun];
        double *maxQueryTime=new double[numRun];
        double *minQueryTime=new double[numRun];
        double *distanceCalculatedTimes=new double[numRun];
        double *maxDistanceCalculatedTimes=new double[numRun];
        double *minDistanceCalculatedTimes=new double[numRun];

        for(int i=0;i<numRun;i++)
        {
            queryTime[i]=0;
            maxQueryTime[i]=DBL_MIN;
            minQueryTime[i]=DBL_MAX;
            distanceCalculatedTimes[i]=0;
            maxDistanceCalculatedTimes[i]=DBL_MIN;
            minDistanceCalculatedTimes[i]=DBL_MAX;

            output<<"*****************************************************"<<endl;
            double searchRadius=minRadius+step*i;
            output<<"search radius is:"<<searchRadius<<endl;
            
            int count=1;

            int startQuery=(firstQuery<0) ? 0:firstQuery;
            int endQuery=(lastQuery>queryList->size()) ? queryList->size():lastQuery;

            vector<CIndexObject*> newQueryList;
            vector<CIndexObject*>::iterator ite=queryList->begin();
            newQueryList.insert(newQueryList.begin(),ite+startQuery,ite+endQuery);

            ite=newQueryList.begin();
            vector<CIndexObject*> mpvtreeSearchResults;

            for(;ite!=newQueryList.end();ite++)
            {
                output<<"----------------------------------------"<<endl;
                output<<"q:"<<count<<endl;
                CRangeQuery q(minRadius+step*i,*ite);

                distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();
                GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
                vector<CIndexObject*> mvptreeSearchResults=index->search(&q);
                GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
                distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(index->getMetric())->getCounter();

                double t=(userEndTime.dwLowDateTime - userStartTime.dwLowDateTime) / 10000000.00;
                queryTime[i]+=t;
                if(maxQueryTime[i]<t)
                    maxQueryTime[i]=t;
                if(minQueryTime[i]>t)
                    minQueryTime[i]=t;
                int d=distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch;

                distanceCalculatedTimes[i]+=d;
                if(maxDistanceCalculatedTimes[i]<d)
                    maxDistanceCalculatedTimes[i]=d;
                if(minDistanceCalculatedTimes[i]>d)
                    minDistanceCalculatedTimes[i]=d;
	            output<<"mvptree search results size:"<<mvptreeSearchResults.size()<<"\ndistance calculation function called:"<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<" times"<<"\ntime elapsed during search:"<<(userEndTime.dwLowDateTime - userStartTime.dwLowDateTime)/10000000.00<<endl;
                
                count++;

                //vector<CIndexObject*> listdataobject=index->getAllDataPoints();

                if(verify)
                {
                    vector<CIndexObject*> linerSearchResults;

	                CListIndex* listIndex = new CListIndex(cmetric);
                    listIndex->bulkLoad(*rawData);

                    distanceFunctionCounterBeforSearch=dynamic_cast<CountedMetric*>(listIndex->getMetric())->getCounter();
                    GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalStartTime,&userStartTime);
                    linerSearchResults=listIndex->search(&q);
                    GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&kernalEndTime,&userEndTime);
                    distanceFunctionCounterAfterSearch=dynamic_cast<CountedMetric*>(listIndex->getMetric())->getCounter();

	                output<<"liner search results size:"<<linerSearchResults.size()<<"\ndistance calculation function called:"<<distanceFunctionCounterAfterSearch-distanceFunctionCounterBeforSearch<<" times"<<"\ntime elapsed during search is:"<<(userEndTime.dwLowDateTime - userStartTime.dwLowDateTime)/10000000.00<<endl;

                }

                output<<"----------------------------------------"<<endl;
            }

            queryTime[i]/=count-1;
            distanceCalculatedTimes[i]/=count-1;
            
            output<<"average time elapsed during search in previously built mvptree:"<<queryTime[i]<<"  maximum time elapsed during search:"<<maxQueryTime[i]<<"   minimum time elapsed during search:"<<minQueryTime[i]<<"\n";
            output<<"*****************************************************"<<endl;
        }

    }
}