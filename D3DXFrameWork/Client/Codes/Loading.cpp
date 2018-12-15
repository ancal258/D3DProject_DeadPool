#include "stdafx.h"
#include "..\Headers\Loading.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Management.h"
#include "Scene_Field.h"
#include "Camera_Debug.h"
#include "Camera_Cinematic.h"
#include "Camera_Minigun.h"
// GameObject
#include "Terrain.h"
#include "Player.h"
#include "DP_Sword.h"
#include "DP_Gun.h"
#include "Static_Object.h"
#include "Event_Cube.h"
#include "Effect.h"
#include "Dog.h"
#include "Brawler01.h"
#include "Brawler02.h"
#include "Brawler_Knife.h"
#include "Brawler_ElectricBaton.h"
#include "SkyDom.h"
#include "Airplane.h"
#include "Minigun.h"
#include "Prop.h"
#include "Static_Airplane.h"

//Effect
#include "Effect_Mesh.h"
#include "Effect_RectParticle.h"
// UI
#include "UI_CrossHair.h"
#include "UI_HPBar.h"
#include "TalkBox.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphic_Device(pGraphicDev)
	,m_pObject_Manager(CObject_Manager::GetInstance())
{
}

HRESULT CLoading::Add_Object_Prototype(const _uint & iSceneID, const _tchar * pProtoTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Object_Prototype(iSceneID, pProtoTag, pPrototype);
}

HRESULT CLoading::Add_Object(const _uint & iPSceneID, const _tchar * pProtoTag, const _uint & iSceneID, const _tchar * pLayerTag, CGameObject ** ppGameObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Object(iPSceneID, pProtoTag, iSceneID, pLayerTag, ppGameObject);
}

HRESULT CLoading::Ready_Loading(SCENEID eSceneID)
{
	m_eSceneID = eSceneID;
	m_isFinish = false;

	InitializeCriticalSection(&m_Critical_Section);

	// 내 코드를 읽어나가기위한 새로운 흐름을 생성한다.
	m_hThread = (HANDLE)_beginthreadex(nullptr, 1, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return NOERROR;
}



HRESULT CLoading::Loading_Stage_APT()
{
	//SetWindowText(g_hWnd, L"Loading...");

	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	if (FAILED(Ready_Static_Prototype_Component()))
		return E_FAIL;
	lstrcpy(m_szString, L"APT_Mesh_Load...");
	if (FAILED(Ready_Componet_Prototype_SceneAPT()))
		return E_FAIL;

	lstrcpy(m_szString, L"APT_Prototype...");
	if (FAILED(Ready_Stage_Prototype_GameObject_SceneAPT()))
		return E_FAIL;
	lstrcpy(m_szString, L"Layer_BackGround...");
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;
	lstrcpy(m_szString, L"Layer_Player...");
	if (FAILED(Ready_Layer_Player(L"Layer_Player")))
		return E_FAIL;
	lstrcpy(m_szString, L"Layer_Camera...");
	if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Debug", L"Layer_Camera",0.25f)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Target", L"Layer_Camera", 0.25f)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Cinematic", L"Layer_Camera",0.25f)))
		return E_FAIL;
	SetUp_CameraMove();

	lstrcpy(m_szString, L"Layer_StaticUI...");
	if (FAILED(Ready_Static_Prototype_UI()))
		return E_FAIL;

	lstrcpy(m_szString, L"Layer_UI...");
	if (FAILED(Ready_UI_SceneAPT()))
		return E_FAIL;

	if (FAILED(Load_Static_Object(L"../Bin/DataFiles/StaticO11bject1.dat")))
		return E_FAIL;

	//현재 테스트 케이스
	if (FAILED(Load_Event_Cube(L"../Bin/DataFiles/EventCube_APT.dat")))
		return E_FAIL;

	//Dog
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Dog", SCENE_STAGE, L"Layer_Dog")))
		return E_FAIL;

	////TEST////
	if (FAILED(Ready_Effect()))
		return E_FAIL;


	// For.Add_EffectSwordHeavy03R
	CGameObject* pUI;
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_UI_TalkBox_White", SCENE_STAGE, L"Layer_UI", &pUI)))
		return E_FAIL;
	((CTalkBox*)pUI)->Set_Info(_vec2((g_iBackCX >> 1) - 200, 200), L"Hi my friend~");
	 

	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_UI_TalkBox_Orange", SCENE_STAGE, L"Layer_UI", &pUI)))
		return E_FAIL;
	((CTalkBox*)pUI)->Set_Info(_vec2((g_iBackCX >> 1) + 200, 400), L"fxxk your self");

	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_UI_TalkBox_Orange", SCENE_STAGE, L"Layer_UI", &pUI)))
		return E_FAIL;
	((CTalkBox*)pUI)->Set_Info(_vec2((g_iBackCX >> 1) + 200, 600), L"This TalkBox has auto length function");

	////////////////

	//for (size_t i = 0; i < 10; i++)
	//{
	//	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Effect", SCENE_STAGE, L"Layer_Effect")))
	//		return E_FAIL;
	//}

	//MCIWndClose(m_hVideo);
	m_isFinish = true;
	lstrcpy(m_szString, L"Loading Complete");

	//SetWindowText(g_hWnd, L"Complete");

	return NOERROR;
}

