#ifndef QUERY_H
#define QUERY_H
#include "../objects/IndexObject.h"

class CQuery
{
public:
	CQuery(CIndexObject*);
	~CQuery(void);

	void setQueryObject(CIndexObject*);
	CIndexObject* getQueryObject() const;

private:
	CIndexObject *q;
};
#endif
