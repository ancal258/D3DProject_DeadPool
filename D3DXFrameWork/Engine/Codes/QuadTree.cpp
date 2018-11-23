#include "..\Headers\QuadTree.h"
#include "Frustum.h"

CQuadTree::CQuadTree(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_fRadius(0.f)
{
	m_pGraphic_Device->AddRef();

	ZeroMemory(m_pChild, sizeof(CQuadTree*) * CHILD_END);
	ZeroMemory(m_pNeighbor, sizeof(CQuadTree*) * NEIGHBOR_END);

}

HRESULT CQuadTree::Ready_QuadTree(const _vec3* pVerticesPos, const _uint& iNumVerticesX, const _uint& iNumVerticesZ)
{
	m_pVerticesPos = pVerticesPos;

	m_iCorner[CORNER_LT] = iNumVerticesX * iNumVerticesZ - iNumVerticesX;
	m_iCorner[CORNER_RT] = iNumVerticesX * iNumVerticesZ - 1;
	m_iCorner[CORNER_RB] = iNumVerticesX - 1;
	m_iCorner[CORNER_LB] = 0;

	m_iCenter = 0;
	for (size_t i = 0; i < CORNER_END; ++i)
		m_iCenter += m_iCorner[i];

	m_iCenter = m_iCenter >> 2;

	m_fRadius = D3DXVec3Length(&(pVerticesPos[m_iCorner[CORNER_LT]] - pVerticesPos[m_iCenter]));

	Make_ChildQuadTree(pVerticesPos);

	Set_Neighbor();

	return NOERROR;
}

HRESULT CQuadTree::Ready_QuadTree(const _vec3 * pVerticesPos, const _uint & iLT, const _uint & iRT, const _uint & iRB, const _uint & iLB)
{
	m_pVerticesPos = pVerticesPos;

	m_iCorner[CORNER_LT] = iLT;
	m_iCorner[CORNER_RT] = iRT;
	m_iCorner[CORNER_RB] = iRB;
	m_iCorner[CORNER_LB] = iLB;

	m_iCenter = 0;
	for (size_t i = 0; i < CORNER_END; ++i)
		m_iCenter += m_iCorner[i];

	m_iCenter = m_iCenter >> 2;

	m_fRadius = D3DXVec3Length(&(pVerticesPos[m_iCorner[CORNER_LT]] - pVerticesPos[m_iCenter]));

	return NOERROR;
}

void CQuadTree::Make_ChildQuadTree(const _vec3* pVerticesPos)
{
	if (1 == m_iCorner[CORNER_RT] - m_iCorner[CORNER_LT])
		return;

	_uint			iLC, iTC, iRC, iBC;

	iLC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_LB]) >> 1;
	iTC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_RT]) >> 1;
	iRC = (m_iCorner[CORNER_RT] + m_iCorner[CORNER_RB]) >> 1;
	iBC = (m_iCorner[CORNER_LB] + m_iCorner[CORNER_RB]) >> 1;

	m_pChild[CHILD_LT] = CQuadTree::Create(m_pGraphic_Device, pVerticesPos, m_iCorner[CORNER_LT], iTC, m_iCenter, iLC);
	m_pChild[CHILD_RT] = CQuadTree::Create(m_pGraphic_Device, pVerticesPos, iTC, m_iCorner[CORNER_RT], iRC, m_iCenter);
	m_pChild[CHILD_RB] = CQuadTree::Create(m_pGraphic_Device, pVerticesPos, m_iCenter, iRC, m_iCorner[CORNER_RB], iBC);
	m_pChild[CHILD_LB] = CQuadTree::Create(m_pGraphic_Device, pVerticesPos, iLC, m_iCenter, iBC, m_iCorner[CORNER_LB]);

	for (size_t i = 0; i < CHILD_END; ++i)
	{
		if (nullptr != m_pChild[i])
			m_pChild[i]->Make_ChildQuadTree(pVerticesPos);
	}
}

