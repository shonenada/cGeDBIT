#ifndef STRINGOBJECT_H
#define STRINGOBJECT_H

/** @file
* @brief This class is a wrapper of over String to implement IndexObject
* @author Qihang Li,Zhiyong Xu
* @date 2013.3.24
* @version <version number>
* @note
* detailed description
*/

#include "IndexObject.h"
#include "../util/ObjectFactory.h"
#include<string>
#include<fstream>
#include<vector>

using namespace std;

/**
* @class CStringObject
* @brief This class is a wrapper of over String to implement IndexObject
* @author Qihang Li,Zhiyong Xu
* @note
* detailed description
*/
class CStringObject:public CIndexObject
{
private:
	static const long serialVersionUID = 7630078213101669086L;

	string data;



public:
	/**
	* @brief a constructor.
	*/
	CStringObject(void);

	/**
	* @brief Builds an instance from a double array.
	*/
	CStringObject(string line);

	/**
	* @return the data
	*/
	string getData();

	/**
	* @brief A static function to load data from a file.
	*/	
	static vector<shared_ptr<CIndexObject> >* CStringObject::loadData(string filename,int maxsize);

	/**@brief write the instance of this class to hard disk
	*@param out out putstream,used to write inforamtion to the hard disk
	*@return return the size of information which has been wrote to the hard disk
	*/
	virtual int writeExternal(ofstream &out);
	/**@load a instance of this class from the file stored in hard disk
	*@param in input stream,used to read information from hard disk
	*@return return the size of information which has been read from the hard disk
	*/
	virtual int readExternal(ifstream &in);

	/**@brief return the name of a instance of this class
	*@return return the name of a instance of this class
	*/
	static CreateFuntion getConstructor();
	static void* CreateInstance();
	/** @brief a destructor, do nothing.  */
	virtual ~CStringObject(void);
};

#endif