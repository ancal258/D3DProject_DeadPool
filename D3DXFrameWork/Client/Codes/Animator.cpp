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

HRESULT CAnimator::Ready_Animator(CMesh_Dynamic* pMeshCom, CTransform* pTransformCom, CNavigation* pNavigationCom)
{
	m_pMeshCom = pMeshCom;
	m_pMeshCom->AddRef();
	m_pTransformCom = pTransformCom;
	m_pTransformCom->AddRef();
	m_pNavigationCom = pNavigationCom;
	m_pNavigationCom->AddRef();

	return NOERROR;
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
		m_pNavigationCom->Move_OnNavigation(m_pTransformCom, 8.7f, fTimeDelta*0.9f);
	}
	else if (m_pInput_Device->Get_DIKeyState(DIK_DOWN) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(NOGUN_WALK_B);
		m_pTransformCom->Go_Straight(-23.3, fTimeDelta*0.9f);
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


}

void CAnimator::Update_Animation_FIELD(const _float & fTimeDelta)
{

	//if (m_ArrayFieldState[SWORLD_LIGHT_01] == true)
	//{
	//	m_pMeshCom->Set_AnimationSet(SWORLD_LIGHT_01);
	//	if (true == m_pMeshCom->Is_Finish())
	//		m_ArrayFieldState[SWORLD_LIGHT_01] = false;
	//}
	//if (m_ArrayFieldState[SWORLD_LIGHT_02] == true)
	//{
	//	m_pMeshCom->Set_AnimationSet(SWORLD_LIGHT_02);
	//	if (true == m_pMeshCom->Is_Finish())
	//		m_ArrayFieldState[SWORLD_LIGHT_02] = false;
	//}
	//if (m_ArrayFieldState[SWORLD_LIGHT_03] == true)
	//{
	//	m_pMeshCom->Set_AnimationSet(SWORLD_LIGHT_03);
	//	if (true == m_pMeshCom->Is_Finish())
	//		m_ArrayFieldState[SWORLD_LIGHT_03] = false;
	//}


	//if (m_pInput_Device->Get_DIKeyState(DIK_Q) & 0x8000)
	//{
	//	m_ArrayFieldState[SWORLD_LIGHT_01] = true;
	//}
	//else if (m_pInput_Device->Get_DIKeyState(DIK_E) & 0x8000)
	//{
	//	m_ArrayFieldState[SWORLD_LIGHT_02] = true;
	//}
	//else if (m_pInput_Device->Get_DIKeyState(DIK_R) & 0x8000)
	//{
	//	m_ArrayFieldState[SWORLD_LIGHT_03] = true;
	//}

	if (m_pInput_Device->Get_DIKeyState(DIK_Q) & 0x8000)
	{
		m_ReservationList.push_back(SWORLD_LIGHT_01);
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_E) & 0x8000)
	{
		m_ReservationList.push_back(SWORLD_LIGHT_02);
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_R) & 0x8000)
	{
		m_ReservationList.push_back(SWORLD_LIGHT_03);
	}

	if (m_ReservationList.size() == 0)
		m_pMeshCom->Set_AnimationSet(SWORD_IDLE);
	else
	{ 
		m_pMeshCom->Set_AnimationSet(*m_ReservationList.begin());
	}

	if (true == m_pMeshCom->Is_Finish())
	{
		if (m_ReservationList.size() != 0)
			m_ReservationList.erase(m_ReservationList.begin());
	}


	if (true == m_pMeshCom->Get_ChangeMatrix())
	{
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &(_vec3)m_CombinedRootMatrix->m[3]);
	}



		//m_pMeshCom->Set_AnimationSet(SWORLD_LIGHT_01);
		//m_pMeshCom->Set_AnimationSet(SWORLD_LIGHT_02);
		//m_pMeshCom->Set_AnimationSet(SWORLD_LIGHT_03);

	//for (size_t i = 0; i < FIELD_END; i++)
	//{
	//	if (true == m_ArrayFieldState[i])
	//		return;
	//}


}

CAnimator * CAnimator::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CMesh_Dynamic* pMeshCom, CTransform* pTransformCom, CNavigation* pNavigationCom)
{
	CAnimator*      pInstance = new CAnimator(pGraphic_Device);

	if (FAILED(pInstance->Ready_Animator(pMeshCom, pTransformCom, pNavigationCom)))
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
