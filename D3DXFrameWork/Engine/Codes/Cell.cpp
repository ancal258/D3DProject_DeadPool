#include "..\Headers\Cell.h"

CCell::CCell(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

HRESULT CCell::Ready_Cell(const _vec3 * pPoints, const _ulong& dwIndex)
{
	ZeroMemory(m_pNeighbor, sizeof(CCell*)* NEIGHBOR_END);
	memcpy(&m_vPoint, pPoints, sizeof(_vec3) * POINT_END);

	m_dwIndex = dwIndex;
	_vec3		vTmp = pPoints[POINT_B] - pPoints[POINT_A];
	m_vDir[LINE_AB] = _vec2(vTmp.x, vTmp.z);
	m_vNormal[LINE_AB] = _vec2(vTmp.z * -1, vTmp.x);

	vTmp = m_vPoint[POINT_C] - m_vPoint[POINT_B];
	m_vDir[LINE_BC] = _vec2(vTmp.x, vTmp.z);
	m_vNormal[LINE_BC] = _vec2(vTmp.z * -1.f, vTmp.x);

	vTmp = m_vPoint[POINT_A] - m_vPoint[POINT_C];
	m_vDir[LINE_CA] = _vec2(vTmp.x, vTmp.z);
	m_vNormal[LINE_CA] = _vec2(vTmp.z * -1.f, vTmp.x);

	for (size_t i = 0; i < LINE_END; ++i)
	{
		D3DXVec2Normalize(&m_vNormal[i], &m_vNormal[i]);
	}

	return NOERROR;
}

_bool CCell::Compare_Point(const _vec3 * pSourPoint, const _vec3 * pDestPoint)
{
	if (m_vPoint[POINT_A] == *pSourPoint)
	{
		if (m_vPoint[POINT_B] == *pDestPoint)
			return true;

		if (m_vPoint[POINT_C] == *pDestPoint)
			return true;
	}

	if (m_vPoint[POINT_B] == *pSourPoint)
	{
		if (m_vPoint[POINT_A] == *pDestPoint)
			return true;


		if (m_vPoint[POINT_C] == *pDestPoint)
			return true;
	}

	if (m_vPoint[POINT_C] == *pSourPoint)
	{
		if (m_vPoint[POINT_A] == *pDestPoint)
			return true;

		if (m_vPoint[POINT_B] == *pDestPoint)
			return true;
	}

	return false;
}

_bool CCell::Is_Move(const _vec3 * pPosition, _ulong * pIndex)
{
	// pIndex = �ű� �ε��� ��ġ

	for (size_t i = 0; i < 3; i++)
	{
		_vec3 vTmp = *pPosition - m_vPoint[i];
		_vec2 vDir = _vec2(vTmp.x, vTmp.z);

		if (0 < D3DXVec2Dot(&m_vNormal[i], D3DXVec2Normalize(&vDir, &vDir)))
		{
			if (nullptr == m_pNeighbor[i])
			{
				return false;
			}
			else
			{
				*pIndex = m_pNeighbor[i]->m_dwIndex;
				return true;
			}
		}
	}
	*pIndex = m_dwIndex;
	return true;
}

_bool CCell::Is_Sliding(const _vec3 * pPosition, _ulong * pIndex)
{
	for (size_t i = 0; i < 3; ++i)
	{
		_vec3	vTmp = *pPosition - m_vPoint[i];
		_vec2	vDir = _vec2(vTmp.x, vTmp.z);

		if (0 < D3DXVec2Dot(&m_vNormal[i], D3DXVec2Normalize(&vDir, &vDir)))
		{
			if (nullptr == m_pNeighbor[i])
				return false;
			else
			{
				*pIndex = m_pNeighbor[i]->m_dwIndex;
				return true;
			}

		}
	}

	*pIndex = m_dwIndex;

	return true;
}

_vec2 CCell::Sliding_Move(const _vec3 * pLook, _ulong dwSlideIndex)
{
	_vec2 vLook = _vec2((*pLook).x,(*pLook).z);
	_vec2 vReplectLook = _vec2((vLook * -1).x, (vLook * -1).y);
	_vec2 vProj = m_vNormal[dwSlideIndex];
	vProj *= (D3DXVec2Dot(&m_vNormal[dwSlideIndex], &vReplectLook)); // ���� ����
	
	return vLook + vProj;
}

void CCell::Render_Cell(LPD3DXLINE pLine)
{
	_vec3		vPoint[4];

	vPoint[0] = m_vPoint[POINT_A];
	vPoint[1] = m_vPoint[POINT_B];
	vPoint[2] = m_vPoint[POINT_C];
	vPoint[3] = m_vPoint[POINT_A];

	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	_matrix		matTransform;
	D3DXMatrixIdentity(&matTransform);

	for (size_t i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
		if (vPoint[i].z < 0.0f)
			vPoint[i].z = 0.f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	pLine->SetWidth(2.0f);

	pLine->Begin();

	pLine->DrawTransform(vPoint, 4, &matTransform, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	pLine->End();
}

CCell * CCell::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3* pPoints, const _ulong& dwIndex)
{
	CCell*		pInstance = new CCell(pGraphic_Device);

	if (FAILED(pInstance->Ready_Cell(pPoints, dwIndex)))
	{
		_MSG_BOX("CCell Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CCell::Free()
{
	Safe_Release(m_pGraphic_Device);
}

