#include "stdafx.h"
#include "..\Headers\Minigun.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"
#include "Airplane.h"

_USING(Client)

CMinigun::CMinigun(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CMinigun::CMinigun(const CMinigun & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMinigun::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CMinigun::Ready_GameObject()
{

	if (FAILED(CMinigun::Ready_Component()))
		return E_FAIL;

	m_pAirplane = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Airplane", 0);
	if (nullptr == m_pAirplane)
		return E_FAIL;

	m_matRotationY = m_pMeshCom->Get_FrameMatrix("L_Misc02_XM");
	if (nullptr == m_matRotationY)
		return E_FAIL;
	//m_pHandMatrix[1] = m_pMeshCom->Get_FrameMatrixByName("R_Weapon01_Wpn_XW");
	//if (nullptr == m_pHandMatrix[1])
	//	return E_FAIL;
	//m_pRootMatrix = m_pMeshCom->Get_FrameMatrixByName("C_Root_Reference_XR");
	//if (nullptr == m_pRootMatrix)
	//	return E_FAIL;
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(160, 100, 60));


	//Update_HandMatrix();
	m_pMeshCom->Set_AnimationSet(0);

	return NOERROR;
}

_int CMinigun::Update_GameObject(const _float & fTimeDelta)
{
	//_matrix	matRotZ;
	//D3DXMatrixRotationZ(&matRotZ, 180 * fTimeDelta);

	//D3DXVec3TransformNormal((_vec3*)&m_matRotationY->m[0][0], (_vec3*)&m_matRotationY->m[0][0], &matRotZ);
	//D3DXVec3TransformNormal((_vec3*)&m_matRotationY->m[1][0], (_vec3*)&m_matRotationY->m[1][0], &matRotZ);
	//D3DXVec3TransformNormal((_vec3*)&m_matRotationY->m[2][0], (_vec3*)&m_matRotationY->m[2][0], &matRotZ);

	//m_matRotationY = m_pMeshCom->Get_FrameMatrix("L_Misc02_XM");

	//Update_HandMatrix();






	return _int();
}

_int CMinigun::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Set_ParentMatrix(*dynamic_cast<const CAirplane*>(m_pAirplane)->Get_WorldMatrix());
	_vec3 vTest = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	m_pTransformCom->Update_Matrix();



	m_pMeshCom->Set_AnimationSet(0);
	m_pMeshCom->Play_AnimationSet(fTimeDelta);


	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
		return -1;
	return _int();
}

void CMinigun::Render_GameObject()
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
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);


}

HRESULT CMinigun::Ready_Component()
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
	m_pMeshCom = (CMesh_Dynamic*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Minigun");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Navigation
	m_pNavigationCom = (CNavigation*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Navigation");
	if (FAILED(Add_Component(L"Com_Navigation", m_pNavigationCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CMinigun::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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


	Safe_Release(pEffect);

	return NOERROR;
}

CMinigun * CMinigun::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMinigun*		pInstance = new CMinigun(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMinigun::Clone_GameObject()
{
	CMinigun*		pInstance = new CMinigun(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CMinigun Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CMinigun::Update_HandMatrix()
{

	return NOERROR;
}

void CMinigun::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pMeshCom);

	CGameObject::Free();
}
