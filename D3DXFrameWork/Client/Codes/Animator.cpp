#include "stdafx.h"
#include "..\Headers\Animator.h"
#include "Component_Manager.h"
#include "Input_Device.h"
#include "Object_Manager.h"

_USING(Client)

CAnimator::CAnimator(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device(pGraphic_Device)
	,m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
	m_pGraphic_Device->AddRef();
}

HRESULT CAnimator::Ready_Animator(CMesh_Dynamic* pMeshCom, CTransform* pTransformCom, CNavigation* pNavigationCom, _uint iSceneNum)
{
	m_pMeshCom = pMeshCom;
	m_pMeshCom->AddRef();
	m_pTransformCom = pTransformCom;
	m_pTransformCom->AddRef();
	m_pNavigationCom = pNavigationCom;
	m_pNavigationCom->AddRef();
	m_pMeshCom->RegistCallbackFunc(bind(&CAnimator::AnimFinish, this));
	m_pMeshCom->RegistCallbackCheckPair(bind(&CAnimator::CheckPair, this, placeholders::_1, placeholders::_2));
	m_pMeshCom->RegistCallbackCheckComboPair(bind(&CAnimator::CheckCombo, this, placeholders::_1));
	m_pMeshCom->RegistCallbackCheckComboTime(bind(&CAnimator::CheckComboTime, this, placeholders::_1));
	m_iSceneNum = iSceneNum;
	Ready_Pair();
	return NOERROR;
}

void CAnimator::Ready_Pair()
{
	m_vecBlendPair.reserve(3);
	//SIT_IDLE_BREATH, SIT_IDLE_HANDMOVE, SIT_GETUP, SIT_SITDOWN,
	//	NOGUN_IDLE00, NOGUN_IDLE01, NOGUN_IDLE02, NOGUN_IDLE03, NOGUN_IDLE04, NOGUN_IDLE05, NOGUN_IDLE06,
	//	NOGUN_WALK_F, NOGUN_WALK_FL, NOGUN_WALK_FR, NOGUN_WALK_L, NOGUN_WALK_R, NOGUN_WALK_B, INTERACT_DOG,
	//	ANIM_END
	//pair<_uint, _uint> p2 = make_pair(NOGUN_WALK_F, NOGUN_IDLE00);
	// APT - Blending 될 애니메이션들
	if (0 == m_iSceneNum)
	{
		m_vecBlendPair.push_back(make_pair(NOGUN_IDLE00, NOGUN_WALK_F));
		m_vecBlendPair.push_back(make_pair(NOGUN_WALK_F, NOGUN_IDLE00));
		m_vecBlendPair.push_back(make_pair(SIT_GETUP, NOGUN_IDLE00));
	}
	else // FIELD - Blending 될 애니메이션들
	{
		// Blend
		m_vecBlendPair.push_back(make_pair(SWORD_IDLE, SWORD_RUN_FORWARD));
		m_vecBlendPair.push_back(make_pair(SWORD_RUN_FORWARD, SWORD_IDLE));
		m_vecBlendPair.push_back(make_pair(SWORD_IDLE, AIM_IDLE));
		m_vecBlendPair.push_back(make_pair(AIM_IDLE, SWORD_IDLE));
		//m_vecBlendPair.push_back(make_pair(SWORLD_LIGHT_02, SWORLD_LIGHT_03));
		//m_vecBlendPair.push_back(make_pair(SWORLD_HEAVY_01, SWORLD_HEAVY_02));
		//m_vecBlendPair.push_back(make_pair(SWORLD_HEAVY_02, SWORLD_HEAVY_03));


		// Combo
		m_vecComboPair.push_back(make_pair(SWORLD_LIGHT_01, SWORLD_LIGHT_02));
		m_vecComboPair.push_back(make_pair(SWORLD_LIGHT_02, SWORLD_LIGHT_03));
		m_vecComboPair.push_back(make_pair(SWORLD_HEAVY_01, SWORLD_HEAVY_02));
		m_vecComboPair.push_back(make_pair(SWORLD_HEAVY_02, SWORLD_HEAVY_03));

		// ComboTime
		m_vecComboTime.push_back(make_pair(SWORLD_LIGHT_01, 70));
		m_vecComboTime.push_back(make_pair(SWORLD_LIGHT_02, 60));
		m_vecComboTime.push_back(make_pair(SWORLD_HEAVY_01, 52));
		m_vecComboTime.push_back(make_pair(SWORLD_HEAVY_02, 100));
	}
}

