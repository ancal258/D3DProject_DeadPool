#include "stdafx.h"
#include "..\Headers/Player.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Input_Device.h"
#include "Camera_Cinematic.h"
#include "Camera_Debug.h"
#include "Camera_Target.h"
#include "Animator.h"


_USING(Client)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{

}

const _matrix * CPlayer::Get_WorldMatrix() const
{
	return m_pTransformCom->Get_WorldMatrix();
}

const _matrix * CPlayer::Get_RealMatrix() const
{
	return &m_RealMatrix;
}

_bool CPlayer::Get_IsButtonDown(_uint iIndex)
{
	return m_pAnimator->Get_IsButtonDown(iIndex);
}

_bool CPlayer::Get_IsReservation()
{
	return m_pAnimator->Get_IsReservation();
}

_uint CPlayer::Get_AnimState()
{
	return m_pAnimator->Get_AnimState();
}

void CPlayer::Plus_HP(_uint i)
{
	m_iHP += i;
	if (m_iHP > 100)
		m_iHP = 100;
}

HRESULT CPlayer::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CPlayer::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	m_fMouseSence = 3.f;
	//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(54, 0.f, 12.65f));
	//m_pTransformCom->Go_Straight(0.8,1);
	m_pInput_Device = CInput_Device::GetInstance();
	m_pInput_Device->AddRef();

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
	m_iHP = 100;
	//m_pMeshCom->Set_AnimationSet(SIT_GETUP);
	D3DXMatrixIdentity(&m_RealMatrix);
	m_pNavigationCom->Update_LastMatrix(&m_RealMatrix);


	Get_Graphic_Device()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);

	return NOERROR;
}

_int CPlayer::Update_GameObject(const _float & fTimeDelta)
{
	Camera_Update(fTimeDelta);
	
	//Stage APT Animation
	if (nullptr != m_pAnimator)
	{
		if (0 == m_iStageNum)
		{
			CLight_Manager::GetInstance()->Set_PointInfo(CLight_Manager::STATE_POSITION, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION) ,0);
			m_pAnimator->Update_Animation(fTimeDelta);
		}
		if (1 == m_iStageNum)
		{
			m_pAnimator->Update_Animation_FIELD(fTimeDelta);

		}
	}

	m_pTransformCom->Update_Matrix();
	Update_HandMatrix();


	return _int();
}

_int CPlayer::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	
	if (nullptr == m_pAnimator)
		return 1;
	// 공격
	if (true == m_pAnimator->Get_IsFHS())
	{
		if (m_fCartoonOffset < 5)
			m_fCartoonOffset += 15 * fTimeDelta;
		if (m_fOutlineOffset > 0)
			m_fOutlineOffset -= 3 * fTimeDelta;
		CInput_Device::GetInstance()->Set_SlowMotion(true);
		m_isCartoon = true;
	}
	// 끝
	else
	{
		m_fCartoonOffset = 1.f;
		m_fOutlineOffset = 1.f;
		CInput_Device::GetInstance()->Set_SlowMotion(false);
		m_isCartoon = false;
	}
	//
	CObject_Manager* pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return -1;

	pObject_Manager->AddRef();

	const CGameObject* pTerrain = pObject_Manager->Get_ObjectPointer(SCENE_STAGE, L"Layer_BackGround", 0);
	if (nullptr == pTerrain)
	{
		Safe_Release(pObject_Manager);
		return -1;
	}

	const CBuffer_Terrain* pBufferCom = (const CBuffer_Terrain*)pTerrain->Get_ComponentPointer(L"Com_Buffer");
	if (nullptr == pBufferCom)
	{
		Safe_Release(pObject_Manager);
		return -1;
	}

	const CTransform* pTransformCom = (const CTransform*)pTerrain->Get_ComponentPointer(L"Com_Transform");
	if (nullptr == pTransformCom)
	{
		Safe_Release(pObject_Manager);
		return -1;
	}

	if (1 == m_iStageNum)
	{
		if (nullptr != m_pAnimator)
		{
			m_pAnimator->Last_Update_Animation_FIELD(fTimeDelta);
		}
	}


	m_RealMatrix = *m_pTransformCom->Get_WorldMatrix();
	m_RealMatrix.m[3][0] = m_CombinedRootMatrix.m[3][0];
	m_RealMatrix.m[3][1] = m_CombinedRootMatrix.m[3][1];
	m_RealMatrix.m[3][2] = m_CombinedRootMatrix.m[3][2];

	//_vec3      vPosition = *(_vec3*)m_RealMatrix.m[3];
	_vec3      vPosition = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	*(_vec3*)m_RealMatrix.m[3] = pBufferCom->SetHeight_OnTerrain((_vec3*)m_RealMatrix.m[3], pTransformCom);
	vPosition.y = (*(_vec3*)m_RealMatrix.m[3]).y;


	//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);
	m_pTransformCom->Update_Matrix();

	// 현재 RealMatrix 값과 과거의 Matrix값을 비교 처리
	m_pNavigationCom->Compute_Animation(&m_RealMatrix, m_pTransformCom, 5.f, fTimeDelta);
	// LastMatrix Update
	m_pNavigationCom->Update_LastMatrix(&m_RealMatrix);

	if(false == m_pRendererCom->Get_SecondTrigger())
		if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_PLAYER, this)))
			return -1;

	Safe_Release(pObject_Manager);

	return _int();
}

