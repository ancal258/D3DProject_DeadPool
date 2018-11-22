
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"

// My Include
#include "Light_Manager.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Graphic_Device.h"

#include "ToolTerrain.h"
#include "ToolCamera.h"
#include "Player.h"
#include "CamPoint.h"
#include "EventCube.h"

#include "Static_Object.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MY_TIMER 10
// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

HRESULT CToolView::Ready_Componet_Prototype()
{
	if (FAILED(Ready_Componet_Prototype_Static()))
		return E_FAIL;
	if (FAILED(Ready_Componet_Prototype_SceneAPT()))
		return E_FAIL;
	if (FAILED(Ready_Componet_Prototype_SceneMap()))
		return E_FAIL;
	if (FAILED(Ready_Componet_Prototype_SceneStatic()))
		return E_FAIL;


	return NOERROR;
}

HRESULT CToolView::Ready_Componet_Prototype_Static()
{
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Transform", CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Shader_Terrain", CShader::Create(m_pGraphic_Device, L"Bin/ShaderFiles/Shader_Terrain.fx"))))
		return E_FAIL;
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Shader_Mesh", CShader::Create(m_pGraphic_Device, L"Bin/ShaderFiles/Shader_StaticMesh.fx"))))
		return E_FAIL;
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Texture_Terrain", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"Bin/Resources/Textures/Terrain/Grass_%d.tga", 4))))
		return E_FAIL;
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Texture_Filter", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"Bin/Splatting.bmp"))))
		return E_FAIL;
	// For.Component_Collider_Box
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Collider_Box", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_BOUNDINGBOX))))
		return E_FAIL;
	// For.Component_Collider_Sphere
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Collider_Sphere", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_SPHERE))))
		return E_FAIL;
	// For.Component_Navigation
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Navigation", CNavigation::Create(m_pGraphic_Device, L"../Bin/Data/Navigation.dat"))))
		return E_FAIL;
	return NOERROR;
}

HRESULT CToolView::Ready_Componet_Prototype_SceneAPT()
{
	// For.Component_Mesh_Another_Sofa
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Another_Sofa", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Another_Sofa.x"))))
		return E_FAIL;
	// For.Component_Mesh_Another_BigSofa
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Another_BigSofa", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Another_BigSofa.x"))))
		return E_FAIL;
	// For.Component_Mesh_Bicycle
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Bicycle", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Bicycle.x"))))
		return E_FAIL;
	// For.Component_Mesh_BookCase
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_BookCase", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"BookCase.x"))))
		return E_FAIL;
	// For.Component_Mesh_DogPoolBed
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_DogPoolBed", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"DogPoolBed.x"))))
		return E_FAIL;
	// For.Component_Mesh_DP_Bed_01
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_DP_Bed_01", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"DP_Bed_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_DP_Bed_02
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_DP_Bed_02", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"DP_Bed_02.x"))))
		return E_FAIL;
	// For.Component_Mesh_DP_Sofa
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_DP_Sofa", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"DP_Sofa.x"))))
		return E_FAIL;
	// For.Component_Mesh_KitchenTable
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_KitchenTable", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"KitchenTable.x"))))
		return E_FAIL;
	// For.Component_Mesh_NeonSign_01
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_NeonSign_01", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"NeonSign_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_NeonSign_02
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_NeonSign_02", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"NeonSign_02.x"))))
		return E_FAIL;
	// For.Component_Mesh_NeonSign_03
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_NeonSign_03", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"NeonSign_03.x"))))
		return E_FAIL;
	// For.Component_Mesh_NeonSign_04
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_NeonSign_04", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"NeonSign_04.x"))))
		return E_FAIL;
	// For.Component_Mesh_Poster_01
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Poster_01", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Poster_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_Poster_02
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Poster_02", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Poster_02.x"))))
		return E_FAIL;
	// For.Component_Mesh_RubberDuckie
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_RubberDuckie", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"RubberDuckie.x"))))
		return E_FAIL;
	// For.Component_Mesh_Rug
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Rug", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Rug.x"))))
		return E_FAIL;
	// For.Component_Mesh_TV
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_TV", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"TV.x"))))
		return E_FAIL;
	// For.Component_Mesh_TV_Remote
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_TV_Remote", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"TV_Remote.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wall_01
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Wall_01", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wall_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wall_01b
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Wall_01b", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wall_01b.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wall_Corner
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Wall_Corner", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wall_Corner.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wall_SideBoard
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Wall_SideBoard", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wall_SideBoard.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wall_SideBoardTopSide
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Wall_SideBoardTopSide", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wall_SideBoardTopSide.x"))))
		return E_FAIL;
	// For.Component_Mesh_Weight_Barbel
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Weight_Barbel", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Weight_Barbel.x"))))
		return E_FAIL;
	// For.Component_Mesh_Weight_Bench
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Weight_Bench", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Weight_Bench.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wooden_Door
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Wooden_Door", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wooden_Door.x"))))
		return E_FAIL;
	// For.Component_Mesh_Wooden_DoorFrame
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Wooden_DoorFrame", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/APT/", L"Wooden_DoorFrame.x"))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CToolView::Ready_Componet_Prototype_SceneMap()
{
	return NOERROR;
}

