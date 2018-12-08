#include "stdafx.h"
#include "..\Headers\Back_Loading.h"
#include "Component_Manager.h"



_USING(Client)

CBack_Loading::CBack_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CBack_Loading::CBack_Loading(const CBack_Loading & rhs)
	: CGameObject(rhs)
{

}

HRESULT CBack_Loading::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CBack_Loading::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(2.f, 2.f, 2.f);




	return NOERROR;
}

_int CBack_Loading::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CBack_Loading::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_PRIORITY, this)))
		return -1;

	return _int();
}

void CBack_Loading::Render_GameObject()
{
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

HRESULT CBack_Loading::Ready_Component()
{
	CComponent_Manager*			pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Com_Transform
	m_pTransformCom = (CTransform*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	m_pRendererCom = (CRenderer*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	// For.Com_Buffer
	m_pBufferCom = (CBuffer_RcTex*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_LOGO, L"Component_Shader_Logo");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(SCENE_LOGO, L"Component_Texture_BackGround");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CBack_Loading::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix			matTmp;
	D3DXMatrixIdentity(&matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");



	Safe_Release(pEffect);

	return NOERROR;
}

CBack_Loading * CBack_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBack_Loading*		pInstance = new CBack_Loading(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBack_Loading::Clone_GameObject()
{
	CBack_Loading*		pInstance = new CBack_Loading(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBack_Loading Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBack_Loading::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
