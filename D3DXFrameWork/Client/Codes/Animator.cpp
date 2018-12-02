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
	}
	else // FIELD - Blending 될 애니메이션들
	{
		m_vecBlendPair.push_back(make_pair(SWORLD_LIGHT_01, SWORLD_LIGHT_02));

	}
}

void CAnimator::Update_Animation(const _float & fTimeDelta)
{
	if (m_ArrayAnimState[SIT_GETUP] == true)
	{
		m_pMeshCom->Set_AnimationSet(SIT_GETUP);
		if (true == m_pMeshCom->Is_Finish())
			m_ArrayAnimState[SIT_GETUP] = false;
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
		m_ArrayAnimState[SIT_GETUP] = true;
	}
	else if (m_pInput_Device->Get_DIKeyState(DIK_T) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(INTERACT_DOG);
	}
	else
	{
		if (m_ArrayAnimState[SIT_GETUP] == false)
		{
			int iIndex = rand() % 6 + 4;
			m_pMeshCom->Set_AnimationSet(NOGUN_IDLE00);
		}
	}



	m_pMeshCom->Play_AnimationSet(fTimeDelta);
}

void CAnimator::Update_Animation_FIELD(const _float & fTimeDelta)
{

	if (m_pInput_Device->Get_DIKeyState(DIK_Q) & 0x8000)
	{
		Input_Push_Back(SWORLD_LIGHT_01);
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_E) & 0x8000)
	{
		Input_Push_Back(SWORLD_LIGHT_02);
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_R) & 0x8000)
	{
		Input_Push_Back(SWORLD_LIGHT_03);
	}


	if (m_ReservationList.size() == 0)
	{
		m_pMeshCom->Set_AnimationSet(SWORD_IDLE);
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
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &(_vec3)m_CombinedRootMatrix->m[3]);
		m_pTransformCom->Update_Matrix();

		m_pMeshCom->Set_AnimationSet(SWORD_IDLE);
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
