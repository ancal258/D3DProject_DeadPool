#include "stdafx.h"
#include "..\Headers\UI_CrossHair.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Player.h"

_USING(Client)

CUI_CrossHair::CUI_CrossHair(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CUI_CrossHair::CUI_CrossHair(const CUI_CrossHair & rhs)
	: CGameObject(rhs)
{

}

HRESULT CUI_CrossHair::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CUI_CrossHair::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	const CGameObject* pPlayer = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);
	if (nullptr == pPlayer)
		return E_FAIL;
	
	m_pPlayer = (CPlayer*)pPlayer;

	m_fX = g_iBackCX >> 1;
	m_fY = g_iBackCY >> 1;
	m_fSizeX = 32.0f;
	m_fSizeY = 32.0f;

	return NOERROR;
}

_int CUI_CrossHair::Update_GameObject(const _float & fTimeDelta)
{
	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_fX - (g_iBackCX >> 1)
		, (g_iBackCY >> 1) - m_fY, 0.f));

	return _int();
}

_int CUI_CrossHair::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Update_Matrix();

	
	if (true == m_pPlayer->Get_IsButtonDown(8))
	{
		if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_UI, this)))
			return -1;
	}

	return _int();
}

void CUI_CrossHair::Render_GameObject()
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

HRESULT CUI_CrossHair::Ready_Component()
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
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Shader_UI");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_UI_CrossHair");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CUI_CrossHair::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix			matView, matProj;

	D3DXMatrixIdentity(&matView);

	// 직교투영행렬을 만들어주기위한 함수.
	D3DXMatrixOrthoLH(&matProj, g_iBackCX, g_iBackCY, 0.0f, 1.f);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_DiffuseTexture", m_dwTextureIdx);



	Safe_Release(pEffect);

	return NOERROR;
}

CUI_CrossHair * CUI_CrossHair::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_CrossHair*		pInstance = new CUI_CrossHair(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CUI_Test Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_CrossHair::Clone_GameObject()
{
	CUI_CrossHair*		pInstance = new CUI_CrossHair(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CUI_CrossHair Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_CrossHair::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