HRESULT CLoading::Loading_Stage_FIELD()
{
	if (FAILED(CLight_Manager::GetInstance()->Clear_Light()))
		return E_FAIL;

	if (FAILED(Ready_LightInfo_FIELD()))
		return E_FAIL;
	if (FAILED(Ready_Static_Prototype_Component()))
		return E_FAIL;
	lstrcpy(m_szString, L"Field_Mesh_Load...");
	if (FAILED(Ready_Component_Prototype_SceneFIELD()))
		return E_FAIL;
	lstrcpy(m_szString, L"Field_Prototype...");
	if (FAILED(Ready_Stage_Prototype_GameObject_SceneFIELD()))
		return E_FAIL;
	lstrcpy(m_szString, L"Layer_BackGround...");
	if (FAILED(Ready_Layer_BackGround_FIELD(L"Layer_BackGround")))
		return E_FAIL;
	lstrcpy(m_szString, L"Layer_Player...");
	if (FAILED(Ready_Layer_Player_FIELD(L"Layer_Player")))
		return E_FAIL;
	lstrcpy(m_szString, L"Layer_Camera...");
	if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Debug", L"Layer_Camera", 0.25f)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Target", L"Layer_Camera", 0.25f)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Cinematic", L"Layer_Camera", 0.25f)))
		return E_FAIL;
	SetUp_CameraMove();

	lstrcpy(m_szString, L"Layer_StaticUI...");
	if (FAILED(Ready_Static_Prototype_UI()))
		return E_FAIL;

	lstrcpy(m_szString, L"Layer_UI...");
	if (FAILED(Ready_UI_SceneFIELD()))
		return E_FAIL;

	lstrcpy(m_szString, L"Layer_Object...");
	if (FAILED(Ready_Layer_Object()))
		return E_FAIL;

	if (FAILED(Load_Static_Object(L"../Bin/DataFiles/StaticObject_Field.dat")))
		return E_FAIL;

	m_isFinish = true;
	lstrcpy(m_szString, L"Loading Complete");

	return NOERROR;
}

HRESULT CLoading::Loading_Stage_AIRPLANE()
{
	if (FAILED(CLight_Manager::GetInstance()->Clear_Light()))
		return E_FAIL;

	if (FAILED(Ready_LightInfo_AIRPLANE()))
		return E_FAIL;
	if (FAILED(Ready_Static_Prototype_Component()))
		return E_FAIL;
	lstrcpy(m_szString, L"Field_Mesh_Load...");
	if (FAILED(Ready_Component_Prototype_SceneFIELD()))
		return E_FAIL;
	lstrcpy(m_szString, L"Field_Prototype...");
	if (FAILED(Ready_Stage_Prototype_GameObject_SceneFIELD()))
		return E_FAIL;
	lstrcpy(m_szString, L"Layer_BackGround...");
	if (FAILED(Ready_Layer_BackGround_AIRPLANE(L"Layer_BackGround")))
		return E_FAIL;
	//lstrcpy(m_szString, L"Layer_Player...");
	//if (FAILED(Ready_Layer_Player_FIELD(L"Layer_Player")))
	//	return E_FAIL;

	lstrcpy(m_szString, L"Layer_Airplane...");
	if (FAILED(Ready_AIRPLANE()))
		return E_FAIL;

	lstrcpy(m_szString, L"Layer_Camera...");
	if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Debug", L"Layer_Camera", 0.25f)))
		return E_FAIL;

	lstrcpy(m_szString, L"Layer_Camera...");
	if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Minigun", L"Layer_Camera", 0.1f)))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Target", L"Layer_Camera")))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Cinematic", L"Layer_Camera")))
	//	return E_FAIL;
	//SetUp_CameraMove();

	lstrcpy(m_szString, L"Layer_StaticUI...");
	if (FAILED(Ready_Static_Prototype_UI()))
		return E_FAIL;

	lstrcpy(m_szString, L"Layer_UI...");
	if (FAILED(Ready_UI_SceneAIRPLANE()))
		return E_FAIL;
	/////////////////////

	if (FAILED(Load_Static_Object(L"../Bin/DataFiles/StaticObject_Airplane.dat")))
		return E_FAIL;

	/////////////////////


	m_isFinish = true;
	lstrcpy(m_szString, L"Loading Complete");

	return NOERROR;
}

HRESULT CLoading::Ready_LightInfo()
{
	D3DLIGHT9			LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	// For. Player
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(0.7, 0.7f, 0.7f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	LightInfo.Position = _vec3(8.f, 4.f, 13.2f);
	LightInfo.Range = 6.0f;

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	// For. Directional
	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;
	// For. Chair Lamp
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.5f, 0.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 0.5f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.25f, 0.0f, 1.f);
	LightInfo.Position = _vec3(8.f, 4.f, 13.2f);
	LightInfo.Range = 5.0f;

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	// For. Table Lamp
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.5f, 0.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 0.5f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.25f, 0.0f, 1.f);
	LightInfo.Position = _vec3(34.18f, 4.f, 15.5f);
	LightInfo.Range = 8.0f;

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;


	// For. Neon
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(0.f, 0.f, 0.8f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.f, 0.0f, 0.8f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.0f, 0.f, 0.4f, 1.f);
	LightInfo.Position = _vec3(25.73f, 13.f, 1.235f);
	LightInfo.Range = 5.0f;

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(0.58f, 0.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.58f, 0.0f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.29f, 0.f, 0.5f, 1.f);
	LightInfo.Position = _vec3(8.143f, 13.f, 34.57f);
	LightInfo.Range = 6.0f;

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 0.0f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.f, 0.0f, 1.f);
	LightInfo.Position = _vec3(24.75f, 13.f, 34.57f);
	LightInfo.Range = 5.0f;

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CLoading::Ready_LightInfo_FIELD()
{
	D3DLIGHT9			LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	//LightInfo.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.f);
	LightInfo.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	//LightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	// For. Wall_01
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.0f, 0.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.f);
	LightInfo.Position = _vec3(22.1f, 2.f, 10.21f);
	LightInfo.Range = 1.0f;

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	// For. Wall_02
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.0f, 0.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.f);
	LightInfo.Position = _vec3(22.1f, 2.f, 7.67f);
	LightInfo.Range = 1.0f;

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	// For. Wall_03
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.0f, 0.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.f);
	LightInfo.Position = _vec3(34.79f, 2.f, 44.09f);
	LightInfo.Range = 1.0f;

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	// For. LightBase_01
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.0f, 0.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.f);
	LightInfo.Position = _vec3(15.22f, 2.f, 1.85f);
	LightInfo.Range = 1.0f;
	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	// For. LightBase_02
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.0f, 0.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.f);
	LightInfo.Position = _vec3(18.76, 2.f, 1.85f);
	LightInfo.Range = 1.0f;
	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	// For. Lamp_BIG
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.89f, 0.52f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 0.89f, 0.52f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.45f, 0.40f, 1.f);
	LightInfo.Position = _vec3(13.20, 5.f, 25.82f);
	LightInfo.Range = 11.0f;
	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	// For. Lamp01
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.89f, 0.52f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 0.89f, 0.52f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.3f, 0.25f, 0.27f, 1.f);
	LightInfo.Position = _vec3(3.422f, 2.5f, 1.577f);
	LightInfo.Range = 3.0f;
	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	// For. Lamp02
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.89f, 0.52f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 0.89f, 0.52f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.3f, 0.25f, 0.27f, 1.f);
	LightInfo.Position = _vec3(30.62f, 2.5f, 34.86f);
	LightInfo.Range = 3.0f;
	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;


	// For. Lamp03
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.89f, 0.52f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 0.89f, 0.52f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.3f, 0.25f, 0.27f, 1.f);
	LightInfo.Position = _vec3(40.11f, 2.5f, 30.05f);
	LightInfo.Range = 3.0f;
	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;


	// For. Lamp04
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 0.89f, 0.52f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 0.89f, 0.52f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.3f, 0.25f, 0.27f, 1.f);
	LightInfo.Position = _vec3(26.3f, 2.5f, 11.05f);
	LightInfo.Range = 3.0f;
	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	// For. Emergency
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(0.9f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.3f, 0.07f, 0.07f, 1.f);
	LightInfo.Position = _vec3(42.9f, 3.f, 30.4);
	LightInfo.Range = 1.0f;
	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	// For. Emergency02
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Diffuse = D3DXCOLOR(0.9f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.3f, 0.07f, 0.07f, 1.f);
	LightInfo.Position = _vec3(46.97, 3.f, 30.4);
	LightInfo.Range = 1.0f;
	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;



	return NOERROR;
}

