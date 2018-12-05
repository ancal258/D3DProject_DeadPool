#include "stdafx.h"
#include "..\Headers\Brawler.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"

_USING(Client)

CBrawler::CBrawler(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CBrawler::CBrawler(const CBrawler & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBrawler::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CBrawler::Ready_GameObject()
{

	if (FAILED(CBrawler::Ready_Component()))
		return E_FAIL;
	m_pHandMatrix[0] = m_pMeshCom->Get_FrameMatrixByName("L_Weapon01_Wpn_XW");
	if (nullptr == m_pHandMatrix[0])
		return E_FAIL;
	m_pHandMatrix[1] = m_pMeshCom->Get_FrameMatrixByName("R_Weapon01_Wpn_XW");
	if (nullptr == m_pHandMatrix[1])
		return E_FAIL;
	m_pRootMatrix = m_pMeshCom->Get_FrameMatrixByName("C_Root_Reference_XR");
	if (nullptr == m_pRootMatrix)
		return E_FAIL;
	Update_HandMatrix();

	D3DXMatrixIdentity(&m_RealMatrix);

	return NOERROR;
}

_int CBrawler::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Update_HandMatrix();

	for (size_t i = 1; i < 3; i++)
	{
		const CGameObject* pPlayer = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", i);
		if (nullptr == pPlayer)
			break;

		// 디버깅용. 실제론 return true일 때 마다 특정 행동을 취해주자.
		if (true == m_pColliderCom_Body->Collision_Sphere((const CCollider*)pPlayer->Get_ComponentPointer(L"Com_Collider0")) ||
			true == m_pColliderCom_Body->Collision_Sphere((const CCollider*)pPlayer->Get_ComponentPointer(L"Com_Collider1")) ||
			true == m_pColliderCom_Body->Collision_Sphere((const CCollider*)pPlayer->Get_ComponentPointer(L"Com_Collider2")) ||
			true == m_pColliderCom_Body->Collision_Sphere((const CCollider*)pPlayer->Get_ComponentPointer(L"Com_Collider3")) ||
			true == m_pColliderCom_Body->Collision_Sphere((const CCollider*)pPlayer->Get_ComponentPointer(L"Com_Collider4")))
		{
			m_isDamaged = true;
		}
	}

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
		return -1;
	return _int();
}

_int CBrawler::LastUpdate_GameObject(const _float & fTimeDelta)
{


	m_pTransformCom->Update_Matrix();

	return _int();
}

void CBrawler::Render_GameObject()
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
	
	m_pColliderCom->Render_Collider();
	m_pColliderCom_Body->Render_Collider();

}

void CBrawler::CallBackFinish()
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &(_vec3)m_CombinedRootMatrix.m[3]);
	m_pTransformCom->Update_Matrix();
	m_isDamaged = false;
	m_iCurrentIndex = m_iIdleIndex;
	m_pMeshCom->Set_AnimationSet(m_iIdleIndex);
	
}

HRESULT CBrawler::Ready_Component()
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

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Navigation
	m_pNavigationCom = (CNavigation*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Navigation");
	if (FAILED(Add_Component(L"Com_Navigation", m_pNavigationCom)))
		return E_FAIL;

	// Form.Com_Collider
	m_pColliderCom = (CCollider*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collider_Box");
	if (FAILED(Add_Component(L"Com_Collider", m_pColliderCom)))
		return E_FAIL;
	m_pColliderCom->SetUp_Collider(&m_CombinedRootMatrix, &_vec3(50, 140, 50), &_vec3(0.0f, 0.f, 0.f), &_vec3(0.f, 70.f, 0.f));

	m_pColliderCom_Body = (CCollider*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collider_Sphere");
	if (FAILED(Add_Component(L"Com_Collider_Body", m_pColliderCom_Body)))
		return E_FAIL;
	m_pColliderCom_Body->SetUp_Collider(m_pTransformCom->Get_WorldMatrix(), &_vec3(60, 60, 60), &_vec3(0.0f, 0.f, 0.f), &_vec3(0.f, 100.f, 0.f));

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CBrawler::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CBrawler::Update_HandMatrix()
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

void CBrawler::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pColliderCom_Body);

	CGameObject::Free();
}
