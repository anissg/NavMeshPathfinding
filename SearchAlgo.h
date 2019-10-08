#pragma once

#include<vector>
#include<d3dx9.h> 
#include "DxStructs.h"
#include "poly2tri/poly2tri.h"

class SearchAlgo
{
protected:
    typedef std::vector<p2t::Point*>	node;
    std::vector<node>		_vTree;
    std::vector<p2t::Triangle*>	_vListTriangles;
    p2t::Point* _pPosInit;
    p2t::Point* _pPosEnd;
    std::vector<p2t::Point*>		_vListTargets;
    std::vector<p2t::Triangle*>	_vTrianglePath;

    IDirect3DVertexBuffer9* _pVB;
    IDirect3DVertexBuffer9* _pVBTriangles;
    CUSTOMVERTEX			_squareVertices[6];
    D3DMATERIAL9			_materiau;
    DWORD					_currentColor;
    D3DXVECTOR2* lines;
    std::vector<p2t::Point*>			_vListLines;

    int						_iNumTriangles;
    int						_iNumPoints;

public:
    ~SearchAlgo();
    void SetMesh(std::vector<p2t::Triangle*>	vListTriangles);
    void SetPosInit(p2t::Point* pPosInit);
    void SetDestinyTargets(std::vector<p2t::Point*> vListTargets);
    void SetEndPoint(p2t::Point* pPosEnd);
    std::vector<p2t::Triangle*> GetPath();
    virtual p2t::Triangle* GetSearchPath(p2t::Triangle* pTriangleInit, p2t::Triangle* pTriangleEnd) = 0;
    void Draw(LPDIRECT3DDEVICE9 pDXDevice);
    void Update(LPDIRECT3DDEVICE9 p_dx_Device);
};