HRESULT CToolView::Ready_Componet_Prototype_SceneStatic()
{
	// For.Component_Mesh_Player
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Player", CMesh_Dynamic::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/DynamicMesh/PlayerXFile/", L"DP_APT_00.x"))))
		return E_FAIL;
	// For.Component_Mesh_Tree
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Stone", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/Tree/", L"Tree01.x"))))
		return E_FAIL;
	// For.Component_Mesh_Rock_Wall
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Rock_Wall", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/Rock/", L"Rock_Wall_01.x"))))
		return E_FAIL;
	// For.Component_Mesh_Rock_Bridge
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Mesh_Rock_Bridge", CMesh_Static::Create(m_pGraphic_Device, L"Bin/Resources/Meshes/StaticMesh/DeadPoolMesh/Rock/", L"Rock_Bridge_01.x"))))
		return E_FAIL;


	return NOERROR;
}

HRESULT CToolView::Ready_GameObject_Prototype()
{
	if (FAILED(Ready_GameObject_Prototype_Static()))
		return E_FAIL;
	if (FAILED(Ready_GameObject_Prototype_SceneAPT()))
		return E_FAIL;
	if (FAILED(Ready_GameObject_Prototype_SceneMap()))
		return E_FAIL;
	if (FAILED(Ready_GameObject_Prototype_SceneStatic()))
		return E_FAIL;


	return NOERROR;
}

HRESULT CToolView::Ready_GameObject_Prototype_Static()
{
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Terrain", CToolTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Camera", CToolCamera::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_CamPoint", CCamPoint::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_EventCube", CEventCube::Create(m_pGraphic_Device))))
		return E_FAIL;
	return NOERROR;
}

HRESULT CToolView::Ready_GameObject_Prototype_SceneAPT()
{
	// For.GameObject_Another_Sofa
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Another_Sofa", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Another_Sofa"))))
		return E_FAIL;
	// For.GameObject_Another_BigSofa
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Another_BigSofa", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Another_BigSofa"))))
		return E_FAIL;
	// For.GameObject_Bicycle
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Bicycle", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Bicycle"))))
		return E_FAIL;
	// For.GameObject_BookCase
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_BookCase", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_BookCase"))))
		return E_FAIL;
	// For.GameObject_DogPoolBed
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_DogPoolBed", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_DogPoolBed"))))
		return E_FAIL;
	// For.GameObject_DP_Bed_01
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_DP_Bed_01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_DP_Bed_01"))))
		return E_FAIL;
	// For.GameObject_DP_Bed_02
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_DP_Bed_02", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_DP_Bed_02"))))
		return E_FAIL;
	// For.GameObject_DP_Sofa
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_DP_Sofa", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_DP_Sofa"))))
		return E_FAIL;
	// For.GameObject_KitchenTable
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_KitchenTable", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_KitchenTable"))))
		return E_FAIL;
	// For.GameObject_NeonSign_01
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_NeonSign_01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_NeonSign_01"))))
		return E_FAIL;
	// For.GameObject_NeonSign_02
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_NeonSign_02", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_NeonSign_02"))))
		return E_FAIL;
	// For.GameObject_NeonSign_03
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_NeonSign_03", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_NeonSign_03"))))
		return E_FAIL;
	// For.GameObject_NeonSign_04
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_NeonSign_04", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_NeonSign_04"))))
		return E_FAIL;
	// For.GameObject_Poster_01
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Poster_01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Poster_01"))))
		return E_FAIL;
	// For.GameObject_Poster_02
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Poster_02", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Poster_02"))))
		return E_FAIL;
	// For.GameObject_RubberDuckie
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_RubberDuckie", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_RubberDuckie"))))
		return E_FAIL;
	// For.GameObject_Rug
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Rug", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Rug"))))
		return E_FAIL;
	// For.GameObject_TV
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_TV", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_TV"))))
		return E_FAIL;
	// For.GameObject_TV_Remote
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_TV_Remote", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_TV_Remote"))))
		return E_FAIL;
	// For.GameObject_Wall_01
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Wall_01", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Wall_01"))))
		return E_FAIL;
	// For.GameObject_Wall_01b
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Wall_01b", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Wall_01b"))))
		return E_FAIL;
	// For.GameObject_Wall_Corner
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Wall_Corner", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Wall_Corner"))))
		return E_FAIL;
	// For.GameObject_Wall_SideBoard
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Wall_SideBoard", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Wall_SideBoard"))))
		return E_FAIL;
	// For.GameObject_Wall_SideBoardTopSide
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Wall_SideBoardTopSide", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Wall_SideBoardTopSide"))))
		return E_FAIL;
	// For.GameObject_Weight_Barbel
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Weight_Barbel", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Weight_Barbel"))))
		return E_FAIL;
	// For.GameObject_Weight_Bench
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Weight_Bench", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Weight_Bench"))))
		return E_FAIL;
	// For.GameObject_Wooden_Door
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Wooden_Door", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Wooden_Door"))))
		return E_FAIL;
	// For.GameObject_Wooden_DoorFrame
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Wooden_DoorFrame", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Wooden_DoorFrame"))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CToolView::Ready_GameObject_Prototype_SceneMap()
{
	return NOERROR;
}

