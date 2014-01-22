#include "../../HeaderFiles/query/SeachTask.h"

#include <Windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")  

#include "../../HeaderFiles/util/MemMonitor.h"



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

vector<shared_ptr<CIndexObject> >* CSeachTask:: search(CRangeQuery &rq)
{
    //showMemoryInfo4("testSearchTask","rootBegin");


    filePointer = 0;
    long rootAddress;

    ifstream in(indexName,ios::binary);

    in.seekg(-(long)sizeof(long),ios::end);

    in.read((char*)(&rootAddress),sizeof(long));

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
    delete(type);


    root->searchExternal(rq,rootAddress,in,metric,result,searchList,dataType);

    //in.close();




    deque<long>::iterator nodeAddressToGet;
    long nodeAddress;

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
    searchList;
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
    delete(type);

    nodeAddress+=6*sizeof(char);
    node->searchExternal(q,nodeAddress,in,metric,result,searchList,dataType);
    /*node.reset();*/

    //showMemoryInfo4("testSearchTask","eachNodeEnd");
}