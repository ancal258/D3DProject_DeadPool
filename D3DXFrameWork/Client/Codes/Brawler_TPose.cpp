#include "stdafx.h"
#include "..\Headers\Brawler_TPose.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Brawler_Knife.h"
CBrawler_TPose::CBrawler_TPose(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CBrawler(pGraphic_Device)
{
}

CBrawler_TPose::CBrawler_TPose(const CBrawler_TPose & rhs)
	: CBrawler(rhs)
{
}

HRESULT CBrawler_TPose::Ready_GameObject_Prototype()
{
	if (FAILED(CBrawler::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CBrawler_TPose::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	if (FAILED(CBrawler::Ready_GameObject()))
		return E_FAIL;

	CGameObject* pKnife = nullptr;
	if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_Brawler_Knife", SCENE_STAGE, L"Layer_Brawler_Knife", &pKnife)))
		return E_FAIL;
	if (nullptr != pKnife)
		dynamic_cast<CBrawler_Knife*>(pKnife)->SetUp_ParentPointer(this);

	m_pTransformCom->Scaling(0.05f, 0.05f, 0.05f);
	m_pTransformCom->Set_AngleY(D3DXToRadian(rand() % 180));
	_vec3 vPosition = _vec3(rand() % 45 + 10, 0.f, rand() % 45 + 10);


	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);

	m_pMeshCom->Set_AnimationSet(0);

	return NOERROR;
}

_int CBrawler_TPose::Update_GameObject(const _float & fTimeDelta)
{
	m_pMeshCom->Play_AnimationSet(fTimeDelta);

	return CBrawler::Update_GameObject(fTimeDelta);
}

_int CBrawler_TPose::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return CBrawler::LastUpdate_GameObject(fTimeDelta);
}

void CBrawler_TPose::Render_GameObject()
{


	CBrawler::Render_GameObject();
}

HRESULT CBrawler_TPose::Ready_Component()
{
	CComponent_Manager*         pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;
	pComponent_Manager->AddRef();

	// For.Com_Mesh
	m_pMeshCom = (CMesh_Dynamic*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Brawler_TPose");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

CBrawler_TPose * CBrawler_TPose::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBrawler_TPose*      pInstance = new CBrawler_TPose(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBrawler01 Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBrawler_TPose::Clone_GameObject()
{
	CBrawler_TPose*      pInstance = new CBrawler_TPose(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBrawler_TPose Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBrawler_TPose::Free()
{
	Safe_Release(m_pMeshCom);

	CBrawler::Free();
}
