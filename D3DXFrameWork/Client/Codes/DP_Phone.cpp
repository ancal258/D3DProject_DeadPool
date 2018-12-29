#include "stdafx.h"
#include "..\Headers\DP_Phone.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Player.h"
#include "Effect_Trail.h"
_USING(Client)

CDP_Phone::CDP_Phone(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CDP_Phone::CDP_Phone(const CDP_Phone & rhs)
	: CGameObject(rhs)
	, m_iSide(rhs.m_iSide)
{
}

HRESULT CDP_Phone::Ready_GameObject_Prototype(_uint iSide)
{
	m_iSide = iSide;
	return NOERROR;
}

HRESULT CDP_Phone::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(SetUp_PlayerPointer()))
		return E_FAIL;

	//if (0 == m_iSide)
	//	m_pTransformCom->Set_AngleZ(D3DXToRadian(90.0f));
	//else if (1 == m_iSide)
	m_pTransformCom->Set_AngleY(D3DXToRadian(180.0f));
	m_pTransformCom->Set_AngleX(D3DXToRadian(180.0f));
	m_isActive = false;
	return NOERROR;
}

_int CDP_Phone::Update_GameObject(const _float & fTimeDelta)
{

	return _int();
}

_int CDP_Phone::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	if (m_isActive == false)
		return 0;
	m_pTransformCom->Set_ParentMatrix(m_pPlayer->Get_HandMatrix(m_iSide));

	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_PLAYER, this)))
		return -1;


	return _int();
}

void CDP_Phone::Render_GameObject()
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

	//m_pColliderCom->Render_Collider();
	//for (size_t i = 0; i < 5; i++)
	//{
	//	m_pColliderCom_Sphere[i]->Render_Collider();
	//}
}

HRESULT CDP_Phone::Ready_Component()
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

	// For.Com_Mesh
	m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Phone");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;


	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CDP_Phone::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	pEffect->SetVector("g_vMtrlSpecular", &_vec4(1.f, 1.f, 1.f, 1.f)); // Ȯ���ϱ� ���� 1,1,1,1 �� �ʱ�ȭ �ؼ� ó��. (���������� ���)
	pEffect->SetFloat("g_fPower", 30.0f);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);


	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CDP_Phone::SetUp_PlayerPointer()
{
	CObject_Manager*			pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return E_FAIL;

	pObject_Manager->AddRef();

	m_pPlayer = (CPlayer*)pObject_Manager->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);
	if (nullptr == m_pPlayer)
		return E_FAIL;

	Safe_Release(pObject_Manager);

	return NOERROR;
}

CDP_Phone * CDP_Phone::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iSide)
{
	CDP_Phone*		pInstance = new CDP_Phone(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype(iSide)))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDP_Phone::Clone_GameObject()
{
	CDP_Phone*		pInstance = new CDP_Phone(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CDP_Phone Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDP_Phone::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	CGameObject::Free();
}
