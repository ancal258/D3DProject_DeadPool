#pragma once

#include "VIBuffer.h"

// 2D : ViewPort Space 
// 3D : Local Space

// 3차원 로컬스페이스 상에 정의되는 정점들이다.
// 네모 형태를 그린다. 
// 텍스쳐가 맵핑된다.

_BEGIN(Engine)

class _ENGINE_DLL CBuffer_RcCol final : public CVIBuffer
{
private:
	explicit CBuffer_RcCol(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuffer_RcCol(const CBuffer_RcCol& rhs);
	virtual ~CBuffer_RcCol() = default;
public:
	HRESULT Ready_VIBuffer();
	virtual void Render_VIBuffer();
public:
	static CBuffer_RcCol* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END