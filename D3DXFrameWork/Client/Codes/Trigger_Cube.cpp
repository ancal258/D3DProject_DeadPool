#include "stdafx.h"
#include "Trigger_Cube.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "TalkBox.h"
#include "Input_Device.h"
#include "Font_Manager.h"
#include "SubtitleManager.h"
#include "Player.h"

CTrigger_Cube::CTrigger_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTrigger_Cube::CTrigger_Cube(const CTrigger_Cube & rhs)
	: CGameObject(rhs)
{
}

void CTrigger_Cube::Set_StateInfo(_vec3 * vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);
}

void CTrigger_Cube::Create_TalkBox(_tchar * pTalk)
{
	CGameObject* pUI;
	if (0 == m_iKind)
	{
		if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_UI_TalkBox_White", SCENE_LOGO, L"Layer_UI", &pUI)))
			return;
		((CTalkBox*)pUI)->Set_Info(_vec2((g_iBackCX >> 1) - 250, 200 + 100 * m_fOffsetY), pTalk);
		++m_iKind;
	}
	else if (1 == m_iKind)
	{
		if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_UI_TalkBox_Orange", SCENE_LOGO, L"Layer_UI", &pUI)))
			return;
		((CTalkBox*)pUI)->Set_Info(_vec2((g_iBackCX >> 1) - 50, 300 + 100 * (m_fOffsetY - 1)), pTalk);
		--m_iKind;
	}
	++m_fOffsetY;
	m_vTalkBox.push_back((CTalkBox*)pUI);
}

void CTrigger_Cube::Add_String(_tchar * pString)
{
	_tchar *szTmp = new _tchar[128];
	lstrcpy(szTmp, pString);

	m_vevString.push_back(szTmp);
}

void CTrigger_Cube::Add_ButtonString(_tchar * pString)
{
	_tchar *szTmp = new _tchar[128];
	lstrcpy(szTmp, pString);

	m_vecButtonString.push_back(szTmp);
}

HRESULT CTrigger_Cube::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CTrigger_Cube::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CTrigger_Cube::Update_GameObject(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	
	if (false == m_isCol)
		return 0;

	if (m_fTimeAcc > 2.f)
	{
		// 삽입된 문자열의 갯수보다 작을 경우 2초마다 Talk Box를 생성한다.
		if (m_vevString.size() - 1 >= m_iBoxNum)
		{
			m_pSubtitle = CSubtitle_Manager::GetInstance()->Get_Subtitle();
			Create_TalkBox(m_vevString[m_iBoxNum]);
			m_fTimeAcc = 0;
			++m_iBoxNum;
		}
		// 문자열이 모두 소비된 경우 모든 상자를 지운다.
		else
		{
			for (auto& TalkBox : m_vTalkBox)
			{
				TalkBox->Set_Lived(false);
				Set_Lived(false);
			}
		}
	}

	if (true == m_pRendererCom->Get_SecondTrigger())
	{
		for (auto& TalkBox : m_vTalkBox)
		{
			TalkBox->Set_Lived(false);
			//Set_Lived(false);
		}
	}

	return _int();
}

_int CTrigger_Cube::LastUpdate_GameObject(const _float & fTimeDelta)
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


	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_UI, this)))
		return -1;

	//Safe_Release(pObject_Manager);

	return _int();
}

void CTrigger_Cube::Render_GameObject()
{
	m_pColliderCom->Render_Collider();

	if (true == m_isCol)
	{
		_matrix	   matTransform, matScale, matTranslate;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTranslate, (g_iBackCX >> 1) - 150, 80, 0.f);
		matTransform = matScale * matTranslate;
		CFont_Manager::GetInstance()->Render_Font(L"Font_Number", m_pSubtitle, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), &matTransform);
	}
}

HRESULT CTrigger_Cube::Ready_Component()
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

HRESULT CTrigger_Cube::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	return NOERROR;
}

CTrigger_Cube * CTrigger_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrigger_Cube*		pInstance = new CTrigger_Cube(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CTrigger_Cube Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrigger_Cube::Clone_GameObject()
{
	CTrigger_Cube*		pInstance = new CTrigger_Cube(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CTrigger_Cube Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrigger_Cube::Free()
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