void CAnimator::Update_Animation(const _float & fTimeDelta)
{
	if (m_ArrayAnimState[SIT_GETUP] == false)
	{
		m_pMeshCom->Set_AnimationSet(SIT_GETUP);
		if (true == m_pMeshCom->Is_Finish())
			m_ArrayAnimState[SIT_GETUP] = true;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_UP) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(NOGUN_WALK_F);
		m_pNavigationCom->Move_OnNavigation(m_pTransformCom, 8.7f, fTimeDelta);
	}
	else if (m_pInput_Device->Get_DIKeyState(DIK_DOWN) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(NOGUN_WALK_B);
		m_pTransformCom->Go_Straight(-23.3, fTimeDelta);
	}
	else if (m_pInput_Device->Get_DIKeyState(DIK_Q) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(NOGUN_WALK_FL);
	}
	else if (m_pInput_Device->Get_DIKeyState(DIK_E) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(NOGUN_WALK_FR);
	}
	else if (m_pInput_Device->Get_DIKeyState(DIK_R) & 0x8000)
	{
		m_iSit = SIT_GETUP;
	}
	else if (m_pInput_Device->Get_DIKeyState(DIK_T) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(INTERACT_DOG);
	}
	else
	{
		if (m_ArrayAnimState[SIT_GETUP] == true)
		{
			int iIndex = rand() % 6 + 4;
			m_pMeshCom->Set_AnimationSet(NOGUN_IDLE00);
		}
		else
			m_pMeshCom->Set_AnimationSet(m_iSit);
		
	}


	m_pMeshCom->Play_AnimationSet(fTimeDelta);
}

void CAnimator::Update_Animation_FIELD(const _float & fTimeDelta)
{
	m_iState = m_iLastState;

	if (m_pInput_Device->Get_DIKeyState(DIK_Q) & 0x8000)
	{
		m_iLastState = STATE_SWORD;

		if (m_isKeyDown[Q] == false)
		{
			m_isKeyDown[Q] = true;
			_uint iAdd = CheckSwordLight();
			Input_Push_Back(SWORLD_LIGHT_01 + iAdd);
		}
	}
	else
		m_isKeyDown[Q] = false;

	if (m_pInput_Device->Get_DIKeyState(DIK_E) & 0x8000)
	{
		m_iLastState = STATE_SWORD;

		if (m_isKeyDown[E] == false)
		{
			m_isKeyDown[E] = true;
			_uint iAdd = CheckSwordHeavy();
			Input_Push_Back(SWORLD_HEAVY_01 + iAdd);
		}
	}
	else
		m_isKeyDown[E] = false;

	if (m_pInput_Device->Get_DIKeyState(DIK_W) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(SWORD_RUN_FORWARD);
		m_pTransformCom->Go_Straight(15.3, fTimeDelta);
		m_iState = STATE_RUN;
	}


	//LBUTTON --> 총기로 바뀜
	if (m_pInput_Device->Get_DIMouseState(CInput_Device::DIM_LBUTTON)& 0x8000)
	{
		m_iLastState = STATE_AIM;
		if (m_isKeyDown[LBUTTON] == false)
		{
			m_isKeyDown[LBUTTON] = true;
		}
	}
	else
	{
		m_isKeyDown[LBUTTON] = false;
		m_iLastState = SWORD_IDLE;
	}

	//RBUTTON --> 카메라 줌
	if (m_pInput_Device->Get_DIMouseState(CInput_Device::DIM_RBUTTON) & 0x8000)
	{
		if (m_isKeyDown[RBUTTON] == false)
		{
			m_isKeyDown[RBUTTON] = true;
		}
	}
	else
	{
		m_isKeyDown[RBUTTON] = false;
	}


	if (m_ReservationList.size() == 0)
	{
		if (m_iState == STATE_SWORD)
			m_pMeshCom->Set_AnimationSet(SWORD_IDLE);
		else if (m_iState == STATE_AIM)
			m_pMeshCom->Set_AnimationSet(AIM_IDLE);
		else if (m_iState == STATE_RUN)
			m_pMeshCom->Set_AnimationSet(SWORD_RUN_FORWARD);
	}
	else
	{
		m_pMeshCom->Set_AnimationSet(*m_ReservationList.begin());
	}

	m_pMeshCom->Play_AnimationSet(fTimeDelta);




}

