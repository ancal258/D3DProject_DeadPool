#pragma once

#include "EffectT.h"

class CEffect_TexBloodT final : public CEffectT
{
private:
	explicit CEffect_TexBloodT(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_TexBloodT(const CEffect_TexBloodT& rhs);
	virtual ~CEffect_TexBloodT() = default;

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
	static CEffect_TexBloodT* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