void CPlayer::Render_GameObject()
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
	pEffect->BeginPass(2);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	//m_pNavigationCom->Render_Navigation();

	//m_pColliderCom->Render_Collider();

	//Render_Axis();

}

void CPlayer::Render_Axis()
{
	_vec3      vPointX[2], vPointY[2], vPointZ[2];
	_vec3	   vPointCamera[2];

	vPointX[0] = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	vPointX[1] = vPointX[0] + *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT) * 100;

	vPointY[0] = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	vPointY[1] = vPointY[0] + *m_pTransformCom->Get_StateInfo(CTransform::STATE_UP) * 100;

	vPointZ[0] = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	vPointZ[1] = vPointZ[0] + *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK) * 100;

	vPointCamera[0] = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	vPointCamera[1] = vPointCamera[0] + m_vCam * 100;

	LPD3DXLINE            pLine = nullptr;
	if (FAILED(D3DXCreateLine(Get_Graphic_Device(), &pLine)))
		return;

	_matrix      matView, matProj;

	Get_Graphic_Device()->GetTransform(D3DTS_VIEW, &matView);
	Get_Graphic_Device()->GetTransform(D3DTS_PROJECTION, &matProj);

	_matrix      matTransform;
	D3DXMatrixIdentity(&matTransform);

	for (size_t i = 0; i < 2; ++i)
	{
		D3DXVec3TransformCoord(&vPointX[i], &vPointX[i], &matView);
		D3DXVec3TransformCoord(&vPointX[i], &vPointX[i], &matProj);

		D3DXVec3TransformCoord(&vPointY[i], &vPointY[i], &matView);
		D3DXVec3TransformCoord(&vPointY[i], &vPointY[i], &matProj);

		D3DXVec3TransformCoord(&vPointZ[i], &vPointZ[i], &matView);
		D3DXVec3TransformCoord(&vPointZ[i], &vPointZ[i], &matProj);

		D3DXVec3TransformCoord(&vPointCamera[i], &vPointCamera[i], &matView);
		D3DXVec3TransformCoord(&vPointCamera[i], &vPointCamera[i], &matProj);

	}

	pLine->SetWidth(5.0f);

	pLine->Begin();

	pLine->DrawTransform(vPointX, 2, &matTransform, D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	pLine->DrawTransform(vPointY, 2, &matTransform, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	pLine->DrawTransform(vPointZ, 2, &matTransform, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	pLine->DrawTransform(vPointCamera, 2, &matTransform, D3DXCOLOR(1.f, 0.f, 1.f, 1.f));

	pLine->End();

	Safe_Release(pLine);
}

HRESULT CPlayer::SetUp_Camera()
{
	if (nullptr == m_pCamera_Debug)
		m_pCamera_Debug = (CCamera_Debug*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 0);

	if (nullptr == m_pCamera_Target)
		m_pCamera_Target = (CCamera_Target*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 1);

	if (nullptr == m_pCamera_Cinematic)
	{
		m_pCamera_Cinematic = (CCamera_Cinematic*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 2);
		//if (FAILED(SetUp_CameraMove())) // 나중에 FilePath 받아서 만들자.
		//	return E_FAIL;
	}
	return NOERROR;
}

HRESULT CPlayer::SetUp_StageInfo(_uint iStage)
{
	m_iStageNum = iStage;

	if (0 == m_iStageNum)
	{
		m_pTransformCom->Scaling(0.05f, 0.05f, 0.05f);
		m_pTransformCom->Set_AngleY(D3DXToRadian(48));
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(16.0f, 0.f, 13.73f));
	}
	 
	if (1 == m_iStageNum)
	{
		m_pTransformCom->Scaling(0.01f, 0.01f, 0.01f);
		m_pTransformCom->Set_AngleY(D3DXToRadian(48));
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(5.f, 0.f, 5.f));

		m_pRendererCom->Set_Trigger(true);
	}

	m_pAnimator = CAnimator::Create(Get_Graphic_Device(), m_pMeshCom, m_pTransformCom, m_pNavigationCom, m_iStageNum);
	if (nullptr != m_pAnimator)
	{
		m_pAnimator->SetUp_RootMatrix(&m_RealMatrix);
		m_pAnimator->SetUp_MouseRotate(&m_fRotate,&m_fTimeAcc);

	}

	return NOERROR;
}

