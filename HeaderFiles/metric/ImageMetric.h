#ifndef IMAGEMETRIC_H
#define IMAGEMETRIC_H

/**
 * Computes the distance between images. Designed for the GeDBIT image dataset,
 * <code>ImageMetric</code> may not work for other datasets.
 */
#include "Metric.h"
#include "../objects/Image.h"
#include "../objects/IndexObject.h"


#include <cmath>

using namespace std;

class CImageMetric :
	public CMetric
{
public:

	CImageMetric();
	~CImageMetric();


	/**
     * @param one
     *        the {@link Object} over which the keys are defined.
     * @param two
     *        the other {@link Object} over which the keys are defined.
     */
    double getDistance(CIndexObject *one, CIndexObject *two);

	/**
     * @param one
     *        the {@link Image} over which the keys are defined.
     * @param two
     *        the other {@link Image} over which the keys are defined.
     */
    double getDistance(CImage *one, CImage *two);

	/**
     * @param one
     *        the {@link Image} over which the keys are defined.
     * @param two
     *        the other {@link Image} over which the keys are defined.
     * @param FeaIndex
     *        the feature on which distance is to be computed.
     */
    double getDistance_Fea(CImage *one, CImage *two, int FeaIndex);

private:

	/**
     * Feature number for each image. We have three features for each image: structure, texture,
     * histogram.
     */
	int static const feaNum = 3;

	 /**
     * Number of floats that can represent each feature.
     */
	int feaLength[3];

	/**
     * Distance function selection for each feature.
     */
    bool minBool[3];

	/**
     * The weights of each feature in the computation of distance.
     */
    double weights[3];

	/**
     * The max distance for each feature.
     */
    double maxDist[3];
};
#endif
