#pragma once

#include "EffectC.h"

class CEffect_BloodSplat final : public CEffectC
{
private:
	explicit CEffect_BloodSplat(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_BloodSplat(const CEffect_BloodSplat& rhs);
	virtual ~CEffect_BloodSplat() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

private:
	HRESULT Ready_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	_uint		m_iIndex = 0;

public:
	static CEffect_BloodSplat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