HRESULT CLoading::Ready_LightInfo_AIRPLANE()
{
	D3DLIGHT9			LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	//LightInfo.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.f);
	LightInfo.Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.5f);
	//LightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLoading::Ready_Effect()
{
	CComponent_Manager*		pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;
	pComponent_Manager->AddRef();
	// For. Effect_Mesh

	// For.Component_Mesh_EffectSwordHeavy03R
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_EffectSwordHeavy03R", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/Effect/Sword/", L"Sword_Heavy_03R.x"))))
		return E_FAIL;
	// For.Prototype_EffectSwordHeavy03R
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_EffectSwordHeavy03R", CEffect_Mesh::Create(Get_Graphic_Device()))))
		return E_FAIL;

	// For.Add_EffectSwordHeavy03R
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_EffectSwordHeavy03R", SCENE_STAGE, L"Layer_MeshEffect")))
		return E_FAIL;



	// For. Particle

	// For.Component_Texture_Blood
	lstrcpy(m_szString, L"../Bin/Resources/Textures/Effect/Blood/blood_4x4_1_subUV_%d.png");
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Blood", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, m_szString, 16))))
		return E_FAIL;
	// For.Prototype_EffectRectParticle
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_EffectRectParticle", CEffect_RectParticle::Create(Get_Graphic_Device()))))
		return E_FAIL;

	for (size_t i = 0; i < 10; i++)
	{
		if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_EffectRectParticle", SCENE_STAGE, L"Layer_ParticleEffect")))
			return E_FAIL;
	}
	// For.Add_EffectRectParticle
	

	Safe_Release(pComponent_Manager);
	return NOERROR;
}

