#ifndef INDEXOBJECT_H
#define INDEXOBJECT_H

#include <iostream>
#include <fstream>

using namespace std;

class CIndexObject
{
public:
    CIndexObject(void);
    ~CIndexObject(void);

    virtual int getSize()const;
	
};
#endif
