/** @file
* @brief This class is a wrapper of over String to implement IndexObject
* @author Qihang Li,Zhiyong Xu
* @date 2013.3.24
* @version <version number>
* @note
* modified writeExternal() and readExternal() by Zhiyong Xu 2013/07/24
* detailed description
*/

#include "../../HeaderFiles/objects/StringObject.h"


CStringObject::CStringObject(void)
{
}

CStringObject::CStringObject(string line)
{
	data=line;
}

/**
* @return the data
*/
string CStringObject:: getData()
{
	return data;
}

CStringObject::~CStringObject(void)
{
	
}

/**
* @param[in] fileName the filename of the source file
* @exception <exception-object> <exception description>
* @return a vector stored string fragments.
* @note
* This function will get data from a format file, which contain some string informations,
*  and then save as a CStringObject type and store in a vector.
* Details:
* The first line of the file should have two integers, separated by white
* space. The first is the dimension of the DoubleVector, the second is the
* total number of data points. Each following line is a DoubleVector, with
* each dimension separated by white space.
*/
vector<shared_ptr<CIndexObject> >* CStringObject ::loadData(string filename,int maxsize)
{
	ifstream infile(filename);
	vector<shared_ptr<CIndexObject> >* data=new vector<shared_ptr<CIndexObject> >;

	if(!infile.is_open()){
		cerr << "Stop! Cannot open the file." << endl;
		return data;
	}

	int numData=0;
	string Line;	//a DoubleVector, with each dimension separated by white space
	char buffer[100];
	infile.getline(buffer, 100);
	Line = string(buffer);
	while(Line!=""&&numData<maxsize)
	{
		shared_ptr<CStringObject> temp((new CStringObject(Line)));
		data->push_back(temp);
		infile.getline(buffer, 100);
		Line = string(buffer);
		numData++;
	}
	return data;
}


/**@brief write the instance of this class to hard disk
*@param out out putstream,used to write inforamtion to the hard disk
*@return return the size of information which has been wrote to the hard disk
*/
int CStringObject ::writeExternal(ofstream &out)
{
	int size=0;

	int length=data.size()+1;// the '\0' char.

	out.write((char*)(&length),sizeof(int));
	size += sizeof(int);

	out.write(data.c_str(),length*sizeof(char));
	size += length*sizeof(char);

	return size;
}

/**@load a instance of this class from the file stored in hard disk
*@param in input stream,used to read information from hard disk
*@return return the size of information which has been read from the hard disk
*/
int CStringObject ::readExternal(ifstream &in)
{
	int size=0;
	int length=0;
	data.empty();

	in.read((char*)(&length),sizeof(int));
	size += sizeof(int);

	char *temp=new char[length];
	in.read(temp,length*sizeof(char));
	size += length*sizeof(char);

	data=string(temp);
	delete [] temp;
	return size;
}

/**@brief return the name of a instance of this class
*@return return the name of a instance of this class
*/
CreateFuntion CStringObject::getConstructor()
{
	return &CreateInstance;
}

void* CStringObject::CreateInstance()
{
	return new CStringObject();
}
