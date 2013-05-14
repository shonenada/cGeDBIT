#include "../../HeaderFiles/objects/Image.h"

/**
 * This is the key of Image Object. This object stores an array of floating
 * numbers representing the key for image object.
 * image dataset, might not work for other datasets.
 * 
 */

/*
 * A constructor
 * 
 * Details:
 * - The m_Feas_Len will be initialized to '66'.
 * - The m_Feas will be initialized to '0'.
 * - The max_Dist_Len will be initialized to '3'. 
 * - The max_Dist will be initialized to '1.0', '60.0', '1.0'.
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

/*
 * A constructor
 * @param FLen : The length of the feas[]
 * @param feas : An array of floats over which the feature values are defined.
 *
 * Details:
 * - The FLen will be initialized to m_Feas_Len. 
 * - The feas will be initialized to m_Feas.
 */
CImage::CImage(int FLen , float feas[])
{
	int i;

	m_Feas_Len = FLen ; 
    m_Feas = new float[m_Feas_Len];
	for ( i = 0; i < m_Feas_Len; i++)
		m_Feas[i] = feas[i];
}

/* 
 * A constructor
 * @param FLen : The length of the feas[]
 * @param DLen : The length of the maxDist[]
 * @param feas : an array of floats over which the feature values are defined.
 * @param maxDist : an array of floats over which the dis values are defined.
 * 
 * Details:
 * - The FLen will be initialized to m_Feas_Len. 
 * - The feas will be initialized to m_Feas.
 * - The DLen will be initialized to max_Dist_Len.
 * - The maxDist will be initialized to max_Dist.
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

/*
 * A destructor
 *
 * Details:
 * - The m_Feas and max_Dist will be deleted.
 */
CImage::~CImage()
{
	delete[] m_Feas;
	delete[] max_Dist;
}

/*
 * @param index
 * @return 
 *
 * Details:
 * - Returns the value of index subscript corresponds in m_Feas
 */
float CImage::getFeature(int index)
{
	return m_Feas[index];
}

/*
 * @return 
 *
 * Details:
 * - Returns the m_Feas_Len.
 */
int CImage::getSize()
{
	return m_Feas_Len;
}
/*
 * @param CIndexObject *oThat
 *
 * Details:
 * - The CIndexObject types of object will be changed into CImage types.
 * - Compare the CIndexObject types of object to the CImage types of object.
 * - If "this" is equal to "that", then returns '0'.
 * - If the length of "this" is larger than the length of "that", then return '-1'.
 * - If the length of "that" is larger than the length of "this", then return '1'.
 * - If their length is the same, then compare the features one by one.
 */
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

/*bool CImage::equals(CIndexObject *other)
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
}*/

/*
 * Detail:
 * - A string will be defined.
 * - Some details will be added to the string.
 */
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

/* 
 * 
*/
int CImage::hashCode()
{
	int result = 17;
	for (int i = 0; i < m_Feas_Len; i++)
	{
		result = 37 * result + m_Feas[i];
	}
	return result;
}

/**
 * @param string fileName : The name of the file containing the data
 * @param int cImage_Num : The number of datas
 * @param int feas_Num : The number of features
 */
vector<CIndexObject*>* CImage::loadData(string fileName , int cImage_Num, int  feas_Num)
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
	return &a;
}
