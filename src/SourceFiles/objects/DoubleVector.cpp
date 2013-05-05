#include "../../HeaderFiles/objects/DoubleVector.h"

CDoubleVector::CDoubleVector(void)
{
	dim=0;
	dataList=0;
	size=0;
}

CDoubleVector::CDoubleVector(double *_data,int _length)
{
    dim = _length;
    dataList = new double[dim];
    for (int i = 0;i<dim;i++)
    {
        dataList[i]=_data[i];
    }

	size=0;
}


CDoubleVector::~CDoubleVector(void)
{
	cout<<"release CDoubleVector"<<endl;
}


double* CDoubleVector::getData() const
{
    return dataList;
}

int CDoubleVector::getLen() const
{
    return dim;
}



vector<CIndexObject*> CDoubleVector::loadData(string _fileName)
{
	ifstream in(_fileName);
	int num, dim, i, j;
	vector<CIndexObject*> a;
	double* data=NULL;
	CIndexObject* temp=NULL;

	in >> num >> dim;	
	
	for(i=0; i<num; i++)
	{
		data = new double[dim];
		for(j=0; j<dim; j++)
		{
			in >> data[j];
		}
		
		temp = new CDoubleVector(data, dim);
		a.push_back(temp);
	}

    return a;
}




void CDoubleVector::write(text_oarchive &out)
{    
    out << *this;
}

void CDoubleVector::read(text_iarchive &in)
{    
    in >> *this;

}

int CDoubleVector::getSize() const 
{
    return size;
}












