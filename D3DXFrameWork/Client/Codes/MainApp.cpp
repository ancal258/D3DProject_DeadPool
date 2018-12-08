#include "stdafx.h"
#include "..\Headers\MainApp.h"
#include "Management.h"
#include "Input_Device.h"
#include "Component_Manager.h"
#include "Scene_Logo.h"
#include "Scene_Intro.h"
#include "Camera_Debug.h"
#include "Camera_Target.h"
#include "Camera_Cinematic.h"
#include "Test.h"


_USING(Client)

CMainApp::CMainApp()
	: m_pManagement(CManagement::GetInstance())
{
	m_pManagement->AddRef();
}

HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(Ready_Default_Setting(CGraphic_Device::MODE_WIN, g_iBackCX, g_iBackCY)))
		return E_FAIL;	

	if (FAILED(Ready_Static_Component()))
		return E_FAIL;

	if (FAILED(Ready_Static_GameObject()))
		return E_FAIL;	

	if (FAILED(Ready_Start_Scene(SCENE_LOGO)))
		return E_FAIL;
	//if (FAILED(Ready_Gara()))
	//	return E_FAIL;

	return NOERROR;
}

_int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	if (nullptr == m_pManagement)
		return -1;

	CInput_Device::GetInstance()->Inquire_Input_State();	
	if(nullptr != m_pScene)
		m_iSceneID = ((CScene_Intro*)m_pScene)->Get_SceneID();

	return m_pManagement->Update_Management(fTimeDelta);
}

void CMainApp::Render_MainApp()
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pRenderer)
		return;
	if (SCENE_INTRO != m_iSceneID)
	{
		m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
		m_pGraphic_Device->BeginScene();

		// 진짜 객체들을 출력.
		m_pRenderer->Render_Renderer();


		// 각씬에서 필요한 디버깅적 요소정도만 출력.
		m_pManagement->Render_Management();

		m_pGraphic_Device->EndScene();
		m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);
	}


}

HRESULT CMainApp::Ready_Default_Setting(CGraphic_Device::WINMODE eMode, const _uint& iBackCX, const _uint& iBackCY)
{
	// For.Graphic_Device
	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(g_hWnd, eMode, iBackCX, iBackCY, &m_pGraphic_Device)))
		return E_FAIL;

	// For.Input_Device
	if (FAILED(CInput_Device::GetInstance()->Ready_Input_Device(g_hInst, g_hWnd)))
		return E_FAIL;
	
	// For.Management
	if (nullptr == m_pManagement)
		return E_FAIL;

	if (FAILED(m_pManagement->Ready_Management(SCENE_END)))
		return E_FAIL;

	


	return NOERROR;
}

HRESULT CMainApp::Ready_Static_Component()
{
	CComponent_Manager*	pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Component_Buffer_RcTex
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RcTex", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Buffer_RcCol
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Buffer_RcCol", CBuffer_RcCol::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Renderer	
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_pRenderer->AddRef();
	// For.Component_Transform
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Component_Frustum
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Frustum", CFrustum::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Component_Shader_UI
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STATIC, L"Component_Shader_UI", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_UI.fx"))))
		return E_FAIL;

	Safe_Release(pComponent_Manager);	

	/*m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);*/


	return NOERROR;
}

HRESULT CMainApp::Ready_Static_GameObject()
{
	// For.GameObject_Camera_Debug
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(SCENE_STATIC, L"GameObject_Camera_Debug", CCamera_Debug::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.GameObject_Camera_Target
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(SCENE_STATIC, L"GameObject_Camera_Target", CCamera_Target::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.GameObject_Camera_Cinematic
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(SCENE_STATIC, L"GameObject_Camera_Cinematic", CCamera_Cinematic::Create(m_pGraphic_Device))))
		return E_FAIL;
	//// For.GameObject_Test
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(SCENE_STATIC, L"GameObject_Test", CTest::Create(m_pGraphic_Device))))
		return E_FAIL;



	return NOERROR;
}

HRESULT CMainApp::Ready_Start_Scene(SCENEID eID)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	CScene*			pScene = nullptr;
	m_iSceneID = eID;
	switch (eID)
	{
	case SCENE_LOGO:
		m_pScene = CScene_Logo::Create(m_pGraphic_Device);
		break;
	case SCENE_INTRO:
		m_pScene = CScene_Intro::Create(m_pGraphic_Device);
		break;
	}

	if (nullptr == m_pScene)
		return E_FAIL;

	if (FAILED(m_pManagement->SetUp_CurrentScene(m_pScene)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainApp::Ready_Gara()
{
	HANDLE			hFile = 0;
	_ulong			dwByte = 0;

	hFile = CreateFile(L"../Bin/DataFiles/Navigation.dat", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	_ulong		dwNumCell = 4;

	WriteFile(hFile, &dwNumCell, sizeof(_ulong), &dwByte, nullptr);

	_vec3			vPoint[3];

	vPoint[0] = _vec3(0.0f, 0.f, 7.f);
	vPoint[1] = _vec3(7.f, 0.f, 0.f);
	vPoint[2] = _vec3(0.0f, 0.f, 0.f);
	WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);

	vPoint[0] = _vec3(0.0f, 0.f, 7.f);
	vPoint[1] = _vec3(7.f, 0.f, 7.f);
	vPoint[2] = _vec3(7.f, 0.f, 0.f);
	WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);

	vPoint[0] = _vec3(0.0f, 0.f, 14.f);
	vPoint[1] = _vec3(7.f, 0.f, 7.f);
	vPoint[2] = _vec3(0.0f, 0.f, 7.f);
	WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);

	vPoint[0] = _vec3(7.f, 0.f, 7.f);
	vPoint[1] = _vec3(14.0f, 0.f, 0.f);
	vPoint[2] = _vec3(7.f, 0.f, 0.f);
	WriteFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);

	return NOERROR;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Ready_MainApp()))
	{
		_MSG_BOX("CMainApp Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{	
	Safe_Release(m_pGraphic_Device);

	Safe_Release(m_pManagement);

	Safe_Release(m_pRenderer);
	
	CManagement::Release_Engine();	
}
