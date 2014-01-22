#pragma once
#include "../index/Index.h"
#include "Query.h"
#include "RangeQuery.h"
#include "../index/IndexNode.h"
#include "../util/ObjectFactory.h"
#include "../index/MVPInternalNode.h"
#include "../index/MVPLeafNode.h"

#include <memory>
#include <deque>

class CSeachTask
{
public:
    CSeachTask(string indexName,string dataType,CMetric &metric);
    ~CSeachTask(void);

	int getResultSize(){return result.size();}
    vector<shared_ptr<CIndexObject> >* search(CRangeQuery &q);;
	void SMTSearch(CRangeQuery &rq);
	void gSMTSearch(CRangeQuery &rq,vector<shared_ptr<CIndexObject> > *&res);
private:

   
    void searchEachNode(long &nodeAddress,CRangeQuery &q,ifstream &in);

    string indexName;
    string indexType;
    string dataType;

    long filePointer;

    CMetric &metric;
    deque<long> searchList;
    vector<shared_ptr<CIndexObject> > result;


};

