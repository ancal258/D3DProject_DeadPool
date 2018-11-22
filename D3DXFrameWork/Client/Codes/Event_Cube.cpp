#include "stdafx.h"
#include "Event_Cube.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "Light_Manager.h"
#include "Player.h"

CEvent_Cube::CEvent_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CEvent_Cube::CEvent_Cube(const CEvent_Cube & rhs)
	: CGameObject(rhs)
{
}

void CEvent_Cube::Set_StateInfo(_vec3 * pRight, _vec3 * pUp, _vec3 * vLook, _vec3 * vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, pRight);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_UP, pUp);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, vLook);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);

}

HRESULT CEvent_Cube::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEvent_Cube::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	return NOERROR;
}

_int CEvent_Cube::Update_GameObject(const _float & fTimeDelta)
{

	return _int();
}

_int CEvent_Cube::LastUpdate_GameObject(const _float & fTimeDelta)
{



	const CGameObject* pPlayer = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);
	if (nullptr == pPlayer)
		return -1;

	// 디버깅용. 실제론 return true일 때 마다 특정 행동을 취해주자.

	if (false == m_isCol)
	{
		if (true == m_pColliderCom->Collision_OBB((const CCollider*)pPlayer->Get_ComponentPointer(L"Com_Collider")))
		{
			m_isCol = true;
			((CPlayer*)pPlayer)->Set_EventNum(m_iEventNum);
			((CPlayer*)pPlayer)->SetUp_CameraMove();
		}
	}


	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	//Safe_Release(pObject_Manager);

	return _int();
}

void CEvent_Cube::Render_GameObject()
{
	m_pColliderCom->Render_Collider();
}

HRESULT CEvent_Cube::Ready_Component()
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

	// For.Com_Collider
	m_pColliderCom = (CCollider*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collider_Box");
	if (FAILED(Add_Component(L"Com_Collider", m_pColliderCom)))
		return E_FAIL;
	m_pColliderCom->SetUp_Collider(m_pTransformCom->Get_WorldMatrix(), &_vec3(2, 2, 2), &_vec3(0, 0, 0), &_vec3(0, 0, 0));
	m_pMesh = m_pColliderCom->Get_Mesh();
	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CEvent_Cube::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	pEffect->SetVector("g_vMtrlSpecular", &_vec4(1.f, 1.f, 1.f, 1.f)); // 확인하기 위해 1,1,1,1 로 초기화 해서 처리. (재질정보가 없어서)
	pEffect->SetFloat("g_fPower", 30.0f);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	pEffect->SetBool("g_isCol", m_isCol);


	Safe_Release(pEffect);

	return NOERROR;
}

CEvent_Cube * CEvent_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEvent_Cube*		pInstance = new CEvent_Cube(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CEvent_Cube Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEvent_Cube::Clone_GameObject()
{
	CEvent_Cube*		pInstance = new CEvent_Cube(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CEvent_Cube Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEvent_Cube::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}
