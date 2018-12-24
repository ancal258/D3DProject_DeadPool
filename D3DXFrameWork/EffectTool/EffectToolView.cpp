
// EffectToolView.cpp : CEffectToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "EffectTool.h"
#endif

#include "EffectToolDoc.h"
#include "EffectToolView.h"

// My Include
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Timer_Manager.h"
#include "Frame_Manager.h"
// GameObject
#include "ToolEffect.h"
#include "ToolCamera_Effect.h"
#include "ToolBase.h"
#include "Effect_Mesh.h"
#include "Effect_RectParticle.h"
#include "Effect_BloodT.h"
#include "Effect_ExplosionT.h"
#include "Effect_CloudT.h"
#include "Effect_TexBloodT.h"
#include "Effect_BloodMistT.h"
#include "Effect_GunFlashT.h"
#include "Effect_GunCapT.h"
#include "Effect_CloudBrightT.h"
#include "ParentEffect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define MY_TIMER 10


// CEffectToolView

IMPLEMENT_DYNCREATE(CEffectToolView, CView)

BEGIN_MESSAGE_MAP(CEffectToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CEffectToolView ����/�Ҹ�

CEffectToolView::CEffectToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CEffectToolView::~CEffectToolView()
{
}

BOOL CEffectToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CEffectToolView �׸���

void CEffectToolView::OnDraw(CDC* /*pDC*/)
{
	CEffectToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CEffectToolView �μ�

BOOL CEffectToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CEffectToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CEffectToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CEffectToolView ����

#ifdef _DEBUG

void CEffectToolView::AssertValid() const
{
	CView::AssertValid();
}

void CEffectToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEffectToolDoc* CEffectToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEffectToolDoc)));
	return (CEffectToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CEffectToolView �޽��� ó����


void CEffectToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CTimer_Manager* pTimer_Manager = CTimer_Manager::GetInstance();

	CFrame_Manager* pFrame_Manager = CFrame_Manager::GetInstance();


	_float		fTimeDelta_Default = pTimer_Manager->Compute_Timer(L"Timer_Default");

	if (true == pFrame_Manager->Permit_Call(L"Frame_60", fTimeDelta_Default))
	{
		_float		fTimeDelta_60 = pTimer_Manager->Compute_Timer(L"Timer_60");
		CObject_Manager::GetInstance()->Update_Object_Manager(fTimeDelta_60);
		CObject_Manager::GetInstance()->LastUpdate_Object_Manager(fTimeDelta_60);
	}


	CInput_Device::GetInstance()->Inquire_Input_State();

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	m_pGraphic_Device->BeginScene();

	if (nullptr != m_pRenderer)
		m_pRenderer->Render_Renderer();

	m_pGraphic_Device->EndScene();
	m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);

	CView::OnTimer(nIDEvent);
}


void CEffectToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	SetTimer(MY_TIMER, 30, 0);

	CTimer_Manager* pTimer_Manager = CTimer_Manager::GetInstance();
	if (nullptr == pTimer_Manager)
		return;
	pTimer_Manager->AddRef();

	// For.Timer_Default
	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_Default")))
		return;

	// For.Timer_60
	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_60")))
		return;

	CFrame_Manager* pFrame_Manager = CFrame_Manager::GetInstance();
	if (nullptr == pFrame_Manager)
		return;
	pFrame_Manager->AddRef();

	// For.Frame_60
	if (FAILED(pFrame_Manager->Add_Frame(L"Frame_60", 200)))
		return;



	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(m_hWnd, CGraphic_Device::MODE_WIN, g_iBackCX, g_iBackCY, &m_pGraphic_Device)))
		return;
	// For.Input_Device

	HINSTANCE hInst = AfxGetInstanceHandle();
	if (FAILED(CInput_Device::GetInstance()->Ready_Input_Device(hInst, m_hWnd)))
		return;

	CComponent_Manager::GetInstance()->Reserve_Component_Manager(1);
	CObject_Manager::GetInstance()->Reserve_Object_Manager(1);	
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	// �Լ� ȣ�� ��

	// Light
	D3DLIGHT9			LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f);
	LightInfo.Ambient = LightInfo.Diffuse;
	LightInfo.Direction = _vec3(1.f, 1.f, 1.f);

	if (FAILED(CLight_Manager::GetInstance()->Add_Light(m_pGraphic_Device, &LightInfo)))
		return;
	//

	
	if (FAILED(Ready_Component()))
		return;

	if (FAILED(Ready_Prototype()))
		return;

	if (FAILED(Ready_Layer_Camera()))
		return;

	if (FAILED(Layer_Object()))
		return;


}

