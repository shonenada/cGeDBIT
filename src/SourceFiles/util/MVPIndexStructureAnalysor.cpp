#include "..\..\HeaderFiles\index\MVPIndex.h"

#include "..\..\HeaderFiles\index\IndexNode.h"
#include "..\..\HeaderFiles\index\MVPInternalNode.h"
#include "..\..\HeaderFiles\index\MVPLeafNode.h"

#include "..\..\HeaderFiles\objects\IndexObject.h"
#include "..\..\HeaderFiles\objects\DoubleVector.h"
#include "..\..\HeaderFiles\objects\DNA.h"
#include "..\..\HeaderFiles\objects\RNA.h"
#include "..\..\HeaderFiles\objects\Peptide.h"
#include "..\..\HeaderFiles\objects\StringObject.h"
#include "..\..\HeaderFiles\objects\Image.h"
#include "..\..\HeaderFiles\objects\TandemSpectra.h"
#include "..\..\HeaderFiles\query\SeachTask.h"

#include "..\..\HeaderFiles\metric\DNAMetric.h"
#include "..\..\HeaderFiles\metric\CountedMetric.h"
#include "..\..\HeaderFiles\metric\EditDistance.h"
#include "..\..\HeaderFiles\metric\ImageMetric.h"
//#include "..\..\HeaderFiles\metric\MSMSMetric.h"
#include "..\..\HeaderFiles\metric\RNAMetric.h"
#include "..\..\HeaderFiles\metric\PeptideMetric.h"
#include "..\..\HeaderFiles\metric\EuclideanDistance.h"

#include<iomanip>
#define MVPISA_INFO_OUTPUT
#undef MVPISA_INFO_OUTPUT


struct traversalTask
{
	string type;
	shared_ptr<CIndexNode> currentNode;
	int nodeHeight;
	int currentNodeLayerNO;
	int parentNodeLayerNO;

};

