#include "..\..\HeaderFiles\indexalgorithm\PCAOnFFT.h"
#include "..\..\HeaderFiles\indexalgorithm\IncrementalSelection.h"
#include "..\..\HeaderFiles\indexalgorithm\FFTPivotSelectionMethod.h"
#include "..\..\HeaderFiles\indexalgorithm\PCA.h"
#include <stdio.h>
#include <gsl\gsl_rng.h>
#include <gsl\gsl_matrix_double.h>
#include <gsl\gsl_vector_double.h>
#include <gsl\gsl_blas.h>
#include <gsl\gsl_eigen.h>
#include <gsl\gsl_linalg.h>
#include <gsl\gsl_statistics.h>
#include <math.h>
#include <time.h>

CPCAOnFFT::CPCAOnFFT(int _FFTScale) : FFTScale(_FFTScale){}

CPCAOnFFT::~CPCAOnFFT(){}

vector<int> CPCAOnFFT::selectPivots(CMetric *metric, vector<shared_ptr<CIndexObject> > &data, 
                                    int numPivots)
{
    const int dataSize = data.size();
    
    if(dataSize <= numPivots)
    {
        vector<int> pivots;
        for (int i = 0; i < dataSize; ++i)
            pivots.push_back(i);
        return CIncrementalSelection::removeDuplicate(metric, data, pivots);
    }

    CFFTPivotSelectionMethod FFTMethod;
    vector<int> fftResult = FFTMethod.selectPivots(metric, data, numPivots);

    if(fftResult.size() <= min(numPivots, dataSize))
        return fftResult;

    gsl_matrix* matrix = gsl_matrix_alloc(dataSize, fftResult.size());
    for (int row = 0; row < dataSize; ++row)
    {
        for (int col = fftResult.size() - 1; col > -1 ; --col)
        {
            gsl_matrix_set(matrix, row, col, 
                metric->getDistance( data.at(row).get(), data.at(fftResult.at(col)).get() )); 
        }
    }

    gsl_matrix* pcaResult = gsl_matrix_alloc( numPivots, fftResult.size());

    try
    {
        pcaResult = CPCA::EMPCA( matrix, numPivots);
    }
    catch(exception e)
    {
        cout<< "Exception!  data size =" << dataSize 
            << " , num pivots = " << numPivots << endl;  
        cout<< e.what() << endl;

        if (numPivots >= fftResult.size())
            return fftResult;
        else
        {
            fftResult.resize(numPivots);
            return fftResult;
        }
    }

    vector<int> selectByAngleResult 
        = CPCA::pivotSelectionByPCAResultAngle( pcaResult, numPivots);

    vector<int> finalResult;
    for (int i = 0; i < numPivots; ++i)
    {
        finalResult.push_back( fftResult.at( selectByAngleResult.at(i) ) );
    }

    return finalResult;
}

vector<int> CPCAOnFFT::selectPivots(CMetric *metric, vector<shared_ptr<CIndexObject> > &data, 
                                    int first, int size, int numPivots)
{
    vector<int> a;
    return a;
}


