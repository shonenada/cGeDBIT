#ifndef TANDEMSPECTRA_H
#define TANDEMSPECTRA_H

/**
* TandemSpectra represents a given spectra and its attached precursor mass.
* 
* @author Smriti Ramakrishnan, Willard
* @version 2004.11.29
*/

#include "IndexObject.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
using namespace std;
class CTandemSpectra:
	public CIndexObject
{
public :

	/**
	* Necessary for readExternal() and writeExternal().
	*/
	CTandemSpectra();

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
	CTandemSpectra(double *,int,double);
	/**
	* @return the precursor mass for this tandem spectra.
	*/
	double getPrecursorMass();
	/**
	* @return the dataList for this tandem spectra.
	*/
	double* getSpectra();
	/**
	* @return the length for this tandem spectra.
	*/
	int getLen();

	int getSize();

	static const int size ;

	static vector<CIndexObject* >* loadData(string );
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
	void   toString();


	~CTandemSpectra();



private:

	double precursorMass;

	double *dataList;

	int length;



};
#endif