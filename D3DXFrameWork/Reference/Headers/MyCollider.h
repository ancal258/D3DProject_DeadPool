#pragma once
#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL CMyCollider final : public  CComponent
{
private:
	explicit CMyCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMyCollider(const CMyCollider& rhs);
	virtual ~CMyCollider() = default;
public:
	HRESULT Ready_MyCollider();


public:
	static CMyCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END