void analysisCurrentNode(vector<shared_ptr<traversalTask> > &taskList,ifstream &iofilen,shared_ptr<traversalTask> &task,long filePointer,int &totalInternalNumber,int &totalLeafNumber,char* dataType,int &tempLayerNodeNumber,int &layerInternalNodeChildNumber,int &layerLeafNodeChildNumber)
{
	
	if(!strcmp(task->type.c_str(),"LNODE"))
	{
		/*shared_ptr<CMVPLeafNode> lnode;
		lnode.reset(&(CMVPLeafNode()));*/
		totalLeafNumber++;

	}
	else
	{
		totalInternalNumber++;

		char type[6]="";
		shared_ptr<CIndexNode> node;
		shared_ptr<traversalTask> ttask;
		traversalTask *t=0;
		for(int i=0;i<dynamic_cast<CMVPInternalNode*>(task->currentNode.get())->getChildeSize();i++)
		{
			filePointer=(dynamic_cast<CMVPInternalNode*>(task->currentNode.get()))->getSubTreeRootAddress(i);
			//cout<<"child "<<i<<" filePointer:"<<filePointer;
			iofilen.seekg(filePointer,ios::beg);
			iofilen.read(type,6*sizeof(char));
			//cout<<"  type:"<<type<<endl;
			if(!strcmp(type,"LNODE"))
			{
				layerLeafNodeChildNumber++;
				node=dynamic_pointer_cast<CMVPLeafNode,CIndexNode>(node);
				node.reset(new CMVPLeafNode);
				node.get()->readExternal(iofilen,string(dataType));
					
				t=new traversalTask();
				t->currentNode=node;
				t->nodeHeight=task->nodeHeight+1;
				t->parentNodeLayerNO=task->currentNodeLayerNO;
				t->type="LNODE";
				t->currentNodeLayerNO=layerInternalNodeChildNumber+layerLeafNodeChildNumber;
						
				ttask.reset(t);
				taskList.push_back(ttask);
			}
			else
			{
				layerInternalNodeChildNumber++;

				node=dynamic_pointer_cast<CMVPInternalNode,CIndexNode>(node);
				node.reset(new CMVPInternalNode);
				node.get()->readExternal(iofilen,string(dataType));

				/*if(layerInternalNodeChildNumber+layerLeafNodeChildNumber==247)
				{
					cout<<"parent node height:"<<task->nodeHeight<<" parent node layer id:"<<tempLayerNodeNumber<<" task->currentNodeLayerN0:"<<task->currentNodeLayerNO<<"	 current node height:"<<task->nodeHeight+1<<"	 currrent node layer id"<<layerInternalNodeChildNumber+layerLeafNodeChildNumber<<endl;
				}*/

				t=new traversalTask();
				t->currentNode=node;
				t->nodeHeight=task->nodeHeight+1;
				t->parentNodeLayerNO=task->currentNodeLayerNO;
				t->type="INODE";
				t->currentNodeLayerNO=layerInternalNodeChildNumber+layerLeafNodeChildNumber;
						
				ttask.reset(t);
				taskList.push_back(ttask);

			}
		}
	}

}
void getMVPIndexStructureInfo(char *&indexFileName,char *& dataType,char *& outputFileName)
{
	cout<<"start get mvp-tree structure information!"<<endl;
	char newIndexFileName[100]=".\\data\\";
	strcat(newIndexFileName,indexFileName);

	//char newOutputFileName[100]=".\\data\\";
	//strcat(newOutputFileName,outputFileName);


	long filePointer=0;

	if(strcmp(dataType,"vector")==0)
	{
		CObjectFactory::objectRegister("vector",CDoubleVector::getConstructor());
	}
	else if(!strcmp(dataType,"spectra")) 
	{
		//metric = new CMSMSMetric;
		//queryList=CTandemSpectra::loadData(newQueryFile,lastQuery);
		//CObjectFactory::objectRegister("spectra",CTandemSpectra::getConstructor());
	}
	else if(!strcmp(dataType,"string")) 
	{
		CObjectFactory::objectRegister("string",CStringObject::getConstructor());
	}
	else if(!strcmp(dataType,"rna")) 
	{
		//CObjectFactory::objectRegister("rna",CRNA::getConstructor());
	}
	else if(!strcmp(dataType,"image")) 
	{
		CObjectFactory::objectRegister("image",CImage::getConstructor());
	}
	else if(!strcmp(dataType,"dna")) 
	{
		CObjectFactory::objectRegister("dna",CDNA::getConstructor());
	}
	else if(!strcmp(dataType,"peptide"))
	{
		CObjectFactory::objectRegister("peptide",CPeptide::getConstructor());
	}

	ifstream iofilen(newIndexFileName,ios::in|ios::binary);

	if(!iofilen)
		cout<<"open file "<<newIndexFileName<<" failed!"<<endl;

	iofilen.seekg(-(long)sizeof(long),ios::end);
	iofilen.read((char*)&filePointer,sizeof(long));
	iofilen.seekg(filePointer,ios::beg);

	char type[6]="";
	iofilen.read(type,6*sizeof(char));
	filePointer+=6*sizeof(char);

	shared_ptr<CIndexNode> node;
	vector<shared_ptr<traversalTask> > taskList;

	if(!strcmp(type,"INODE"))
	{
		node=dynamic_pointer_cast<CMVPInternalNode,CIndexNode>(node);
		node.reset(new CMVPInternalNode());
		node.get()->readExternal(iofilen,string(dataType));
	}
	else if(!strcmp(type,"LNODE"))
	{
		node=dynamic_pointer_cast<CMVPLeafNode,CIndexNode>(node);
		node.reset(new CMVPLeafNode());
		node.get()->readExternal(iofilen,string(dataType));
	}
	
	shared_ptr<traversalTask> task;
	task.reset(new traversalTask());
	task->type=string(type);
	task->currentNode=node;
	task->parentNodeLayerNO=0;
	task->nodeHeight=1;
	taskList.push_back(task);
	
	int tempLayerNodeNumber=0;				//number of nodes per-layer
	int tempLayerInternalNodeChildNumber=0;	//number of internal node children of current node
	int tempLayerLeafNodeChildNumber=0;		//number of leaf node children of current node
	int tempTreeHeight=1;					//height of mvp tree

	int totalInternalNumber=0;
	int totalLeafNumber=0;

	while(taskList.size()>0)
	{
		vector<shared_ptr<traversalTask> >::iterator ite=taskList.begin();

		task=*ite;
		
		taskList.erase(ite);

		if(task->nodeHeight==1)
		{
			tempLayerNodeNumber=1;
			tempLayerInternalNodeChildNumber=0;
			tempLayerLeafNodeChildNumber=0;
			tempTreeHeight=1;

			analysisCurrentNode(taskList,iofilen,task,filePointer,totalInternalNumber,totalLeafNumber,dataType,tempLayerNodeNumber,tempLayerInternalNodeChildNumber,tempLayerLeafNodeChildNumber);
		}
		else if(task->nodeHeight==tempTreeHeight)
		{
			tempLayerNodeNumber++;

			analysisCurrentNode(taskList,iofilen,task,filePointer,totalInternalNumber,totalLeafNumber,dataType,tempLayerNodeNumber,tempLayerInternalNodeChildNumber,tempLayerLeafNodeChildNumber);
		}
		else if(task->nodeHeight!=tempTreeHeight)
		{
			cout<<"height: "<<tempTreeHeight<<" nodeNumber: "<<tempLayerNodeNumber<<" internal node child Number: "<<tempLayerInternalNodeChildNumber<<" leaf node child number:"<<tempLayerLeafNodeChildNumber<<endl;
			
			tempLayerNodeNumber=1;
			tempLayerInternalNodeChildNumber=0;
			tempLayerLeafNodeChildNumber=0;
			tempTreeHeight=task->nodeHeight;
			
			analysisCurrentNode(taskList,iofilen,task,filePointer,totalInternalNumber,totalLeafNumber,dataType,tempLayerNodeNumber,tempLayerInternalNodeChildNumber,tempLayerLeafNodeChildNumber);
		}
	}

	cout<<"**************"<<" total internal node:"<<totalInternalNumber<<" total leaf node:"<<totalLeafNumber<<endl;
}

