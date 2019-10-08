#pragma once
#include "stdAfx.h"
#include "SearchAlgo.h"

class CSearchAStar : public ASearchAlgorithm
{
public: 
    static int _iMaxArea; 
public:
	CSearchAStar(void);
	~CSearchAStar(void);
	Triangle* GetSearchPath(Triangle* pTriangleInit, Triangle* pTriangleEnd);
	float NormalizeDistance(float fDistance);
	float NormalizeArea(float fArea);
};
