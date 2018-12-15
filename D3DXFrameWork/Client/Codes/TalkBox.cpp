#include "stdafx.h"
#include "..\Headers\TalkBox.h"
#include "Component_Manager.h"
#include "Font_Manager.h"
_USING(Client)

CTalkBox::CTalkBox(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CTalkBox::CTalkBox(const CTalkBox & rhs)
	: CGameObject(rhs)
	, m_dwTextureIdx(rhs.m_dwTextureIdx)
{

}

void CTalkBox::Set_Info(_vec2 vPos, _tchar* pTalkString)
{
	m_fX = vPos.x;
	m_fY = vPos.y;
	lstrcpy(m_szTalk, pTalkString);
	m_iStrlen = lstrlen(m_szTalk);

	m_fSizeX = 18 * m_iStrlen;
	m_fSizeY = 100;

}

HRESULT CTalkBox::Ready_GameObject_Prototype(_uint iKind)
{
	m_dwTextureIdx = iKind;

	return NOERROR;
}

HRESULT CTalkBox::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	//m_fX = 500.0f;
	//m_fY = 100.0f;
	//m_fSizeX = 200.0f;
	//m_fSizeY = 200.0f;

	return NOERROR;
}

_int CTalkBox::Update_GameObject(const _float & fTimeDelta)
{
	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_fX - (g_iBackCX >> 1)
		, (g_iBackCY >> 1) - m_fY, 0.f));

	return _int();
}

_int CTalkBox::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

void CTalkBox::Render_GameObject()
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


	_matrix	   matTransform, matScale, matTranslate;
	D3DXMatrixScaling(&matScale, 1.2f, 1.2f, 1.f);
	D3DXMatrixTranslation(&matTranslate, m_fX/* - (m_fSizeX * 0.8f)*/ , m_fY - 15, 0.f);
	matTransform = matScale * matTranslate;
	CFont_Manager::GetInstance()->Render_Font(L"Font_Badaboom", m_szTalk, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), &matTransform);
	

	Safe_Release(pEffect);
}

HRESULT CTalkBox::Ready_Component()
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
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_UI_TalkBox");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CTalkBox::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

CTalkBox * CTalkBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iKind)
{
	CTalkBox*		pInstance = new CTalkBox(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype(iKind)))
	{
		_MSG_BOX("Prototype_CUI_Test Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTalkBox::Clone_GameObject()
{
	CTalkBox*		pInstance = new CTalkBox(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CTalkBox Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTalkBox::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