/********************************************************************************************************************************************************************************************************************/
void threadFunc(shared_ptr<CSeachTask> st,CRangeQuery *rq,std::thread::id &pth_id)
{

#ifdef MVPISA_INFO_OUTPUT
	cout<<"MVPISA::threadFunc"<<__LINE__<<" "<<this_thread::get_id()<<endl;
	cout<<"MVPISA::ThreadFunc::parentTh_id "<<pth_id<<endl;
#endif

	st->SMTSearch(*rq);
	/*for(int i=0;i<tmp->size();i++)
		result->push_back(tmp->at(i));*/
	/*result=st->SMTSearch(*rq);*/

}
/*****************************************************************************************************************************************************************************************************************************************/
int gThreadNum=0;
std::mutex gthread_num_mutex;
struct pblock
{
	//pblock():queryObjectList(0),searchRadius(0),index(0),dataType(0),newIndexName(0),metric(0),num(0),resultCounter(0),output(0){}
	pblock(std::mutex &t_mutex):thread_num_mutex(t_mutex){}
	vector<shared_ptr<CIndexObject> > *queryObjectList;
	double searchRadius;
	CIndex *index;
	char *dataType;
	char *newIndexName;
	CMetric *metric;
	int num;
	double *resultCounter;
	int *distanceCounter;
	fstream *output;
	double *searchTime;

	mutex &thread_num_mutex;
};

