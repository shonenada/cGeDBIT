#include "../../HeaderFiles/index/Index.h"

CIndex::CIndex()
{
	//PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log.properties"));

    //loggerObj = Logger::getRoot();
}

CIndex::~CIndex()
{

}

void CIndex:: setDataType(string dataType)
{
}

string CIndex:: getDataType()
{
    return "false";
}

void CIndex ::setFileName(string fileName)
{
}
string CIndex :: getFileName()
{
    return "false";
}

//-----------------------------------------------------------------------------------------------------------------------------
//not for research, just be used to show how to build a index, more detail infomation can be found in SampleMainFunction.cpp
void CIndex::bulkLoad(vector<shared_ptr<CIndexObject> > &dataList)
{
}

void CIndex::writeExternal(string fineName)
{
}

void CIndex::readExternal(string fileName)
{
}
//-----------------------------------------------------------------------------------------------------------------------------