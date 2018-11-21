#include "stdafx.h"
#include "..\Headers\Scene_Stage.h"
#include "Component_Manager.h"
#include "Camera_Debug.h"
#include "Camera_Cinematic.h"
#include "Light_Manager.h"

// GameObject
#include "Terrain.h"
#include "Player.h"
#include "DP_Sword.h"
#include "Static_Object.h"
_USING(Client)

CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{

}

HRESULT CScene_Stage::Ready_Scene()
{
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	if (FAILED(Ready_Stage_Prototype_Component()))
		return E_FAIL;
	if (FAILED(Ready_Componet_Prototype_SceneAPT()))
		return E_FAIL;

	if (FAILED(Ready_Stage_Prototype_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Debug", L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(L"Layer_Player")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Target", L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"GameObject_Camera_Cinematic", L"Layer_Camera")))
		return E_FAIL;
	SetUp_CameraMove();



	if (FAILED(Ready_Layer_Test(L"Layer_Test")))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	if (FAILED(Load_Static_Object(L"../Bin/DataFiles/StaticO11bject1.dat")))
		return E_FAIL;

	return NOERROR;
}

_int CScene_Stage::Update_Scene(const _float & fTimeDelta)
{

	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Stage::LastUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LastUpdate_Scene(fTimeDelta);
}

void CScene_Stage::Render_Scene()
{

}

// 조명을 설정한다.
HRESULT CScene_Stage::Ready_LightInfo()
{
	D3DLIGHT9			LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = LightInfo.Diffuse;
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(Get_Graphic_Device(), &LightInfo)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Stage_Prototype_Component()
{
	CComponent_Manager*		pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;
	pComponent_Manager->AddRef();

	// For.Component_Shader_Terrain
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Terrain", CShader::Create(Get_Graphic_Device(), L"../Bin/ShaderFiles/Shader_Terrain.fx"))))
		return E_FAIL;

	// For.Component_Shader_Mesh
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", CShader::Create(Get_Graphic_Device(), L"../Bin/ShaderFiles/Shader_Mesh.fx"))))
		return E_FAIL;
	// For.Component_Shader_Mesh
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Shader_Collider", CShader::Create(Get_Graphic_Device(), L"../Bin/ShaderFiles/Shader_Collider.fx"))))
		return E_FAIL;

	// For.Component_Texture_Terrain
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Terrain", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Terrain/Grass_%d.tga", 4))))
		return E_FAIL;

	// For.Component_Texture_Filter
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Filter", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Terrain/Filter.bmp"))))
		return E_FAIL;

	// For.Component_Texture_Brush
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Texture_Brush", CTexture::Create(Get_Graphic_Device(), CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Terrain/Brush.png"))))
		return E_FAIL;

	// For.Component_Buffer_Terrain	
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Buffer_Terrain", CBuffer_Terrain::Create(Get_Graphic_Device(), L"../Bin/Resources/Textures/Terrain/Height.bmp", 1.f))))
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

	// For.Component_Mesh_Stone
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Stone", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/TombStone/", L"TombStone.x"))))
		return E_FAIL;
	// For.Component_Mesh_Player
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Player", CMesh_Dynamic::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/DynamicMesh/PlayerXFile/", L"DP_APT_00.x"))))
		return E_FAIL;
	// For.Component_Mesh_Sword
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Mesh_Sword", CMesh_Static::Create(Get_Graphic_Device(), L"../Bin/Resources/Meshes/StaticMesh/Sword/", L"DeadPool_Sword.x"))))
		return E_FAIL;
	// For.Component_Picking
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Picking", CPicking::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.Component_Navigation
	if (FAILED(pComponent_Manager->Add_Component(SCENE_STAGE, L"Component_Navigation", CNavigation::Create(Get_Graphic_Device(), L"../Bin/DataFiles/Navigation.dat"))))
		return E_FAIL;
	Safe_Release(pComponent_Manager);

	return NOERROR;
}



HRESULT CScene_Stage::Ready_Stage_Prototype_GameObject()
{
	// For.GameObject_Terrain
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Terrain", CTerrain::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.GameObject_Player
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Player", CPlayer::Create(Get_Graphic_Device()))))
		return E_FAIL;
	// For.GameObject_Sword_Left
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Sword_Left", CDP_Sword::Create(Get_Graphic_Device(), 0))))
		return E_FAIL;
	// For.GameObject_Sword_Right
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"Prototype_Sword_Right", CDP_Sword::Create(Get_Graphic_Device(), 1))))
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
	// ---------------------추가된 놈들




	return NOERROR;
}

HRESULT CScene_Stage::Ready_Componet_Prototype_SceneAPT()
{
	CComponent_Manager*		pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;
	pComponent_Manager->AddRef();
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
	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CScene_Stage::Load_Static_Object(const _tchar* pPathTag)
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_ulong			dwByte = 0;
	CGameObject* pStatic_Object = nullptr;
	HANDLE			hFile = CreateFile(pPathTag, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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

		if (FAILED(Add_Object(SCENE_STAGE, ObjectInfo.szPrototype_Tag, SCENE_STAGE, L"Layer_Load", &pStatic_Object)))
			return E_FAIL;
		((CStatic_Object*)pStatic_Object)->Set_StateInfo(&ObjectInfo.vRight, &ObjectInfo.vUp, &ObjectInfo.vLook, &ObjectInfo.vPos);
	}

	CloseHandle(hFile);

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Camera(const _tchar * pProtoName, const _tchar * pLayerTag)
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
	ProjDesc.fNear = 0.3f;
	ProjDesc.fFar = 500.f;

	if (FAILED(pCamera->SetUp_CameraInfo(CamDesc, ProjDesc)))
		return E_FAIL;

	const CGameObject* pPlayer = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);

	if (FAILED(pCamera->SetUp_Target(pPlayer)))
		return E_FAIL;

	
	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Test(const _tchar * pLayerTag)
{
	// For.Test
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Test", SCENE_STAGE, pLayerTag)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Player(const _tchar * pLayerTag)
{
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Player", SCENE_STAGE, pLayerTag)))
		return E_FAIL;
	//// For.Sword
	//if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Sword_Left", SCENE_STAGE, pLayerTag)))
	//	return E_FAIL;
	//// For.Sword
	//if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Sword_Right", SCENE_STAGE, pLayerTag)))
	//	return E_FAIL;
	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	// For.Terrain
	if (FAILED(Add_Object(SCENE_STAGE, L"Prototype_Terrain", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::SetUp_CameraMove()
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
