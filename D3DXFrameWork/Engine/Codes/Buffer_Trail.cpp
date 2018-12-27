#include "..\Headers\Buffer_Trail.h"

CBuffer_Trail::CBuffer_Trail(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_Trail::CBuffer_Trail(const CBuffer_Trail & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CBuffer_Trail::Ready_VIBuffer()
{
	return NOERROR;
}

HRESULT CBuffer_Trail::Ready_TrailBuffer()
{
	// For.Vertex Buffer
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 16;
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	// For. Vector
	m_vecVertex.reserve(m_iNumVertices);
	m_vecVertex.push_back(_vec3(-5, 5, 0.f));
	m_vecVertex.push_back(_vec3(-5, -5, 0.f));
	m_vecVertex.push_back(_vec3(5, 5, 0.f));
	m_vecVertex.push_back(_vec3(5, -5, 0.f));
	m_vecVertex.push_back(_vec3(-5, 5, 0.f));
	m_vecVertex.push_back(_vec3(-5, -5, 0.f));
	m_vecVertex.push_back(_vec3(5, 5, 0.f));
	m_vecVertex.push_back(_vec3(5, -5, 0.f));
	m_vecVertex.push_back(_vec3(-5, 5, 0.f));
	m_vecVertex.push_back(_vec3(-5, -5, 0.f));
	m_vecVertex.push_back(_vec3(5, 5, 0.f));
	m_vecVertex.push_back(_vec3(5, -5, 0.f));
	m_vecVertex.push_back(_vec3(-5, 5, 0.f));
	m_vecVertex.push_back(_vec3(-5, -5, 0.f));
	m_vecVertex.push_back(_vec3(5, 5, 0.f));
	m_vecVertex.push_back(_vec3(5, -5, 0.f));


	// For.Index Buffer
	m_iPolygonSize = sizeof(POLYGON16);
	m_iNumPolygons = 14;
	m_IndicesFormat = D3DFMT_INDEX16;

	LPDIRECT3DDEVICE9	pGraphic_Device = Get_Graphic_Device();
	if (nullptr == pGraphic_Device)
		return E_FAIL;
	pGraphic_Device->AddRef();

	if (FAILED(pGraphic_Device->CreateVertexBuffer(m_iNumVertices * m_iStride, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB_Trail, nullptr)))
		return E_FAIL;

	if (FAILED(pGraphic_Device->CreateIndexBuffer(m_iNumPolygons * m_iPolygonSize, 0, m_IndicesFormat, D3DPOOL_MANAGED, &m_pIB_Trail, nullptr)))
		return E_FAIL;

	Safe_Release(pGraphic_Device);

	// 할당된 공간에 값을 채운다.
	VTXTEX*			pVertices = nullptr;

	m_pVB_Trail->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = m_vecVertex[0];
	pVertices[0].vTexUV = _vec2(0.0f, 0.f);

	pVertices[1].vPosition = m_vecVertex[1];
	pVertices[1].vTexUV = _vec2(0.f, 1.f);

	pVertices[2].vPosition = m_vecVertex[2];
	pVertices[2].vTexUV = _vec2(0.166f, 0.f);

	pVertices[3].vPosition = m_vecVertex[3];
	pVertices[3].vTexUV = _vec2(0.166f, 1.f);

	pVertices[4].vPosition = m_vecVertex[4];
	pVertices[4].vTexUV = _vec2(0.332, 0.f);

	pVertices[5].vPosition = m_vecVertex[5];
	pVertices[5].vTexUV = _vec2(0.332, 1.f);

	pVertices[6].vPosition = m_vecVertex[6];
	pVertices[6].vTexUV = _vec2(0.498, 0.f);

	pVertices[7].vPosition = m_vecVertex[7];
	pVertices[7].vTexUV = _vec2(0.498, 1.f);

	pVertices[8].vPosition = m_vecVertex[8];
	pVertices[8].vTexUV = _vec2(0.664, 0.f);

	pVertices[9].vPosition = m_vecVertex[9];
	pVertices[9].vTexUV = _vec2(0.664, 1.f);

	pVertices[10].vPosition = m_vecVertex[10];
	pVertices[10].vTexUV = _vec2(0.83, 0.f);

	pVertices[11].vPosition = m_vecVertex[11];
	pVertices[11].vTexUV = _vec2(0.83, 1.f);

	pVertices[12].vPosition = m_vecVertex[12];
	pVertices[12].vTexUV = _vec2(0.996, 0.f);

	pVertices[13].vPosition = m_vecVertex[13];
	pVertices[13].vTexUV = _vec2(0.996, 1.f);

	pVertices[14].vPosition = m_vecVertex[14];
	pVertices[14].vTexUV = _vec2(1.0f, 0.f);

	pVertices[15].vPosition = m_vecVertex[15];
	pVertices[15].vTexUV = _vec2(1.0f, 1.f);

	m_pVB_Trail->Unlock();

	POLYGON16*		pIndices = nullptr;

	m_pIB_Trail->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 2;
	pIndices[0]._2 = 0;
	pIndices[0]._3 = 1;

	pIndices[1]._1 = 2;
	pIndices[1]._2 = 1;
	pIndices[1]._3 = 3;

	pIndices[2]._1 = 4;
	pIndices[2]._2 = 2;
	pIndices[2]._3 = 3;

	pIndices[3]._1 = 4;
	pIndices[3]._2 = 3;
	pIndices[3]._3 = 5;

	pIndices[4]._1 = 6;
	pIndices[4]._2 = 4;
	pIndices[4]._3 = 5;

	pIndices[5]._1 = 6;
	pIndices[5]._2 = 5;
	pIndices[5]._3 = 7;

	pIndices[6]._1 = 8;
	pIndices[6]._2 = 6;
	pIndices[6]._3 = 7;

	pIndices[7]._1 = 8;
	pIndices[7]._2 = 7;
	pIndices[7]._3 = 9;

	pIndices[8]._1 = 10;
	pIndices[8]._2 = 8;
	pIndices[8]._3 = 9;

	pIndices[9]._1 = 10;
	pIndices[9]._2 = 9;
	pIndices[9]._3 = 11;

	pIndices[10]._1 = 12;
	pIndices[10]._2 = 10;
	pIndices[10]._3 = 11;

	pIndices[11]._1 = 12;
	pIndices[11]._2 = 11;
	pIndices[11]._3 = 13;

	pIndices[12]._1 = 14;
	pIndices[12]._2 = 12;
	pIndices[12]._3 = 13;

	pIndices[13]._1 = 14;
	pIndices[13]._2 = 13;
	pIndices[13]._3 = 15;

	m_pIB_Trail->Unlock();

	return NOERROR;
}

HRESULT CBuffer_Trail::Add_VertexBuffer(_vec3 vVertexBegin, _vec3 vVertexEnd)
{
	if (m_vecVertex.size() == 16)
	{
		m_vecVertex.erase(m_vecVertex.begin());
		m_vecVertex.erase(m_vecVertex.begin());
	}

	m_vecVertex.push_back(vVertexBegin);
	m_vecVertex.push_back(vVertexEnd);

	VTXTEX*			pVertices = nullptr;

	m_pVB_Trail->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = m_vecVertex[0];
	pVertices[0].vTexUV = _vec2(0.0f, 0.f);

	pVertices[1].vPosition = m_vecVertex[1];
	pVertices[1].vTexUV = _vec2(0.f, 1.f);

	pVertices[2].vPosition = m_vecVertex[2];
	pVertices[2].vTexUV = _vec2(0.166f, 0.f);

	pVertices[3].vPosition = m_vecVertex[3];
	pVertices[3].vTexUV = _vec2(0.166f, 1.f);

	pVertices[4].vPosition = m_vecVertex[4];
	pVertices[4].vTexUV = _vec2(0.332, 0.f);

	pVertices[5].vPosition = m_vecVertex[5];
	pVertices[5].vTexUV = _vec2(0.332, 1.f);

	pVertices[6].vPosition = m_vecVertex[6];
	pVertices[6].vTexUV = _vec2(0.498, 0.f);

	pVertices[7].vPosition = m_vecVertex[7];
	pVertices[7].vTexUV = _vec2(0.498, 1.f);

	pVertices[8].vPosition = m_vecVertex[8];
	pVertices[8].vTexUV = _vec2(0.664, 0.f);

	pVertices[9].vPosition = m_vecVertex[9];
	pVertices[9].vTexUV = _vec2(0.664, 1.f);

	pVertices[10].vPosition = m_vecVertex[10];
	pVertices[10].vTexUV = _vec2(0.83, 0.f);

	pVertices[11].vPosition = m_vecVertex[11];
	pVertices[11].vTexUV = _vec2(0.83, 1.f);

	pVertices[12].vPosition = m_vecVertex[12];
	pVertices[12].vTexUV = _vec2(0.996, 0.f);

	pVertices[13].vPosition = m_vecVertex[13];
	pVertices[13].vTexUV = _vec2(0.996, 1.f);

	pVertices[14].vPosition = m_vecVertex[14];
	pVertices[14].vTexUV = _vec2(1.0f, 0.f);

	pVertices[15].vPosition = m_vecVertex[15];
	pVertices[15].vTexUV = _vec2(1.0f, 1.f);

	m_pVB_Trail->Unlock();

	return NOERROR;
}

void CBuffer_Trail::Render_VIBuffer()
{
	LPDIRECT3DDEVICE9 pGraphic_Device = Get_Graphic_Device();
	if (nullptr == pGraphic_Device)
		return;
	pGraphic_Device->AddRef();

	pGraphic_Device->SetStreamSource(0, m_pVB_Trail, 0, m_iStride);
	pGraphic_Device->SetFVF(m_dwVtxFVF);
	pGraphic_Device->SetIndices(m_pIB_Trail);
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
	CBuffer_Trail*		pInstance = new CBuffer_Trail(*this);
	if (FAILED(pInstance->Ready_TrailBuffer()))
	{
		_MSG_BOX("CBuffer_Trail Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuffer_Trail::Free()
{
	Safe_Release(m_pVB_Trail);
	Safe_Release(m_pIB_Trail);
	CVIBuffer::Free();
}
