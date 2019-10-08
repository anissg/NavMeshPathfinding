
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
	Triangle* pTriangleCurrent = pTriangleInit;
	Triangle* pTriangleAnt = pTriangleInit;
	pTriangleCurrent->SetVisited(true);

	while( !bFinded)
	{
		bool bFindNeighbor = false;
		for( int j=0; j<3; j++)
		{
			if( pTriangleCurrent->GetNeighbor(j) != NULL)
			{
				if( pTriangleCurrent->GetNeighbor(j)->IsInterior())
				{
					if( !pTriangleCurrent->GetNeighbor(j)->GetVisited())
					{
						if( pTriangleCurrent->GetNeighbor(j) == pTriangleEnd)
						{
							bFinded = true;
						}
						else
						{
							bFindNeighbor = true;
						}
						pTriangleAnt = pTriangleCurrent;
						pTriangleCurrent = pTriangleCurrent->GetNeighbor(j);
						pTriangleCurrent->_pPrevTriangle = pTriangleAnt;
						pTriangleCurrent->SetVisited(true);
						break;
					}
				}
			}
		}
		if(!bFinded && !bFindNeighbor)
		{
			pTriangleCurrent = pTriangleCurrent->_pPrevTriangle;
		}
	}
	return pTriangleCurrent;
}
