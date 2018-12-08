#include "stdafx.h"
#include "..\Headers\Scene_Logo.h"
#include "Component_Manager.h"
#include "Back_Logo.h"
#include "Scene_Loading.h"
#include "Management.h"
#include "Loading.h"

// UI
#include "UI_Test.h"
_USING(Client)

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{

}

HRESULT CScene_Logo::Ready_Scene()
{

	if (FAILED(Ready_Logo_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Logo_Prototype_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;


	return NOERROR;
}

_int CScene_Logo::Update_Scene(const _float & fTimeDelta)
{
	CInput_Device*	pInput_Device = Get_Input_Device();

	if (nullptr == pInput_Device)
		return -1;

	if (pInput_Device->Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		CScene*		pNewScene = CScene_Loading::Create(Get_Graphic_Device(),SCENE_STAGE);
		if (nullptr == pNewScene)
			return -1;

		if (FAILED(CManagement::GetInstance()->SetUp_CurrentScene(pNewScene)))
			return -1;

		return 0;		
	}


	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Logo::LastUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LastUpdate_Scene(fTimeDelta);
}

void CScene_Logo::Render_Scene()
{

}

HRESULT CScene_Logo::Ready_Logo_Prototype_Component()
{
	CComponent_Manager*		pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;
	pComponent_Manager->AddRef();

	// For.Component_Shader_Logo
	if (FAILED(pComponent_Manager->Add_Component(SCENE_LOGO, L"Component_Shader_Logo", CShader::Create(Get_Graphic_Device(), L"../Bin/ShaderFiles/Shader_Logo.fx"))))
		return E_FAIL;

	// For.Component_Texture_Logo
	if (FAILED(pComponent_Manager->Add_Component(SCENE_LOGO, L"Component_Texture_Logo", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Logo/Logo.png"))))
		return E_FAIL;
	// For.Component_Texture_Filter
	if (FAILED(pComponent_Manager->Add_Component(SCENE_LOGO, L"Component_Texture_UI_Test", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/Test.tga"))))
		return E_FAIL;
	Safe_Release(pComponent_Manager);
	
	return NOERROR;
}

HRESULT CScene_Logo::Ready_Logo_Prototype_GameObject()
{
	// For.GameObject_BackLogo
	if (FAILED(Add_Object_Prototype(SCENE_LOGO, L"GameObject_BackLogo", CBack_Logo::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.GameObject_BackLogo
	if (FAILED(Add_Object_Prototype(SCENE_LOGO, L"GameObject_UI_Test", CUI_Test::Create(Get_Graphic_Device()))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Logo::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	// For.BackGround
	if (FAILED(Add_Object(SCENE_LOGO, L"GameObject_BackLogo", SCENE_LOGO, pLayerTag)))
		return E_FAIL;
	// For.BackGround
	if (FAILED(Add_Object(SCENE_LOGO, L"GameObject_UI_Test", SCENE_LOGO, L"Layer_UI")))
		return E_FAIL;

	return NOERROR;
}

CScene_Logo * CScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Logo*		pInstance = new CScene_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CScene_Logo::Free()
{

	CScene::Free();
}
