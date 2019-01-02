#include "stdafx.h"
#include "..\Headers/Brawler03.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Input_Device.h"
#include "Camera_Minigun.h"
#include "MissionCube.h"
#include "Brawler_Solution.h"
#include "Parent_Effect.h"
_USING(Client)

CBrawler03::CBrawler03(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CBrawler03::CBrawler03(const CBrawler03 & rhs)
	: CGameObject(rhs)
{

}

void CBrawler03::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPos);
}

HRESULT CBrawler03::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CBrawler03::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	m_pTransformCom->Scaling(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_AngleY(D3DXToRadian(m_fAngle));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(3, 1.f, 3));
	//m_pTransformCom->Go_Straight(0.8,1);

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

	if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_Brawler_Solution", SCENE_STAGE, L"Layer_Brawler_Solution", &m_pWeapon)))
		return E_FAIL;
	if (nullptr != m_pWeapon)
		dynamic_cast<CBrawler_Solution*>(m_pWeapon)->SetUp_ParentPointer(this);

	m_pMeshCom->Set_AnimationSet(0);
	return NOERROR;
}

_int CBrawler03::Update_GameObject(const _float & fTimeDelta)
{

	m_Hit = FALSE;

	if (FAILED(CInput_Device::GetInstance()->Picking_ToCollider(m_pColliderMesh, m_pTransformCom, &m_Hit, &m_fDist)))
		return -1;

	Update_HandMatrix();

	m_pMeshCom->Set_AnimationSet(0);

	m_pMeshCom->Play_AnimationSet(fTimeDelta);


	return _int();
}

_int CBrawler03::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;




	if (TRUE == m_Hit)
	{
		if (true == CInput_Device::GetInstance()->Is_MinDist(m_fDist))
		{
			CGameObject* pEffect;
			_vec3 vPos = (_vec3)m_pTransformCom->Get_WorldMatrix()->m[3];
			vPos.y += 1.f;
			if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_Parent_BloodExplosion", SCENE_STAGE, L"Layer_Effect", &pEffect)))
				return E_FAIL;
			((CParent_Effect*)pEffect)->Set_EffectInfo(16, 16, 0, 1.f, 0, &_vec3(3, 3, 3), &vPos, &_vec3(1, 0, 1), &_vec3(0, 0, 0), true, false, 0.f, 1, 1);

			if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_Parent_BloodSplat", SCENE_STAGE, L"Layer_Effect", &pEffect)))
				return E_FAIL;
			((CParent_Effect*)pEffect)->Set_EffectInfo(0, 0, 0, 1.f, 0, &_vec3(2, 2, 2), &vPos, &_vec3(1, 0, 1), &_vec3(0, 0, 0), false, true, 0.f, 4, 1);

			if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_Parent_BloodCloud", SCENE_STAGE, L"Layer_Effect", &pEffect)))
				return E_FAIL;
			((CParent_Effect*)pEffect)->Set_EffectInfo(0, 0, 0.007, 1.f, 0, &_vec3(2, 2, 2), &vPos, &_vec3(1, 0, 1), &_vec3(0, 1, 0), false, true, 0.f, 4, 1);

			// ÇÇ ÀÌÆåÆ®
			m_pWeapon->Set_Lived(false);
			Set_Lived(false);
		}
	}
	
	m_pTransformCom->Update_Matrix();

	CCamera_Minigun*	pCamera = (CCamera_Minigun*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 1);
	if (nullptr == pCamera)
		return -1;
	if (false == pCamera->Culling_ToFrustum(m_pTransformCom, nullptr, 150.f))
	{
		if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
			return -1;
	}

	return _int();
}

void CBrawler03::Render_GameObject()
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


}


HRESULT CBrawler03::Ready_Component()
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
	m_pMeshCom = (CMesh_Dynamic*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Brawler03");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// Form.Com_Collider
	m_pColliderCom = (CCollider*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collider_Box");
	if (FAILED(Add_Component(L"Com_Collider", m_pColliderCom)))
		return E_FAIL;
	m_pColliderCom->SetUp_Collider(m_pTransformCom->Get_WorldMatrix(), &_vec3(50, 140, 50), &_vec3(0.0f, 0.f, 0.f), &_vec3(0.f, 70.f, 0.f));
	m_pColliderMesh = m_pColliderCom->Get_Mesh();
	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CBrawler03::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CBrawler03::Update_HandMatrix()
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

CBrawler03 * CBrawler03::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBrawler03*      pInstance = new CBrawler03(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CPlayer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBrawler03::Clone_GameObject()
{
	CBrawler03*      pInstance = new CBrawler03(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBrawler03 Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBrawler03::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}