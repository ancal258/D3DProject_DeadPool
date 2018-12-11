#include "stdafx.h"
#include "..\Headers\Scene_AirPlane.h"
#include "Component_Manager.h"



_USING(Client)

CScene_AirPlane::CScene_AirPlane(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{

}

HRESULT CScene_AirPlane::Ready_Scene()
{


	return NOERROR;
}

_int CScene_AirPlane::Update_Scene(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_AirPlane::LastUpdate_Scene(const _float & fTimeDelta)
{
	if (FAILED(CInput_Device::GetInstance()->Update_MinDist()))
		return E_FAIL;

	return CScene::LastUpdate_Scene(fTimeDelta);
}

void CScene_AirPlane::Render_Scene()
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


CScene_AirPlane * CScene_AirPlane::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_AirPlane*		pInstance = new CScene_AirPlane(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_AirPlane Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CScene_AirPlane::Free()
{
	if (FAILED(CObject_Manager::GetInstance()->Clear_Object(SCENE_STAGE)))
		return;

	if (FAILED(CComponent_Manager::GetInstance()->Clear_Component(SCENE_STAGE)))
		return;

	CScene::Free();
}
