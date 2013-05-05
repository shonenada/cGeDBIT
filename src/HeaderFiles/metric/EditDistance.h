#ifndef EDITDISTANCE_H
#define EDITDISTANCE_H

#include <string>

#include "..\..\HeaderFiles\objects\IndexObject.h"
#include "..\..\HeaderFiles\objects\StringObject.h"


using namespace std;
#pragma once

class CEditDistance
{
public:
	CEditDistance(void);
	CEditDistance( int gapCost, int substitutionCost);

	~CEditDistance(void);
	int gapCost;
	int substitutionCost;
	//double getDistance (Object *one, Object *two)
	double getDistance (CIndexObject *one, CIndexObject *two);

	/**
  * Computes the distance between two {@link String}s
  * @param first the first String to compute edit distance
  * @param second the second String to compute edit distance
  * @return the edit distance between the two arguments
  */
	double getDistance (CStringObject *first, CStringObject *second);
	double getDistance(string one, string two);

private:
	static const long serialVersionUID ; 
	int minimum(int a,int b,int c);
	


};
#endif
