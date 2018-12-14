#include "stdafx.h"
#include "..\Headers\Effect_Mesh.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"
#include "Airplane.h"
#include "Static_Airplane.h"
_USING(Client)

CEffect_Mesh::CEffect_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CEffect_Mesh::CEffect_Mesh(const CEffect_Mesh & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Mesh::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEffect_Mesh::Ready_GameObject()
{

	if (FAILED(CEffect_Mesh::Ready_Component()))
		return E_FAIL;


	m_pTransformCom->Scaling(0.03f, 0.03f, 0.03f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(15, 3, 10));


	return NOERROR;
}

_int CEffect_Mesh::Update_GameObject(const _float & fTimeDelta)
{
	m_fU += fTimeDelta;
	//m_fV += fTimeDelta;

	m_pTransformCom->Update_Matrix();

	return _int();
}

_int CEffect_Mesh::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;


	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_ALPHA, this)))
		return -1;
	return _int();
}

void CEffect_Mesh::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return;



	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);


}

HRESULT CEffect_Mesh::Ready_Component()
{
	CComponent_Manager*         pComponent_Manager = CComponent_Manager::GetInstance();
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

	// For.Com_Mesh
	m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_EffectSwordHeavy03R");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Navigation
	m_pNavigationCom = (CNavigation*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Navigation");
	if (FAILED(Add_Component(L"Com_Navigation", m_pNavigationCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CEffect_Mesh::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix         matView, matProj;

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

	pEffect->SetVector("g_vMtrlDiffuse", &_vec4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetVector("g_vMtrlAmbient", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
	pEffect->SetVector("g_vMtrlSpecular", &_vec4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetFloat("g_fPower", 20.f);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	pEffect->SetFloat("g_fU", m_fU);
	pEffect->SetFloat("g_fV", m_fV);

	Safe_Release(pEffect);

	return NOERROR;
}

CEffect_Mesh * CEffect_Mesh::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_Mesh*		pInstance = new CEffect_Mesh(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_Mesh::Clone_GameObject()
{
	CEffect_Mesh*		pInstance = new CEffect_Mesh(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CEffect_Mesh Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CEffect_Mesh::Update_HandMatrix()
{
	if (nullptr == m_pHandMatrix[0] ||
		nullptr == m_pHandMatrix[1] ||
		nullptr == m_pRootMatrix)
		return E_FAIL;

	m_CombinedHandMatrix[0] = *m_pHandMatrix[0] * *m_pTransformCom->Get_WorldMatrix();
	m_CombinedHandMatrix[1] = *m_pHandMatrix[1] * *m_pTransformCom->Get_WorldMatrix();
	m_CombinedRootMatrix = *m_pRootMatrix * *m_pTransformCom->Get_WorldMatrix();

	return NOERROR;
}

void CEffect_Mesh::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pMeshCom);

	CGameObject::Free();
}