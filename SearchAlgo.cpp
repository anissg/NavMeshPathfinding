#include "SearchAlgo.h"

SearchAlgo::~SearchAlgo(void)
{
}

void SearchAlgo::SetMesh(std::vector<p2t::Triangle*>	vListTriangles)
{
    _vListTriangles = vListTriangles;
    _currentColor = 0xffffffff;
    for (int i = 0; i < _vListTriangles.size(); i++)
    {
        _vListTriangles[i]->SetVisited(false);
        _vListTriangles[i]->ResetCoutAcumulatif();
        _vListTriangles[i]->_pPrevTriangle = NULL;
    }

    for (int i = 0; i < _vListTriangles.size(); i++)
    {
        _vListTriangles[i]->SetMiddlePoint();
    }
}
void ASearchAlgorithm::SetPosInit(Point* pPosInit)
{
    _pPosInit = pPosInit;
}
void ASearchAlgorithm::SetDestinyTargets(vector<Point*> vListTargets)
{
    _vListTargets = vListTargets;
}

void ASearchAlgorithm::SetEndPoint(Point* pPosEnd)
{
    _pPosEnd = pPosEnd;
}


vector<Triangle*> ASearchAlgorithm::GetPath()
{
    bool bFinded = false;
    bool bTriangleInit = false;
    bool bTriangleEnd = false;
    Triangle* pTriangleInit;
    Triangle* pTriangleEnd;
    for (int i = 0; i < _vListTriangles.size(); i++)
    {
        if (_vListTriangles[i]->IsPointInTriangle(_pPosInit))
        {
            pTriangleInit = _vListTriangles[i];
            bTriangleInit = true;
        }
        if (_vListTriangles[i]->IsPointInTriangle(_pPosEnd))
        {
            pTriangleEnd = _vListTriangles[i];
            bTriangleEnd = true;
        }
    }
    if (bTriangleInit && bTriangleEnd)
    {
        Triangle* pTriangleCurrent = GetSearchPath(pTriangleInit, pTriangleEnd);

        _vTrianglePath.clear();
        _vTrianglePath.push_back(pTriangleCurrent);
        while (pTriangleCurrent != pTriangleInit)
        {
            pTriangleCurrent = pTriangleCurrent->_pPrevTriangle;
            _vTrianglePath.push_back(pTriangleCurrent);
        }
    }
    return _vTrianglePath;
}
void ASearchAlgorithm::Update(LPDIRECT3DDEVICE9 pDXDevice)
{
    _iNumTriangles = _vTrianglePath.size();
    _iNumPoints = _iNumTriangles * 3;



    if (_vTrianglePath.size() >= 1)
    {
        D3DXVECTOR3* point = new D3DXVECTOR3[_iNumPoints];
        int k = 0;
        int j = 0;
        for (int i = 0; i < _iNumTriangles; i++, j++)
        {
            Triangle* t = _vTrianglePath[i];
            Point* p1 = 0;
            Point* p2 = 0;
            Point* pMid = 0;

            if (i == 0)
                _vListLines.push_back(new Point(_pPosEnd->x, _pPosEnd->y));
            else if (i == _vTrianglePath.size() - 1)
                _vListLines.push_back(new Point(_pPosInit->x, _pPosInit->y));
            else _vListLines.push_back(new Point(_vTrianglePath[i]->GetMiddlePoint()->x, _vTrianglePath[i]->GetMiddlePoint()->y));

            if (i < _vTrianglePath.size() - 1)
            {
                for (int e = 0; e < 3; e++)
                {
                    for (int f = 0; f < 3; f++)
                    {
                        if (_vTrianglePath[i]->GetPoint(e) == _vTrianglePath[i + 1]->GetPoint(f))
                        {
                            if (p1 == 0)
                            {
                                p1 = _vTrianglePath[i]->GetPoint(e);
                            }
                            else if (p2 == 0 && p1 != _vTrianglePath[j]->GetPoint(e))
                            {
                                p2 = _vTrianglePath[i]->GetPoint(e);
                            }
                        }
                    }
                }
                pMid = new Point((float)(p1->x + p2->x) / 2.0f, (float)(p1->y + p2->y) / 2.0f);
                _vListLines.push_back(new Point(pMid->x, pMid->y));
            }

            point[k] = D3DXVECTOR3(t->GetPoint(0)->x, t->GetPoint(0)->y, 0);
            k++;
            point[k] = D3DXVECTOR3(t->GetPoint(1)->x, t->GetPoint(1)->y, 0);
            k++;
            point[k] = D3DXVECTOR3(t->GetPoint(2)->x, t->GetPoint(2)->y, 0);
            k++;
        }

        lines = new D3DXVECTOR2[_vListLines.size()];
        for (int l = 0; l < _vListLines.size(); l++)
        {
            lines[l] = D3DXVECTOR2(_vListLines[l]->x, -_vListLines[l]->y + 768);
        }

        pDXDevice->CreateVertexBuffer(
            _iNumPoints * sizeof(OURCUSTOMVERTEX),
            0,
            D3DFVF_XYZ | D3DFVF_NORMAL,
            D3DPOOL_DEFAULT,
            &_pVBTriangles,
            NULL);

        CUSTOMVERTEX* sommets = new CUSTOMVERTEX[_iNumPoints];
        _pVBTriangles->Lock(0, 0, (void**)& sommets, 0);
        for (int i = 0; i < _iNumPoints; i++)
        {
            sommets[i].x = point[i].x;
            sommets[i].y = point[i].y;
            sommets[i].z = point[i].z;
            sommets[i].color = _currentColor;
        }

        _pVBTriangles->Unlock();

        delete[] point;

    }
}
void ASearchAlgorithm::Draw(LPDIRECT3DDEVICE9 pDXDevice)
{
    if (_iNumTriangles >= 1)
    {/*
        ZeroMemory( &_materiau, sizeof(D3DMATERIAL9));
        _materiau.Ambient.r = 1.0f;
        _materiau.Ambient.g = 0.0f;
        _materiau.Ambient.b = 0.0f;
        _materiau.Ambient.a = 1.0f;

        pDXDevice->SetMaterial(&_materiau);*/
        // Source des sommets
        pDXDevice->SetStreamSource(0, _pVBTriangles, 0, sizeof(OURCUSTOMVERTEX));

        // Type des sommets
        pDXDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL);

        // Affichage
        pDXDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        pDXDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, _iNumTriangles);


        LPD3DXLINE line;
        D3DXCreateLine(pDXDevice, &line);
        line->Begin();
        line->Draw(lines, _vListLines.size(), 0xFF00FF00);
        line->End();
        line->Release();


    }

}