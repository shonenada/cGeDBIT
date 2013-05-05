#include "../../HeaderFiles/objects/DoubleIndexObjectPair.h"

CDoubleIndexObjectPair::CDoubleIndexObjectPair()
{
	object=0;
}

CDoubleIndexObjectPair::CDoubleIndexObjectPair(double d,CIndexObject *obj)
{
	dist = d;
	object=obj;
}

CDoubleIndexObjectPair::~CDoubleIndexObjectPair()
{

}

double CDoubleIndexObjectPair::getDouble()
{
	return dist;
}

CIndexObject* CDoubleIndexObjectPair::getObject()
{
	return object;
}

void CDoubleIndexObjectPair::setDouble(double value)
{
	dist = value;

}

void CDoubleIndexObjectPair::setObject(CIndexObject *obj)
{
	object=obj;
}
