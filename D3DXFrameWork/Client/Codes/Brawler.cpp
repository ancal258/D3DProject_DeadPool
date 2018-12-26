#include "stdafx.h"
#include "..\Headers\Brawler.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"
#include "Brawler_Manager.h"

#include "Camera_Target.h"
#include "Player.h"

_USING(Client)

CBrawler::CBrawler(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CBrawler::CBrawler(const CBrawler & rhs)
	: CGameObject(rhs)
	, m_iStageNum(rhs.m_iStageNum)
{
}

void CBrawler::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPos);
}

HRESULT CBrawler::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CBrawler::Ready_GameObject()
{

	if (FAILED(CBrawler::Ready_Component()))
		return E_FAIL;
	m_pHandMatrix[0] = m_pMeshCom->Get_FrameMatrixByName("L_Weapon01_Wpn_XW");
	if (nullptr == m_pHandMatrix[0])
		return E_FAIL;
	m_pHandMatrix[1] = m_pMeshCom->Get_FrameMatrixByName("R_Weapon01_Wpn_XW");
	if (nullptr == m_pHandMatrix[1])
		return E_FAIL;
	m_pRootMatrix = m_pMeshCom->Get_FrameMatrixByName("C_Root_Reference_XR");
	if (nullptr == m_pRootMatrix)
		return E_FAIL;
	Update_HandMatrix();

	D3DXMatrixIdentity(&m_RealMatrix);
	CBrawler_Manager::GetInstance()->Add_Object(this);
	if (1 == m_iStageNum)
	{
		for (size_t i = 0; i < 3; i++)
		{
			m_pPlayer[i] = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", i);
			if (nullptr == m_pPlayer[i])
				return E_FAIL;
		}
	}
	if (2 == m_iStageNum)
	{

	}

	return NOERROR;
}

_int CBrawler::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;


	if (1 == m_iStageNum)
		CBrawler::Update_Stage_Field(fTimeDelta);
	else if (2 == m_iStageNum)
		CBrawler::Update_Stage_Airplane(fTimeDelta);


	if (FAILED(isHitScan()))
		return E_FAIL;
	return _int();
}

_int CBrawler::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (1 == m_iStageNum)
		CBrawler::LastUpdate_Stage_Field(fTimeDelta);
	else if (2 == m_iStageNum)
		CBrawler::LastUpdate_Stage_Airplane(fTimeDelta);


	return _int();
}

void CBrawler::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return;



	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
	
	//m_pColliderCom->Render_Collider();
	//m_pColliderCom_Body->Render_Collider();
	//m_pColliderCom_Head->Render_Collider();
	//m_pColliderCom_Search->Render_Collider();
}

void CBrawler::CallBackFinish()
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &(_vec3)m_CombinedRootMatrix.m[3]);
	m_pTransformCom->Update_Matrix();
	//STATE_DEATH_F, STATE_DEATH_L, STATE_DEATH_LL, STATE_DEATH_R,STATE_DEATH_RL
	if (4 <= m_iCurrentIndex && m_iCurrentIndex <= 8)
	{
		m_pWeapon->Set_Lived(false);
		CBrawler_Manager::GetInstance()->Clear_Object(this);
		Set_Lived(false);
	}
	else if (2 == m_iCurrentIndex)
	{
		m_fAttackAcc = 0.f;
		m_isAttack = false;
		m_isCompute = false;
	}
	m_iCurrentIndex = m_iIdleIndex;
	m_pMeshCom->Set_AnimationSet(m_iIdleIndex);
	
}

