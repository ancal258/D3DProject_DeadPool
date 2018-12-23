#include "..\Headers\Buffer_Trail.h"

CBuffer_Trail::CBuffer_Trail(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_Trail::CBuffer_Trail(const CBuffer_Trail & rhs)
	: CVIBuffer(rhs)
	, m_vecVertex(rhs.m_vecVertex)
{

}

HRESULT CBuffer_Trail::Ready_VIBuffer()
{
	// For.Vertex Buffer
	m_iStride = sizeof(VTXCOL);
	m_iNumVertices = 4;
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	
	// For. Vector
	m_vecVertex.reserve(m_iNumVertices);
	m_vecVertex.push_back(_vec3(-5, 5, 0.f));
	m_vecVertex.push_back(_vec3(-5, -5, 0.f));
	m_vecVertex.push_back(_vec3(5, 5, 0.f));
	m_vecVertex.push_back(_vec3(5, -5, 0.f));


	// For.Index Buffer
	m_iPolygonSize = sizeof(POLYGON16);
	m_iNumPolygons = 2;
	m_IndicesFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	// 할당된 공간에 값을 채운다.
	VTXCOL*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = m_vecVertex[0];
	pVertices[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[1].vPosition = m_vecVertex[1];
	pVertices[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[2].vPosition = m_vecVertex[2];
	pVertices[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[3].vPosition = m_vecVertex[3];
	pVertices[3].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	m_pVB->Unlock();

	POLYGON16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 2;
	pIndices[0]._2 = 0;
	pIndices[0]._3 = 1;

	pIndices[1]._1 = 2;
	pIndices[1]._2 = 1;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CBuffer_Trail::Add_VertexBuffer(_vec3 vVertexBegin, _vec3 vVertexEnd)
{
	if (m_vecVertex.size() == 4)
	{
		m_vecVertex.erase(m_vecVertex.begin());
		m_vecVertex.erase(m_vecVertex.begin());
	}

	m_vecVertex.push_back(vVertexBegin);
	m_vecVertex.push_back(vVertexEnd);

	VTXCOL*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = m_vecVertex[0];
	pVertices[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[1].vPosition = m_vecVertex[1];
	pVertices[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[2].vPosition = m_vecVertex[2];
	pVertices[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	pVertices[3].vPosition = m_vecVertex[3];
	pVertices[3].dwColor = D3DXCOLOR(1.f, 0.f, 0.0f, 1.f);

	m_pVB->Unlock();

	return NOERROR;
}

void CBuffer_Trail::Render_VIBuffer()
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

CBuffer_Trail * CBuffer_Trail::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuffer_Trail*		pInstance = new CBuffer_Trail(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer()))
	{
		_MSG_BOX("CBuffer_Trail Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CBuffer_Trail::Clone_Component()
{
	return new CBuffer_Trail(*this);
}

void CBuffer_Trail::Free()
{

	CVIBuffer::Free();
}
