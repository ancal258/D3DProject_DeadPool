#include "stdafx.h"
#include "..\Headers\Brawler02.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Brawler_ElectricBaton.h"
CBrawler02::CBrawler02(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CBrawler(pGraphic_Device)
{
}

CBrawler02::CBrawler02(const CBrawler02 & rhs)
	: CBrawler(rhs)
{
}

HRESULT CBrawler02::Ready_GameObject_Prototype()
{
	if (FAILED(CBrawler::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CBrawler02::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	if (FAILED(CBrawler::Ready_GameObject()))
		return E_FAIL;

	CGameObject* pElectricBaton = nullptr;
	if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_Brawler_ElectricBaton", SCENE_STAGE, L"Layer_Brawler_ElectricBaton", &pElectricBaton)))
		return E_FAIL;
	if (nullptr != pElectricBaton)
		dynamic_cast<CBrawler_ElectricBaton*>(pElectricBaton)->SetUp_ParentPointer(this);

	m_pTransformCom->Scaling(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_AngleY(D3DXToRadian(rand() % 180));
	_vec3 vPosition = _vec3(rand() % 75 + 10, 0.f, rand() % 75 + 10);


	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);

	m_iIdleIndex = 0;

	m_pMeshCom->Set_AnimationSet(m_iIdleIndex);

	return NOERROR;
}

_int CBrawler02::Update_GameObject(const _float & fTimeDelta)
{
	m_pMeshCom->Play_AnimationSet(fTimeDelta);

	return CBrawler::Update_GameObject(fTimeDelta);
}

_int CBrawler02::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return CBrawler::LastUpdate_GameObject(fTimeDelta);
}

void CBrawler02::Render_GameObject()
{


	CBrawler::Render_GameObject();
}

HRESULT CBrawler02::Ready_Component()
{
	CComponent_Manager*         pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;
	pComponent_Manager->AddRef();

	// For.Com_Mesh
	m_pMeshCom = (CMesh_Dynamic*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Brawler02");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

CBrawler02 * CBrawler02::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBrawler02*      pInstance = new CBrawler02(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBrawler01 Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBrawler02::Clone_GameObject()
{
	CBrawler02*      pInstance = new CBrawler02(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CBrawler02 Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBrawler02::Free()
{
	Safe_Release(m_pMeshCom);

	CBrawler::Free();
}
