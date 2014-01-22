#include "../../HeaderFiles/query/SeachTask.h"

#include <Windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")  

#include "../../HeaderFiles/util/MemMonitor.h"

#define COUT_INFOMATION
#undef COUT_INFOMATION

#define LOG_FILE
#undef LOG_FILE


void showMemoryInfo4(char * resultsFileName,char* type)  
{  
    char resultFile[100]="./data/";
    strcat(resultFile,resultsFileName);
    ofstream output(resultFile,ofstream::app);
    HANDLE handle=GetCurrentProcess();  
    PROCESS_MEMORY_COUNTERS pmc;  
    GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));  
    output<<type<<endl;
    output<<"内存使用:"<<pmc.WorkingSetSize/1024<<" KB / 峰值内存使用:"<<pmc.PeakWorkingSetSize/1024<<" KB "<<endl;
    output<<"虚拟内存使用:"<<pmc.PagefileUsage/1024<<"KB / 峰值虚拟内存使用  "<<pmc.PeakPagefileUsage/1024<<" KB "<<endl;
    output<<endl;
}


CSeachTask::CSeachTask(string indexName,string dataType,CMetric &metric):metric(metric),indexName(indexName),dataType(dataType)
{
    /*searchList.clear();*/
}


CSeachTask::~CSeachTask(void)
{

}
/***************************************************C++11 STANDARD VERSION MULTI-THREAD SEARCHING IN MVPTHREE BEGIN***************************************************************/
//mutex searchList_mutex;
//mutex fstream_mutex;

//int threadNum=0;

struct pTask
{
	long fileAddress;
	shared_ptr<CMVPInternalNode> inode;
	shared_ptr<CMVPLeafNode> lnode;

	//~pTask(){cout<<fileAddress<<" task destroyed"<<endl;}
};

//rq,rootAddress,in,metric,result,searchList,dataType
struct parallelBlock
{
	parallelBlock(mutex &s_mutex, mutex &f_mutex,mutex &r_mutex,mutex &t_mutex):searchList_mutex(s_mutex),fstream_mutex(f_mutex),result_mutex(r_mutex),thread_num_mutex(t_mutex){}
	vector<shared_ptr<pTask> > *taskList;
	CMetric *metric;
	CRangeQuery *rq;
	ifstream *in;
	vector<shared_ptr<CIndexObject> > *result;
	string *datatype;
	mutex &searchList_mutex;
	mutex &fstream_mutex;
	mutex &result_mutex;
	mutex &thread_num_mutex;
};


