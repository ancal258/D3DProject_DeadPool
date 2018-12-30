#include "stdafx.h"
#include "..\Headers\StaticUI.h"
#include "Component_Manager.h"
#include "Font_Manager.h"
#include "Input_Device.h"
#include "Player.h"
#include "Airplane.h"
#include "Object_Manager.h"
_USING(Client)

CStaticUI::CStaticUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CStaticUI::CStaticUI(const CStaticUI & rhs)
	: CGameObject(rhs)
	, m_dwTextureIdx(rhs.m_dwTextureIdx)
{

}

void CStaticUI::Set_Info(_vec2 vPos, _vec2 vScale)
{
	m_fX = vPos.x;
	m_fY = vPos.y;

	m_fSizeX = vScale.x;
	m_fSizeY = vScale.y;

}

void CStaticUI::Set_Info(_vec2 vPos, _vec2 vScale, _float fDegree)
{
	m_fX = vPos.x;
	m_fY = vPos.y;

	m_fSizeX = vScale.x;
	m_fSizeY = vScale.y;

	m_pTransformCom->Set_AngleZ(D3DXToRadian(fDegree));
}

void CStaticUI::Set_Mission(const _tchar * pMission)
{
	int a = m_dwTextureIdx;
	lstrcpy(m_szMission, pMission);
}

HRESULT CStaticUI::Ready_GameObject_Prototype(_uint iKind)
{
	m_dwTextureIdx = iKind;

	return NOERROR;
}

HRESULT CStaticUI::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	if (0 == m_dwTextureIdx || 1 == m_dwTextureIdx || 12 == m_dwTextureIdx)
	{
		m_pPlayer = (CPlayer*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);
		if (nullptr == m_pPlayer)
			return E_FAIL;
	}
	if (13 == m_dwTextureIdx)
	{
		m_pAirplane = (CAirplane*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Airplane", 0);
		if (nullptr == m_pAirplane)
			return E_FAIL;

		m_dwTextureIdx = 2;
		m_dwSaveIdx = 13;
	}
	return NOERROR;
}

_int CStaticUI::Update_GameObject(const _float & fTimeDelta)
{

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_fX - (g_iBackCX >> 1)
		, (g_iBackCY >> 1) - m_fY, 0.f));

	return _int();
}

_int CStaticUI::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Update_Matrix();

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		m_isLoad = true;
	}
	

	if (true == m_pRendererCom->Get_Trigger())
	{
		if(true == m_isLoad)
			if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_UI, this)))
				return -1;
	}
	return _int();
}

void CStaticUI::Render_GameObject()
{
	if (nullptr == m_pBufferCom ||
		nullptr == m_pShaderCom)
		return;

	// 2번 인덱스일 경우 (13번인덱스일경우도 2이기 때문에 Save도 검사)
	if (2 == m_dwTextureIdx && 0 == m_dwSaveIdx)
	{
		if (0 == lstrlen(m_szMission))
			return;
	}


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

	Render_StaticText();

	Safe_Release(pEffect);
}

void CStaticUI::Render_StaticText()
{


	// DP Point inkStroke Text
	if (1 == m_dwTextureIdx)
	{
		_matrix	   matTransform, matScale, matTranslate;
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 1.f);
		D3DXMatrixTranslation(&matTranslate, m_fX - 123 /*(m_fSizeX * 0.8f)*/, m_fY - 10, 0.f);
		matTransform = matScale * matTranslate;
		_tchar m_szDP_Point[MAX_PATH] = L"";
		wsprintf(m_szDP_Point, L"DP Point : %d", m_pPlayer->Get_DP_Point());
		CFont_Manager::GetInstance()->Render_Font(L"Font_Number", m_szDP_Point , D3DXCOLOR(1.f, 0.54f, 0.f, 1.f), &matTransform);
	}
	// HeadShotPoint inkStroke Text
	else if (0 == m_dwTextureIdx)
	{
		_matrix	   matTransform, matScale, matTranslate;
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 1.f);
		D3DXMatrixTranslation(&matTranslate, m_fX - 155 /*(m_fSizeX * 0.8f)*/, m_fY - 10, 0.f);
		matTransform = matScale * matTranslate;
		_tchar m_szDP_HeadShotPoint[MAX_PATH] = L"";
		wsprintf(m_szDP_HeadShotPoint, L"HeadShot Point : %d", m_pPlayer->Get_HeadSHot_Point());
		CFont_Manager::GetInstance()->Render_Font(L"Font_Number", m_szDP_HeadShotPoint, D3DXCOLOR(1.f, 0.54f, 0.f, 1.f), &matTransform);
	}
	else if (12 == m_dwTextureIdx)
	{
		_matrix	   matTransform, matScale, matTranslate;
		D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 1.f);
		D3DXMatrixTranslation(&matTranslate, m_fX - 110 /*(m_fSizeX * 0.8f)*/, m_fY - 30, 0.f);
		matTransform = matScale * matTranslate;
		_tchar m_szDP_BulletCnt[MAX_PATH] = L"";
		wsprintf(m_szDP_BulletCnt, L"%d", m_pPlayer->Get_BulletCnt());
		CFont_Manager::GetInstance()->Render_Font(L"Font_Number", m_szDP_BulletCnt, D3DXCOLOR(1.f, 0.54f, 0.f, 1.f), &matTransform);
	}
	else if (2 == m_dwTextureIdx)
	{
		_matrix	   matTransform, matScale, matTranslate;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTranslate, (g_iBackCX >> 1) - 150, 80, 0.f);
		matTransform = matScale * matTranslate;
		CFont_Manager::GetInstance()->Render_Font(L"Font_Number", m_szMission, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), &matTransform);
		if (13 == m_dwSaveIdx)
		{
			_matrix	   matTransform, matScale, matTranslate;
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTranslate, (g_iBackCX >> 1) - 150, 80, 0.f);
			matTransform = matScale * matTranslate;
			_tchar m_szDP_BulletCnt[MAX_PATH] = L"";
			wsprintf(m_szDP_BulletCnt, L"Destroy Point : %d", m_pAirplane->Get_DestroyPoint());
			CFont_Manager::GetInstance()->Render_Font(L"Font_Number", m_szDP_BulletCnt, D3DXCOLOR(1.f, 0.54f, 0.f, 1.f), &matTransform);
		}
	}

}

HRESULT CStaticUI::Ready_Component()
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
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_UI_StaticUI");

	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;


	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CStaticUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

CStaticUI * CStaticUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iKind)
{
	CStaticUI*		pInstance = new CStaticUI(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype(iKind)))
	{
		_MSG_BOX("Prototype_CUI_Test Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStaticUI::Clone_GameObject()
{
	CStaticUI*		pInstance = new CStaticUI(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CStaticUI Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticUI::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
