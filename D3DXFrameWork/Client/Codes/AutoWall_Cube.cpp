#include "stdafx.h"
#include "AutoWall_Cube.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "StaticUI.h"
#include "Static_AutoWall.h"

CAutoWall_Cube::CAutoWall_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CAutoWall_Cube::CAutoWall_Cube(const CAutoWall_Cube & rhs)
	: CGameObject(rhs)
{
}

void CAutoWall_Cube::Set_StateInfo(_vec3 * vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);
}


HRESULT CAutoWall_Cube::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CAutoWall_Cube::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pAutoWall[0] = (CStatic_AutoWall*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_AutoWall", 0);
	m_pAutoWall[1] = (CStatic_AutoWall*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_AutoWall", 1);

	if (nullptr == m_pAutoWall[0] ||
		nullptr == m_pAutoWall[1])
		return E_FAIL;

	return NOERROR;
}

_int CAutoWall_Cube::Update_GameObject(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (false == m_isCol)
		return 0;


	if (true == m_pRendererCom->Get_SecondTrigger())
	{

	}

	return _int();
}

_int CAutoWall_Cube::LastUpdate_GameObject(const _float & fTimeDelta)
{



	const CGameObject* pPlayer = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);
	if (nullptr == pPlayer)
		return -1;

	// ������. ������ return true�� �� ���� Ư�� �ൿ�� ��������.

	if (false == m_isCol)
	{
		if (true == m_pColliderCom->Collision_OBB((const CCollider*)pPlayer->Get_ComponentPointer(L"Com_Collider")))
		{
			// �̺�Ʈ ó��.
			m_pAutoWall[0]->Set_OpenDoor(true);
			m_pAutoWall[1]->Set_OpenDoor(true);
			m_isCol = true;
		}
	}
	else
	{
		if (false == m_pColliderCom->Collision_OBB((const CCollider*)pPlayer->Get_ComponentPointer(L"Com_Collider")))
		{
			// �̺�Ʈ ó��.
			m_pAutoWall[0]->Set_OpenDoor(false);
			m_pAutoWall[1]->Set_OpenDoor(false);
			m_isCol = false;
		}
	}


	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_UI, this)))
		return -1;

	//Safe_Release(pObject_Manager);

	return _int();
}

void CAutoWall_Cube::Render_GameObject()
{
	m_pColliderCom->Render_Collider();


}

HRESULT CAutoWall_Cube::Ready_Component()
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
	m_pColliderCom->SetUp_Collider(m_pTransformCom->Get_WorldMatrix(), &_vec3(9, 9, 9), &_vec3(0, 0, 0), &_vec3(0, 3, 0));
	m_pMesh = m_pColliderCom->Get_Mesh();
	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CAutoWall_Cube::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	return NOERROR;
}

CAutoWall_Cube * CAutoWall_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAutoWall_Cube*		pInstance = new CAutoWall_Cube(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CAutoWall_Cube Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAutoWall_Cube::Clone_GameObject()
{
	CAutoWall_Cube*		pInstance = new CAutoWall_Cube(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CAutoWall_Cube Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAutoWall_Cube::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
	CGameObject::Free();
}