HRESULT CBrawler::Ready_Component()
{
	CComponent_Manager*         pComponent_Manager = CComponent_Manager::GetInstance();
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

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Navigation
	m_pNavigationCom = (CNavigation*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Navigation");
	if (FAILED(Add_Component(L"Com_Navigation", m_pNavigationCom)))
		return E_FAIL;

	// Form.Com_Collider
	m_pColliderCom = (CCollider*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collider_Box");
	if (FAILED(Add_Component(L"Com_Collider", m_pColliderCom)))
		return E_FAIL;
	m_pColliderCom->SetUp_Collider(&m_CombinedRootMatrix, &_vec3(50, 140, 50), &_vec3(0.0f, 0.f, 0.f), &_vec3(0.f, 70.f, 0.f));

	m_pColliderCom_Search = (CCollider*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collider_Sphere");
	if (FAILED(Add_Component(L"Com_Collider_Search", m_pColliderCom_Search)))
		return E_FAIL;
	m_pColliderCom_Search->SetUp_Collider(&m_CombinedRootMatrix, &_vec3(1200, 1200, 1200), &_vec3(0.0f, 0.f, 0.f), &_vec3(0.f, 70.f, 0.f));
	
	m_pColliderCom_Body = (CCollider*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collider_Sphere");
	if (FAILED(Add_Component(L"Com_Collider_Body", m_pColliderCom_Body)))
		return E_FAIL;
	m_pColliderCom_Body->SetUp_Collider(&m_CombinedRootMatrix, &_vec3(85, 85, 85), &_vec3(0.0f, 0.f, 0.f), &_vec3(0.f,80.f, 0.f));
	m_pColliderMesh[0] = m_pColliderCom_Body->Get_Mesh();

	m_pColliderCom_Head = (CCollider*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collider_Sphere");
	if (FAILED(Add_Component(L"Com_Collider_Head", m_pColliderCom_Head)))
		return E_FAIL;
	m_pColliderCom_Head->SetUp_Collider(&m_CombinedRootMatrix, &_vec3(20, 20, 20), &_vec3(0.0f, 0.f, 0.f), &_vec3(20.f, 130.f, -20.f));
	m_pColliderMesh[1] = m_pColliderCom_Head->Get_Mesh();


	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CBrawler::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix         matView, matProj;

	Get_Transform(D3DTS_VIEW, &matView);
	Get_Transform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	// For.LightInfo
	const D3DLIGHT9* pLightInfo = CLight_Manager::GetInstance()->Get_LightInfo(0);
	if (nullptr == pLightInfo)
		return E_FAIL;

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	pEffect->SetVector("g_vMtrlDiffuse", &_vec4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetVector("g_vMtrlAmbient", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
	pEffect->SetVector("g_vMtrlSpecular", &_vec4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetFloat("g_fPower", 20.f);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);


	Safe_Release(pEffect);

	return NOERROR;
}

void CBrawler::Compute_PlayerDir()
{
	_vec3 vPlayerPos = static_cast<_vec3>((dynamic_cast<const CPlayer*>(m_pPlayer[0])->Get_RealMatrix()->m[3]));
	//_vec3 vBrawlerPos = static_cast<_vec3>(m_pTransformCom->Get_WorldMatrix()->m[3]);
	_vec3 vBrawlerPos = (_vec3)m_CombinedRootMatrix.m[3];
	m_vPlayerDir = vPlayerPos - vBrawlerPos;
	m_fLength = D3DXVec3Length(&m_vPlayerDir);

	D3DXVec3Normalize(&m_vPlayerDir, &m_vPlayerDir);
	m_vBrawlerLook = static_cast<_vec3>(m_pTransformCom->Get_WorldMatrix()->m[2]);
	D3DXVec3Normalize(&m_vBrawlerLook, &m_vBrawlerLook);
	m_fRadian = D3DXVec3Dot(&m_vBrawlerLook, &m_vPlayerDir);
}

_int CBrawler::Update_Stage_Field(const _float & fTimeDelta)
{
	CCamera_Target*	pCamera = (CCamera_Target*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 1);
	if (nullptr == pCamera)
		return -1;

	_float fRadius = 25.f;

	Update_HandMatrix();

	if (false == m_isDamaged)
	{
		// 플레이어 - Search 컬라이더 충돌.
		if (true == m_pColliderCom_Search->Collision_Sphere((const CCollider*)m_pPlayer[0]->Get_ComponentPointer(L"Com_Collider")))
			m_isSearch = true;
		else
			m_isSearch = false;

		if (true == ((CPlayer*)m_pPlayer[0])->Get_IsReservation()) // 공격이 예약 되어 있을 때 만
		{
			// 칼 - 몬스터 Body 충돌.
			for (size_t i = 1; i < 3; i++)
			{

				if (true == m_pColliderCom_Body->Collision_Sphere((const CCollider*)m_pPlayer[i]->Get_ComponentPointer(L"Com_Collider0")) ||
					true == m_pColliderCom_Body->Collision_Sphere((const CCollider*)m_pPlayer[i]->Get_ComponentPointer(L"Com_Collider1")) ||
					true == m_pColliderCom_Body->Collision_Sphere((const CCollider*)m_pPlayer[i]->Get_ComponentPointer(L"Com_Collider2")) ||
					true == m_pColliderCom_Body->Collision_Sphere((const CCollider*)m_pPlayer[i]->Get_ComponentPointer(L"Com_Collider3")) ||
					true == m_pColliderCom_Body->Collision_Sphere((const CCollider*)m_pPlayer[i]->Get_ComponentPointer(L"Com_Collider4")) ||
					true == m_pColliderCom_Head->Collision_Sphere((const CCollider*)m_pPlayer[i]->Get_ComponentPointer(L"Com_Collider0")) ||
					true == m_pColliderCom_Head->Collision_Sphere((const CCollider*)m_pPlayer[i]->Get_ComponentPointer(L"Com_Collider1")) ||
					true == m_pColliderCom_Head->Collision_Sphere((const CCollider*)m_pPlayer[i]->Get_ComponentPointer(L"Com_Collider2")) ||
					true == m_pColliderCom_Head->Collision_Sphere((const CCollider*)m_pPlayer[i]->Get_ComponentPointer(L"Com_Collider3")) ||
					true == m_pColliderCom_Head->Collision_Sphere((const CCollider*)m_pPlayer[i]->Get_ComponentPointer(L"Com_Collider4")))
				{
					m_isDamaged = true;
					m_fDamegedTime = 0.f;
				}

			}
			//if (m_isDamaged == false)
			//	cout << m_iHP << endl;
			if(true == m_isDamaged)
				m_iHP--;
		}
	}

	if (false == pCamera->Culling_ToFrustum(m_pTransformCom, nullptr, fRadius))
	{
		if(m_fLength < 30.f)
			if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
				return -1;
	}

	return _int();
}

_int CBrawler::LastUpdate_Stage_Field(const _float & fTimeDelta)
{
	CollisionCheck(fTimeDelta);

	// 첫번째 구체 체크
	if (TRUE == m_Hit[0])
	{
		m_fDamegedTime = 0.f;
		m_isDamaged = CInput_Device::GetInstance()->Is_MinDist(m_fDist[0]);
	}
	if (TRUE == m_Hit[1])
	{
		// 두번재 구체 체크 ( 첫번째 구체가 가장 짧은 거리가 아니라면. )
		if (false == m_isDamaged)
		{
			m_fDamegedTime = 0.f;
			m_isDamaged = CInput_Device::GetInstance()->Is_MinDist(m_fDist[1]);
			((CPlayer*)m_pPlayer[0])->Add_HeadShotPoint();
		}
	}
	if (TRUE == m_Hit[0] || TRUE == m_Hit[1])
	{
		m_iHP--;
	}
	m_pTransformCom->Update_Matrix();

	return _int();
}

_int CBrawler::Update_Stage_Airplane(const _float & fTimeDelta)
{
	if (m_eType == TYPE_FRONT)
	{

	}
	else if (m_eType == TYPE_EDGE_L)
	{

	}
	else if (m_eType == TYPE_EDGE_R)
	{

	}
	else if (m_eType == TYPE_STOP)
	{

	}
	else
	{

	}

	return _int();
}

_int CBrawler::LastUpdate_Stage_Airplane(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CBrawler::isHitScan()
{
	const CGameObject* pPlayer = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);
	if (nullptr == pPlayer)
		return NOERROR;
	m_Hit[0] = FALSE;
	m_Hit[1] = FALSE;
	if (true == ((CPlayer*)pPlayer)->Get_IsButtonDown(7))
	{
		if (FAILED(CInput_Device::GetInstance()->Picking_ToCollider(m_pColliderMesh[0], m_pTransformCom, &m_Hit[0], &m_fDist[0])))
			return E_FAIL;

		if (FAILED(CInput_Device::GetInstance()->Picking_ToCollider(m_pColliderMesh[1], m_pTransformCom, &m_Hit[1], &m_fDist[1])))
			return E_FAIL;
	}
	return NOERROR;
}

HRESULT CBrawler::CollisionCheck(_float fTimeDelta)
{
	list<CGameObject*> BrawlerList = *CBrawler_Manager::GetInstance()->Get_BrawlerList();
	_vec3 vMyPos = _vec3(m_pTransformCom->Get_WorldMatrix()->m[3][0], m_pTransformCom->Get_WorldMatrix()->m[3][1], m_pTransformCom->Get_WorldMatrix()->m[3][2]);

	for (auto& pBrawler : BrawlerList)
	{
		_matrix matAnother = *((const CTransform*)pBrawler->Get_ComponentPointer(L"Com_Transform"))->Get_WorldMatrix();
		_vec3 vAnotherPos = _vec3(matAnother.m[3][0], matAnother.m[3][1], matAnother.m[3][2]);
		_vec3 vLen = vMyPos - vAnotherPos;
		if (1 > D3DXVec3Length(&vLen))
		{
			D3DXVec3Normalize(&vLen, &vLen);
			_float fDist = fTimeDelta * 3.1f;
			m_pTransformCom->Set_PlusPosition(vLen, fDist);
		}
	}

	return NOERROR;
}

HRESULT CBrawler::Update_HandMatrix()
{
	if (nullptr == m_pHandMatrix[0] ||
		nullptr == m_pHandMatrix[1] ||
		nullptr == m_pRootMatrix)
		return E_FAIL;

	m_CombinedHandMatrix[0] = *m_pHandMatrix[0] * *m_pTransformCom->Get_WorldMatrix();
	m_CombinedHandMatrix[1] = *m_pHandMatrix[1] * *m_pTransformCom->Get_WorldMatrix();
	m_CombinedRootMatrix = *m_pRootMatrix * *m_pTransformCom->Get_WorldMatrix();

	return NOERROR;
}

void CBrawler::Free()
{

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pColliderCom_Body);
	Safe_Release(m_pColliderCom_Head);
	Safe_Release(m_pColliderCom_Search);
	CGameObject::Free();
}
