#pragma once

#include "EffectC.h"

class CEffect_CloudDark final : public CEffectC
{
private:
	explicit CEffect_CloudDark(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_CloudDark(const CEffect_CloudDark& rhs);
	virtual ~CEffect_CloudDark() = default;

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
	static CEffect_CloudDark* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

