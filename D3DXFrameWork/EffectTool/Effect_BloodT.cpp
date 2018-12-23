#include "stdafx.h"
#include "Effect_BloodT.h"
#include "Component_Manager.h"

CEffect_BloodT::CEffect_BloodT(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffectT(pGraphic_Device)
{
}

CEffect_BloodT::CEffect_BloodT(const CEffect_BloodT & rhs)
	:CEffectT(rhs)
{
}

HRESULT CEffect_BloodT::Ready_GameObject_Prototype()
{
	if (FAILED(CEffectT::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CEffect_BloodT::Ready_GameObject()
{
	if (FAILED(CEffectT::Ready_GameObject()))
		return E_FAIL;
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CEffect_BloodT::Update_GameObject(const _float & fTimeDelta)
{
	m_fAlpha -= fTimeDelta / m_fSurviveTime;

	return CEffectT::Update_GameObject(fTimeDelta);
}

_int CEffect_BloodT::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return CEffectT::LastUpdate_GameObject(fTimeDelta);
}

void CEffect_BloodT::Render_GameObject()
{
	CEffectT::Render_GameObject();
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
	pEffect->BeginPass(3);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CEffect_BloodT::Ready_Component()
{
	CComponent_Manager*			pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Com_Texture
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(0, L"Component_Texture_BloodAnim");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

CEffect_BloodT * CEffect_BloodT::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_BloodT*		pInstance = new CEffect_BloodT(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CEffect_BloodT::Clone_GameObject()
{
	CEffect_BloodT*		pInstance = new CEffect_BloodT(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CToolEffect Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_BloodT::Free()
{
	CEffectT::Free();
}