#ifdef LOG_FILE
void MSTSearchTaskFile(parallelBlock *pb,int &threadNum,fstream &outfile)
#else
void MSTSearchTask(parallelBlock *pb,int &threadNum)
#endif
{
#ifdef COUT_INFOMATION
	//cout<<__FILE__<<__LINE__<<endl;
	cout<<this_thread::get_id()<<"start"<<endl;
	//cout<<"pass"<<endl;
#endif

#ifdef LOG_FILE
	outfile<<__LINE__<<" "<<"***************MSTSearchTask begin"<<endl;
#endif
	pb->searchList_mutex.lock();
	shared_ptr<pTask> t=NULL;
	if(pb->taskList->size()>0)
	{

#ifdef LOG_FILE
	outfile<<__LINE__<<" SMTSearch"<<endl;
#endif
		vector<shared_ptr<pTask> >::iterator ite=pb->taskList->end();
        ite--;
		t=*ite;
		pb->taskList->erase(ite);

#ifdef LOG_FILE
	outfile<<__LINE__<<" SMTSearch"<<endl;
#endif
	}
	pb->searchList_mutex.unlock();

#ifdef COUT_INFOMATION
	cout<<this_thread::get_id()<<"mutex"<<endl;
#endif
	

	deque<long> searchList;
	//cout<<__LINE__<<"t.usecount:"<<t.use_count()<<endl;
	if(t!=NULL)
	{
		//cout<<__LINE__<<"t.usecount:"<<t.use_count()<<endl;
		if(t->inode!=0)
		{
#ifdef COUT_INFOMATION
			cout<<this_thread::get_id()<<"start search internal"<<" fileaddress:"<<t->fileAddress<<endl;
#endif

#ifdef LOG_FILE
	outfile<<__LINE__<<" SMTSearch"<<endl;
#endif
	t->inode->SMTSearchExternal(*(pb->rq),t->fileAddress,*(pb->in),*(pb->metric),pb->result,searchList,*(pb->datatype),pb->fstream_mutex,pb->result_mutex);

#ifdef LOG_FILE
	outfile<<__LINE__<<" SMTSearch"<<endl;
#endif
#ifdef COUT_INFOMATION
	cout<<this_thread::get_id()<<"end search internal"<<endl;
#endif

			//cout<<"search list:"<<searchList.size()<<endl;
			while(searchList.size()>0)
			{
				long filePointer=0;

				deque<long>::iterator ite=searchList.end();
				ite--;

				filePointer=*ite;

				searchList.pop_back();
				
				char type[6]={0};
				pb->fstream_mutex.lock();
				pb->in->seekg(filePointer,ios::beg);
				pb->in->read(type,6*sizeof(char));
				pb->fstream_mutex.unlock();

				shared_ptr<pTask> temp(new pTask);
                
				shared_ptr<CIndexNode> node;

				
				if(!strcmp(type,"INODE"))
				{
					shared_ptr<CMVPInternalNode> newNode = dynamic_pointer_cast<CMVPInternalNode>(node);
					newNode.reset(new CMVPInternalNode());
					//node=dynamic_pointer_cast<CMVPInternalNode>(node);
					//node.reset(new CMVPInternalNode());
					// temp->inode=const_pointer_cast<CMVPInternalNode>(node);
					temp->inode=newNode;
					temp->lnode=NULL;
				}
				else if(!strcmp(type,"LNODE"))
				{
					shared_ptr<CMVPLeafNode> newNode = dynamic_pointer_cast<CMVPLeafNode>(node);
					newNode.reset(new CMVPLeafNode());
					// node=dynamic_pointer_cast<CMVPLeafNode>(node);
					// node.reset(new CMVPLeafNode());
					temp->lnode=newNode;
					temp->inode=NULL;
				}

				filePointer+=6*sizeof(char);

				temp->fileAddress=filePointer;
#ifdef COUT_INFOMATION
	cout<<this_thread::get_id()<<"start push"<<endl;
#endif

				pb->searchList_mutex.lock();
				pb->taskList->push_back(temp);
				pb->searchList_mutex.unlock();

#ifdef LOG_FILE
	outfile<<__LINE__<<" SMTSearch"<<endl;
#endif
#ifdef COUT_INFOMATION
	cout<<this_thread::get_id()<<"end push"<<endl;
#endif
			}

		}
		else
		{
#ifdef COUT_INFOMATION
	cout<<this_thread::get_id()<<"start search leaf node"<<endl;
#endif

#ifdef LOG_FILE
	outfile<<__LINE__<<" SMTSearch"<<endl;
#endif
	t->lnode->SMTSearchExternal(*(pb->rq),t->fileAddress,*(pb->in),*(pb->metric),pb->result,searchList,*(pb->datatype),pb->fstream_mutex,pb->result_mutex);

#ifdef LOG_FILE
	outfile<<__LINE__<<" SMTSearch"<<endl;
#endif
#ifdef COUT_INFOMATION
	cout<<this_thread::get_id()<<"end search leaf node "<<endl;
#endif
		}
	}

	
	threadNum--;
	
#ifdef COUT_INFOMATION
	//cout<<__FILE__<<__LINE__<<endl;
	cout<<this_thread::get_id()<<"over"<<endl;
	//cout<<"pass"<<endl;
#endif

#ifdef LOG_FILE
	outfile<<__LINE__<<"***************MSTSearchTask end"<<endl;
#endif
	//cout<<__LINE__<<"t.usecount:"<<t.use_count()<<endl;
	return;
}