// 최초 호출은 가장 큰 쿼드트리가 호출
void CQuadTree::Culling(CFrustum* pFrustum, D3DXPLANE* pPlanes, POLYGON32* pIndices, _ulong* pNumPolygons)
{
	// if (1 == m_iCorner[CORNER_RT] - m_iCorner[CORNER_LT]) // 가장작은 쿼드트리라면 그린다.
	if (1 == m_iCorner[CORNER_RT] - m_iCorner[CORNER_LT] ||
		true == LevelofDetail()) // 카메라와의 거리가 멀어서 즉시 그려야하는 상황.
	{
		_bool		isIn[CORNER_END] = {
			pFrustum->is_InFrustum(pPlanes, &m_pVerticesPos[m_iCorner[CORNER_LT]], 0.0f),
			pFrustum->is_InFrustum(pPlanes, &m_pVerticesPos[m_iCorner[CORNER_RT]], 0.0f),
			pFrustum->is_InFrustum(pPlanes, &m_pVerticesPos[m_iCorner[CORNER_RB]], 0.0f),
			pFrustum->is_InFrustum(pPlanes, &m_pVerticesPos[m_iCorner[CORNER_LB]], 0.0f),
		};

		_bool		is_DrawNeighbor[NEIGHBOR_END] = { true, true, true, true };

		if (nullptr != m_pNeighbor[NEIGHBOR_LEFT])
			is_DrawNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->LevelofDetail();

		if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
			is_DrawNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->LevelofDetail();

		if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
			is_DrawNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->LevelofDetail();

		if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
			is_DrawNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->LevelofDetail();

		if (is_DrawNeighbor[NEIGHBOR_LEFT] &&
			is_DrawNeighbor[NEIGHBOR_TOP] &&
			is_DrawNeighbor[NEIGHBOR_RIGHT] &&
			is_DrawNeighbor[NEIGHBOR_BOTTOM])
		{
			if (true == isIn[CORNER_LT] ||
				true == isIn[CORNER_RT] ||
				true == isIn[CORNER_RB])
			{
				pIndices[*pNumPolygons]._1 = m_iCorner[CORNER_LT];
				pIndices[*pNumPolygons]._2 = m_iCorner[CORNER_RT];
				pIndices[*pNumPolygons]._3 = m_iCorner[CORNER_RB];
				++(*pNumPolygons);
			}

			if (true == isIn[CORNER_LT] ||
				true == isIn[CORNER_RB] ||
				true == isIn[CORNER_LB])
			{
				pIndices[*pNumPolygons]._1 = m_iCorner[CORNER_LT];
				pIndices[*pNumPolygons]._2 = m_iCorner[CORNER_RB];
				pIndices[*pNumPolygons]._3 = m_iCorner[CORNER_LB];
				++(*pNumPolygons);
			}
			return;
		}

		_uint		iLC, iTC, iRC, iBC;

		iLC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_LB]) >> 1;
		iTC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_RT]) >> 1;
		iRC = (m_iCorner[CORNER_RT] + m_iCorner[CORNER_RB]) >> 1;
		iBC = (m_iCorner[CORNER_LB] + m_iCorner[CORNER_RB]) >> 1;

		// 난 그냥 그려져야되는데, 내 왼쪽 이웃놈이 분열된데.
		if (false == is_DrawNeighbor[NEIGHBOR_LEFT])
		{
			pIndices[*pNumPolygons]._1 = m_iCorner[CORNER_LT];
			pIndices[*pNumPolygons]._2 = m_iCenter;
			pIndices[*pNumPolygons]._3 = iLC;
			++(*pNumPolygons);

			pIndices[*pNumPolygons]._1 = iLC;
			pIndices[*pNumPolygons]._2 = m_iCenter;
			pIndices[*pNumPolygons]._3 = m_iCorner[CORNER_LB];
			++(*pNumPolygons);
		}
		else
		{
			pIndices[*pNumPolygons]._1 = m_iCorner[CORNER_LT];
			pIndices[*pNumPolygons]._2 = m_iCenter;
			pIndices[*pNumPolygons]._3 = m_iCorner[CORNER_LB];
			++(*pNumPolygons);
		}

		if (false == is_DrawNeighbor[NEIGHBOR_TOP])
		{
			pIndices[*pNumPolygons]._1 = m_iCorner[CORNER_LT];
			pIndices[*pNumPolygons]._2 = iTC;
			pIndices[*pNumPolygons]._3 = m_iCenter;
			++(*pNumPolygons);

			pIndices[*pNumPolygons]._1 = m_iCenter;
			pIndices[*pNumPolygons]._2 = iTC;
			pIndices[*pNumPolygons]._3 = m_iCorner[CORNER_RT];
			++(*pNumPolygons);
		}
		else
		{
			pIndices[*pNumPolygons]._1 = m_iCorner[CORNER_LT];
			pIndices[*pNumPolygons]._2 = m_iCorner[CORNER_RT];
			pIndices[*pNumPolygons]._3 = m_iCenter;
			++(*pNumPolygons);
		}

		if (false == is_DrawNeighbor[NEIGHBOR_RIGHT])
		{
			pIndices[*pNumPolygons]._1 = m_iCorner[CORNER_RT];
			pIndices[*pNumPolygons]._2 = iRC;
			pIndices[*pNumPolygons]._3 = m_iCenter;
			++(*pNumPolygons);

			pIndices[*pNumPolygons]._1 = m_iCenter;
			pIndices[*pNumPolygons]._2 = iRC;
			pIndices[*pNumPolygons]._3 = m_iCorner[CORNER_RB];
			++(*pNumPolygons);
		}
		else
		{
			pIndices[*pNumPolygons]._1 = m_iCorner[CORNER_RT];
			pIndices[*pNumPolygons]._2 = m_iCorner[CORNER_RB];
			pIndices[*pNumPolygons]._3 = m_iCenter;
			++(*pNumPolygons);
		}

		if (false == is_DrawNeighbor[NEIGHBOR_BOTTOM])
		{
			pIndices[*pNumPolygons]._1 = m_iCorner[CORNER_LB];
			pIndices[*pNumPolygons]._2 = m_iCenter;
			pIndices[*pNumPolygons]._3 = iBC;
			++(*pNumPolygons);

			pIndices[*pNumPolygons]._1 = iBC;
			pIndices[*pNumPolygons]._2 = m_iCenter;
			pIndices[*pNumPolygons]._3 = m_iCorner[CORNER_RB];
			++(*pNumPolygons);
		}
		else
		{
			pIndices[*pNumPolygons]._1 = m_iCorner[CORNER_LB];
			pIndices[*pNumPolygons]._2 = m_iCenter;
			pIndices[*pNumPolygons]._3 = m_iCorner[CORNER_RB];
			++(*pNumPolygons);
		}

		return;
	}

	// 절두체(카메라시야)랑 쿼드트리랑 겹쳐있으면 계속 자식으로 세분해서 충돌을 비교한다.
	if (true == pFrustum->is_InFrustum(pPlanes, &m_pVerticesPos[m_iCenter], m_fRadius))
	{
		for (size_t i = 0; i < CHILD_END; ++i)
		{
			if (nullptr != m_pChild[i])
				m_pChild[i]->Culling(pFrustum, pPlanes, pIndices, pNumPolygons);
		}
	}
}