HRESULT CLoading::Ready_Static_Prototype_Component()
{
	CComponent_Manager*		pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;
	pComponent_Manager->AddRef();

	// For.Component_Shader_Terrain
	lstrcpy(m_szString, L"../Bin/ShaderFiles/Shader_Terrain.fx");
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Terrain", CShader::Create(Get_Graphic_Device(), m_szString))))
		return E_FAIL;

	// For.Component_Shader_Mesh
	lstrcpy(m_szString, L"../Bin/ShaderFiles/Shader_Mesh.fx");
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", CShader::Create(Get_Graphic_Device(), m_szString))))
		return E_FAIL;
	// For.Component_Shader_Collider
	lstrcpy(m_szString, L"../Bin/ShaderFiles/Shader_Collider.fx");
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Collider", CShader::Create(Get_Graphic_Device(), m_szString))))
		return E_FAIL;
	// For.Component_Shader_Effect
	lstrcpy(m_szString, L"../Bin/ShaderFiles/Shader_Effect.fx");
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Effect", CShader::Create(Get_Graphic_Device(), m_szString))))
		return E_FAIL;
	// For.Component_Shader_Prop
	lstrcpy(m_szString, L"../Bin/ShaderFiles/Shader_Prop.fx");
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Prop", CShader::Create(Get_Graphic_Device(), m_szString))))
		return E_FAIL;


	// For.Component_Texture_Terrain
	lstrcpy(m_szString, L"../Bin/Resources/Textures/Terrain/Grass_%d.tga");
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Terrain", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, m_szString, 4))))
		return E_FAIL;

	// For.Component_Texture_Filter
	lstrcpy(m_szString, L"../Bin/Resources/Textures/Terrain/Filter.bmp");
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Filter", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, m_szString))))
		return E_FAIL;


	// For.Component_Texture_Effect
	lstrcpy(m_szString, L"../Bin/Resources/Textures/Explosion/Explosion%d.png");
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Effect", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, m_szString, 90))))
		return E_FAIL;


	// For.Component_Texture_UI_CrossHair
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_UI_CrossHair", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/CrossHair.png"))))
		return E_FAIL;

	// For.Component_Texture_UI_TalkBox
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_UI_TalkBox", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/WhiteTalkBox_%d.tga",2))))
		return E_FAIL;

	// For.Component_Buffer_Terrain	
	lstrcpy(m_szString, L"../Bin/Resources/Textures/Terrain/Height.bmp");
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Buffer_Terrain", CBuffer_Terrain::Create(Get_Graphic_Device(), m_szString, 1.f))))
		return E_FAIL;
	// For.Component_Buffer_Terrain	
	lstrcpy(m_szString, L"../Bin/Resources/Textures/Terrain/Height2.bmp");
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Buffer_Terrain2", CBuffer_Terrain::Create(Get_Graphic_Device(), m_szString, 4.f))))
		return E_FAIL;
	// For.Component_Buffer_Collider
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Buffer_Collider", CBuffer_Collider::Create(Get_Graphic_Device()))))
		return E_FAIL;

	// For.Component_Collider_Box
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Collider_Box", CCollider::Create(Get_Graphic_Device(), CCollider::TYPE_BOUNDINGBOX))))
		return E_FAIL;

	// For.Component_Collider_Sphere
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Collider_Sphere", CCollider::Create(Get_Graphic_Device(), CCollider::TYPE_SPHERE))))
		return E_FAIL;


	// For.Component_Picking
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Picking", CPicking::Create(Get_Graphic_Device()))))
		return E_FAIL;
	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CLoading::Ready_Static_Prototype_UI()
{
	// For.Prototype_UI_TalkBox_White
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_UI_TalkBox_White", CTalkBox::Create(Get_Graphic_Device(), 0))))
		return E_FAIL;
	// For.Prototype_UI_TalkBox_Orange
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_UI_TalkBox_Orange", CTalkBox::Create(Get_Graphic_Device(), 1))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLoading::Ready_Stage_Prototype_GameObject_SceneAPT()
{
	// For.GameObject_Terrain
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Terrain", CTerrain::Create(Get_Graphic_Device(),0))))
		return E_FAIL;
	// For.GameObject_Player
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Player", CPlayer::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.GameObject_Another_Sofa
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Another_Sofa", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Another_Sofa"))))
		return E_FAIL;
	// For.GameObject_Another_BigSofa
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Another_BigSofa", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Another_BigSofa"))))
		return E_FAIL;
	// For.GameObject_Bicycle
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Bicycle", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Bicycle"))))
		return E_FAIL;
	// For.GameObject_BookCase
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_BookCase", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_BookCase"))))
		return E_FAIL;
	// For.GameObject_DogPoolBed
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_DogPoolBed", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_DogPoolBed"))))
		return E_FAIL;
	// For.GameObject_DP_Bed_01
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_DP_Bed_01", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_DP_Bed_01"))))
		return E_FAIL;
	// For.GameObject_DP_Bed_02
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_DP_Bed_02", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_DP_Bed_02"))))
		return E_FAIL;
	// For.GameObject_DP_Sofa
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_DP_Sofa", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_DP_Sofa"))))
		return E_FAIL;
	// For.GameObject_KitchenTable
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_KitchenTable", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_KitchenTable"))))
		return E_FAIL;
	// For.GameObject_LampHouse
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_LampHouse", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_LampHouse"))))
		return E_FAIL;
	// For.GameObject_NeonSign_01
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_NeonSign_01", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_NeonSign_01"))))
		return E_FAIL;
	// For.GameObject_NeonSign_02
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_NeonSign_02", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_NeonSign_02"))))
		return E_FAIL;
	// For.GameObject_NeonSign_03
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_NeonSign_03", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_NeonSign_03"))))
		return E_FAIL;
	// For.GameObject_NeonSign_04
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_NeonSign_04", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_NeonSign_04"))))
		return E_FAIL;
	// For.GameObject_Poster_01
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Poster_01", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Poster_01"))))
		return E_FAIL;
	// For.GameObject_Poster_02
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Poster_02", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Poster_02"))))
		return E_FAIL;
	// For.GameObject_RubberDuckie
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_RubberDuckie", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_RubberDuckie"))))
		return E_FAIL;
	// For.GameObject_Rug
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Rug", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Rug"))))
		return E_FAIL;
	// For.GameObject_TV
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_TV", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_TV"))))
		return E_FAIL;
	// For.GameObject_TV_Remote
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_TV_Remote", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_TV_Remote"))))
		return E_FAIL;
	// For.GameObject_Wall_01
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Wall_01", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Wall_01"))))
		return E_FAIL;
	// For.GameObject_Wall_01b
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Wall_01b", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Wall_01b"))))
		return E_FAIL;
	// For.GameObject_Wall_Corner
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Wall_Corner", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Wall_Corner"))))
		return E_FAIL;
	// For.GameObject_Wall_SideBoard
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Wall_SideBoard", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Wall_SideBoard"))))
		return E_FAIL;
	// For.GameObject_Wall_SideBoardTopSide
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Wall_SideBoardTopSide", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Wall_SideBoardTopSide"))))
		return E_FAIL;
	// For.GameObject_Weight_Barbel
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Weight_Barbel", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Weight_Barbel"))))
		return E_FAIL;
	// For.GameObject_Weight_Bench
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Weight_Bench", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Weight_Bench"))))
		return E_FAIL;
	// For.GameObject_Wooden_Door
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Wooden_Door", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Wooden_Door"))))
		return E_FAIL;
	// For.GameObject_Wooden_DoorFrame
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Wooden_DoorFrame", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Wooden_DoorFrame"))))
		return E_FAIL;
	// For.GameObject_Celling
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Celling", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Celling"))))
		return E_FAIL;
	// ---------------------추가된 놈들

	// For.GameObject_Event_Cube
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Dog", CDog::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.GameObject_Event_Cube
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Event_Cube", CEvent_Cube::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.GameObject_Effect
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Effect", CEffect::Create(Get_Graphic_Device()))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLoading::Ready_Componet_Prototype_SceneAPT()
{
	CComponent_Manager*		pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;
	pComponent_Manager->AddRef();

	// For.Component_Navigation // For. APT Player
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Navigation", CNavigation::Create(Get_Graphic_Device(), L"../Bin/DataFiles/Navigation.dat"))))
		return E_FAIL;

	// For.Component_Mesh_Another_Sofa
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Another_Sofa", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Another_Sofa.x"))))
		return E_FAIL;
	// For.Component_Mesh_Another_BigSofa
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Another_BigSofa", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Another_BigSofa.x"))))
		return E_FAIL;
	// For.Component_Mesh_Bicycle
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Bicycle", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Bicycle.x"))))
		return E_FAIL;
	// For.Component_Mesh_BookCase
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_BookCase", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"BookCase.x"))))
		return E_FAIL;

	// For.Component_Mesh_DogPoolBed
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_DogPoolBed", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"DogPoolBed.x"))))
		return E_FAIL;
	// For.Component_Mesh_DP_Bed_01
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_DP_Bed_01", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"DP_Bed_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_DP_Bed_02
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_DP_Bed_02", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"DP_Bed_02.x"))))
		return E_FAIL;
	// For.Component_Mesh_DP_Sofa
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_DP_Sofa", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"DP_Sofa.x"))))
		return E_FAIL;
	// For.Component_Mesh_KitchenTable
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_KitchenTable", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"KitchenTable.x"))))
		return E_FAIL;
	// For.Component_Mesh_LampHouse
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_LampHouse", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"LampHouse.x"))))
		return E_FAIL;
	// For.Component_Mesh_NeonSign_01
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_NeonSign_01", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"NeonSign_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_NeonSign_02
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_NeonSign_02", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"NeonSign_02.x"))))
		return E_FAIL;
	// For.Component_Mesh_NeonSign_03
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_NeonSign_03", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"NeonSign_03.x"))))
		return E_FAIL;
	// For.Component_Mesh_NeonSign_04
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_NeonSign_04", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"NeonSign_04.x"))))
		return E_FAIL;
	// For.Component_Mesh_Poster_01
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Poster_01", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Poster_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_Poster_02
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Poster_02", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Poster_02.x"))))
		return E_FAIL;
	// For.Component_Mesh_RubberDuckie
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_RubberDuckie", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"RubberDuckie.x"))))
		return E_FAIL;
	// For.Component_Mesh_Rug
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Rug", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Rug.x"))))
		return E_FAIL;

	// For.Component_Mesh_TV
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_TV", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"TV.x"))))
		return E_FAIL;
	// For.Component_Mesh_TV_Remote
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_TV_Remote", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"TV_Remote.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wall_01
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Wall_01", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wall_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wall_01b
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Wall_01b", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wall_01b.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wall_Corner
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Wall_Corner", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wall_Corner.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wall_SideBoard
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Wall_SideBoard", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wall_SideBoard.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wall_SideBoardTopSide
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Wall_SideBoardTopSide", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wall_SideBoardTopSide.x"))))
		return E_FAIL;
	// For.Component_Mesh_Weight_Barbel
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weight_Barbel", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Weight_Barbel.x"))))
		return E_FAIL;
	// For.Component_Mesh_Weight_Bench
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weight_Bench", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Weight_Bench.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wooden_Door
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Wooden_Door", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wooden_Door.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wooden_DoorFrame
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Wooden_DoorFrame", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wooden_DoorFrame.x"))))
		return E_FAIL;
	// For.Component_Celling
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Celling", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/",L"Celling.x"))))
		return E_FAIL;

	// For.Component_Mesh_Dog
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Dog", CMesh_Dynamic::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/DynamicMesh/Dog/", L"DP_DOG.x"))))
		return E_FAIL;
	// For.Component_Mesh_Player
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Player", CMesh_Dynamic::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/DynamicMesh/PlayerXFile/", L"DP_APT_00.x"))))
		return E_FAIL;
	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CLoading::Ready_UI_SceneAPT()
{

	return NOERROR;
}

