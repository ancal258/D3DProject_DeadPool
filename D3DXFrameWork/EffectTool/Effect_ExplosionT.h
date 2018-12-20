#pragma once

#include "EffectT.h"

class CEffect_ExplosionT final : public CEffectT
{
private:
	explicit CEffect_ExplosionT(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_ExplosionT(const CEffect_ExplosionT& rhs);
	virtual ~CEffect_ExplosionT() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

private:
	HRESULT Ready_Component();


public:
	static CEffect_ExplosionT* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

