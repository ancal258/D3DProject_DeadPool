#include "stdafx.h"
#include "..\Headers\Scene_Loading.h"
#include "Component_Manager.h"
#include "Back_Loading.h"
#include "Scene_Stage.h"
#include "Scene_Field.h"
#include "Scene_AirPlane.h"
#include "Management.h"
#include "Loading.h"
#include "Font_Manager.h"

// UI
#include "UI_Test.h"
_USING(Client)

CScene_Loading::CScene_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{

}

HRESULT CScene_Loading::Ready_Scene(_uint iSceneNum)
{
	if (iSceneNum == SCENE_STAGE)
	{

		if (FAILED(CObject_Manager::GetInstance()->Clear_Object(SCENE_LOGO)))
			return E_FAIL;

		if (FAILED(CComponent_Manager::GetInstance()->Clear_Component(SCENE_LOGO)))
			return E_FAIL;

		m_pLoading = CLoading::Create(Get_Graphic_Device(), SCENE_STAGE);
		if (nullptr == m_pLoading)
			return E_FAIL;
	}
	if (iSceneNum == SCENE_FIELD)
	{
		if (FAILED(CObject_Manager::GetInstance()->Clear_Object(SCENE_STAGE)))
			return E_FAIL;

		if (FAILED(CComponent_Manager::GetInstance()->Clear_Component(SCENE_STAGE)))
			return E_FAIL;

		m_pLoading = CLoading::Create(Get_Graphic_Device(), SCENE_FIELD);
		if (nullptr == m_pLoading)
			return E_FAIL;
	}

	if (iSceneNum == SCENE_AIRPLANE)
	{
		if (FAILED(CObject_Manager::GetInstance()->Clear_Object(SCENE_STAGE)))
			return E_FAIL;

		if (FAILED(CComponent_Manager::GetInstance()->Clear_Component(SCENE_STAGE)))
			return E_FAIL;

		m_pLoading = CLoading::Create(Get_Graphic_Device(), SCENE_AIRPLANE);
		if (nullptr == m_pLoading)
			return E_FAIL;
	}


	if (FAILED(Ready_Logo_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Logo_Prototype_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;


	m_iSceneNum = iSceneNum;

	return NOERROR;
}

_int CScene_Loading::Update_Scene(const _float & fTimeDelta)
{
	CInput_Device*	pInput_Device = Get_Input_Device();

	if (nullptr == pInput_Device)
		return -1;


	if (pInput_Device->Get_DIKeyState(DIK_RETURN) & 0x80)
	{


		if (false == m_pLoading->Get_Finish())
			return 0;



		if (SCENE_STAGE == m_iSceneNum)
		{
			CScene*		pNewScene = CScene_Stage::Create(Get_Graphic_Device());
			if (nullptr == pNewScene)
				return -1;
			if (FAILED(CManagement::GetInstance()->SetUp_CurrentScene(pNewScene)))
				return -1;
		}

		if (SCENE_FIELD == m_iSceneNum)
		{
			CScene*		pNewScene = CScene_Field::Create(Get_Graphic_Device());
			if (nullptr == pNewScene)
				return -1;
			if (FAILED(CManagement::GetInstance()->SetUp_CurrentScene(pNewScene)))
				return -1;
		}

		if (SCENE_AIRPLANE == m_iSceneNum)
		{
			CScene*		pNewScene = CScene_AirPlane::Create(Get_Graphic_Device());
			if (nullptr == pNewScene)
				return -1;
			if (FAILED(CManagement::GetInstance()->SetUp_CurrentScene(pNewScene)))
				return -1;
		}


		return 0;
	}


	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Loading::LastUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LastUpdate_Scene(fTimeDelta);
}

void CScene_Loading::Render_Scene()
{
	_tchar		szLoading[128] = L"";
	lstrcpy(szLoading, m_pLoading->Get_String());
	if (lstrlen(szLoading) > 1)
	{
		_matrix	   matTransform, matScale, matTranslate;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 2.f);
		D3DXMatrixTranslation(&matTranslate, 100, (g_iBackCY)-50, 0.f);
		matTransform = matScale * matTranslate;
		CFont_Manager::GetInstance()->Render_Font(L"Font_Magic", szLoading, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), &matTransform);
	}
}

HRESULT CScene_Loading::Ready_Logo_Prototype_Component()
{
	CComponent_Manager*		pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;
	pComponent_Manager->AddRef();

	// For.Component_Shader_Logo
	if (FAILED(pComponent_Manager->Add_Component(SCENE_LOGO, L"Component_Shader_Logo", CShader::Create(Get_Graphic_Device(), L"../Bin/ShaderFiles/Shader_Logo.fx"))))
		return E_FAIL;

	// For.Component_Texture_Logo
	if (FAILED(pComponent_Manager->Add_Component(SCENE_LOGO, L"Component_Texture_BackGround", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/BackGround/BackGround.tga"))))
		return E_FAIL;

	// For.Component_Texture_Filter
	if (FAILED(pComponent_Manager->Add_Component(SCENE_LOGO, L"Component_Texture_UI_Test", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/Test.tga"))))
		return E_FAIL;
	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CScene_Loading::Ready_Logo_Prototype_GameObject()
{
	// For.GameObject_BackLogo
	if (FAILED(Add_Object_Prototype(SCENE_LOGO, L"GameObject_BackLogo", CBack_Loading::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.GameObject_BackLogo
	if (FAILED(Add_Object_Prototype(SCENE_LOGO, L"GameObject_UI_Test", CUI_Test::Create(Get_Graphic_Device()))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Loading::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	// For.BackGround
	if (FAILED(Add_Object(SCENE_LOGO, L"GameObject_BackLogo", SCENE_LOGO, pLayerTag)))
		return E_FAIL;
	//// For.TestUI
	//if (FAILED(Add_Object(SCENE_LOGO, L"GameObject_UI_Test", SCENE_LOGO, L"Layer_UI")))
	//	return E_FAIL;

	return NOERROR;
}

CScene_Loading * CScene_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device,_uint iSceneNum)
{
	CScene_Loading*		pInstance = new CScene_Loading(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene(iSceneNum)))
	{
		_MSG_BOX("CScene_Loading Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CScene_Loading::Free()
{
	Safe_Release(m_pLoading);

	if (FAILED(CObject_Manager::GetInstance()->Clear_Object(SCENE_LOGO)))
		return;

	if (FAILED(CComponent_Manager::GetInstance()->Clear_Component(SCENE_LOGO)))
		return;

	CScene::Free();
}