HRESULT CLoading::Ready_Component_Prototype_SceneFIELD()
{
	CComponent_Manager*		pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;
	pComponent_Manager->AddRef();

	// For.Component_Navigation // For. Field Player
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Navigation", CNavigation::Create(Get_Graphic_Device(), L"../Bin/DataFiles/TestNav.dat"))))
		return E_FAIL;

	// For.Component_Mesh_SkyDom_Night
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_SkyDom_Night", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/Sky/", L"SkyDom_Night.x"))))
		return E_FAIL;
	// For.Component_Mesh_SkyDom_Dusk
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_SkyDom_Dusk", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/Sky/", L"SkyDom_Dusk.x"))))
		return E_FAIL;
	// For.Component_Mesh_SkyDom_Sunset
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_SkyDom_Sunset", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/Sky/", L"SkyDom_Sunset.x"))))
		return E_FAIL;

	// For.Component_Mesh_Player
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Player", CMesh_Dynamic::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/DynamicMesh/PlayerXFile/", L"DP_Field.x"))))
		return E_FAIL;
	// For.Component_Mesh_Sword
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Sword", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/Sword/", L"DeadPool_Sword.x"))))
		return E_FAIL;
	// For.Component_Mesh_Gun
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Gun", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/Gun/", L"DeadPool_Gun.x"))))
		return E_FAIL;


	// For.Component_Mesh_Brawler01
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Brawler01", CMesh_Dynamic::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/DynamicMesh/Brawler/Brawler01/", L"Brawler01.x"))))
		return E_FAIL;
	// For.Component_Mesh_Brawler02
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Brawler02", CMesh_Dynamic::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/DynamicMesh/Brawler/Brawler02/", L"Brawler02.x"))))
		return E_FAIL;
	// For.Component_Mesh_Brawler_Knife
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Brawler_Knife", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/Brawler/WEP/", L"Brawler_Knife.x"))))
		return E_FAIL;
	// For.Component_Mesh_Brawler_ElectricBaton
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Brawler_ElectricBaton", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/Brawler/WEP/", L"Brawler_ElectricBaton.x"))))
		return E_FAIL;




	////////////////////////////////////////////
	// For.Component_Mesh_
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_AccessRoof", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"AccessRoof.x"))))
		return E_FAIL;
	// For.Component_Mesh_
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_AccessRoof_Door", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"AccessRoof_Door.x"))))
		return E_FAIL;
	// For.Component_Mesh_
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_AccessRoof02", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"AccessRoof02.x"))))
		return E_FAIL;
	// For.Component_Mesh_
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_AccessRoof03", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"AccessRoof03.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_ACVent_01", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"ACVent_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_ACVent_02", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"ACVent_02.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_BackGroundBuilding_02", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"BackGroundBuilding_02.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_BackGroundBuilding_Destroy", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"BackGroundBuilding_Destroy.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_BeerKeg", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"BeerKeg.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_Cabinet", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"Cabinet.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_CircuitBreakers", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"CircuitBreakers.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_EmergencyLight", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"EmergencyLight.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_FlowerBed", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"FlowerBed.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_FlowerBed2", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"FlowerBed2.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_FuelTank", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"FuelTank.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_GenericBox", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"GenericBox.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_GWN_Sign", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"GWN_Sign.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_GWN_Wall01", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"GWN_Wall01.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_GWN_Wall02", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"GWN_Wall02.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_GWN_Wall03", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"GWN_Wall03.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_HallyWayCover", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"HallyWayCover.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_IndustrialBox", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"IndustrialBox.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_IndustrialCart", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"IndustrialCart.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_IndustrialGenerator", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"IndustrialGenerator.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_Ladder", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"Ladder.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_LampStand", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"LampStand.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_Light_Base_01", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"Light_Base_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_Light_Base_02", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"Light_Base_02.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_MainBuilding_01", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"MainBuilding_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_MainBuilding_02", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"MainBuilding_02.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_OutDoorPipe", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"OutDoorPipe.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_Pannel_Wood", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"Pannel_Wood.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_RadioTower01", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"RadioTower01.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_RoofTopAC", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"RoofTopAC.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_Structure_01", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"Structure_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_ToolBox", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"ToolBox.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_Transformer01", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"Transformer01.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_Transformer02", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"Transformer02.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_TrashBagSpilled", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"TrashBagSpilled.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_TriSentinel", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"TriSentinel.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_VentDeco", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"VentDeco.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_Wall_Rail01", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"Wall_Rail01.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_Wall_Rail02", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"Wall_Rail02.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_WallLight", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"WallLight.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_WaterHeater01", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"WaterHeater01.x"))))
		return E_FAIL;
	// For.Component_Mesh_																																								
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_WoodenBox", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/GWN/", L"WoodenBox.x"))))
		return E_FAIL;


	////////////////////////////////////////////

	// For.Component_Mesh_Bicycle
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Bicycle", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Bicycle.x"))))
		return E_FAIL;
	// For.Component_Mesh_Weight_Barbel
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Weight_Barbel", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Weight_Barbel.x"))))
		return E_FAIL;
	// For.Component_Mesh_Helicopter
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Helicopter", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/Helicopter/", L"Helicopter.x"))))
		return E_FAIL;
	// For.Component_Mesh_Prop
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Prop", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/Helicopter/", L"Prop.x"))))
		return E_FAIL;

	/////////////////////////////////////////////

	// For.Component_Texture_HPBar
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_HPBar", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/HPBar/HPBar%d.png", 2))))
		return E_FAIL;


	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CLoading::Ready_UI_SceneFIELD()
{
	//Prototype
	// For.Prototype_UI_CrossHair
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_UI_CrossHair", CUI_CrossHair::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.Prototype_UI_HPBar
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_UI_HPBar", CUI_HPBar::Create(Get_Graphic_Device()))))
		return E_FAIL;


	//GameObject
	if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_UI_CrossHair", SCENE_STAGE, L"Layer_UI", nullptr)))
		return E_FAIL;

	////GameObject
	//for (int i = 0; i < 10; ++i)
	//{
	//	if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_UI_HPBar", SCENE_STAGE, L"Layer_UI", nullptr)))
	//		return E_FAIL;
	//}
	return NOERROR;
}

