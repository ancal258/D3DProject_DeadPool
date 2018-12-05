#include "stdafx.h"
#include "..\Headers\Brawler01.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Brawler_Knife.h"
CBrawler01::CBrawler01(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CBrawler(pGraphic_Device)
{
}

CBrawler01::CBrawler01(const CBrawler01 & rhs)
	:CBrawler(rhs)
{
}

HRESULT CBrawler01::Ready_GameObject_Prototype()
{
	if (FAILED(CBrawler::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CBrawler01::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	if (FAILED(CBrawler::Ready_GameObject()))
		return E_FAIL;

	CGameObject* pKnife = nullptr;
	if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_Brawler_Knife", SCENE_STAGE, L"Layer_Brawler_Knife",&pKnife)))
		return E_FAIL;
	if(nullptr != pKnife)
		dynamic_cast<CBrawler_Knife*>(pKnife)->SetUp_ParentPointer(this);

	m_pTransformCom->Scaling(0.05f, 0.05f, 0.05f);
	m_pTransformCom->Set_AngleY(D3DXToRadian(rand() % 180));
	_vec3 vPosition = _vec3(rand() % 60 + 10, 0.f, rand() % 60 + 10);


	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);

	m_iIdleIndex = 0;
	m_pMeshCom->RegistCallbackFunc(bind(&CBrawler::CallBackFinish, this));
	m_pMeshCom->Set_AnimationSet(m_iIdleIndex);

	return NOERROR;
}

_int CBrawler01::Update_GameObject(const _float & fTimeDelta)
{



	return CBrawler::Update_GameObject(fTimeDelta);
}

_int CBrawler01::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (true == m_isDamaged)
	{
		m_iCurrentIndex = 4;
	}

	m_pMeshCom->Set_AnimationSet(m_iCurrentIndex);

	m_pMeshCom->Play_AnimationSet(fTimeDelta);
	return CBrawler::LastUpdate_GameObject(fTimeDelta);
}

void CBrawler01::Render_GameObject()
{


	CBrawler::Render_GameObject();
}

HRESULT CBrawler01::Ready_Component()
{
	CComponent_Manager*         pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;
	pComponent_Manager->AddRef();

	// For.Com_Mesh
	m_pMeshCom = (CMesh_Dynamic*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Brawler01");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

CBrawler01 * CBrawler01::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBrawler01*      pInstance = new CBrawler01(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBrawler01 Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBrawler01::Clone_GameObject()
{
	CBrawler01*      pInstance = new CBrawler01(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBrawler01 Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBrawler01::Free()
{
	Safe_Release(m_pMeshCom);

	CBrawler::Free();
}
