#include "stdafx.h"
#include "../Headers/Item_Bullet.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "Light_Manager.h"
#include "Player.h"

CItem_Bullet::CItem_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CItem(pGraphic_Device)
{
}

CItem_Bullet::CItem_Bullet(const CItem_Bullet & rhs)
	: CItem(rhs)
{
}

void CItem_Bullet::Set_StateInfo(_vec3 * vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);
}

HRESULT CItem_Bullet::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CItem_Bullet::Ready_GameObject()
{

	if (FAILED(CItem::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.01f, 0.01f, 0.01f);

	return NOERROR;
}

_int CItem_Bullet::Update_GameObject(const _float & fTimeDelta)
{

	return CItem::Update_GameObject(fTimeDelta);
}

_int CItem_Bullet::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return CItem::LastUpdate_GameObject(fTimeDelta);
}

void CItem_Bullet::Render_GameObject()
{
	CItem::Render_GameObject();
}

HRESULT CItem_Bullet::Ready_Component()
{
	CComponent_Manager*			pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Com_Mesh
	m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_ItemBullet");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);
	return NOERROR;
}

CItem_Bullet * CItem_Bullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_Bullet*		pInstance = new CItem_Bullet(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CItem_Bullet Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_Bullet::Clone_GameObject()
{
	CItem_Bullet*		pInstance = new CItem_Bullet(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CItem_Bullet Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Bullet::Free()
{
	Safe_Release(m_pMeshCom);
	CItem::Free();
}