HRESULT CLoading::Ready_AIRPLANE()
{
	// For. AirPlane
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_AirPlane", CAirplane::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.Add_AirPlane
	if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_AirPlane", SCENE_STAGE, L"Layer_Airplane", nullptr)))
		return E_FAIL;


	// For.Add_AirPlane
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Prop", SCENE_STAGE, L"Layer_Airplane_Field")))
		return E_FAIL;

	// For.Minigun
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(SCENE_STAGE, L"Component_Mesh_Minigun", CMesh_Dynamic::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/Helicopter/", L"MinigunFire.x"))))
		return E_FAIL;
	// For.Prototype_Minigun
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Minigun", CMinigun::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.Add_Minigun
	if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_Minigun", SCENE_STAGE, L"Layer_Minigun", nullptr)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CLoading::Ready_UI_SceneAIRPLANE()
{
	//Prototype
	// For.Prototype_UI_CrossHair
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_UI_CrossHair", CUI_CrossHair::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.Prototype_UI_HPBar
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_UI_HPBar", CUI_HPBar::Create(Get_Graphic_Device()))))
		return E_FAIL;


	return NOERROR;
}

HRESULT CLoading::Ready_Stage_Prototype_GameObject_SceneFIELD()
{
	// For.GameObject_Terrain
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Terrain", CTerrain::Create(Get_Graphic_Device(),1))))
		return E_FAIL;
	// For.GameObject_Player
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Player", CPlayer::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.GameObject_Brawler01
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Brawler01_1", CBrawler01::Create(Get_Graphic_Device(), 1))))
		return E_FAIL;
	// For.GameObject_Brawler01
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Brawler01_2", CBrawler01::Create(Get_Graphic_Device(), 2))))
		return E_FAIL;
	// For.GameObject_Brawler01
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Brawler02", CBrawler02::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.GameObject_SkyDom_Night
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_SkyDom_Night", CSkyDom::Create(Get_Graphic_Device(), 0))))
		return E_FAIL;
	// For.GameObject_SkyDom_Dusk
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_SkyDom_Dusk", CSkyDom::Create(Get_Graphic_Device(), 1))))
		return E_FAIL;
	// For.GameObject_SkyDom_Sunset
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_SkyDom_Sunset", CSkyDom::Create(Get_Graphic_Device(),2))))
		return E_FAIL;

	// For.GameObject_Sword_Left
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Sword_Left", CDP_Sword::Create(Get_Graphic_Device(), 0))))
		return E_FAIL;
	// For.GameObject_Sword_Right
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Sword_Right", CDP_Sword::Create(Get_Graphic_Device(), 1))))
		return E_FAIL;
	// For.GameObject_Gun_Left
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Gun_Left", CDP_Gun::Create(Get_Graphic_Device(), 0))))
		return E_FAIL;										 
	// For.GameObject_Gun_Right								 
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Gun_Right", CDP_Gun::Create(Get_Graphic_Device(), 1))))
		return E_FAIL;
	// For.GameObject_Brawler_Knife
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Brawler_Knife", CBrawler_Knife::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.GameObject_Brawler_ElectricBaton
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Brawler_ElectricBaton", CBrawler_ElectricBaton::Create(Get_Graphic_Device()))))
		return E_FAIL;
	//////////////////////////////////////////////
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_AccessRoof", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_AccessRoof"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_AccessRoof_Door", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_AccessRoof_Door"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_AccessRoof02", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_AccessRoof02"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_AccessRoof03", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_AccessRoof03"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_ACVent_01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_ACVent_01"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_ACVent_02", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_ACVent_02"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_BackGroundBuilding_02", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_BackGroundBuilding_02"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_BackGroundBuilding_Destroy", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_BackGroundBuilding_Destroy"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_BeerKeg", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_BeerKeg"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Cabinet", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Cabinet"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_CircuitBreakers", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_CircuitBreakers"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_EmergencyLight", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_EmergencyLight"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_FlowerBed", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_FlowerBed"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_FlowerBed2", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_FlowerBed2"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_FuelTank", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_FuelTank"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_GenericBox", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_GenericBox"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_GWN_Sign", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_GWN_Sign"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_GWN_Wall01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_GWN_Wall01"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_GWN_Wall02", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_GWN_Wall02"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_GWN_Wall03", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_GWN_Wall03"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_HallyWayCover", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_HallyWayCover"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_IndustrialBox", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_IndustrialBox"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_IndustrialCart", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_IndustrialCart"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_IndustrialGenerator", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_IndustrialGenerator"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Ladder", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Ladder"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_LampStand", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_LampStand"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Light_Base_01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Light_Base_01"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Light_Base_02", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Light_Base_02"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_MainBuilding_01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_MainBuilding_01"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_MainBuilding_02", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_MainBuilding_02"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_OutDoorPipe", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_OutDoorPipe"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Pannel_Wood", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Pannel_Wood"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_RadioTower01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_RadioTower01"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_RoofTopAC", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_RoofTopAC"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Structure_01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Structure_01"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_ToolBox", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_ToolBox"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Transformer01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Transformer01"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Transformer02", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Transformer02"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_TrashBagSpilled", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_TrashBagSpilled"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_TriSentinel", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_TriSentinel"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_VentDeco", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_VentDeco"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Wall_Rail01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Wall_Rail01"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Wall_Rail02", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Wall_Rail02"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_WallLight", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_WallLight"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_WaterHeater01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_WaterHeater01"))))
		return E_FAIL;
	// For.Prototype_
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_WoodenBox", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_WoodenBox"))))
		return E_FAIL;
	////////////////////////////////////////////////////

	// For.GameObject_Bicycle
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Bicycle", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Bicycle"))))
		return E_FAIL;
	// For.GameObject_Weight_Barbel
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Weight_Barbel", CStatic_Object::Create(Get_Graphic_Device(), L"Component_Mesh_Weight_Barbel"))))
		return E_FAIL;

	// For.GameObject_Helicopter
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Static_Airplane", CStatic_Airplane::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.GameObject_Helicopter
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Prop", CProp::Create(Get_Graphic_Device()))))
		return E_FAIL;
	//////////////////////////////////////////////////
	return NOERROR;
}

