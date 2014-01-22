/** @file IndexObject.cpp
* @base object of all type of data object
* @author Lei Fuli
* @version 2012
* this is a base object encapsulats the basic information shared by all types of data objects.
*/
#include "../../HeaderFiles/objects/IndexObject.h"


/**none parameter constructor*/
CIndexObject::CIndexObject(void)
{

}


/**destructor*/
CIndexObject::~CIndexObject(void)
{

}

int CIndexObject::writeExternal(ofstream &out)
{
    return 0;
}

int CIndexObject::readExternal(ifstream &in)
{
    return 0;
}