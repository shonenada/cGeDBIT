#include "..\..\HeaderFiles\objects\TandemSpectra.h"

#include <fstream >
#include <vector>
#include <cmath>
#include <string>
using namespace std;

/**
* TandemSpectra represents a given spectra and its attached precursor mass.
* 
* @author Smriti Ramakrishnan, Willard
* @version 2004.11.29
*/


/**
* Necessary for readExternal() and writeExternal().
*/
CTandemSpectra::CTandemSpectra(void )
{

}

/**
* Constructs a TandemSpectra object from a {@link String representing
* the tandem spectra. The only difference between a Spectra and a
* TandemSpectra is that a TandemSpectra includes the precursor mass.
* 
* @param rowID
*        the rowID in the {@link TandemSpectraTable}.
* @param _precursorMass
*        the precursor mass of the spectra itself.
* @param spectra
*        a space-seperated {@link String} representation of a single fragmentation spectra.
*/
CTandemSpectra::CTandemSpectra(double* _data,int _length,double _precursorMass)
{

	precursorMass=_precursorMass;
	length=_length;
	dataList = new double[length];
	for (int i = 0;i<length;i++)
	{
		dataList[i]=_data[i];
	}



}

/**
* @return the precursor mass for this tandem spectra.
*/
double CTandemSpectra::getPrecursorMass()
{
	return precursorMass;

}

vector<CIndexObject*>* CTandemSpectra::loadData(string fileName)
{
	ifstream in(fileName);
	int num,  i;
	in >> num;	
	int rowID;
	in>>rowID;

	vector<CIndexObject*> *a =new vector<CIndexObject*>;
	CIndexObject *temp;
	vector< double > *data;
	for(i=0; i<num; i++)
	{	
		data=new vector<double>;

		double precursorMass;
		in>>precursorMass;

		double _data;
		while(in>>_data)
		{


			if(_data-(int)_data==0) break;
			//	cout<<_data<<" ";
			data->push_back(_data);
			//	cout<<data->back();
		}

		//	cout<<endl;

		int length=data->size();

		double* spectra=new double [length];
		int i;
		for(i=0;i<length;i++)
		{
			spectra[i]=data->at(i);
		}

		temp=new CTandemSpectra(spectra,length,precursorMass);

		a->push_back(temp);

	}

	return a;

}

/**
* @return the dataList for this tandem spectra.
*/
double* CTandemSpectra::getSpectra()
{
	return dataList;
}

/**
* @return the length for this tandem spectra.
*/
int	CTandemSpectra::getLen()
{
	return length;
}


/**
* Returns a string representation of the object. In general, the 
* <code>toString</code> method returns a string that 
* "textually represents" this object. The result should 
* be a concise but informative representation that is easy for a 
* person to read.
* It is recommended that all subclasses override this method.
* <p>
* The <code>toString</code> method for class <code>Object</code> 
* returns a string consisting of the name of the class of which the 
* object is an instance, the at-sign character `<code>@</code>', and 
* the unsigned hexadecimal representation of the hash code of the 
* object. In other words, this method returns a string equal to the 
* value of:
* <blockquote>
* <pre>
* getClass().getName() + '@' + Integer.toHexString(hashCode())
* </pre></blockquote>
*
* @return  a string representation of the object.
*/
void   CTandemSpectra::toString()
{
	cout<<"data(size=" <<getLen()<<", pMass= " <<precursorMass <<") :[";

	int i;
	for(i=0;i<getLen();i++)
		cout<<getSpectra()[i]<<",";

	cout<<"]"<<endl;
}

CTandemSpectra::~CTandemSpectra()
{

}
const int CTandemSpectra::size= 0;

