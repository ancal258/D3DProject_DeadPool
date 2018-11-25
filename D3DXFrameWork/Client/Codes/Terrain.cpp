#include "stdafx.h"
#include "..\Headers\Terrain.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Input_Device.h"
#include "Camera_Debug.h"

_USING(Client)

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{


}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
	, m_MtrlInfo(rhs.m_MtrlInfo)
{

}

HRESULT CTerrain::Ready_GameObject_Prototype()
{
	ZeroMemory(&m_MtrlInfo, sizeof(D3DMATERIAL9));

	return NOERROR;
}

HRESULT CTerrain::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;	

	D3DXCreateTextureFromFileEx(Get_Graphic_Device(), L"../Bin/Splatting.bmp", 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &m_pFilterTexture);



	m_MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	m_MtrlInfo.Emissive = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_MtrlInfo.Power = 40.0f;



	return NOERROR;
}

_int CTerrain::Update_GameObject(const _float & fTimeDelta)
{

	CObject_Manager*		pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return -1;
	pObject_Manager->AddRef();

	// 카메라객체의 주소를 얻어온다.
	//CCamera_Debug*	pCamera = (CCamera_Debug*)pObject_Manager->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 0);
	//if (nullptr == pCamera)
	//	return -1;

	//pCamera->Culling_ToQuadTree(m_pTransformCom, m_pBufferCom);

	Safe_Release(pObject_Manager);

	return _int();
}

_int CTerrain::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))

		return -1;

	return _int();
}

void CTerrain::Render_GameObject()
{
	if (nullptr == m_pBufferCom || 
		nullptr == m_pShaderCom)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;


	pEffect->Begin(nullptr, 0);

	static _uint	iPass = 1;

	if (GetKeyState(VK_NUMPAD0) & 0x8000)	
		iPass = 0;
	
	else if (GetKeyState(VK_NUMPAD1) & 0x8000)
		iPass = 1;

	pEffect->BeginPass(iPass);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CTerrain::Ready_Component()
{
	CComponent_Manager*			pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Com_Transform
	m_pTransformCom = (CTransform*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	m_pRendererCom = (CRenderer*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	// For.Com_Buffer
	m_pBufferCom = (CBuffer_Terrain*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Buffer_Terrain");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Terrain");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Terrain");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	// For.Com_Filter
	m_pFilterCom = (CTexture*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Filter");
	if (FAILED(Add_Component(L"Com_Filter", m_pFilterCom)))
		return E_FAIL;

	// For.Com_Picking
	m_pPickingCom = (CPicking*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Picking");
	if (FAILED(Add_Component(L"Com_Picking", m_pPickingCom)))
		return E_FAIL;


	


	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CTerrain::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");
	
	_matrix			matView, matProj;
	
	Get_Transform(D3DTS_VIEW, &matView);
	Get_Transform(D3DTS_PROJECTION, &matProj);
	
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);	

	// For.LightInfo
	const D3DLIGHT9* pLightInfo = CLight_Manager::GetInstance()->Get_LightInfo(0);
	if (nullptr == pLightInfo)
		return E_FAIL;

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&m_MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&m_MtrlInfo.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&m_MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", m_MtrlInfo.Power);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	pEffect->SetVector("g_vBrushPosition", &_vec4(20.f, 0.f, 10.f, 1.f));
	pEffect->SetFloat("g_fRadius", 5.0f);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_SourTexture", 0);
	m_pTextureCom->SetUp_OnShader(pEffect, "g_DestTexture1", 1);
	m_pTextureCom->SetUp_OnShader(pEffect, "g_DestTexture2", 2);
	m_pTextureCom->SetUp_OnShader(pEffect, "g_DestTexture3", 3);
	// m_pFilterCom->SetUp_OnShader(pEffect, "g_FilterTexture");
	pEffect->SetTexture("g_FilterTexture", m_pFilterTexture);



	Safe_Release(pEffect);

	return NOERROR;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain*		pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CTerrain Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone_GameObject()
{
	CTerrain*		pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CTerrain Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	Safe_Release(m_pPickingCom);
	Safe_Release(m_pFilterTexture);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pFilterCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
