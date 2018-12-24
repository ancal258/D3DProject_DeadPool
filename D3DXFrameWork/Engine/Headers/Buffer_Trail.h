#pragma once

#include "VIBuffer.h"

// 2D : ViewPort Space 
// 3D : Local Space

// 3차원 로컬스페이스 상에 정의되는 정점들이다.
// 네모 형태를 그린다. 
// 텍스쳐가 맵핑된다.

_BEGIN(Engine)

class _ENGINE_DLL CBuffer_Trail final : public CVIBuffer
{
private:
	explicit CBuffer_Trail(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuffer_Trail(const CBuffer_Trail& rhs);
	virtual ~CBuffer_Trail() = default;
public:
	HRESULT Ready_VIBuffer();
	HRESULT Ready_TrailBuffer();
	HRESULT Add_VertexBuffer(_vec3 vVertexBegin, _vec3 vVertexEnd);
	virtual void Render_VIBuffer();
private:
	vector<_vec3>	m_vecVertex;

	LPDIRECT3DVERTEXBUFFER9				m_pVB_Trail = nullptr;
	LPDIRECT3DINDEXBUFFER9				m_pIB_Trail = nullptr;

public:
	static CBuffer_Trail* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END