HRESULT CLoading::Ready_Layer_Camera(const _tchar * pProtoName, const _tchar * pLayerTag, const _float fNear)
{
	// For.Camera_Debug
	CCamera*	pCamera = nullptr;
	if (FAILED(Add_Object(SCENE_STATIC, pProtoName, SCENE_STAGE, pLayerTag, (CGameObject**)&pCamera)))
		return E_FAIL;
	CAMERADESC			CamDesc;
	ZeroMemory(&CamDesc, sizeof(CAMERADESC));
	CamDesc.vEye = _vec3(0.f, 7.f, -5.f);
	CamDesc.vAt = _vec3(0.f, 0.f, 0.f);
	CamDesc.vAxisY = _vec3(0.f, 1.f, 0.f);

	PROJDESC			ProjDesc;
	ZeroMemory(&ProjDesc, sizeof(PROJDESC));
	ProjDesc.fFovy = D3DXToRadian(60.0f);
	ProjDesc.fAspect = _float(g_iBackCX) / g_iBackCY;
	ProjDesc.fNear = fNear;
	ProjDesc.fFar = 500.f;

	if (FAILED(pCamera->SetUp_CameraInfo(CamDesc, ProjDesc)))
		return E_FAIL;

	const CGameObject* pPlayer = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);

	if (FAILED(pCamera->SetUp_Target(pPlayer)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CLoading::Ready_Layer_Test(const _tchar * pLayerTag)
{
	// For.Test
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Test", SCENE_STAGE, pLayerTag)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CLoading::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameObject* pGameObject;
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Player", SCENE_STAGE, pLayerTag,&pGameObject)))
		return E_FAIL;
	((CPlayer*)pGameObject)->SetUp_StageInfo(0);
	//// For.Sword 
	//if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Sword_Left", SCENE_STAGE, pLayerTag)))
	//	return E_FAIL;
	//// For.Sword
	//if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Sword_Right", SCENE_STAGE, pLayerTag)))
	//	return E_FAIL;
	return NOERROR;
}

HRESULT CLoading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	// For.Terrain
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Terrain", SCENE_STAGE, pLayerTag)))
		return E_FAIL;


	
	return NOERROR;
}

HRESULT CLoading::Ready_Layer_Object()
{
	for (size_t i = 0; i < 3; i++)
	{
		if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Brawler01_1", SCENE_STAGE, L"Layer_Brawler01")))
			return E_FAIL;
	}
	for (size_t i = 0; i < 1; i++)
	{
		if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Brawler02", SCENE_STAGE, L"Layer_Brawler02")))
			return E_FAIL;
	}
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Static_Airplane", SCENE_STAGE, L"Layer_Airplane_Field")))
		return E_FAIL;
	// For.Add_AirPlane
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Prop", SCENE_STAGE, L"Layer_Airplane_Field")))
		return E_FAIL;


	//if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Brawler_TPose", SCENE_STAGE, L"Layer_Brawler_TPose")))
	//	return E_FAIL;
	return NOERROR;
}

HRESULT CLoading::Ready_Layer_Player_FIELD(const _tchar * pLayerTag)
{
	CGameObject* pGameObject = nullptr;
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Player", SCENE_STAGE, pLayerTag, &pGameObject)))
		return E_FAIL;
	((CPlayer*)pGameObject)->SetUp_StageInfo(1);
	// For.Sword
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Sword_Left", SCENE_STAGE, pLayerTag)))
		return E_FAIL;
	// For.Sword
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Sword_Right", SCENE_STAGE, pLayerTag)))
		return E_FAIL;
	// For.Gun
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Gun_Left", SCENE_STAGE, pLayerTag)))
		return E_FAIL;
	// For.Gun
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Gun_Right", SCENE_STAGE, pLayerTag)))
		return E_FAIL;
	return NOERROR;
}

HRESULT CLoading::Ready_Layer_BackGround_FIELD(const _tchar * pLayerTag)
{
	// Height맵 따로 받아서 다시 해주자. 테스트용.

	// For.Terrain
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Terrain", SCENE_STAGE, pLayerTag)))
		return E_FAIL;
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_SkyDom_Night", SCENE_STAGE, pLayerTag)))
		return E_FAIL;



	return NOERROR;
}

HRESULT CLoading::Ready_Layer_BackGround_AIRPLANE(const _tchar * pLayerTag)
{
	// For.Terrain
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Terrain", SCENE_STAGE, pLayerTag)))
		return E_FAIL;
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_SkyDom_Dusk", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLoading::Load_Static_Object(const _tchar * pFilePath)
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_ulong			dwByte = 0;
	CGameObject* pStatic_Object = nullptr;
	HANDLE			hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;
	for (;;)
	{
		STATIC_OBJECT_INFO		ObjectInfo;
		ReadFile(hFile, &ObjectInfo, sizeof(STATIC_OBJECT_INFO), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}
		lstrcpy(m_szString, ObjectInfo.szPrototype_Tag);
		if (FAILED(Add_Object(SCENE_STAGE, ObjectInfo.szPrototype_Tag, SCENE_STAGE, L"Layer_Load", &pStatic_Object)))
			return E_FAIL;
		((CStatic_Object*)pStatic_Object)->Set_StateInfo(&ObjectInfo.vRight, &ObjectInfo.vUp, &ObjectInfo.vLook, &ObjectInfo.vPos,ObjectInfo.isOffCulling);
	}

	CloseHandle(hFile);

	return NOERROR;
}

HRESULT CLoading::Load_Event_Cube(const _tchar * pFilePath)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGameObject* pCube = nullptr;
	_ulong		dwByte = 0;
	HANDLE			hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	//WriteFile(hFile, &pEventTag, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
	//WriteFile(hFile, &((CEventCube*)EventCube)->Get_ObjectInfo(), sizeof(_vec3), &dwByte, nullptr);
	for (;;)
	{
		_tchar pEventTag[128];
		_vec3 vPosition;
		_matrix matTmp;
		D3DXMatrixIdentity(&matTmp);

		ReadFile(hFile, &pEventTag, sizeof(_tchar) * 128, &dwByte, nullptr);
		ReadFile(hFile, &vPosition, sizeof(_vec3), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}
		if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Event_Cube", SCENE_STAGE, L"Layer_Event_Cube", &pCube)))
			return E_FAIL;

		matTmp.m[3][0] = vPosition.x;
		matTmp.m[3][1] = vPosition.y;
		matTmp.m[3][2] = vPosition.z;
		((CEvent_Cube*)pCube)->Set_StateInfo(&(_vec3)matTmp.m[0], &(_vec3)matTmp.m[1], &(_vec3)matTmp.m[2], &(_vec3)matTmp.m[3]);
		((CEvent_Cube*)pCube)->Set_EventTag(pEventTag);

	}

	CloseHandle(hFile);

	return NOERROR;
}