void CSeachTask::SMTSearch(CRangeQuery &rq)
{

#ifdef LOG_FILE
	char f_n[100]=".\\log\\";
	std::thread::id perQueryObjectThread=this_thread::get_id();
	stringstream temp;
	string id_str;
	temp<<perQueryObjectThread;
	id_str=temp.str();
	strcat_s(f_n,id_str.c_str());
	strcat_s(f_n,".log");

	fstream outfile(f_n,ios::out|ios::app);
	outfile<<__LINE__<<" "<<perQueryObjectThread<<" ************************************SMTSearch begin"<<endl;
#endif

	int threadNum=0;
	mutex searchList_mutex;
	mutex fstream_mutex;
	mutex result_list_mutex;
	mutex thread_num_mutex;

#ifdef COUT_INFOMATION
	cout<<__FILE__<<__LINE__<<"                      radius:"<<rq.getRadius()<<" start"<<endl;
	cout<<this_thread::get_id()<<endl;
	cout<<"pass"<<endl;
#endif

	filePointer = 0;
	long rootAddress=0;

#ifdef COUT_INFOMATION
	cout<<__FILE__<<__LINE__<<endl;
	cout<<this_thread::get_id()<<endl;
	cout<<"pass"<<endl;
#endif

#ifdef LOG_FILE
	outfile<<__LINE__<<" "<<perQueryObjectThread<<endl;
#endif

	ifstream in(indexName,ios::binary);
	in.seekg(-(long)sizeof(long),ios::end);

	in.read((char*)(&rootAddress),sizeof(long));

	in.seekg(rootAddress,ios::beg);

	shared_ptr<CIndexNode> root;

	char type[6]={0};
	in.read(type,6*sizeof(char));

	vector<shared_ptr<pTask> > taskList;
	shared_ptr<pTask> tmp(new pTask);

#ifdef COUT_INFOMATION
	cout<<__FILE__<<__LINE__<<endl;
	cout<<this_thread::get_id()<<endl;
	cout<<"pass"<<endl;
#endif
#ifdef LOG_FILE
	outfile<<__LINE__<<" "<<perQueryObjectThread<<endl;
#endif

	if(!strcmp(type,"INODE"))
	{
		shared_ptr<CMVPInternalNode> newRoot = dynamic_pointer_cast<CMVPInternalNode>(root);
		newRoot.reset(new CMVPInternalNode());

		// root=dynamic_pointer_cast<CMVPInternalNode>(root);
		// root.reset(new CMVPInternalNode());

		// tmp->inode=shared_ptr<CMVPInternalNode>(newRoot.get());
		
		tmp->inode=newRoot;
		//tmp->inode=const_pointer_cast<CMVPInternalNode>(root);
		tmp->lnode=NULL;
	}
	else if(!strcmp(type,"LNODE"))
	{
		shared_ptr<CMVPLeafNode> newRoot = dynamic_pointer_cast<CMVPLeafNode>(root);
		newRoot.reset(new CMVPLeafNode());
		// root=dynamic_pointer_cast<CMVPLeafNode>(root);
		// root.reset(new CMVPLeafNode());
		tmp->lnode=newRoot;
		tmp->inode=NULL;
	}

#ifdef LOG_FILE
	outfile<<__LINE__<<" "<<perQueryObjectThread<<endl;
#endif

	rootAddress+=6*sizeof(char);

	tmp->fileAddress=rootAddress;

	taskList.push_back(tmp);

#ifdef LOG_FILE
	outfile<<__LINE__<<" "<<perQueryObjectThread<<endl;
#endif

	parallelBlock pb(searchList_mutex,fstream_mutex,result_list_mutex,thread_num_mutex);
	{
		pb.datatype=&dataType;
		pb.in=&in;
		pb.metric=&metric;
		pb.result=&result;
		pb.rq=&rq;
		pb.taskList=&taskList;
	}
#ifdef COUT_INFOMATION
	cout<<__FILE__<<__LINE__<<endl;
	cout<<this_thread::get_id()<<" "<<threadNum<<endl;
	cout<<"pass"<<endl;
#endif

#ifdef LOG_FILE
	outfile<<__LINE__<<" "<<perQueryObjectThread<<endl;
#endif

	while(taskList.size()>0||threadNum>0)
	{    
		//if(taskList.size()>0||threadNum<4)
		if(taskList.size()>0)
		{

			threadNum++;

#ifdef LOG_FILE
			std::thread childThread(MSTSearchTaskFile,&pb,std::ref(threadNum),std::ref(outfile));
#else
			std::thread childThread(MSTSearchTask,&pb,std::ref(threadNum));
#endif
			childThread.detach();
		}
		else
		{
#ifdef WIN32
			Sleep(1000);
#else
			//sleep(1);
#endif
		}
	}
#ifdef COUT_INFOMATION
	cout<<__FILE__<<__LINE__<<"                      radius:"<<rq.getRadius()<<" end"<<endl;
	cout<<this_thread::get_id()<<endl;
	cout<<"pass"<<endl;
#endif

#ifdef LOG_FILE
	outfile<<__LINE__<<" "<<perQueryObjectThread<<endl;
#endif
	while(taskList.size()>0||threadNum>0)
	{
#ifdef WIN32
			Sleep(1000);
#else
			//sleep(1);
#endif
	}

	in.close();

#ifdef LOG_FILE
	outfile<<__LINE__<<" "<<perQueryObjectThread<<" ************************************SMTSearch end"<<endl;
#endif
	//return &result;
}

