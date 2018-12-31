#include "stdafx.h"
#include "..\Headers\FadeOutUI.h"
#include "Component_Manager.h"
#include "Font_Manager.h"
#include "Input_Device.h"
#include "Player.h"
#include "Object_Manager.h"
_USING(Client)

CFadeOutUI::CFadeOutUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CFadeOutUI::CFadeOutUI(const CFadeOutUI & rhs)
	: CGameObject(rhs)
	, m_dwTextureIdx(rhs.m_dwTextureIdx)
{

}

void CFadeOutUI::Set_Info(_vec2 vPos, _vec2 vScale)
{
	m_fX = vPos.x;
	m_fY = vPos.y;

	m_fSizeX = vScale.x;
	m_fSizeY = vScale.y;

}

void CFadeOutUI::Set_Info(_vec2 vPos, _vec2 vScale, _float fDegree)
{
	m_fX = vPos.x;
	m_fY = vPos.y;

	m_fSizeX = vScale.x;
	m_fSizeY = vScale.y;

	m_pTransformCom->Set_AngleZ(D3DXToRadian(fDegree));
}

_bool CFadeOutUI::Get_EndFadeOut()
{

	if (m_fAlpha > 1.f)
		return true;
	else
		return false;
}

HRESULT CFadeOutUI::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CFadeOutUI::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(g_iBackCX, g_iBackCY, 1.f);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0, 0, 0.f));

	return NOERROR;
}

_int CFadeOutUI::Update_GameObject(const _float & fTimeDelta)
{
	//if (GetKeyState(VK_NUMPAD1) & 0x8000)
	//	m_fAlpha = 1.f;
	if(true == m_isEndStage)
		m_fAlpha += fTimeDelta;

	return _int();
}

_int CFadeOutUI::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Update_Matrix();




	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_UI, this)))
		return -1;
	return _int();
}

void CFadeOutUI::Render_GameObject()
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
	pEffect->BeginPass(3);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	//Render_StaticText();

	Safe_Release(pEffect);
}

void CFadeOutUI::Render_StaticText()
{
	// DP Point inkStroke Text
	//if (1 == m_dwTextureIdx)
	//{
	//	_matrix	   matTransform, matScale, matTranslate;
	//	D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 1.f);
	//	D3DXMatrixTranslation(&matTranslate, m_fX - 123 /*(m_fSizeX * 0.8f)*/, m_fY - 10, 0.f);
	//	matTransform = matScale * matTranslate;
	//	_tchar m_szDP_Point[MAX_PATH] = L"";
	//	wsprintf(m_szDP_Point, L"DP Point : %d", m_pPlayer->Get_DP_Point());
	//	CFont_Manager::GetInstance()->Render_Font(L"Font_Number", m_szDP_Point, D3DXCOLOR(1.f, 0.54f, 0.f, 1.f), &matTransform);
	//}
}

HRESULT CFadeOutUI::Ready_Component()
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
	m_pBufferCom = (CBuffer_TextUI*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Shader_UI");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_FadeOut");

	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;


	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CFadeOutUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	pEffect->SetFloat("g_fAlpha", m_fAlpha);
	m_pTextureCom->SetUp_OnShader(pEffect, "g_DiffuseTexture", m_dwTextureIdx);



	Safe_Release(pEffect);

	return NOERROR;
}

CFadeOutUI * CFadeOutUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFadeOutUI*		pInstance = new CFadeOutUI(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CUI_Test Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFadeOutUI::Clone_GameObject()
{
	CFadeOutUI*		pInstance = new CFadeOutUI(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CFadeOutUI Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFadeOutUI::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
