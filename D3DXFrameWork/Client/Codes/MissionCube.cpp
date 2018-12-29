#include "stdafx.h"
#include "MissionCube.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "TalkBox.h"
#include "Input_Device.h"
#include "Font_Manager.h"
#include "SubtitleManager.h"
#include "Player.h"
#include "StaticUI.h"
CMissionCube::CMissionCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CMissionCube::CMissionCube(const CMissionCube & rhs)
	: CGameObject(rhs)
{
}

void CMissionCube::Set_StateInfo(_vec3 * vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);
}


HRESULT CMissionCube::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CMissionCube::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pSubtitle = CSubtitle_Manager::GetInstance()->Get_Mission();
	m_pStaticUI = (CStaticUI*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_UI", 2);
	if (nullptr == m_pStaticUI)
		return E_FAIL;
	return NOERROR;
}

_int CMissionCube::Update_GameObject(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (false == m_isCol)
		return 0;

	m_pStaticUI->Set_Mission(m_pSubtitle);

	if (true == m_pRendererCom->Get_SecondTrigger())
	{

	}

	return _int();
}

_int CMissionCube::LastUpdate_GameObject(const _float & fTimeDelta)
{



	const CGameObject* pPlayer = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);
	if (nullptr == pPlayer)
		return -1;

	// 디버깅용. 실제론 return true일 때 마다 특정 행동을 취해주자.

	if (false == m_isCol)
	{
		if (true == m_pColliderCom->Collision_OBB((const CCollider*)pPlayer->Get_ComponentPointer(L"Com_Collider")))
		{
			// 이벤트 처리.
			m_isCol = true;
		}
	}
	if (true == m_isMissionClear)
	{
		m_pStaticUI->Set_Mission(L"");
		Set_Lived(false);
	}

	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_UI, this)))
		return -1;

	//Safe_Release(pObject_Manager);

	return _int();
}

void CMissionCube::Render_GameObject()
{
	m_pColliderCom->Render_Collider();


}

HRESULT CMissionCube::Ready_Component()
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
	m_pColliderCom->SetUp_Collider(m_pTransformCom->Get_WorldMatrix(), &_vec3(2, 2, 2), &_vec3(0, 0, 0), &_vec3(0, 2, 0));
	m_pMesh = m_pColliderCom->Get_Mesh();
	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CMissionCube::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	return NOERROR;
}

CMissionCube * CMissionCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMissionCube*		pInstance = new CMissionCube(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CMissionCube Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMissionCube::Clone_GameObject()
{
	CMissionCube*		pInstance = new CMissionCube(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CMissionCube Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMissionCube::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);
	for (auto& pString : m_vevString)
		Safe_Delete_Array(pString);

	for (auto& pString : m_vecButtonString)
		Safe_Delete_Array(pString);
	CGameObject::Free();
}
