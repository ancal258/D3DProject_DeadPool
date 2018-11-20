#pragma once

#include "VIBuffer.h"

// 2D : ViewPort Space 
// 3D : Local Space

// 3���� ���ý����̽� �� ���ǵǴ� �������̴�.
// �׸� ���¸� �׸���. 
// �ؽ��İ� ���εȴ�.

_BEGIN(Engine)

class _ENGINE_DLL CBuffer_Collider final : public CVIBuffer
{
private:
	explicit CBuffer_Collider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuffer_Collider(const CBuffer_Collider& rhs);
	virtual ~CBuffer_Collider() = default;
public:
	HRESULT Ready_VIBuffer();
	virtual void Render_VIBuffer();
public:
	static CBuffer_Collider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END