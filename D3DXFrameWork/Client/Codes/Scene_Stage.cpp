#include "stdafx.h"
#include "..\Headers\Scene_Stage.h"
#include "Component_Manager.h"
#include "Scene_Loading.h"
#include "Management.h"
#include "Loading.h"
#include "Object_Manager.h"
#include "FadeOutUI.h"

_USING(Client)

CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{

}

HRESULT CScene_Stage::Ready_Scene()
{


	return NOERROR;
}

_int CScene_Stage::Update_Scene(const _float & fTimeDelta)
{
	CInput_Device*	pInput_Device = Get_Input_Device();
	if (nullptr == pInput_Device)
		return -1;

	if (pInput_Device->Get_DIKeyState(DIK_L) & 0x80)
	{
		m_isButton = true;
		CFadeOutUI* pFadeOut = (CFadeOutUI*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_UI_FadeOut", 0);
		pFadeOut->Set_EndStage();
	}
	if (true == m_isButton)
	{
		CFadeOutUI* pFadeOut = (CFadeOutUI*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_UI_FadeOut", 0);
		if (true == pFadeOut->Get_EndFadeOut())
			m_isNext = true;
	}
	if (true == m_isNext)
	{
		CScene*		pNewScene = CScene_Loading::Create(Get_Graphic_Device(), SCENE_FIELD);
		if (nullptr == pNewScene)
			return -1;

		if (FAILED(CManagement::GetInstance()->SetUp_CurrentScene(pNewScene)))
			return -1;
		return 0;
	}

	if (pInput_Device->Get_DIKeyState(DIK_K) & 0x80)
	{
		CScene*		pNewScene = CScene_Loading::Create(Get_Graphic_Device(), SCENE_AIRPLANE);
		if (nullptr == pNewScene)
			return -1;

		if (FAILED(CManagement::GetInstance()->SetUp_CurrentScene(pNewScene)))
			return -1;
		return 0;
	}

	m_fTimeAcc += fTimeDelta;

	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Stage::LastUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LastUpdate_Scene(fTimeDelta);
}

void CScene_Stage::Render_Scene()
{
	++m_dwRenderCnt;

	if (1.f <= m_fTimeAcc)
	{
		wsprintf(m_szFPS, L"fps:%d", m_dwRenderCnt);
		m_dwRenderCnt = 0;
		m_fTimeAcc = 0.f;
	}
	SetWindowText(g_hWnd, m_szFPS);
}


CScene_Stage * CScene_Stage::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Stage*		pInstance = new CScene_Stage(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_Stage Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CScene_Stage::Free()
{
	CScene::Free();
}
