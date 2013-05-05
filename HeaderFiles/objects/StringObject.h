#ifndef STRINGOBJECT_H
#define STRINGOBJECT_H

#include "IndexObject.h"
#include<string>
#include<fstream>
#include<vector>

using namespace std;

/**
* This class is a wrapper of over String to implement IndexObject
* 
* @author Rui Mao
* @version 2011.07.21
*/
class CStringObject:public CIndexObject
{
private:
	static const long   serialVersionUID = 7630078213101669086L;

	string data;	

public:
	CStringObject(void);

	/**
	* Builds an instance from a double array.
	* 
	* @param rowID
	* @param data
	*            the double array containning all the elements. cannot be null
	*/
	CStringObject(string line);


	string getData();	//return the data


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

	static vector<CIndexObject* >* loadData(string filename) ;//throw Exception

	~CStringObject(void);
};

#endif