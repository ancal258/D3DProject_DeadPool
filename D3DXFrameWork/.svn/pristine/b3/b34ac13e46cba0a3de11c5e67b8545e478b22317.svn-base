#pragma once

#include "Component.h"

_BEGIN(Engine)

class CTransform;
class CNavigation;
class _ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;
public:
	_uint	Get_NumVerticesX() const {
		return m_iNumVerticesX; }
	_uint	Get_NumVerticesY() const {
		return m_iNumVerticesY;	}
	_uint	Get_NumVerticesZ() const {
		return m_iNumVerticesZ; }
	const _vec3* Get_VerticesPos() const {
		return m_pVertices; }

public:
	HRESULT Ready_VIBuffer();
	virtual _bool Picking_ToBuffer(const _vec3* pRayPos, const _vec3* pRayDir, const CTransform* pTransCom, _vec3* pOut) { return false; }
	virtual _bool Picking_ToBuffer(const _vec3* pRayPos, const _vec3* pRayDir, const CTransform* pTransCom, CNavigation* pNavigation, _vec3* pOut) { return false; }
protected:
	_vec3*								m_pVertices = nullptr;
protected:
	LPDIRECT3DVERTEXBUFFER9				m_pVB = nullptr;
	_uint								m_iStride = 0;
	_uint								m_iNumVertices = 0;
	_uint								m_iNumVerticesX = 0;
	_uint								m_iNumVerticesY = 0;
	_uint								m_iNumVerticesZ = 0;
	_ulong								m_dwVtxFVF = 0;

protected:
	LPDIRECT3DINDEXBUFFER9				m_pIB = nullptr;
	_uint								m_iPolygonSize = 0;
	_uint								m_iNumPolygons = 0;
	D3DFORMAT							m_IndicesFormat;
public:
	virtual CComponent* Clone_Component() = 0;
protected:
	virtual void Free();

};

_END