//void globalThreadFunc(vector<shared_ptr<CIndexObject> > &queryObjectList,double searchRadius,CIndex *index,char *dataType,char *newIndexName,CMetric *metric,int num,double *resultCounter,fstream *output)
//{
//	vector<shared_ptr<CIndexObject> >::iterator ite=queryObjectList.begin();
//	
//	vector<shared_ptr<CIndexObject> > result;
//
//	for(;ite!=queryObjectList.end();ite++)
//	{
//		CRangeQuery rq(searchRadius,*ite);
//		shared_ptr<CSeachTask> searchTask(new CSeachTask(newIndexName,dataType,*metric));
//		thread newt(threadFunc,searchTask,result);
//		newt.join();
//		resultCounter[num]+=searchTask->getResultSize();
//	}
//
//	gThreadNum--;
//}

void globalThreadFunc(pblock *&pb)
{
	//vector<shared_ptr<CIndexObject> >::iterator ite=pb->queryObjectList->begin();
	
	//vector<shared_ptr<CIndexObject> > result;
#ifdef MVPISA_INFO_OUTPUT
	cout<<"MVPISA::globalThreadFunc"<<__LINE__<<" "<<this_thread::get_id()<<endl;
#endif

#ifdef WIN32
	FILETIME kernelStart,kernelEnd;
	FILETIME userStart,userEnd;
	FILETIME dummy;
#else
	struct tmp;
#endif

	CountedMetric *cm=dynamic_cast<CountedMetric*>(pb->metric);

	for(int i=0;i<pb->queryObjectList->size();i++)
	{
		//vector<shared_ptr<CIndexObject> >::iterator ite=pb->queryObjectList->begin();
		CRangeQuery rq(pb->searchRadius,pb->queryObjectList->at(i));//第二个参数是指针，但不是智能指针的引用
		shared_ptr<CSeachTask> searchTask(new CSeachTask(pb->newIndexName,pb->dataType,*(pb->metric)));
		//std::thread newt(threadFunc,searchTask,&rq,pth_id);
		//newt.join();

		int startCounter=cm->getCounter();

#ifdef WIN32
		GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelStart,&userStart);
#else
		clock_t begint=times(NULL);
#endif
		//searchTask->SMTSearch(rq);
		searchTask->search(rq);

#ifdef WIN32
		GetThreadTimes(GetCurrentThread(),&dummy,&dummy,&kernelEnd,&userEnd);
#else
		clock_t endt=times(NULL);
#endif

#ifdef MVPISA_INFO_OUTPUT
		cout<<"queryOL.size:"<<pb->queryObjectList->size()<<"thread:"<<this_thread::get_id()<<" num:"<<pb->num<<" radius:"<<pb->searchRadius<<endl;
#endif
		
		pb->resultCounter[pb->num]+=searchTask->getResultSize();
		pb->distanceCounter[pb->num]+=cm->getCounter()-startCounter;
		

#ifdef WIN32
		pb->searchTime[pb->num]+=(userEnd.dwLowDateTime+kernelEnd.dwLowDateTime-userStart.dwLowDateTime-kernelEnd.dwLowDateTime)/10000000.00;
#else
		pb->searchTime[pb->num]+=(double)(endt.tms_cutime-begint.tms_cutime)/CLOCKS_PER_SEC;
#endif
	}
#ifdef MVPISA_INFO_OUTPUT		
		stringstream temp;
        temp<<pth_id;
		string tempFileName(".\\log\\");
		tempFileName+=temp.str();
		tempFileName+="-MVPIndexStructureAnalysor.txt";
		ofstream outf(tempFileName.c_str());
		outf<<"GMTFunc over"<<endl;
#endif

	//for(;ite!=pb->queryObjectList->end();ite++)
	//{
	//	CRangeQuery rq(pb->searchRadius,*ite);
	//	shared_ptr<CSeachTask> searchTask(new CSeachTask(pb->newIndexName,pb->dataType,*(pb->metric)));
	//	thread newt(threadFunc,searchTask,&rq,&result,pth_id);
	//	newt.join();
	//	//cout<<"MVPISA::globalThreadFunc"<<endl;
	//	//cout<<"pth_id:"<<pth_id<<" this_thid:"<<this_thread::get_id()<<" num:"<<pb->num<<" radius:"<<pb->searchRadius<<" resultsize:"<<searchTask->getResultSize()<<endl;
	//	pb->resultCounter[pb->num]+=searchTask->getResultSize();
	//}