HRESULT CToolView::Ready_GameObject_Prototype_SceneStatic()
{
	// For.GameObject_Test
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Test", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Weight_Barbel"))))
		return E_FAIL;
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Rock_Bridge", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Rock_Bridge"))))
		return E_FAIL;
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Rock_Wall", CStatic_Object::Create(m_pGraphic_Device, L"Component_Mesh_Rock_Wall"))))
		return E_FAIL;
	// For.GameObject_Player
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Player", CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;
	return NOERROR;
}

HRESULT CToolView::Ready_Camera_Info()
{
	CToolCamera*	pCameraTool = nullptr;
	if (FAILED(Ready_Layer_Object(L"Prototype_Camera", L"Layer_Camera", (CGameObject**)&pCameraTool)))
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

	if (FAILED(pCameraTool->SetUp_CameraInfo(CamDesc, ProjDesc)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CToolView::Ready_LightInfo()
{
	D3DLIGHT9			LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = LightInfo.Diffuse;
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(m_pGraphic_Device, &LightInfo)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CToolView::Ready_Layer_Object(const _tchar * pPrototypeTag, const _tchar * pLayerTag, CGameObject** ppGameObject)
{
	if (FAILED(CObject_Manager::GetInstance()->Add_Object(0, pPrototypeTag, 0, pLayerTag, ppGameObject)))
		return E_FAIL;



	return NOERROR;
}
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (nullptr == m_pGraphic_Device)
		return;

	CObject_Manager::GetInstance()->Update_Object_Manager(0.f);
	CObject_Manager::GetInstance()->LastUpdate_Object_Manager(0.f);

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	m_pGraphic_Device->BeginScene();

	if (nullptr != m_pRenderer)
		m_pRenderer->Render_Renderer();

	m_pGraphic_Device->EndScene();
	m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	SetTimer(MY_TIMER, 30, 0);

	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(m_hWnd, CGraphic_Device::MODE_WIN, g_iBackCX, g_iBackCY, &m_pGraphic_Device)))
		return;
	// For.Input_Device

	HINSTANCE hInst = AfxGetInstanceHandle();
	if (FAILED(CInput_Device::GetInstance()->Ready_Input_Device(hInst, m_hWnd)))
		return;

	CComponent_Manager::GetInstance()->Reserve_Component_Manager(1);
	CObject_Manager::GetInstance()->Reserve_Object_Manager(1);

	if (FAILED(Ready_LightInfo()))
		return;

	if (FAILED(Ready_Componet_Prototype()))
		return;
	if (FAILED(Ready_GameObject_Prototype()))
		return;
	if (FAILED(Ready_Camera_Info()))
		return;


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nIDEvent == MY_TIMER)
	{
		CObject_Manager::GetInstance()->Update_Object_Manager(0.f);
		CObject_Manager::GetInstance()->LastUpdate_Object_Manager(0.f);
		CInput_Device::GetInstance()->Inquire_Input_State();

		m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
		m_pGraphic_Device->BeginScene();

		if (nullptr != m_pRenderer)
			m_pRenderer->Render_Renderer();

		m_pGraphic_Device->EndScene();
		m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);
	}


	CView::OnTimer(nIDEvent);
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	CView::OnLButtonDown(nFlags, point);
}


void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CGameObject* pTerrain = const_cast<CGameObject*>(CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0));
	if (nullptr != pTerrain)
	{
		if (STATE_ADD_OBJECT == ((CToolTerrain*)pTerrain)->Get_MouseState())
		{
			_vec3 PickingPoint = ((CToolTerrain*)pTerrain)->Get_PickingPoint();
			CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->Set_PickingPoint(PickingPoint);
		}
		if (STATE_ADD_NAV == ((CToolTerrain*)pTerrain)->Get_MouseState())
		{
			vector<_vec3>* pVecPoint = ((CToolTerrain*)pTerrain)->Get_vecPoint();
			CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->Set_VecPointNav(pVecPoint);
		}
		if (STATE_ADD_CAM == ((CToolTerrain*)pTerrain)->Get_MouseState())
		{
			_vec3 PickingPoint = ((CToolTerrain*)pTerrain)->Get_PickingPoint();
			CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->Add_CamPoint(PickingPoint);
		}
	}
	CView::OnLButtonUp(nFlags, point);
}
