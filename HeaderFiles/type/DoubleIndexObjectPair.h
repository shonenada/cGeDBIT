#ifndef DOUBLEINDEXOBJECTPAIR_H
#define DOUBLEINDEXOBJECTPAIR_H

#include "../objects/IndexObject.h"
#include <vector>

class CDoubleIndexObjectPair
{
public:
	CDoubleIndexObjectPair(double d, CIndexObject* o);
	CDoubleIndexObjectPair();
	~CDoubleIndexObjectPair();

	double getDouble();
	CIndexObject* getObject();

	void setDouble(double value);
	void setObject(CIndexObject* obj);

private:
	double dist;
	CIndexObject* object;
};
#endif