void CQuadTree::Set_Neighbor()
{
	if (1 == m_iCorner[CORNER_RT] - m_iCorner[CORNER_LT])
		return;

	m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pChild[CHILD_RT];
	m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChild[CHILD_LB];

	if (nullptr != m_pNeighbor[NEIGHBOR_LEFT])
		m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RT];
	if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
		m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_LB];


	m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pChild[CHILD_LT];
	m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChild[CHILD_RB];

	if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
		m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LT];
	if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
		m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_RB];

	m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_TOP] = m_pChild[CHILD_RT];
	m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pChild[CHILD_LB];

	if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
		m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LB];
	if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
		m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_RT];

	m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_TOP] = m_pChild[CHILD_LT];
	m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pChild[CHILD_RB];

	if (nullptr != m_pNeighbor[NEIGHBOR_LEFT])
		m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RB];
	if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
		m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_LT];

	for (size_t i = 0; i < CHILD_END; i++)
	{
		if (nullptr != m_pChild[i])
			m_pChild[i]->Set_Neighbor();
	}
}

_bool CQuadTree::LevelofDetail()
{
	_vec3		vCamPosition;

	_matrix		matView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	memcpy(&vCamPosition, &matView.m[3][0], sizeof(_vec3));

	_float		fCamDistance = D3DXVec3Length(&(vCamPosition - m_pVerticesPos[m_iCenter]));

	_float		fWidth = D3DXVec3Length(&(m_pVerticesPos[m_iCorner[CORNER_RT]] - m_pVerticesPos[m_iCorner[CORNER_LT]]));

	if (fCamDistance * 0.2f >= fWidth)
		return true;

	return _bool(false);
}

CQuadTree * CQuadTree::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3* pVerticesPos, const _uint& iNumVerticesX, const _uint& iNumVerticesZ)
{
	CQuadTree*		pInstance = new CQuadTree(pGraphic_Device);

	if (FAILED(pInstance->Ready_QuadTree(pVerticesPos, iNumVerticesX, iNumVerticesZ)))
	{
		_MSG_BOX("CQuadTree Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CQuadTree * CQuadTree::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3 * pVerticesPos, const _uint & iLT, const _uint & iRT, const _uint & iRB, const _uint & iLB)
{
	CQuadTree*		pInstance = new CQuadTree(pGraphic_Device);

	if (FAILED(pInstance->Ready_QuadTree(pVerticesPos, iLT, iRT, iRB, iLB)))
	{
		_MSG_BOX("Child CQuadTree Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;

}

void CQuadTree::Free()
{
	for (auto& pQuadTree : m_pChild)
	{
		Safe_Release(pQuadTree);
	}


	Safe_Release(m_pGraphic_Device);
}
