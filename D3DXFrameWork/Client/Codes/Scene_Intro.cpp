#include "stdafx.h"
#include "..\Headers\Scene_Intro.h"
#include "Component_Manager.h"
#include "Back_Logo.h"
#include "Scene_Logo.h"
#include "Management.h"
#include "Loading.h"

_USING(Client)

CScene_Intro::CScene_Intro(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{

}

HRESULT CScene_Intro::Ready_Scene()
{

	//m_hVideo = MCIWndCreate(g_hWnd, 0, WS_VISIBLE | WS_CHILD | MCIWNDF_NOPLAYBAR, L"../Bin/Resources/Video/Marvel Intro HD.wmv");
	//if (0 == m_hVideo)
	//	return E_FAIL;

	//MoveWindow(m_hVideo, 0, 0, g_iBackCX, g_iBackCY, FALSE);
	//SetWindowPos(m_hVideo, HWND_TOPMOST, 0, 0, g_iBackCX, g_iBackCY, SWP_NOZORDER);

	//MCIWndPlay(m_hVideo);


	return NOERROR;

}

_int CScene_Intro::Update_Scene(const _float & fTimeDelta)
{



	return 0;

	//return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Intro::LastUpdate_Scene(const _float & fTimeDelta)
{


	if (SCENE_LOGO == m_iSceneID)
	{
		MCIWndDestroy(m_hVideo);
		CScene*		pNewScene = CScene_Logo::Create(Get_Graphic_Device());
		if (nullptr == pNewScene)
			return -1;

		if (FAILED(CManagement::GetInstance()->SetUp_CurrentScene(pNewScene)))
			return -1;

		return 0;
	}
	CInput_Device*	pInput_Device = Get_Input_Device();
	pInput_Device->AddRef();

	if (nullptr == pInput_Device)
		return -1;

	if (pInput_Device->Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		m_iSceneID = SCENE_LOGO;
	}

	Safe_Release(pInput_Device);
	return 0;
	//return CScene::LastUpdate_Scene(fTimeDelta);
}

void CScene_Intro::Render_Scene()
{

}

CScene_Intro * CScene_Intro::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Intro*		pInstance = new CScene_Intro(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_Intro Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CScene_Intro::Free()
{

	CScene::Free();
}