void CPlayer::Camera_Update(const _float& fTimeDelta)
{
	if (nullptr == m_pAnimator)
		return;
	//if (nullptr == m_pCamera_Debug ||
	//	nullptr == m_pCamera_Target ||
	//	nullptr == m_pCamera_Cinematic)
	//	return;


	CInput_Device* pInput_Device = CInput_Device::GetInstance();
	pInput_Device->AddRef();
	// Camera SetUp
	if (FAILED(SetUp_Camera()))
		return;

	if (nullptr != m_pCamera_Debug&&
		nullptr != m_pCamera_Target&&
		nullptr != m_pCamera_Cinematic)
	{
		if (m_pInput_Device->Get_DIKeyState(DIK_NUMPAD8) & 0x8000)
		{
			m_Camera_State = CAMERA_DEBUG;
			m_pCamera_Debug->Set_IsCameraOn(true);
			m_pCamera_Target->Set_IsCameraOn(false);
			m_pCamera_Cinematic->Set_IsCameraOn(false);
		}
		if (m_pInput_Device->Get_DIKeyState(DIK_NUMPAD9) & 0x8000)
		{
			m_Camera_State = CAMERA_TARGET;
			m_pCamera_Debug->Set_IsCameraOn(false);
			m_pCamera_Target->Set_IsCameraOn(true);
			m_pCamera_Cinematic->Set_IsCameraOn(false);
		}

		if (CAMERA_CINEMATIC == m_Camera_State)
		{
			if (true == m_pCamera_Cinematic->Get_Finish())
			{
				m_Camera_State = CAMERA_TARGET;
				m_pCamera_Debug->Set_IsCameraOn(false);
				m_pCamera_Target->Set_IsCameraOn(true);
				m_pCamera_Cinematic->Set_IsCameraOn(false);
			}
		}
	}

	// Debug
	if (m_Camera_State != CAMERA_TARGET)
	{
		if (m_pInput_Device->Get_DIKeyState(DIK_RIGHT) & 0x8000)
		{
			m_pTransformCom->RotationY(D3DXToRadian(90.0f), fTimeDelta);
		}
		if (m_pInput_Device->Get_DIKeyState(DIK_LEFT) & 0x8000)
		{
			m_pTransformCom->RotationY(D3DXToRadian(-90.0f), fTimeDelta);
		}
	}
	else
	{
		if (m_dwMouseMove[0] = pInput_Device->Get_DIMouseMove(CInput_Device::DIMM_X))
		{
			//if (false == m_pAnimator->Get_IsReservation())
			//{
			//	m_pTransformCom->RotationY(D3DXToRadian(m_dwMouseMove[0] * m_fMouseSence), fTimeDelta);
			//}
			//else
			//{
			//	m_fRotate = D3DXToRadian(m_dwMouseMove[0] * m_fMouseSence);
			//	m_fTimeAcc += m_fRotate * fTimeDelta;
			//}
		}

		if (m_dwMouseMove[1] = pInput_Device->Get_DIMouseMove(CInput_Device::DIMM_Y))
		{
			//  m_pTransformCom->RotationX(D3DXToRadian(m_dwMouseMove[1] * m_fMouseSence), fTimeDelta);
		}

		// 카메라가 바라보는 방향을 기준으로 회전하며 이동하게 된다.
		if (false == m_pAnimator->Get_IsReservation())
		{
			if (pInput_Device->Get_DIKeyState(DIK_W) & 0x8000 && pInput_Device->Get_DIKeyState(DIK_D) & 0x8000 &&
				!(pInput_Device->Get_DIKeyState(DIK_A) & 0x8000) && !(pInput_Device->Get_DIKeyState(DIK_S) & 0x8000))
			{
				// Camera의 Eye 포지션
				_vec3 vCameraEye = ((CCamera_Target*)m_pCamera_Target)->Get_CameraEye();
				_vec3 vPlayerCross = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK) + *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT) * -1;
				// Camera->Player 바라보는 벡터.
				vCameraEye.y = 0;
				_vec3 vDir = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION) - vCameraEye;
				vDir.y = 0;
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3Normalize(&vPlayerCross, &vPlayerCross);
				_vec3 vCross;
				D3DXVec3Cross(&vCross, &vDir, &vPlayerCross);
				if (vCross.y < 0)
				{
					if (0.99f >= D3DXVec3Dot(&vDir, &vPlayerCross))
						m_pTransformCom->RotationY(5.f, fTimeDelta);
				}
				else
				{
					if (0.99f >= D3DXVec3Dot(&vDir, &vPlayerCross))
						m_pTransformCom->RotationY(-5.f, fTimeDelta);
				}
			}
			else if (pInput_Device->Get_DIKeyState(DIK_W) & 0x8000 && pInput_Device->Get_DIKeyState(DIK_A) & 0x8000 &&
				!(pInput_Device->Get_DIKeyState(DIK_D) & 0x8000) && !(pInput_Device->Get_DIKeyState(DIK_S) & 0x8000))
			{
				// Camera의 Eye 포지션
				_vec3 vCameraEye = ((CCamera_Target*)m_pCamera_Target)->Get_CameraEye();
				_vec3 vPlayerCross = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK) + *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
				// Camera->Player 바라보는 벡터.
				vCameraEye.y = 0;
				_vec3 vDir = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION) - vCameraEye;
				vDir.y = 0;
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3Normalize(&vPlayerCross, &vPlayerCross);
				_vec3 vCross;
				D3DXVec3Cross(&vCross, &vDir, &vPlayerCross);
				if (vCross.y < 0)
				{
					if (0.99f >= D3DXVec3Dot(&vDir, &vPlayerCross))
						m_pTransformCom->RotationY(10, fTimeDelta);
				}
				else
				{
					if (0.99f >= D3DXVec3Dot(&vDir, &vPlayerCross))
						m_pTransformCom->RotationY(-10, fTimeDelta);
				}
			}

			else if (pInput_Device->Get_DIKeyState(DIK_W) & 0x8000 && !(pInput_Device->Get_DIKeyState(DIK_D) & 0x8000) && !(pInput_Device->Get_DIKeyState(DIK_A) & 0x8000) && !(pInput_Device->Get_DIKeyState(DIK_S) & 0x8000))
			{
				// Camera의 Eye 포지션
				_vec3 vCameraEye = ((CCamera_Target*)m_pCamera_Target)->Get_CameraEye();
				_vec3 vPlayerLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
				// Camera->Player 바라보는 벡터.
				vCameraEye.y = 0;
				_vec3 vDir = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION) - vCameraEye;
				vDir.y = 0;
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
				m_vCam = vDir;
				_vec3 vCross;
				D3DXVec3Cross(&vCross, &vDir, &vPlayerLook);
				if (vCross.y < 0)
				{
					if (0.99f >= D3DXVec3Dot(&vDir, &vPlayerLook))
						m_pTransformCom->RotationY(10, fTimeDelta);
				}
				else
				{
					if (0.99f >= D3DXVec3Dot(&vDir, &vPlayerLook))
						m_pTransformCom->RotationY(-10, fTimeDelta);
				}
			}
			else if (pInput_Device->Get_DIKeyState(DIK_S) & 0x8000 && !(pInput_Device->Get_DIKeyState(DIK_D) & 0x8000) && !(pInput_Device->Get_DIKeyState(DIK_A) & 0x8000) && !(pInput_Device->Get_DIKeyState(DIK_W) & 0x8000))
			{
				// Camera의 Eye 포지션
				_vec3 vCameraEye = ((CCamera_Target*)m_pCamera_Target)->Get_CameraEye();
				_vec3 vPlayerLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK) * -1;
				if(15 == m_pAnimator->Get_iState())
					vPlayerLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);

				// Camera->Player 바라보는 벡터.
				vCameraEye.y = 0;
				_vec3 vDir = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION) - vCameraEye;
				vDir.y = 0;
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
				m_vCam = vDir;
				_vec3 vCross;
				D3DXVec3Cross(&vCross, &vDir, &vPlayerLook);
				if (vCross.y < 0)
				{
					if (0.99f >= D3DXVec3Dot(&vDir, &vPlayerLook))
						m_pTransformCom->RotationY(10, fTimeDelta);
				}
				else
				{
					if (0.99f >= D3DXVec3Dot(&vDir, &vPlayerLook))
						m_pTransformCom->RotationY(-10, fTimeDelta);
				}
			}


			else if (pInput_Device->Get_DIKeyState(DIK_D) & 0x8000 && !(pInput_Device->Get_DIKeyState(DIK_W) & 0x8000) && !(pInput_Device->Get_DIKeyState(DIK_A) & 0x8000) && !(pInput_Device->Get_DIKeyState(DIK_S) & 0x8000))
			{
				// Camera의 Eye 포지션
				_vec3 vCameraEye = ((CCamera_Target*)m_pCamera_Target)->Get_CameraEye();
				_vec3 vPlayerRight = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT) * -1;
				// Camera->Player 바라보는 벡터.
				vCameraEye.y = 0;
				_vec3 vDir = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION) - vCameraEye;
				vDir.y = 0;
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);
				_vec3 vCross;
				D3DXVec3Cross(&vCross, &vDir, &vPlayerRight);
				if (vCross.y < 0)
				{
					if (0.99f >= D3DXVec3Dot(&vDir, &vPlayerRight))
						m_pTransformCom->RotationY(10, fTimeDelta);
				}
				else
				{
					if (0.99f >= D3DXVec3Dot(&vDir, &vPlayerRight))
						m_pTransformCom->RotationY(-10, fTimeDelta);
				}
			}

			else if (pInput_Device->Get_DIKeyState(DIK_A) & 0x8000 && !(pInput_Device->Get_DIKeyState(DIK_D) & 0x8000) && !(pInput_Device->Get_DIKeyState(DIK_W) & 0x8000) && !(pInput_Device->Get_DIKeyState(DIK_S) & 0x8000))
			{
				// Camera의 Eye 포지션
				_vec3 vCameraEye = ((CCamera_Target*)m_pCamera_Target)->Get_CameraEye();
				_vec3 vPlayerRight = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
				// Camera->Player 바라보는 벡터.
				vCameraEye.y = 0;
				_vec3 vDir = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION) - vCameraEye;
				vDir.y = 0;
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);
				_vec3 vCross;
				D3DXVec3Cross(&vCross, &vDir, &vPlayerRight);
				if (vCross.y < 0)
				{
					if (0.99f >= D3DXVec3Dot(&vDir, &vPlayerRight))
						m_pTransformCom->RotationY(10, fTimeDelta);
				}
				else
				{
					if (0.99f >= D3DXVec3Dot(&vDir, &vPlayerRight))
						m_pTransformCom->RotationY(-10, fTimeDelta);
				}
			}
		}

		

	}

	Safe_Release(pInput_Device);
}

