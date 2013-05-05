#ifndef DOUBLEVECTOR_H
#define DOUBLEVECTOR_H

#include "IndexObject.h"


#include <fstream>
#include <string>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

class CDoubleVector :
    public CIndexObject
{
public:

    CDoubleVector(void);
    CDoubleVector(double *,int);
    ~CDoubleVector(void);


	static vector<CIndexObject*> loadData(string fileName);

    double* getData() const;
    int getLen() const;

    virtual int getSize() const;

	virtual void write(text_oarchive &out);
    virtual void read(text_iarchive &in);
private:

    friend class boost::serialization::access;
    template<class Archive>
    
    void save(Archive & ar,const unsigned int version) const
    {
        ar & boost::serialization::base_object<CIndexObject>(*this);
        ar & dim;
        for (int i=0;i<dim;i++)
            ar & dataList[i];        
    }
    
	template<class Archive>
    void load(Archive & ar,const unsigned int version)
    {        
        ar & boost::serialization::base_object<CIndexObject>(*this);
        ar & dim;
        dataList = new double[dim];
        for (int i=0;i<dim;i++)
            ar&dataList[i];

    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    //serialization method from Boost

    int dim;
    double* dataList;

    int size;
};
#endif