#include "../Headers/Buffer_TextUI.h"

CBuffer_TextUI::CBuffer_TextUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_TextUI::CBuffer_TextUI(const CBuffer_TextUI & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CBuffer_TextUI::Ready_VIBuffer()
{
	// For.Vertex Buffer
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	// For.Index Buffer
	m_iPolygonSize = sizeof(POLYGON16);
	m_iNumPolygons = 2;
	m_IndicesFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	// 할당된 공간에 값을 채운다.
	VTXTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec3(1.f, 1.f, 0.f);
	pVertices[0].vTexUV = _vec2(0.0f, 0.f);

	pVertices[1].vPosition = _vec3(0, 1.f, 0.f);
	pVertices[1].vTexUV = _vec2(1.0f, 0.f);

	pVertices[2].vPosition = _vec3(0, 0, 0.f);
	pVertices[2].vTexUV = _vec2(1.0f, 1.f);

	pVertices[3].vPosition = _vec3(1.f, 0, 0.f);
	pVertices[3].vTexUV = _vec2(0.0f, 1.f);

	m_pVB->Unlock();

	POLYGON16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

void CBuffer_TextUI::Render_VIBuffer()
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

CBuffer_TextUI * CBuffer_TextUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuffer_TextUI*		pInstance = new CBuffer_TextUI(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer()))
	{
		_MSG_BOX("CBuffer_TextUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CBuffer_TextUI::Clone_Component()
{
	return new CBuffer_TextUI(*this);
}

void CBuffer_TextUI::Free()
{

	CVIBuffer::Free();
}
