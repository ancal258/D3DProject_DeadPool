#include "stdafx.h"
#include "..\Headers/BikiniGirl.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Input_Device.h"
#include "Camera_Cinematic.h"
#include "Camera_Debug.h"
#include "Camera_Target.h"
#include "Player.h"

_USING(Client)

CBikiniGirl::CBikiniGirl(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CBikiniGirl::CBikiniGirl(const CBikiniGirl  & rhs)
	: CGameObject(rhs)
{

}

HRESULT CBikiniGirl::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CBikiniGirl::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	m_pTransformCom->Scaling(0.01f, 0.01f, 0.01f);
	//m_pTransformCom->Set_AngleY(D3DXToRadian(0));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(43.5637f, 0.f, 12.5f));
	//m_pTransformCom->Go_Straight(0.8,1);


	m_pPlayer = (CPlayer*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);
	if (nullptr == m_pPlayer)
		return E_FAIL;


	m_pMeshCom->Set_AnimationSet(m_iIndex);
	return NOERROR;
}

_int CBikiniGirl::Update_GameObject(const _float & fTimeDelta)
{

	if (true == m_pColliderCom->Collision_OBB((const CCollider*)m_pPlayer->Get_ComponentPointer(L"Com_Collider")))
	{
		// 이벤트 처리.
		m_pMeshCom->Set_AnimationSet(STATE_HELP);
		m_isCol = true;
	}
	else
	{
		m_pMeshCom->Set_AnimationSet(STATE_IDLE);
	}


	m_pMeshCom->Play_AnimationSet(fTimeDelta);


	return _int();
}

_int CBikiniGirl::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;




	m_pTransformCom->Update_Matrix();


	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
		return -1;


	return _int();
}

void CBikiniGirl::Render_GameObject()
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


HRESULT CBikiniGirl::Ready_Component()
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
	m_pMeshCom = (CMesh_Dynamic*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_BikiniGirl");
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
	m_pColliderCom->SetUp_Collider(m_pTransformCom->Get_WorldMatrix(), &_vec3(100, 100, 100), &_vec3(0.0f, 0.f, 0.f), &_vec3(0.f, 25.f, 250.f));

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CBikiniGirl::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

CBikiniGirl * CBikiniGirl::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBikiniGirl*      pInstance = new CBikiniGirl(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CPlayer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBikiniGirl::Clone_GameObject()
{
	CBikiniGirl*      pInstance = new CBikiniGirl(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBikiniGirl Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBikiniGirl::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}