#ifdef MVPISA_INFO_OUTPUT
	cout<<"GMTFunc over"<<endl;
#endif
	//cout<<"num:"<<pb->num<<" resultNum:"<<pb->resultCounter[pb->num]<<endl;
	gthread_num_mutex.lock();
	gThreadNum--;
	gthread_num_mutex.unlock();
}

void globalMultithreadSearch(vector<shared_ptr<CIndexObject> > *queryObjectsList,CIndex *&index,char *&dataType,char *&newIndexName,double maxRadius,double minRadius,double step,CountedMetric *&cmetric, char *&resultFileName,int dataSize)
{
	char nResultFileName[100]=".\\data\\";
	strcat(nResultFileName,resultFileName);

	cout<<"result file name:"<<nResultFileName<<endl;
	fstream output(nResultFileName,ios::app);

	int runNumber=floor((maxRadius-minRadius)/step)+1;

	double *resultCounter=new double[runNumber];
	int *distanceCounter=new int[runNumber];
	double *searchTime=new double[runNumber];
	for(int i=0;i<runNumber;i++)
	{
		distanceCounter[i]=0;
		resultCounter[i]=0;
		searchTime[i]=0;
	}

	CMetric *metric=0;
	CountedMetric *cm=0;

	vector<pblock*> task;
	for(int i=0;i<runNumber;i++)
	{
		if(strcmp(dataType,"vector")==0)
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

		cm=new CountedMetric(metric);

		pblock *pb=new pblock(gthread_num_mutex);
		{
			pb->dataType=dataType;
			pb->index=index;
			pb->metric=cm;
			pb->newIndexName=newIndexName;
			pb->output=&output;
			pb->queryObjectList=queryObjectsList;
			pb->resultCounter=resultCounter;
			pb->distanceCounter=distanceCounter;
			pb->searchTime=searchTime;
		}

		task.push_back(pb);
	}

	for(int i=0;i<runNumber;i++)
	{
		pblock *pb=task.at(i);
		
		double searchRadius=minRadius+step*i;
		gthread_num_mutex.lock();
		gThreadNum++;
		gthread_num_mutex.unlock();
		pb->num=i;
		pb->searchRadius=searchRadius;
#ifdef MVPISA_INFO_OUTPUT
		cout<<"pb@"<<pb<<endl;
#endif
		std::thread newThread(globalThreadFunc,pb);
		newThread.detach();
	}

	while(gThreadNum>0)
	{
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}
#ifdef MVPISA_INFO_OUTPUT
	cout<<"GMThread over!"<<endl;
#endif
	//cout<<__FILE__<<__LINE__<<"over"<<endl;
	//for(int i=0;i<runNumber;i++)
		//cout<<__LINE__<<" "<<resultCounter[i]<<endl;
	for(int i=0;i<runNumber;i++)
		output<<dataSize
		      <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<(minRadius+step*i)
			  <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(15)<<(distanceCounter[i]/(queryObjectsList->size()))
			  <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(15)<<(searchTime[i]/(queryObjectsList->size()))
			  <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(15)<<(resultCounter[i]/(queryObjectsList->size()))
		      <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"
			  <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"
			  <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"
			  <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"
			  <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(25)<<"****"
			  <<setiosflags(ios::right)<<setiosflags(ios::fixed)<<setw(20)<<"****"<<endl;
	//cout<<__FILE__<<__LINE__<<"over"<<endl;
	for(int i=0;i<task.size();i++)
		delete task.at(i);
	//cout<<__FILE__<<__LINE__<<"over"<<endl;
}
/**********************************************************************************************************************************************************************************************/