/***************************************************C++11 STANDARD VERSION MULTI-THREAD SEARCHING IN MVPTHREE END***************************************************************/
/***************************************************C++11 STANDARD VERSION MULTI-THREAD SEARCHING IN MVPTHREE GLOBAL START***************************************************************/

void CSeachTask::gSMTSearch(CRangeQuery &rq,vector<shared_ptr<CIndexObject> > *&res)
{
#ifdef LOG_FILE
	char f_n[100]=".\\log\\";
	std::thread::id perQueryObjectThread=this_thread::get_id();
	stringstream temp;
	string id_str;
	temp<<perQueryObjectThread;
	id_str=temp.str();
	strcat_s(f_n,id_str.c_str());
	strcat_s(f_n,".log");

	fstream outfile(f_n,ios::out|ios::app);
	outfile<<__LINE__<<" "<<perQueryObjectThread<<" ************************************SMTSearch begin"<<endl;
#endif

	int threadNum=0;
	mutex searchList_mutex;
	mutex fstream_mutex;
	mutex result_list_mutex;
	mutex thread_num_mutex;

#ifdef COUT_INFOMATION
	cout<<__FILE__<<__LINE__<<"                      radius:"<<rq.getRadius()<<" start"<<endl;
	cout<<this_thread::get_id()<<endl;
	cout<<"pass"<<endl;
#endif
	filePointer = 0;
	long rootAddress=0;
#ifdef COUT_INFOMATION
	cout<<__FILE__<<__LINE__<<endl;
	cout<<this_thread::get_id()<<endl;
	cout<<"pass"<<endl;
#endif
	ifstream in(indexName,ios::binary);
	in.seekg(-(long)sizeof(long),ios::end);

	in.read((char*)(&rootAddress),sizeof(long));

	in.seekg(rootAddress,ios::beg);

	shared_ptr<CIndexNode> root;

	char type[6]={0};
	in.read(type,6*sizeof(char));

	vector<shared_ptr<pTask> > taskList;
	shared_ptr<pTask> tmp(new pTask);
#ifdef COUT_INFOMATION
	cout<<__FILE__<<__LINE__<<endl;
	cout<<this_thread::get_id()<<endl;
	cout<<"pass"<<endl;
#endif
	if(!strcmp(type,"INODE"))
	{
		shared_ptr<CMVPInternalNode> newRoot = dynamic_pointer_cast<CMVPInternalNode>(root);
		newRoot.reset(new CMVPInternalNode());

		// root=dynamic_pointer_cast<CMVPInternalNode>(root);
		// root.reset(new CMVPInternalNode());

		// tmp->inode=shared_ptr<CMVPInternalNode>(newRoot.get());
		
		tmp->inode=newRoot;
		//tmp->inode=const_pointer_cast<CMVPInternalNode>(root);
		tmp->lnode=NULL;
	}
	else if(!strcmp(type,"LNODE"))
	{
		shared_ptr<CMVPLeafNode> newRoot = dynamic_pointer_cast<CMVPLeafNode>(root);
		newRoot.reset(new CMVPLeafNode());
		// root=dynamic_pointer_cast<CMVPLeafNode>(root);
		// root.reset(new CMVPLeafNode());
		tmp->lnode=newRoot;
		tmp->inode=NULL;
	}
	
	rootAddress+=6*sizeof(char);

	tmp->fileAddress=rootAddress;

	taskList.push_back(tmp);

	parallelBlock pb(searchList_mutex,fstream_mutex,result_list_mutex,thread_num_mutex);
	{
		pb.datatype=&dataType;
		pb.in=&in;
		pb.metric=&metric;
		pb.result=&result;
		pb.rq=&rq;
		pb.taskList=&taskList;
	}
#ifdef COUT_INFOMATION
	cout<<__FILE__<<__LINE__<<endl;
	cout<<this_thread::get_id()<<endl;
	cout<<"pass"<<endl;
#endif
	while(taskList.size()>0||threadNum>0)
	{    
		//cout<<pb.rq->getRadius()<<" "<<threadNum<<endl;

		if(taskList.size()>0&&threadNum<4)
		{
			threadNum++;

#ifdef LOG_FILE
			std::thread childThread(MSTSearchTaskFile,&pb,std::ref(threadNum),std::ref(outfile));
#else
			std::thread childThread(MSTSearchTask,&pb,std::ref(threadNum));
#endif
			childThread.join();
		}
		else
		{
#ifdef WIN32
			//Sleep(1000);
#else
			//sleep(1);
#endif
		}
	}
#ifdef COUT_INFOMATION
	cout<<__FILE__<<__LINE__<<"                      radius:"<<rq.getRadius()<<" end"<<endl;
	cout<<this_thread::get_id()<<endl;
	cout<<"pass"<<endl;
#endif
	res=&result;
	//root->searchExternal(rq,rootAddress,in,metric,result,searchList,dataType);
}

