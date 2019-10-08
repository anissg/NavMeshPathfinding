
#include "stdAfx.h"
#include "CSearchDjikstra.h"

CSearchDjikstra::CSearchDjikstra(void)
{
}

CSearchDjikstra::~CSearchDjikstra(void)
{
}
Triangle* CSearchDjikstra::GetSearchPath(Triangle* pTriangleInit, Triangle* pTriangleEnd)
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



    while (!bFinded)
    {
        ///check the best node from open list
        float fMinDistance = 100000000.0f;
        bool bFindNeighbor = false;
        int iIndexMinDistance = -1;
        for (int k = 0; k < vNodesOpen.size(); k++)
        {
            if (vNodesOpen[k] == pTriangleEnd)
            {
                bFinded = true;
                pTriangleCurrent = vNodesOpen[k];
            }
            else
            {
                if (vCosts[k] < fMinDistance)
                {
                    fMinDistance = vCosts[k];
                    iIndexMinDistance = k;
                    bFindNeighbor = true;
                }
            }
        }

        if (!bFinded)
        {
            pTriangleCurrent = vNodesOpen[iIndexMinDistance];
            for (int j = 0; j < 3; j++)
            {
                if (pTriangleCurrent->GetNeighbor(j) != NULL &&
                    pTriangleCurrent->GetNeighbor(j)->IsInterior() &&
                    pTriangleCurrent->GetNeighbor(j) != pTriangleAnt &&
                    !pTriangleCurrent->GetNeighbor(j)->GetVisited()
                    )
                {
                    Point* pMiddlePoint = pTriangleCurrent->GetNeighbor(j)->GetMiddlePoint();
                    float fDistance = pMiddlePoint->DistanceToPoint(pEnd);

                    bool bFound = false;
                    for (int k = 0; k < vNodesOpen.size(); k++)
                    {
                        if (vNodesOpen[k] == pTriangleCurrent->GetNeighbor(j))
                        {
                            if (vCosts[k] > /*fMinDistance +*/ fDistance)
                            {
                                vNodesOpen[k] = pTriangleCurrent->GetNeighbor(j);
                                vCosts[k] =/*fMinDistance +*/ fDistance;
                                bFound = true;
                            }
                        }
                    }
                    if (!bFound)
                    {
                        pTriangleCurrent->GetNeighbor(j)->_pPrevTriangle = pTriangleCurrent;
                        vNodesOpen.push_back(pTriangleCurrent->GetNeighbor(j));
                        vCosts.push_back(/*fMinDistance + */fDistance);
                    }
                }
            }
            pTriangleCurrent->SetVisited(true);
            vNodesOpen.erase(vNodesOpen.begin() + iIndexMinDistance);
            vCosts.erase(vCosts.begin() + iIndexMinDistance);
        }
    }
    return pTriangleCurrent;
}
