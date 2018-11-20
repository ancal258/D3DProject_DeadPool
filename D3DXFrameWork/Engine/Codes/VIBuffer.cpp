#include "..\Headers\VIBuffer.h"
#include "Transform.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pVertices(nullptr)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesY(rhs.m_iNumVerticesY)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
	, m_pIB(rhs.m_pIB)
	, m_iPolygonSize(rhs.m_iPolygonSize)
	, m_iNumPolygons(rhs.m_iNumPolygons)
	, m_IndicesFormat(rhs.m_IndicesFormat)
	, m_pVertices(rhs.m_pVertices)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

HRESULT CVIBuffer::Ready_VIBuffer()
{
	LPDIRECT3DDEVICE9	pGraphic_Device = Get_Graphic_Device();
	if (nullptr == pGraphic_Device)
		return E_FAIL;
	pGraphic_Device->AddRef();

	if (FAILED(pGraphic_Device->CreateVertexBuffer(m_iNumVertices * m_iStride, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	if (FAILED(pGraphic_Device->CreateIndexBuffer(m_iNumPolygons * m_iPolygonSize, 0, m_IndicesFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	Safe_Release(pGraphic_Device);

	return NOERROR;
}

void CVIBuffer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	if (false == is_Clone())
		Safe_Delete_Array(m_pVertices);

	CComponent::Free();
}