HRESULT CLoading::SetUp_CameraMove()
{

	const CCamera_Cinematic* pCamera = (CCamera_Cinematic*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 2);

	vector<_vec3> vecEye;
	vecEye.push_back(_vec3(40, 30, 10));
	vecEye.push_back(_vec3(30, 20, 15));
	vecEye.push_back(_vec3(20, 10, 25));
	vecEye.push_back(_vec3(10, 20, 30));

	const_cast<CCamera_Cinematic*>(pCamera)->SetUp_CameraMove(vecEye, _vec3(0, 0, 0), 5);


	return NOERROR;
}

_uint CLoading::Thread_Main(void * pArg)
{
	CLoading* pLoading = (CLoading*)pArg;

	HRESULT			hr = 0;

	EnterCriticalSection(pLoading->Get_Critical_Section());

	switch (pLoading->Get_SceneID())
	{
	case SCENE_STAGE:
		hr = pLoading->Loading_Stage_APT();
		break;
	case SCENE_FIELD:
		hr = pLoading->Loading_Stage_FIELD();
		break;
	case SCENE_AIRPLANE:
		hr = pLoading->Loading_Stage_AIRPLANE();
		break;
	}

	if (FAILED(hr))
	{
		LeaveCriticalSection(pLoading->Get_Critical_Section());
		return -1;
	}

	LeaveCriticalSection(pLoading->Get_Critical_Section());

	return 0;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eSceneID)
{
	CLoading*		pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eSceneID)))
	{
		_MSG_BOX("CLoading Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoading::Free()
{
	// 새롭게 생성한 스레드가 자기할일(Thread_Main)을 완전히 종료할때ㅑ까지 기다려.
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);

}