/***************************************************C++11 STANDARD VERSION MULTI-THREAD SEARCHING IN MVPTHREE GLOBAL END***************************************************************/
vector<shared_ptr<CIndexObject> >* CSeachTask:: search(CRangeQuery &rq)
{
    //showMemoryInfo4("testSearchTask","rootBegin");


    filePointer = 0;
    long rootAddress;

    ifstream in(indexName,ios::binary);

    in.seekg(-(long)sizeof(long),ios::end);

    in.read((char*)(&rootAddress),sizeof(long));
	//cout<<"rootaddress:"<<rootAddress<<endl;
    in.seekg(rootAddress,ios::beg);


    shared_ptr<CIndexNode> root;


    char* type = new char[6];
    in.read(type,6*sizeof(char));
    if(!strcmp(type,"INODE"))
    {
        root = dynamic_pointer_cast<CMVPInternalNode> (root);
        root.reset(new CMVPInternalNode());
    }
    else
    {
        root = dynamic_pointer_cast<CMVPLeafNode> (root);
        root.reset( new CMVPLeafNode());
    }
    rootAddress += 6*sizeof(char);
    delete [] type;


    root->searchExternal(rq,rootAddress,in,metric,result,searchList,dataType);

    //in.close();




    deque<long>::iterator nodeAddressToGet;
    long nodeAddress;

	//cout<<"search list:"<<searchList.size()<<endl;

    while(searchList.size()>0)
    {

        //in.open(indexName,ios::binary);


        nodeAddressToGet=this->searchList.end();

        nodeAddressToGet--;

        nodeAddress= *nodeAddressToGet;

        searchList.pop_back();

        searchEachNode(nodeAddress,rq,in);

        //in.close();

        /*in.clear();*/

    }

    //CMemMonitor::maxMem;

    in.close();
    //CMemMonitor::updateMem();
    //showMemoryInfo4("testSearchTask","allEnd");
    return &result;
}

void CSeachTask::searchEachNode(long &nodeAddress,CRangeQuery &q,ifstream &in)
{
    //showMemoryInfo4("testSearchTask","eachNodeBegin");

    in.seekg(nodeAddress);

    shared_ptr< CIndexNode> node;

    char* type = new char[6];
    in.read(type,6*sizeof(char));
    if(!strcmp(type,"INODE"))
    {
        node = dynamic_pointer_cast<CMVPInternalNode> (node);
        node.reset(new CMVPInternalNode());
    }
    else
    {
        node = dynamic_pointer_cast<CMVPLeafNode> (node);
        node.reset( new CMVPLeafNode());
    }
    delete [] type;

    nodeAddress+=6*sizeof(char);
    node->searchExternal(q,nodeAddress,in,metric,result,searchList,dataType);
    /*node.reset();*/

    //showMemoryInfo4("testSearchTask","eachNodeEnd");
}