void CAnimator::Last_Update_Animation_FIELD(const _float & fTimeDelta)
{
}

void CAnimator::Input_Push_Back(_uint iIndex)
{
	// 4개까지 예약 가능
	if (m_ReservationList.size() == 4)
		return;
	if (m_ReservationList.size() == 0)
		m_ReservationList.push_back(iIndex);

	_uint iTmp = *(--m_ReservationList.end());
	if (iTmp != iIndex)
		m_ReservationList.push_back(iIndex);
}

void CAnimator::AnimFinish()
{
	if (1 == m_iSceneNum)
	{
		if (m_ReservationList.size() != 0)
		{
			m_ReservationList.erase(m_ReservationList.begin());
		}
		if(STATE_RUN != m_iState)
			m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &(_vec3)m_CombinedRootMatrix->m[3]);
		m_pTransformCom->Update_Matrix();
		if (m_ReservationList.size() != 0)
			m_pMeshCom->Set_AnimationSet(*m_ReservationList.begin());
		else
		{
			if (STATE_SWORD == m_iState)
				m_pMeshCom->Set_AnimationSet(SWORD_IDLE);
			else if (STATE_AIM == m_iState)
				m_pMeshCom->Set_AnimationSet(AIM_IDLE);
			else if (STATE_RUN == m_iState)
				m_pMeshCom->Set_AnimationSet(SWORD_RUN_FORWARD);
		}


	}
}

_bool CAnimator::CheckPair(_uint iFirst, _uint iSecond)
{
	for (auto& Pair : m_vecBlendPair)
	{
		if (Pair.first == iFirst)
		{
			if (Pair.second == iSecond)
				return true;
		}
	}
	return false;
}

_bool CAnimator::CheckCombo(_uint iCurrentIdx)
{
	// if (iCurrentIdx 랑 m_ReservationList.begin()이 콤보 관계라면)
	// return true;
	if (m_ReservationList.size() < 2)
		return false;

	for (auto& Pair : m_vecComboPair)
	{
		if (Pair.first == iCurrentIdx)
		{
			if (Pair.second == *(++m_ReservationList.begin()))
			{
				AnimFinish();
				return true;
			}
		}
	}
	return false;
}

_uint CAnimator::CheckComboTime(_uint iCurrentIdx)
{
	for (auto& Pair : m_vecComboTime)
	{
		if (Pair.first == iCurrentIdx)
		{
			return Pair.second;
		}
	}
	return 0;
}

_uint CAnimator::CheckSwordLight()
{
	if (m_ReservationList.size() == 0)
		return 0;


	auto iter = m_ReservationList.end();
	if (*(--iter) == SWORLD_LIGHT_01)
		return 1;
	else if (*iter == SWORLD_LIGHT_02)
		return 2;
	else
		return 0;
}

_uint CAnimator::CheckSwordHeavy()
{
	if (m_ReservationList.size() == 0)
		return 0;

	auto iter = m_ReservationList.end();
	if (*(--iter) == SWORLD_HEAVY_01)
		return 1;
	else if (*iter == SWORLD_HEAVY_02)
		return 2;
	else
		return 0;
}

CAnimator * CAnimator::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CMesh_Dynamic* pMeshCom, CTransform* pTransformCom, CNavigation* pNavigationCom, _uint iSceneNum)
{
	CAnimator*      pInstance = new CAnimator(pGraphic_Device);

	if (FAILED(pInstance->Ready_Animator(pMeshCom, pTransformCom, pNavigationCom, iSceneNum)))
	{
		_MSG_BOX("CAnimator Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimator::Free()
{
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pNavigationCom);


	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
}
