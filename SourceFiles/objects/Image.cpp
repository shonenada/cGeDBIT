#include "../../HeaderFiles/objects/Image.h"

/**
 * This is the key of Image Object. This object stores an array of floating
 * numbers representing the key for image object.
 * image dataset, might not work for other datasets.
 * 
 * @author Wenguo Liu, Ru Mao, Willard
 * @version 2013.03.27
 */

/**
 * A constructor
 *  @param string sid          Identity string of this DNA sequence.
 *  @param string sequence     Sequence of this DNA.
 *  Details:
 *  - The first string parameter will be assigned to _sequenceid.
 *  - The second string parameter will be assigned to _sequence.
 *  - Property _size will be assigned by _sequence's size.
 *  - Each characters in sequence will translate into symbol id and stored in _symbolIDs.
*/
CImage::CImage()
{
	m_Feas_Len = 66;
	m_Feas = new float[66];
	for(int i = 0 ; i < 66; i++)
		m_Feas[i] =  0 ; 

	max_Dist_Len = 3;
	max_Dist = new double[3];
	max_Dist[0]=1.0;
	max_Dist[1]=60.0;
	max_Dist[2]=1.0;
}

/**
 * @param FLen
 * @param feas an array of floats over which the feature values are defined.
 */
CImage::CImage(int FLen , float feas[])
{
	int i;

	m_Feas_Len = FLen ; 
    m_Feas = new float[m_Feas_Len];
	for ( i = 0; i < m_Feas_Len; i++)
		m_Feas[i] = feas[i];
}

/**
 * @param FLen
 * @param DLen
 * @param feas an array of floats over which the feature values are defined.
 * @param maxDist
 */
CImage::CImage(int FLen , int DLen , float feas[] , double maxDist[])
{  
	int i;
	m_Feas_Len = FLen ; 
	max_Dist_Len = DLen;
	
    m_Feas = new float[m_Feas_Len];
	for (i = 0; i < m_Feas_Len; i++)
		m_Feas[i] = feas[i];

	max_Dist = new double[max_Dist_Len];
	for (i = 0; i < max_Dist_Len; i++)
		max_Dist[i] = maxDist[i];
		
	if (maxDist != NULL)
	{
		max_Dist = new double[max_Dist_Len];
		for (int i = 0; i < max_Dist_Len; i++)
			max_Dist[i] = maxDist[i];
	}
}

CImage::~CImage()
{
	delete[] m_Feas;
	delete[] max_Dist;
}

/**
 * @param index
 * @return
 */
float CImage::getFeature(int index)
{
	return m_Feas[index];
}

/**
 * @Override
 */
int CImage::getSize()
{
	return m_Feas_Len;
}

int CImage::compareTo(CIndexObject *oThat)
{
	CImage  *that = dynamic_cast<CImage *> (oThat);//将CIndexObject对象指针转化为CImage对象指针 

	if (this == that) 
		return 0;

	if (this->m_Feas_Len < that->m_Feas_Len)
		return -1;
	else if (this->m_Feas_Len > that->m_Feas_Len)
		return 1;

	else
	{
		for (int i = 0; i < m_Feas_Len; i++)
		{
			if (this->m_Feas[i] < that->m_Feas[i])
				return -1;
			else if (this->m_Feas[i] > that->m_Feas[i])
				return 1;
		}
		return 0;
	}
}

bool CImage::equals(CIndexObject *other)
{
	 if (other == this)
		 return true;


	 CImage *image = dynamic_cast<CImage *> (other);
	 if (this->m_Feas_Len != image->m_Feas_Len)
		 return false;
	 for (int i = 0; i < this->m_Feas_Len; i++)
	 {
		 if (abs(m_Feas[i] - image->m_Feas[i]) > 1.0e-10)
		 {
			 return false;
		 }
	 }
     return true;
}

string CImage::toString()
{
	string result = "ImageKeyObject, length :";
	char *tem = new char [100];
	sprintf(tem, "%f", this->m_Feas_Len);
	result.append(tem).append(", offset : ");
	for(int i = 0; i < this->m_Feas_Len; i++)
	{
		sprintf(tem, "%f", this->m_Feas[i]);
		result.append(tem).append(", ");
	}
	return result;
}

vector<CIndexObject*> CImage::loadData(string fileName , int cImage_Num, int  feas_Num)
{
	ifstream in(fileName);
	int i,j;
	vector<CIndexObject*> a;
	float* data = NULL;
	CIndexObject* temp = NULL;

	for(i=0;i<cImage_Num;i++)
	{
		data = new float[feas_Num];
		for(j=0;j<feas_Num;j++)
		{
			in>>data[j];
		}
		temp = new CImage(feas_Num,data);
		a.push_back(temp);
	}
	return a;
}