HRESULT CEffectToolView::Ready_Layer_Object(const _tchar * pPrototypeTag, const _tchar * pLayerTag, CGameObject** ppGameObject)
{
	if (FAILED(CObject_Manager::GetInstance()->Add_Object(0, pPrototypeTag, 0, pLayerTag, ppGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CEffectToolView::Ready_Component()
{
	CComponent_Manager*		pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;
	pComponent_Manager->AddRef();
	// For.Component_Buffer_RcTex
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Buffer_RcTex", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Buffer_RcCol
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Buffer_RcCol", CBuffer_RcCol::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Renderer	
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_pRenderer->AddRef();
	// For.Component_Transform
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Transform", CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Shader_Mesh
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Shader_Mesh", CShader::Create(m_pGraphic_Device, L"Bin/ShaderFiles/Shader_Mesh.fx"))))
		return E_FAIL;
	// For.Component_Shader_Collider
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Shader_Collider", CShader::Create(m_pGraphic_Device, L"Bin/ShaderFiles/Shader_Collider.fx"))))
		return E_FAIL;
	// For.Component_Shader_Effect
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Shader_Effect", CShader::Create(m_pGraphic_Device, L"Bin/ShaderFiles/Shader_Effect.fx"))))
		return E_FAIL;
	// For.Component_Shader_Prop
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Shader_Prop", CShader::Create(m_pGraphic_Device, L"Bin/ShaderFiles/Shader_Prop.fx"))))
		return E_FAIL;

	/////////////////////////////////////////////////////////////////////
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Texture_Effect", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Client/Bin/Resources/Textures/Explosion/Explosion%d.png", 90))))
		return E_FAIL;
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Texture_BloodAnim", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Client/Bin/Resources/Textures/Effect/BloodAnim/Blood_Anim_%d.png", 24))))
		return E_FAIL;
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Texture_Blood", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Client/Bin/Resources/Textures/Effect/Blood/blood_4x4_1_subUV_%d.png", 16))))
		return E_FAIL;
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Texture_BloodMist", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Client/Bin/Resources/Textures/Effect/Blood/blood_surface_%d.tga", 2))))
		return E_FAIL;
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Texture_Explosion", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Client/Bin/Resources/Textures/Effect/Explosion/8x8_anim_Explosion_%d.tga", 64))))
		return E_FAIL;
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Texture_ExplosionCloud", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Client/Bin/Resources/Textures/Effect/ExplosionCloud/4x4_Clouds_Dark_CLR_%d.tga", 16))))
		return E_FAIL;
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Texture_BrightCloud", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Client/Bin/Resources/Textures/Effect/CloudBright/4x4_Smoke_Clouds_TXT_%d.tga", 16))))
		return E_FAIL;
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Texture_Base", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"Bin/Textures/BaseTexture.png"))))
		return E_FAIL;

	// For.Component_Mesh_EffectSwordHeavy03R
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Mesh_EffectSwordHeavy03R", CMesh_Static::Create(m_pGraphic_Device, L"../Client/Bin/Resources/Meshes/Effect/Sword/", L"Sword_Heavy_03R.x"))))
		return E_FAIL;


	// For.Component_Mesh_GunCap
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Mesh_GunCap", CMesh_Static::Create(m_pGraphic_Device, L"../Client/Bin/Resources/Meshes/Effect/Gun/", L"Gun_Cap.x"))))
		return E_FAIL;

	// For.Component_Mesh_GunFlash
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Mesh_GunFlash", CMesh_Static::Create(m_pGraphic_Device, L"../Client/Bin/Resources/Meshes/Effect/Gun/", L"Gun_Flash.x"))))
		return E_FAIL;

	// For.Component_Mesh_MinigunCap
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Mesh_MinigunCap", CMesh_Static::Create(m_pGraphic_Device, L"../Client/Bin/Resources/Meshes/Effect/MiniGun/", L"Minigun_Cap.x"))))
		return E_FAIL;

	// For.Component_Mesh_MinigunFlash
	if (FAILED(pComponent_Manager->Add_Component(0, L"Component_Mesh_MinigunFlash", CMesh_Static::Create(m_pGraphic_Device, L"../Client/Bin/Resources/Meshes/Effect/MiniGun/", L"Minigun_Flash.x"))))
		return E_FAIL;


	Safe_Release(pComponent_Manager);


	return NOERROR;
}

HRESULT CEffectToolView::Ready_Prototype()
{

	CObject_Manager*		pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return E_FAIL;
	pObject_Manager->AddRef();

	// For. Camera
	if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(0, L"Prototype_Camera", CToolCamera_Effect::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For. Prototype_ToolBase
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_ToolBase", CToolBase::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For. Effect_Explosion
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_Explosion", CToolEffect::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Prototype_EffectSwordHeavy03R
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_EffectSwordHeavy03R", CEffect_Mesh::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Prototype_EffectRectParticle
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_EffectRectParticle", CEffect_RectParticle::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Prototype_EffectBlood
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_EffectBlood", CEffect_BloodT::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Prototype_EffectExplosion
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_EffectExplosion", CEffect_ExplosionT::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Prototype_EffectCloud
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_EffectCloud", CEffect_CloudT::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Prototype_EffectCloud
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_EffectCloudBright", CEffect_CloudBrightT::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Prototype_EffectTexBlood
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_EffectTexBlood", CEffect_TexBloodT::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Prototype_EffectTexBlood
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_EffectBloodMist", CEffect_BloodMistT::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Prototype_EffectGunFlash
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_EffectGunFlash", CEffect_GunFlashT::Create(m_pGraphic_Device))))
		return E_FAIL;
	// For.Prototype_EffectGunCap
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_EffectGunCap", CEffect_GunCapT::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Prototype_ParentEffect
	if (FAILED(pObject_Manager->Add_Object_Prototype(0, L"Prototype_ExplosionParent", CParentEffect::Create(m_pGraphic_Device, L"Prototype_Explosion"))))
		return E_FAIL;




	Safe_Release(pObject_Manager);
	return NOERROR;
}

HRESULT CEffectToolView::Ready_Layer_Camera()
{
	CToolCamera_Effect*		pCameraTool = nullptr;
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

HRESULT CEffectToolView::Layer_Object()
{
	if (FAILED(Ready_Layer_Object(L"Prototype_ToolBase", L"Layer_Background", nullptr)))
		return E_FAIL;

	//for (size_t i = 0; i < 10; i++)
	//{
	//	if (FAILED(Ready_Layer_Object(L"Prototype_Explosion", L"Layer_Effect", nullptr)))
	//		return E_FAIL;

	//}

	return NOERROR;
}
