#pragma once

#include "EffectT.h"

class CEffect_BloodT final : public CEffectT
{
private:
	explicit CEffect_BloodT(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_BloodT(const CEffect_BloodT& rhs);
	virtual ~CEffect_BloodT() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

private:
	HRESULT Ready_Component();


public:
	static CEffect_BloodT* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

