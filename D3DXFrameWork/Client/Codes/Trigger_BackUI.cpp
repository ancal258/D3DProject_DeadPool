#include "stdafx.h"
#include "Trigger_BackUI.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "StaticBackUI.h"
#include "Input_Device.h"
#include "Player.h"

CTrigger_BackUI::CTrigger_BackUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTrigger_BackUI::CTrigger_BackUI(const CTrigger_BackUI & rhs)
	: CGameObject(rhs)
{
}

void CTrigger_BackUI::Set_StateInfo(_vec3 * vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);
}

void CTrigger_BackUI::Create_BackUI()
{
	CGameObject* pUI;

	if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_UI_StaticBackUI_FakeError", SCENE_STAGE, L"Layer_UI", &pUI)))
		return;
	((CStaticBackUI*)pUI)->Set_Info(_vec2(g_iBackCX>>1, g_iBackCY>>1), _vec2(g_iBackCX, g_iBackCY));
	
	m_pBackUI = (CStaticBackUI*)pUI;
}

void CTrigger_BackUI::Add_String(_tchar * pString)
{
	_tchar *szTmp = new _tchar[128];
	lstrcpy(szTmp, pString);

	m_vevString.push_back(szTmp);
}

void CTrigger_BackUI::Add_ButtonString(_tchar * pString)
{
	_tchar *szTmp = new _tchar[128];
	lstrcpy(szTmp, pString);

	m_vecButtonString.push_back(szTmp);
}

HRESULT CTrigger_BackUI::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CTrigger_BackUI::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	return NOERROR;
}

_int CTrigger_BackUI::Update_GameObject(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (false == m_isCol)
		return 0;

	if (m_fTimeAcc > 4.f)
		m_pRendererCom->Set_SecondTrigger(false);

	if (true == m_pRendererCom->Get_Trigger())
	{
		m_pBackUI->Set_Lived(false);
		//m_pRendererCom->Set_Trigger(true);
	}

	return _int();
}

_int CTrigger_BackUI::LastUpdate_GameObject(const _float & fTimeDelta)
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
			m_fTimeAcc = 0.f;
			// BackUI 생성.
			m_pRendererCom->Set_SecondTrigger(true);
			Create_BackUI();

		}
	}


	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	//Safe_Release(pObject_Manager);

	return _int();
}

void CTrigger_BackUI::Render_GameObject()
{
	m_pColliderCom->Render_Collider();
}

HRESULT CTrigger_BackUI::Ready_Component()
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

HRESULT CTrigger_BackUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	return NOERROR;
}

CTrigger_BackUI * CTrigger_BackUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrigger_BackUI*		pInstance = new CTrigger_BackUI(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CTrigger_BackUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrigger_BackUI::Clone_GameObject()
{
	CTrigger_BackUI*		pInstance = new CTrigger_BackUI(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CTrigger_BackUI Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrigger_BackUI::Free()
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
