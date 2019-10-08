#pragma once
#include "SearchAlgo.h"

class CSearchDjikstra : public SearchAlgo
{
public:
    CSearchDjikstra(void);
	~CSearchDjikstra(void);
    virtual p2t::Triangle* GetSearchPath(p2t::Triangle* pTriangleInit, p2t::Triangle* pTriangleEnd);
};
