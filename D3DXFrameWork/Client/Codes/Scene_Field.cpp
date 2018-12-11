#include "stdafx.h"
#include "..\Headers\Scene_Field.h"
#include "Component_Manager.h"
#include "Scene_Loading.h"
#include "Management.h"


_USING(Client)

CScene_Field::CScene_Field(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{

}

HRESULT CScene_Field::Ready_Scene()
{


	return NOERROR;
}

_int CScene_Field::Update_Scene(const _float & fTimeDelta)
{
	CInput_Device*	pInput_Device = Get_Input_Device();
	if (nullptr == pInput_Device)
		return -1;

	if (pInput_Device->Get_DIKeyState(DIK_L) & 0x80)
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

_int CScene_Field::LastUpdate_Scene(const _float & fTimeDelta)
{
	if (FAILED(CInput_Device::GetInstance()->Update_MinDist()))
		return E_FAIL;

	return CScene::LastUpdate_Scene(fTimeDelta);
}

void CScene_Field::Render_Scene()
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


CScene_Field * CScene_Field::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Field*		pInstance = new CScene_Field(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_Field Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CScene_Field::Free()
{

	CScene::Free();
}
