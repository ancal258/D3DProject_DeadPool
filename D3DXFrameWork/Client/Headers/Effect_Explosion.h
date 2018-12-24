#pragma once

#include "EffectC.h"

class CEffect_Explosion final : public CEffectC
{
private:
	explicit CEffect_Explosion(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_Explosion(const CEffect_Explosion& rhs);
	virtual ~CEffect_Explosion() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

private:
	HRESULT Ready_Component();


public:
	static CEffect_Explosion* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

