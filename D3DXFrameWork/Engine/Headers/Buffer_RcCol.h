#pragma once

#include "VIBuffer.h"

// 2D : ViewPort Space 
// 3D : Local Space

// 3���� ���ý����̽� �� ���ǵǴ� �������̴�.
// �׸� ���¸� �׸���. 
// �ؽ��İ� ���εȴ�.

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