#include "stdafx.h"
#include "Effect_Explosion.h"
#include "Component_Manager.h"

CEffect_Explosion::CEffect_Explosion(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffectC(pGraphic_Device)
{
}

CEffect_Explosion::CEffect_Explosion(const CEffect_Explosion & rhs)
	: CEffectC(rhs)
{
}

HRESULT CEffect_Explosion::Ready_GameObject_Prototype()
{
	if (FAILED(CEffectC::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CEffect_Explosion::Ready_GameObject()
{
	if (FAILED(CEffectC::Ready_GameObject()))
		return E_FAIL;
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CEffect_Explosion::Update_GameObject(const _float & fTimeDelta)
{
	m_fAlpha -= fTimeDelta / m_fSurviveTime;

	return CEffectC::Update_GameObject(fTimeDelta);
}

_int CEffect_Explosion::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return CEffectC::LastUpdate_GameObject(fTimeDelta);
}

void CEffect_Explosion::Render_GameObject()
{
	CEffectC::Render_GameObject();
	if (nullptr == m_pBufferCom ||
		nullptr == m_pShaderCom)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;


	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CEffect_Explosion::Ready_Component()
{
	CComponent_Manager*			pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Com_Texture
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Explosion");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

CEffect_Explosion * CEffect_Explosion::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Explosion*		pInstance = new CEffect_Explosion(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CEffectExplosion Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CEffect_Explosion::Clone_GameObject()
{
	CEffect_Explosion*		pInstance = new CEffect_Explosion(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CEffectExplosion Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Explosion::Free()
{
	CEffectC::Free();
}
