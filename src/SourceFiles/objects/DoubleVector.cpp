/** @file DoubleVector.cpp
* @describe a kind of object
* @author Fuli Lei
* @version 2012-12-09
*/
#include "../../HeaderFiles/objects/DoubleVector.h"

#include <Windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")  


void showMemoryInfo5(char * resultsFileName,char* type)  
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

#include <sstream>

/**@brief none parameter constructor*/
CDoubleVector::CDoubleVector()
{
    dim=0;
    dataList=0;

}

/**@brief constructor with two parameters
* @param data a double array represents the liner structure
* @param length length of the liner structure
*/
CDoubleVector::CDoubleVector(double *data,int length)
{
    dim = length;
    dataList = new double[dim];
    for (int i = 0;i<dim;i++)
    {
        dataList[i]=data[i];
    }


}

/**@brief destructure*/
CDoubleVector::~CDoubleVector()
{
    delete[](dataList);        
    /*cout<<"release CDoubleVector"<<endl;*/
}

/**@brief get the data list encapsulated in the objects
* @return return the memory address of the data list
*/
double* CDoubleVector::getData() const
{
    return dataList;
}

/**@brief get the length of the data list
*@return return an value of int represents the length of the data list.
*/
int CDoubleVector::getLen() const
{
    return dim;
}


/**@brief load raw data from hard disk file and package the data into a objects of CDoubleVector,then return the vector with all generated objects
* @param fileName name of the file that contains all the raw data waiting to be load
* @param maxDataNum maximum number of data list to be load from the file
* @param dimension length of each data list
* @return  return a vector contains all the objects generated before.
*/
vector<shared_ptr<CIndexObject> >* CDoubleVector::loadData(string fileName,int maxDataNum,int dimension)
{
    ifstream in(fileName);
    int num, dim, i, j;
    vector<shared_ptr<CIndexObject> > *a=new vector<shared_ptr<CIndexObject> >;


    double* data=NULL;
    shared_ptr<CDoubleVector> temp=NULL;

    string str="";

    in >> num >> dim;	
    getline(in,str);

    dim = dim>dimension ? dimension:dim;
    num = num>maxDataNum ? maxDataNum:num;

    for(i=0;i<num;i++)
    {
        getline(in,str);
        stringstream newStr(str);
        data = new double[dim];

        for(j=0; j<dim; j++)
        {
            newStr>>data[j];
        }

        temp.reset(new CDoubleVector(data, dim));
        a->push_back(temp);
    }

    return a;
}


/**@brief write the instance of this class to hard disk
*@param out out putstream,used to write inforamtion to the hard disk
*@return return the size of information which has been wrote to the hard disk
*/
int CDoubleVector ::writeExternal(ofstream &out)
{
    int size=0;
    out.write((char*) (&dim),sizeof(int));
    size += sizeof(int);
    /*for(int i =0;i<dim;i++)
    {
    out.write((char*)(&dataList[i]),sizeof(double));
    size += sizeof(double);
    }*/
    out.write((char*)dataList,dim*sizeof(double));
    size += dim*sizeof(double);

    return size;

}

/**@load a instance of this class from the file stored in hard disk
*@param in input stream,used to read information from hard disk
*@return return the size of information which has been read from the hard disk
*/
int CDoubleVector ::readExternal(ifstream &in)
{
    int size=0;     

    in.read((char*)(&dim),sizeof(int));    
    size+=sizeof(int);

    dataList = new double[dim];

    in.read((char*)dataList,dim*sizeof(double));    
    size+=dim*sizeof(double);

    return size;

}

/**@brief return the name of a instance of this class
*@return return the name of a instance of this class
*/
CreateFuntion CDoubleVector::getConstructor()
{
    CreateFuntion constructor =& CreateInstance;
    return constructor;
}

void* CDoubleVector:: CreateInstance()
{
    return new CDoubleVector();
}









