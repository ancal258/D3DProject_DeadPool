#include "stdafx.h"
#include "..\Headers\Brawler01.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Brawler_Knife.h"
#include "UI_HPBar.h"
#include "Player.h"
CBrawler01::CBrawler01(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CBrawler(pGraphic_Device)
{
}

CBrawler01::CBrawler01(const CBrawler01 & rhs)
	:CBrawler(rhs)
{
}

HRESULT CBrawler01::Ready_GameObject_Prototype(_uint iStageNum)
{
	if (FAILED(CBrawler::Ready_GameObject_Prototype()))
		return E_FAIL;
	m_iStageNum = iStageNum;
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

	m_pTransformCom->Scaling(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_AngleY(D3DXToRadian(rand() % 180));
	_vec3 vPosition = _vec3(rand() % 16 + 2.8f, 0.f, rand() % 16 + 16.f);

	m_iHP = 4;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);

	CGameObject* pHP_Bar = nullptr;
	if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_UI_HPBar", SCENE_STAGE, L"Layer_UI", &pHP_Bar)))
		return E_FAIL;
	if (nullptr != pHP_Bar)
		dynamic_cast<CUI_HPBar*>(pHP_Bar)->Set_Position(this);

	m_iIdleIndex = STATE_IDLE;
	m_pMeshCom->RegistCallbackFunc(bind(&CBrawler::CallBackFinish, this));
	m_pMeshCom->Set_AnimationSet(m_iIdleIndex);

	return NOERROR;
}

_int CBrawler01::Update_GameObject(const _float & fTimeDelta)
{
	if (1 == m_iStageNum)
		Update_Stage_Field(fTimeDelta);
	else if (2 == m_iStageNum)
		Update_Stage_Airplane(fTimeDelta);

	return CBrawler::Update_GameObject(fTimeDelta);
}

_int CBrawler01::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (1 == m_iStageNum)
		LastUpdate_Stage_Field(fTimeDelta);
	else if (2 == m_iStageNum)
		LastUpdate_Stage_Airplane(fTimeDelta);


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
_int CBrawler01::Update_Stage_Field(const _float & fTimeDelta)
{
	Compute_PlayerDir();
	//cout << m_fLength << endl;
	if (true == m_isSearch && false == m_isDamaged)
	{
		if (D3DXVec3Dot(&m_vBrawlerLook, &m_vPlayerDir) < 0.998f)
		{
			_vec3 vCross;

			// 몬스터의 Look과 Player와 몬스터의 방향벡터를 외적해서 좌측/우측 판단.
			D3DXVec3Cross(&vCross, &m_vBrawlerLook, &m_vPlayerDir);
			if (vCross.y >= 0)
				m_pTransformCom->RotationY(D3DXToRadian(360.f), fTimeDelta);
			else
				m_pTransformCom->RotationY(D3DXToRadian(-360.f), fTimeDelta);
		}
		if (m_fLength > 2.f)
		{
			m_pTransformCom->Go_Straight(4.2f, fTimeDelta);
			m_iCurrentIndex = STATE_RUN_F;
		}
		else
			m_iCurrentIndex = STATE_IDLE;
	}
	else
		m_iCurrentIndex = STATE_IDLE;

	return _int();
}
_int CBrawler01::LastUpdate_Stage_Field(const _float & fTimeDelta)
{

	if (true == m_isDamaged)
	{
		m_fDamegedTime += fTimeDelta;

		if (m_fDamegedTime < 0.3f)
		{
			_float fDist = fTimeDelta * -3.6f;
			m_pTransformCom->Set_PlusPosition(m_vPlayerDir, fDist);
		}
		
		if (m_fDamegedTime > 1)
			m_isDamaged = false;
		//Set_DeathIndex();
		//m_iCurrentIndex = m_iDeathIndex;
	}
	if (m_iHP <= 0)
	{
		Set_DeathIndex();
		m_iCurrentIndex = m_iDeathIndex;
	}

	m_pMeshCom->Set_AnimationSet(m_iCurrentIndex);

	m_pMeshCom->Play_AnimationSet(fTimeDelta);

	return _int();
}
_int CBrawler01::Update_Stage_Airplane(const _float & fTimeDelta)
{
	return _int();
}
_int CBrawler01::LastUpdate_Stage_Airplane(const _float & fTimeDelta)
{
	return _int();
}
void CBrawler01::Set_DeathIndex()
{
	if (true == m_isCallDeathIdx)
		return;
	if (m_fRadian > 0.819f)
		m_iDeathIndex = STATE_DEATH_F;
	else
	{
		_vec3 vCross;

		// 몬스터의 Look과 Player와 몬스터의 방향벡터를 외적해서 좌측/우측 판단.
		D3DXVec3Cross(&vCross, &m_vBrawlerLook, &m_vPlayerDir);
		if (vCross.y >= 0)
		{
			if (10 > m_fLength)
				m_iDeathIndex = STATE_DEATH_L;
			else
				m_iDeathIndex = STATE_DEATH_LL;
		}
		else
		{
			if (10 > m_fLength)
				m_iDeathIndex = STATE_DEATH_R;
			else
				m_iDeathIndex = STATE_DEATH_RL;
		}

	}
	m_isCallDeathIdx = true;
}

CBrawler01 * CBrawler01::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iStageNum)
{
	CBrawler01*      pInstance = new CBrawler01(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype(iStageNum)))
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
