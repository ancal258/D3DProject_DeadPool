#include "stdafx.h"
#include "../Headers/Item_Taco.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "Light_Manager.h"
#include "Player.h"

CItem_Taco::CItem_Taco(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CItem(pGraphic_Device)
{
}

CItem_Taco::CItem_Taco(const CItem_Taco & rhs)
	: CItem(rhs)
{
}

void CItem_Taco::Set_StateInfo(_vec3 * vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);
}

HRESULT CItem_Taco::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CItem_Taco::Ready_GameObject()
{

	if (FAILED(CItem::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.01f, 0.01f, 0.01f);

	return NOERROR;
}

_int CItem_Taco::Update_GameObject(const _float & fTimeDelta)
{

	return CItem::Update_GameObject(fTimeDelta);
}

_int CItem_Taco::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return CItem::LastUpdate_GameObject(fTimeDelta);
}

void CItem_Taco::Render_GameObject()
{
	CItem::Render_GameObject();
}

HRESULT CItem_Taco::Ready_Component()
{
	CComponent_Manager*			pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Com_Mesh
	m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_ItemTaco");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);
	return NOERROR;
}

CItem_Taco * CItem_Taco::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_Taco*		pInstance = new CItem_Taco(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CItem_Taco Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_Taco::Clone_GameObject()
{
	CItem_Taco*		pInstance = new CItem_Taco(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CItem_Taco Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Taco::Free()
{
	Safe_Release(m_pMeshCom);
	CItem::Free();
}
