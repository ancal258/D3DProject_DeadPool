#include "..\Headers\Buffer_Collider.h"

CBuffer_Collider::CBuffer_Collider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_Collider::CBuffer_Collider(const CBuffer_Collider & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CBuffer_Collider::Ready_VIBuffer()
{
	// For.Vertex Buffer
	m_iStride = sizeof(VTXCOL);
	m_iNumVertices = 8;
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	// For.Index Buffer
	m_iPolygonSize = sizeof(POLYGON16);
	m_iNumPolygons = 12;
	m_IndicesFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	// ÇÒ´çµÈ °ø°£¿¡ °ªÀ» Ã¤¿î´Ù.
	VTXCOL*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec3(0.5f, 1.f, -0.5f);
	pVertices[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[1].vPosition = _vec3(-0.5f, 1.f, -0.5f);
	pVertices[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[2].vPosition = _vec3(-0.5f, 0.f, -0.5f);
	pVertices[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[3].vPosition = _vec3(0.5f, 0.f, -0.5f);
	pVertices[3].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[4].vPosition = _vec3(0.5f, 1.f, 0.5f);
	pVertices[4].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[5].vPosition = _vec3(-0.5f, 1.f, 0.5f);
	pVertices[5].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[6].vPosition = _vec3(-0.5f, 0.f, 0.5f);
	pVertices[6].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[7].vPosition = _vec3(0.5f, 0.f, 0.5f);
	pVertices[7].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	m_pVB->Unlock();

	POLYGON16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	// ¾Æ·§¸é
	pIndices[0]._1 = 0;
	pIndices[0]._2 = 2;
	pIndices[0]._3 = 1;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 3;
	pIndices[1]._3 = 2;
	
	// À­¸é
	pIndices[2]._1 = 4;
	pIndices[2]._2 = 5;
	pIndices[2]._3 = 6;

	pIndices[3]._1 = 4;
	pIndices[3]._2 = 6;
	pIndices[3]._3 = 7;
	// ¿·¸é 1
	pIndices[4]._1 = 7;
	pIndices[4]._2 = 6;
	pIndices[4]._3 = 2;

	pIndices[5]._1 = 7;
	pIndices[5]._2 = 2;
	pIndices[5]._3 = 3;
	// ¿·¸é 2
	pIndices[6]._1 = 4;
	pIndices[6]._2 = 7;
	pIndices[6]._3 = 3;

	pIndices[7]._1 = 4;
	pIndices[7]._2 = 3;
	pIndices[7]._3 = 0;
	// ¿·¸é 3
	pIndices[8]._1 = 5;
	pIndices[8]._2 = 4;
	pIndices[8]._3 = 0;

	pIndices[9]._1 = 5;
	pIndices[9]._2 = 0;
	pIndices[9]._3 = 1;
	// ¿·¸é 4
	pIndices[10]._1 = 6;
	pIndices[10]._2 = 5;
	pIndices[10]._3 = 1;

	pIndices[11]._1 = 6;
	pIndices[11]._2 = 1;
	pIndices[11]._3 = 2;

	m_pIB->Unlock();

	return NOERROR;
}

void CBuffer_Collider::Render_VIBuffer()
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

CBuffer_Collider * CBuffer_Collider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuffer_Collider*		pInstance = new CBuffer_Collider(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer()))
	{
		_MSG_BOX("CBuffer_Collider Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CBuffer_Collider::Clone_Component()
{
	return new CBuffer_Collider(*this);
}

void CBuffer_Collider::Free()
{

	CVIBuffer::Free();
}
