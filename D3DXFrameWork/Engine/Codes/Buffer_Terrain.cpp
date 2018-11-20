#include "..\Headers\Buffer_Terrain.h"
#include "Picking.h"
#include "Transform.h"
#include "Navigation.h"

CBuffer_Terrain::CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_Terrain::CBuffer_Terrain(const CBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_pPixel(rhs.m_pPixel)	
	, m_fInterval(rhs.m_fInterval)
	, m_pIsNavigation(rhs.m_pIsNavigation)
{

}

HRESULT CBuffer_Terrain::Ready_VIBuffer(const _uint& iNumVertexX, const _uint& iNumVertexZ, const _float& fInterval)
{
	// For.Vertex Buffer
	m_fInterval = fInterval;
	m_iStride = sizeof(VTXNORTEX);

	m_iNumVerticesX = iNumVertexX;
	m_iNumVerticesY = 1;
	m_iNumVerticesZ = iNumVertexZ;

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesY * m_iNumVerticesZ;
	m_pVertices = new _vec3[m_iNumVertices];
	m_pIsNavigation = new _bool[m_iNumVertices];
	ZeroMemory(m_pIsNavigation, sizeof(_bool)*m_iNumVertices);

	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	// For.Index Buffer
	m_iPolygonSize = sizeof(POLYGON32);
	m_iNumPolygons = (iNumVertexX - 1) * (iNumVertexZ - 1) * 2;
	m_IndicesFormat = D3DFMT_INDEX32;	

	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	// 할당된 공간에 값을 채운다.
	VTXNORTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < iNumVertexZ; i++)
	{
		for (size_t j = 0; j < iNumVertexX; j++)
		{
			_uint		iIndex = i * iNumVertexX + j;

			pVertices[iIndex].vPosition = _vec3(j * fInterval, 0.f, i * fInterval);
			m_pVertices[iIndex] = pVertices[iIndex].vPosition;
			pVertices[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _vec2(j / _float(iNumVertexX - 1), i / _float(iNumVertexZ - 1));
		}
	}

	_uint			iTriangleIdx = 0;

	POLYGON32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_vec3		vNormal;
	_vec3		vSour, vDest;

	for (size_t i = 0; i < iNumVertexZ - 1; i++)
	{
		for (size_t j = 0; j < iNumVertexX - 1; j++)
		{
			_uint		iIndex = i * iNumVertexX + j;

			pIndices[iTriangleIdx]._1 = iIndex + iNumVertexX;
			pIndices[iTriangleIdx]._2 = iIndex + iNumVertexX + 1;
			pIndices[iTriangleIdx]._3 = iIndex + 1;

			vSour = pVertices[pIndices[iTriangleIdx]._3].vPosition - pVertices[pIndices[iTriangleIdx]._2].vPosition;
			vDest = pVertices[pIndices[iTriangleIdx]._2].vPosition - pVertices[pIndices[iTriangleIdx]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			D3DXVec3Normalize(&vNormal, &vNormal);

			pVertices[pIndices[iTriangleIdx]._1].vNormal += vNormal;
			pVertices[pIndices[iTriangleIdx]._2].vNormal += vNormal;
			pVertices[pIndices[iTriangleIdx]._3].vNormal += vNormal;

			++iTriangleIdx;
			
			

			pIndices[iTriangleIdx]._1 = iIndex + iNumVertexX;
			pIndices[iTriangleIdx]._2 = iIndex + 1;
			pIndices[iTriangleIdx]._3 = iIndex;

			vSour = pVertices[pIndices[iTriangleIdx]._3].vPosition - pVertices[pIndices[iTriangleIdx]._2].vPosition;
			vDest = pVertices[pIndices[iTriangleIdx]._2].vPosition - pVertices[pIndices[iTriangleIdx]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			D3DXVec3Normalize(&vNormal, &vNormal);

			pVertices[pIndices[iTriangleIdx]._1].vNormal += vNormal;
			pVertices[pIndices[iTriangleIdx]._2].vNormal += vNormal;
			pVertices[pIndices[iTriangleIdx]._3].vNormal += vNormal;

			++iTriangleIdx;			
		}
	}

	for (size_t i = 0; i < m_iNumVertices; ++i)	
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);	

	m_pVB->Unlock();
	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CBuffer_Terrain::Ready_VIBuffer(const _tchar * pHeightMapPath, const _float & fInterval)
{
	m_fInterval = fInterval;

	_ulong			dwByte = 0;

	HANDLE			hFile = CreateFile(pHeightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	ReadFile(hFile, &m_fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

	ReadFile(hFile, &m_ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	m_iNumVerticesX = m_ih.biWidth;;
	m_iNumVerticesY = 1;
	m_iNumVerticesZ = m_ih.biHeight;;

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesY * m_iNumVerticesZ;


	m_pPixel = new _ulong[m_ih.biWidth * m_ih.biHeight];

	ReadFile(hFile, m_pPixel, sizeof(_ulong) * (m_ih.biWidth * m_ih.biHeight), &dwByte, nullptr);

	CloseHandle(hFile);

	// For.Vertex Buffer
	m_iStride = sizeof(VTXNORTEX);
	m_pVertices = new _vec3[m_iNumVertices];

	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	// For.Index Buffer
	m_iPolygonSize = sizeof(POLYGON32);
	m_iNumPolygons = (m_ih.biWidth - 1) * (m_ih.biHeight - 1) * 2;
	m_IndicesFormat = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	// 할당된 공간에 값을 채운다.
	VTXNORTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i <  m_ih.biHeight; i++)
	{
		for (size_t j = 0; j <  m_ih.biWidth; j++)
		{
			_uint		iIndex = i * m_ih.biWidth + j;

			pVertices[iIndex].vPosition = _vec3(j * fInterval, (m_pPixel[iIndex] & 0x000000ff) / 5.f, i * fInterval);
			m_pVertices[iIndex] = pVertices[iIndex].vPosition;
			pVertices[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _vec2(j / _float(m_ih.biWidth - 1), i / _float(m_ih.biHeight - 1));
		}
	}



	_uint			iTriangleIdx = 0;

	_vec3		vNormal;
	_vec3		vSour, vDest;


	POLYGON32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (size_t i = 0; i < m_ih.biHeight - 1; i++)
	{
		for (size_t j = 0; j < m_ih.biWidth - 1; j++)
		{
			_uint		iIndex = i * m_ih.biWidth + j;

			pIndices[iTriangleIdx]._1 = iIndex + m_ih.biWidth;
			pIndices[iTriangleIdx]._2 = iIndex + m_ih.biWidth + 1;
			pIndices[iTriangleIdx]._3 = iIndex + 1;

			vSour = pVertices[pIndices[iTriangleIdx]._3].vPosition - pVertices[pIndices[iTriangleIdx]._2].vPosition;
			vDest = pVertices[pIndices[iTriangleIdx]._2].vPosition - pVertices[pIndices[iTriangleIdx]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			D3DXVec3Normalize(&vNormal, &vNormal);

			pVertices[pIndices[iTriangleIdx]._1].vNormal += vNormal;
			pVertices[pIndices[iTriangleIdx]._2].vNormal += vNormal;
			pVertices[pIndices[iTriangleIdx]._3].vNormal += vNormal;

			++iTriangleIdx;

			pIndices[iTriangleIdx]._1 = iIndex + m_ih.biWidth;
			pIndices[iTriangleIdx]._2 = iIndex + 1;
			pIndices[iTriangleIdx]._3 = iIndex;

			vSour = pVertices[pIndices[iTriangleIdx]._3].vPosition - pVertices[pIndices[iTriangleIdx]._2].vPosition;
			vDest = pVertices[pIndices[iTriangleIdx]._2].vPosition - pVertices[pIndices[iTriangleIdx]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			D3DXVec3Normalize(&vNormal, &vNormal);

			pVertices[pIndices[iTriangleIdx]._1].vNormal += vNormal;
			pVertices[pIndices[iTriangleIdx]._2].vNormal += vNormal;
			pVertices[pIndices[iTriangleIdx]._3].vNormal += vNormal;

			++iTriangleIdx;
		}
	}

	for (size_t i = 0; i < m_iNumVertices; ++i)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

	m_pVB->Unlock();

	m_pIB->Unlock();

	return NOERROR;
}

_vec3 CBuffer_Terrain::SetHeight_OnTerrain(const _vec3* pPosition, const CTransform* pTransform_Terrain) const
{
	_matrix			matWorldInv;
	D3DXMatrixInverse(&matWorldInv, nullptr, pTransform_Terrain->Get_WorldMatrix());

	// 체크해야할 점을 지형의 로컬스페이스로 이동시키낟.
	_vec3			vPosition;
	D3DXVec3TransformCoord(&vPosition, pPosition, &matWorldInv);

	// 로컬영역상에서의 높이를 구한다.
	_uint		iIndex = _uint(vPosition.z / m_fInterval) * m_iNumVerticesX + _uint(vPosition.x / m_fInterval);

	_uint		iIndices[4] = { 
		iIndex + m_iNumVerticesX,
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,
		iIndex 
	};

	_float		fRatioX = (vPosition.x - m_pVertices[iIndices[0]].x) / m_fInterval;
	_float		fRatioZ = (m_pVertices[iIndices[0]].z - vPosition.z) / m_fInterval;

	_float		fHeight[4] = {
		m_pVertices[iIndices[0]].y,
		m_pVertices[iIndices[1]].y,
		m_pVertices[iIndices[2]].y,
		m_pVertices[iIndices[3]].y,
	};

	if (fRatioX > fRatioZ) // 오른쪽 상단삼각형안에 있다.
	{
		vPosition.y = fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}

	else // 왼쪽 하단 삼각형안에 있다.
	{
		vPosition.y = fHeight[0] + (fHeight[3] - fHeight[0]) * fRatioZ + (fHeight[2] - fHeight[3]) * fRatioX;
	}

	D3DXVec3TransformCoord(&vPosition, &vPosition, pTransform_Terrain->Get_WorldMatrix());

	return vPosition;
}

_bool CBuffer_Terrain::Picking_ToBuffer(const _vec3* pRayPos, const _vec3* pRayDir, const CTransform* pTransCom, _vec3* pOut)
{
	if (nullptr == m_pVertices)
		return  false;

	_vec3		vVerticesPosInWorld[3];
	_uint		iIndices[3];

	_float		fU, fV, fDist;

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;


			iIndices[0] = iIndex + m_iNumVerticesX;
			iIndices[1] = iIndex + m_iNumVerticesX + 1;
			iIndices[2] = iIndex + 1;

			D3DXVec3TransformCoord(&vVerticesPosInWorld[0], &m_pVertices[iIndices[0]], pTransCom->Get_WorldMatrix());
			D3DXVec3TransformCoord(&vVerticesPosInWorld[1], &m_pVertices[iIndices[1]], pTransCom->Get_WorldMatrix());
			D3DXVec3TransformCoord(&vVerticesPosInWorld[2], &m_pVertices[iIndices[2]], pTransCom->Get_WorldMatrix());


			if (TRUE == D3DXIntersectTri(&vVerticesPosInWorld[0], &vVerticesPosInWorld[1], &vVerticesPosInWorld[2], pRayPos, pRayDir, &fU, &fV, &fDist))
			{
				*pOut = *pRayPos + *pRayDir * fDist;
				if (1 >= m_iMouseState)
					Draw_ToBuffer(pOut);
				else if(2 == m_iMouseState || 3 == m_iMouseState) // enum 2 || 3 일때 ( 스플래팅 상태 )
					Draw_ToTexture(pOut);
				return true;
			}

			iIndices[0] = iIndex + m_iNumVerticesX;
			iIndices[1] = iIndex + 1;
			iIndices[2] = iIndex;

			D3DXVec3TransformCoord(&vVerticesPosInWorld[0], &m_pVertices[iIndices[0]], pTransCom->Get_WorldMatrix());
			D3DXVec3TransformCoord(&vVerticesPosInWorld[1], &m_pVertices[iIndices[1]], pTransCom->Get_WorldMatrix());
			D3DXVec3TransformCoord(&vVerticesPosInWorld[2], &m_pVertices[iIndices[2]], pTransCom->Get_WorldMatrix());
			
			if (TRUE == D3DXIntersectTri(&vVerticesPosInWorld[0], &vVerticesPosInWorld[1], &vVerticesPosInWorld[2], pRayPos, pRayDir, &fU, &fV, &fDist))
			{
				*pOut = *pRayPos + *pRayDir * fDist;
				if (1 >= m_iMouseState)
					Draw_ToBuffer(pOut);
				else if (2 <= m_iMouseState && m_iMouseState <= 5) // enum 2 || 3 일때 ( 스플래팅 상태 )
					Draw_ToTexture(pOut);
				return true;
			}
		}
	}

	return _bool(false);
}

_bool CBuffer_Terrain::Picking_ToBuffer(const _vec3* pRayPos, const _vec3* pRayDir, const CTransform* pTransCom, CNavigation* pNavigation , _vec3* pOut)
{
	if (nullptr == m_pVertices)
		return  false;

	_vec3		vVerticesPosInWorld[3];
	_uint		iIndices[3];

	_float		fU, fV, fDist;

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;


			iIndices[0] = iIndex + m_iNumVerticesX;
			iIndices[1] = iIndex + m_iNumVerticesX + 1;
			iIndices[2] = iIndex + 1;

			D3DXVec3TransformCoord(&vVerticesPosInWorld[0], &m_pVertices[iIndices[0]], pTransCom->Get_WorldMatrix());
			D3DXVec3TransformCoord(&vVerticesPosInWorld[1], &m_pVertices[iIndices[1]], pTransCom->Get_WorldMatrix());
			D3DXVec3TransformCoord(&vVerticesPosInWorld[2], &m_pVertices[iIndices[2]], pTransCom->Get_WorldMatrix());


			if (TRUE == D3DXIntersectTri(&vVerticesPosInWorld[0], &vVerticesPosInWorld[1], &vVerticesPosInWorld[2], pRayPos, pRayDir, &fU, &fV, &fDist))
			{

				Draw_ToNavigation(pOut, pNavigation);
				return true;
			}

			iIndices[0] = iIndex + m_iNumVerticesX;
			iIndices[1] = iIndex + 1;
			iIndices[2] = iIndex;

			D3DXVec3TransformCoord(&vVerticesPosInWorld[0], &m_pVertices[iIndices[0]], pTransCom->Get_WorldMatrix());
			D3DXVec3TransformCoord(&vVerticesPosInWorld[1], &m_pVertices[iIndices[1]], pTransCom->Get_WorldMatrix());
			D3DXVec3TransformCoord(&vVerticesPosInWorld[2], &m_pVertices[iIndices[2]], pTransCom->Get_WorldMatrix());

			if (TRUE == D3DXIntersectTri(&vVerticesPosInWorld[0], &vVerticesPosInWorld[1], &vVerticesPosInWorld[2], pRayPos, pRayDir, &fU, &fV, &fDist))
			{
				*pOut = *pRayPos + *pRayDir * fDist;
				Draw_ToNavigation(pOut, pNavigation);
				return true;
			}
		}
	}

	return _bool(false);
}

void CBuffer_Terrain::Render_VIBuffer()
{
	LPDIRECT3DDEVICE9 pGraphic_Device = Get_Graphic_Device();
	if (nullptr == pGraphic_Device)
		return;
	pGraphic_Device->AddRef();

	pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	pGraphic_Device->SetFVF(m_dwVtxFVF);
	pGraphic_Device->SetIndices(m_pIB);
	pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPolygons);

	
	Safe_Release(pGraphic_Device);
}

void CBuffer_Terrain::Draw_ToBuffer(_vec3* pOut)
{
	VTXNORTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	_vec3 vPickingPos = *pOut;
	for (size_t k = 0; k < m_iNumVerticesZ - 1; k++)
	{
		for (size_t u = 0; u < m_iNumVerticesX - 1; u++)
		{
			_uint		iIndex = k * m_iNumVerticesX + u;

			_float fLength = D3DXVec3Length(&(vPickingPos - m_pVertices[iIndex]));
			if (fLength <= m_fRadius)
			{
				_float fPlusY = fLength / m_fRadius; // 0 ~ 1 --> 0~90
				fPlusY = cosf((fPlusY*0.5f) * 3.14159f);
				if (0 == m_iMouseState)
					pVertices[iIndex].vPosition = _vec3(m_pVertices[iIndex].x, m_pVertices[iIndex].y + fPlusY * m_fPower, m_pVertices[iIndex].z);
				if (1 == m_iMouseState)
					pVertices[iIndex].vPosition = _vec3(m_pVertices[iIndex].x, m_pVertices[iIndex].y - fPlusY * m_fPower, m_pVertices[iIndex].z);

				m_pVertices[iIndex] = pVertices[iIndex].vPosition;
				pVertices[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
			}
		}
	}



	m_pVB->Unlock();
}

void CBuffer_Terrain::Draw_ToTexture(_vec3 * pOut)
{
	D3DXCreateTextureFromFileEx(Get_Graphic_Device(), L"Bin/Splatting.bmp", m_iNumVerticesX, m_iNumVerticesZ, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &m_pSplattingTexture);
	D3DLOCKED_RECT		LockRect;
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	m_pSplattingTexture->LockRect(0, &LockRect, nullptr, 0); // DISCARD  :  쓰기 전용 덮어 쓰기

	DWORD* pColor = (DWORD*)LockRect.pBits;

	_vec3 vPickingPos = *pOut;
	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint iIndex = i * m_iNumVerticesX + j;
			//pColor[iIndex] = D3DXCOLOR(1, 1, 1, 1); // xClr;

			_float fLength = D3DXVec3Length(&(vPickingPos - m_pVertices[iIndex]));
			if (fLength <= m_fRadius)
			{
				//_float fGrayScale = fLength / m_fRadius; // 0 ~ 1 --> 0~90
				//fGrayScale = cosf((fGrayScale*0.5f) * 3.14159f); // 0 ~ 1
				D3DXCOLOR xClr;
				if (2 == m_iMouseState)
				{
					//xClr = D3DXCOLOR(fGrayScale, fGrayScale, fGrayScale, fGrayScale);
					xClr = D3DXCOLOR(1, 0, 0, 0);
				}
				else if (3 == m_iMouseState)
				{
					//xClr = D3DXCOLOR(1 - fGrayScale, 1 - fGrayScale, 1 - fGrayScale, 1 - fGrayScale);
					xClr = D3DXCOLOR(0, 1, 0, 0);
				}
				else if (4 == m_iMouseState)
				{
					xClr = D3DXCOLOR(0, 0, 1, 0);
				}
				else if (5 == m_iMouseState)
				{
					xClr = D3DXCOLOR(0, 0, 0, 1);
				}
				pColor[iIndex] = xClr; // xClr;
			}
		}
	}

	m_pSplattingTexture->UnlockRect(0);
	D3DXSaveTextureToFile(L"Bin/Splatting.bmp", D3DXIFF_BMP, m_pSplattingTexture, nullptr);

	return;
}

void CBuffer_Terrain::Draw_ToNavigation(_vec3 * pOut, CNavigation* pNavigation)
{
	_vec3 vPickingPos = *pOut;
	for (size_t k = 0; k < m_iNumVerticesZ - 2; k+=2)
	{
		for (size_t u = 0; u < m_iNumVerticesX - 2; u+=2)
		{
			_uint		iIndex = k * m_iNumVerticesX + u;

			_float fLength = D3DXVec3Length(&(vPickingPos - m_pVertices[iIndex]));
			if (fLength <= m_fRadius) // 피킹 원 안에 들어있을 때
			{
				if (false == m_pIsNavigation[iIndex])
				{
					m_pIsNavigation[iIndex] = true;
					_uint		iLeft[3], iRight[3];
					_vec3 m_vArray[3];
					//우삼
					iLeft[0] = iIndex + m_iNumVerticesX*2;
					iLeft[1] = iIndex + 2;
					iLeft[2] = iIndex;
					//우삼
					iRight[0] = iIndex + m_iNumVerticesX*2;
					iRight[1] = iIndex + m_iNumVerticesX*2 + 2;
					iRight[2] = iIndex + 2;

					m_vArray[0] = m_pVertices[iLeft[0]];
					m_vArray[1] = m_pVertices[iLeft[1]];
					m_vArray[2] = m_pVertices[iLeft[2]];
					pNavigation->Add_Cell(m_vArray);
					m_vArray[0] = m_pVertices[iRight[0]];
					m_vArray[1] = m_pVertices[iRight[1]];
					m_vArray[2] = m_pVertices[iRight[2]];
					pNavigation->Add_Cell(m_vArray);
				}
			}
		}
	}
}

CBuffer_Terrain * CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint& iNumVertexX, const _uint& iNumVertexZ, const _float& fInterval)
{
	CBuffer_Terrain*		pInstance = new CBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer(iNumVertexX, iNumVertexZ, fInterval)))
	{
		_MSG_BOX("CBuffer_Terrain Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBuffer_Terrain * CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMapPath, const _float& fInterval)
{
	CBuffer_Terrain*		pInstance = new CBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer(pHeightMapPath, fInterval)))
	{
		_MSG_BOX("CBuffer_Terrain Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CBuffer_Terrain::Clone_Component()
{
	return new CBuffer_Terrain(*this);
}

void CBuffer_Terrain::Free()
{
	if(false == is_Clone())
		Safe_Delete_Array(m_pPixel);
	
	CVIBuffer::Free();
}
