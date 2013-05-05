#include "../../HeaderFiles/objects/StringObject.h"

/**
* This class is a wrapper of over String to implement IndexObject
* 
* @author Rui Mao
* @version 2011.07.21
*/
CStringObject::CStringObject(void)
{
}

/**
* Builds an instance from a double array.
* 
* @param rowID
* @param data
*            the double array containning all the elements. cannot be null
*/
CStringObject::CStringObject(string line)
{
	data=line;
}

/**
* @return the string
*/
string CStringObject:: getData() 
{
	return data;
}



CStringObject::~CStringObject(void)
{
}


/**
* The first line of the file should have two integers, separated by white
* space. The first is the dimension of the DoubleVector, the second is the
* total number of data points. Each following line is a DoubleVector, with
* each dimension separated by white space.
* 
* @param fileName
*            the filename of the source file
* @param size
*            number of data points to read
*/
vector<CIndexObject* >* CStringObject::loadData(string filename)
{
	ifstream in(filename);


	int dimension , maxSize;
	in>>dimension>>maxSize;

	vector<CIndexObject*> *a =new vector<CIndexObject*>;

	int numData=0;

	string Line;	//a DoubleVector, with each dimension separated by white space
	string temp1,temp2;
	in>>temp1>>temp2;
	Line=temp1+" "+temp2;


	while(Line!=""&&numData<maxSize)
	{

		a->push_back(new CStringObject(Line));
		in>>temp1>>temp2;
		Line=temp1+" "+temp2;
		numData++;
	}

	return a;
}