HRESULT CPlayer::SetUp_CameraMove()
{
	if (nullptr != m_pCamera_Debug&&
		nullptr != m_pCamera_Target&&
		nullptr != m_pCamera_Cinematic)
	{
		Load_CamData(m_pEventTag);
		m_Camera_State = CAMERA_CINEMATIC;
		m_pCamera_Debug->Set_IsCameraOn(false);
		m_pCamera_Target->Set_IsCameraOn(false);
		m_pCamera_Cinematic->Set_IsCameraOn(true);
	}
	return NOERROR;
}

HRESULT CPlayer::Ready_Component()
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

	// For.Com_Mesh
	m_pMeshCom = (CMesh_Dynamic*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Player");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
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
	m_pColliderCom->SetUp_Collider(&m_RealMatrix, &_vec3(50, 140, 50), &_vec3(0.0f, 0.f, 0.f), &_vec3(0.f, 70.f, 0.f));


	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	pEffect->SetFloat("g_fCartoonOffset", m_fCartoonOffset);
	pEffect->SetFloat("g_fOutlineOffset", m_fOutlineOffset);
	pEffect->SetBool("g_isCartoon", m_isCartoon);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);


	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CPlayer::Update_HandMatrix()
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

HRESULT CPlayer::Load_CamData(const _tchar * pFileName)
{

	_tchar szFilePath[128];// = ;
	ZeroMemory(szFilePath, sizeof(_tchar) * 128);
	lstrcpy(szFilePath, L"../Bin/DataFiles/");
	lstrcat(szFilePath, pFileName);

	HANDLE			hFile = CreateFile(szFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_ulong			dwByte = 0;
	_uint iNumPoints = 0;
	_float fTime = 0;
	_vec3 vLookAt;
	_vec3 vPoint;
	vector<_vec3> vecEye;


	ReadFile(hFile, &iNumPoints, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &fTime, sizeof(_float), &dwByte, nullptr);
	ReadFile(hFile, &vLookAt, sizeof(_vec3), &dwByte, nullptr);
	for (int i = 0; i < iNumPoints; ++i)
	{
		ReadFile(hFile, &vPoint, sizeof(_vec3), &dwByte, nullptr);
		vecEye.push_back(vPoint);
	}

	CloseHandle(hFile);

	m_pCamera_Cinematic->SetUp_CameraMove(vecEye, vLookAt, fTime);

	return NOERROR;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*      pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CPlayer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone_GameObject()
{
	CPlayer*      pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CPlayer Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	Safe_Release(m_pAnimator);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pInput_Device);

	CGameObject::Free();
}