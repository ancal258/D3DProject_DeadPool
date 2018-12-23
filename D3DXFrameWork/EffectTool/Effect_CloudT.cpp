#include "stdafx.h"
#include "Effect_CloudT.h"
#include "Component_Manager.h"

CEffect_CloudT::CEffect_CloudT(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffectT(pGraphic_Device)
{
}

CEffect_CloudT::CEffect_CloudT(const CEffect_CloudT & rhs)
	: CEffectT(rhs)
{
}

HRESULT CEffect_CloudT::Ready_GameObject_Prototype()
{
	if (FAILED(CEffectT::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CEffect_CloudT::Ready_GameObject()
{
	if (FAILED(CEffectT::Ready_GameObject()))
		return E_FAIL;
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_iIndex = rand() % 16;


	return NOERROR;
}

_int CEffect_CloudT::Update_GameObject(const _float & fTimeDelta)
{
	// m_fSurviveTime --> 3�ʸ� fTimeDelta / 3;
	m_fAlpha -= fTimeDelta / m_fSurviveTime;

	return CEffectT::Update_GameObject(fTimeDelta);
}

_int CEffect_CloudT::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return CEffectT::LastUpdate_GameObject(fTimeDelta);
}

void CEffect_CloudT::Render_GameObject()
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
	pEffect->BeginPass(0);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CEffect_CloudT::Ready_Component()
{
	CComponent_Manager*			pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Com_Texture
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(0, L"Component_Texture_ExplosionCloud");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CEffect_CloudT::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix			matView, matProj;

	Get_Transform(D3DTS_VIEW, &matView);
	Get_Transform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_fAlpha", m_fAlpha);
	m_pTextureCom->SetUp_OnShader(pEffect, "g_DiffuseTexture", _uint(m_iIndex));



	Safe_Release(pEffect);

	return NOERROR;
}

CEffect_CloudT * CEffect_CloudT::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_CloudT*		pInstance = new CEffect_CloudT(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CEffect_CloudT::Clone_GameObject()
{
	CEffect_CloudT*		pInstance = new CEffect_CloudT(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CToolEffect Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_CloudT::Free()
{
	CEffectT::Free();
}
