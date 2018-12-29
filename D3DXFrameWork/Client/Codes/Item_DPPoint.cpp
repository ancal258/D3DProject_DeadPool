#include "stdafx.h"
#include "../Headers/Item_DPPoint.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "Light_Manager.h"
#include "Player.h"

CItem_DPPoint::CItem_DPPoint(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CItem(pGraphic_Device)
{
}

CItem_DPPoint::CItem_DPPoint(const CItem_DPPoint & rhs)
	: CItem(rhs)
{
}

void CItem_DPPoint::Set_StateInfo(_vec3 * vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);
}

HRESULT CItem_DPPoint::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CItem_DPPoint::Ready_GameObject()
{

	if (FAILED(CItem::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.01f, 0.01f, 0.01f);

	return NOERROR;
}

_int CItem_DPPoint::Update_GameObject(const _float & fTimeDelta)
{

	return CItem::Update_GameObject(fTimeDelta);
}

_int CItem_DPPoint::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (true == m_isCol)
	{
		const CGameObject* pPlayer = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);
		if (nullptr == pPlayer)
			return -1;

		((CPlayer*)pPlayer)->Plus_DP_Point((rand() % 100) * 10 + 10);
		Set_Lived(false);

		m_pTransformCom->Update_Matrix();
	}

	return CItem::LastUpdate_GameObject(fTimeDelta);
}

void CItem_DPPoint::Render_GameObject()
{
	CItem::Render_GameObject();
}

HRESULT CItem_DPPoint::Ready_Component()
{
	CComponent_Manager*			pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Com_Mesh
	m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_DPPoint");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);
	return NOERROR;
}

CItem_DPPoint * CItem_DPPoint::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_DPPoint*		pInstance = new CItem_DPPoint(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CItem_DPPoint Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_DPPoint::Clone_GameObject()
{
	CItem_DPPoint*		pInstance = new CItem_DPPoint(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CItem_DPPoint Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_DPPoint::Free()
{
	Safe_Release(m_pMeshCom);
	CItem::Free();
}
