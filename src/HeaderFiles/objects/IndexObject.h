#ifndef INDEXOBJECT_H
#define INDEXOBJECT_H

#include <iostream>
#include <fstream>

#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace std;

using namespace boost::archive;

class CIndexObject
{
public:
    CIndexObject(void);
    ~CIndexObject(void);

    virtual int getSize()const;
    
	
	virtual void write(text_oarchive &_out);
    virtual void read(text_iarchive &_in);
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar,const unsigned int version)
    {
    }
};
#endif
