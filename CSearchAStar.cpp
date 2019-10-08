#include "CSearchAStar.h"

int CSearchAStar::_iMaxArea = 0;
CSearchAStar::CSearchAStar(void)
{
}

CSearchAStar::~CSearchAStar(void)
{
}

virtual p2t::Triangle* GetSearchPath(p2t::Triangle* pTriangleInit, p2t::Triangle* pTriangleEnd)
{
	bool bFinded = false;
	Point* pEnd = pTriangleEnd->GetMiddlePoint();
	Triangle* pTriangleCurrent = pTriangleInit;
	Triangle* pTriangleAnt = pTriangleInit;
	vector<Triangle*> vNodesOpen;
	vNodesOpen.push_back(pTriangleInit);

	float fLastCost = 0.0f;
	vector<float> vCosts;
	vCosts.push_back(fLastCost);

	Point* pMiddlePoint = pTriangleCurrent->GetMiddlePoint();
	float fLastDistance = pMiddlePoint->DistanceToPoint(pEnd);
	vector<float> vDistance;
	vDistance.push_back(fLastDistance);

	while( !bFinded)
	{
		///check the best node from open list
		float fMinValue = 100000000.0f;
		bool bFindNeighbor = false;
		int iIndexMinDistance = -1;
		for(int k = 0; k< vNodesOpen.size(); k++)
		{
			if( vNodesOpen[k] == pTriangleEnd)
			{
				bFinded = true;
				pTriangleCurrent = vNodesOpen[k];
			}
			else
			{
				if((vCosts[k] + vDistance[k]) < fMinValue)
				{
					fMinValue = vCosts[k] + vDistance[k];
					iIndexMinDistance = k;
					bFindNeighbor = true;
				}
			}
		}

		if(!bFinded)
		{
			pTriangleCurrent = vNodesOpen[iIndexMinDistance];
			for( int j=0; j<3; j++)
			{
				if( pTriangleCurrent->GetNeighbor(j) != NULL &&
					pTriangleCurrent->GetNeighbor(j)->IsInterior() &&
					pTriangleCurrent->GetNeighbor(j) != pTriangleAnt &&
					!pTriangleCurrent->GetNeighbor(j)->GetVisited()
					)
				{
					Point* pMiddlePoint = pTriangleCurrent->GetNeighbor(j)->GetMiddlePoint();
					float fDistance = pMiddlePoint->DistanceToPoint(pEnd);

					Point* pMiddlePointCurrent = pTriangleCurrent->GetMiddlePoint();
					float fDistanceNextHop = pMiddlePointCurrent->DistanceToPoint(pMiddlePoint);

					bool bFound = false;
					for(int k = 0; k < vNodesOpen.size(); k++)
					{
						if(vNodesOpen[k] == pTriangleCurrent->GetNeighbor(j))
						{
							if((vCosts[k] + vDistance[k])> (vCosts[iIndexMinDistance] + fDistanceNextHop + fDistance ))
							{
								pTriangleCurrent->GetNeighbor(j)->_pPrevTriangle = pTriangleCurrent;
								vNodesOpen[k] = pTriangleCurrent->GetNeighbor(j);
								vCosts[k] = vCosts[iIndexMinDistance] + fDistanceNextHop;
								vDistance[k] = fDistance;
								bFound = true;
							}
						}
					}
					if(!bFound)
					{
						pTriangleCurrent->GetNeighbor(j)->_pPrevTriangle = pTriangleCurrent;
						vNodesOpen.push_back(pTriangleCurrent->GetNeighbor(j));
						vCosts.push_back(vCosts[iIndexMinDistance] + fDistanceNextHop);
						vDistance.push_back(fDistance);
					}
				}
			}
			pTriangleCurrent->SetVisited(true);
			vNodesOpen.erase(vNodesOpen.begin() + iIndexMinDistance);
			vCosts.erase(vCosts.begin() + iIndexMinDistance);
			vDistance.erase(vDistance.begin() + iIndexMinDistance);
		}
	}
	return pTriangleCurrent;
}

float CSearchAStar::NormalizeDistance(float fDistance)
{

	float fWidth = 1280.0f;
	float fHeight = 768.0f;
	float fMaxDistance = sqrt((fWidth * fWidth) + (fHeight * fHeight));
	float fNormalizedDistance = fDistance / fMaxDistance;
	return fNormalizedDistance;
}

float CSearchAStar::NormalizeArea(float fArea)
{	
	float fWidth = 1280.0f;
	float fHeight = 768.0f;
	float fMaxArea = (fWidth * fHeight) / 2.0f;
	
	CSearchAStar::_iMaxArea =  100;
	fMaxArea =  500;
	float fNormalizedArea = ((-1.0f * fArea) / fMaxArea) + 1;
	return fNormalizedArea;
}