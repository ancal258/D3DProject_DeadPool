#include "stdafx.h"
#include "..\Headers\Camera_Target.h"
#include "Component_Manager.h"
#include "Player.h"

_USING(Client)

CCamera_Target::CCamera_Target(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{


}
CCamera_Target::CCamera_Target(const CCamera_Target& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Target::Ready_GameObject_Prototype()
{
	if (FAILED(CCamera::Ready_GameObject_Prototype()))
		return E_FAIL;



	return NOERROR;
}

HRESULT CCamera_Target::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	m_fCamSpeed = 10.f;
	m_fAngle[0] = D3DXToRadian(48.f);
	return NOERROR;
}
_int CCamera_Target::Update_GameObject(const _float & fTimeDelta)
{
	if (false == m_isCameraOn)
		return 0;


	D3DVIEWPORT9		ViewPort;
	Get_Graphic_Device()->GetViewport(&ViewPort);

	POINT			pt;
	pt.x = ViewPort.Width * 0.5f;
	pt.y = ViewPort.Height * 0.5f;

	ClientToScreen(g_hWnd, &pt);


	SetCursorPos(pt.x, pt.y);

	return CCamera::Update_GameObject(fTimeDelta);
}

_int CCamera_Target::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (false == m_isCameraOn)
		return 0;
	if (nullptr == m_pTarget &&
		nullptr == m_pTargetWorldMatrix &&
		nullptr == m_pTargetMouseMove)
		return 0;

	if (nullptr != m_pFrustumCom)
		m_pFrustumCom->Update_Frustum(&m_matView, &m_matProj);

	MouseEvent();

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD1))
		m_Projection_Desc.fFovy -= D3DXToRadian(20*fTimeDelta);
	else
	{
		if (m_Projection_Desc.fFovy <= D3DXToRadian(60))
			m_Projection_Desc.fFovy += D3DXToRadian(10 * fTimeDelta);
	}
	_vec3 vTargetPos = m_pTargetWorldMatrix->m[3];
	m_Camera_Desc.vEye = vTargetPos;
	//m_Camera_Desc.vEye.x += 3.f;
	m_Camera_Desc.vEye.y = vTargetPos.y + m_fCameraEyeY;
	m_Camera_Desc.vEye.z = vTargetPos.z - m_fCameraEyeZ;



	_vec3 vLook = vTargetPos - m_Camera_Desc.vEye;
	m_Camera_Desc.vAt = m_Camera_Desc.vEye + vLook;
	m_Camera_Desc.vAt.x += m_fCameraAtX;
	m_Camera_Desc.vAt.y = m_Camera_Desc.vEye.y + m_fCameraAtY;
	m_Camera_Desc.vAt.z += m_fCameraAtZ;
	//m_Camera_Desc.vAt.x += m_fCameraX;



	_matrix		matRotY;
	_vec3		vDist;
	if (0 != m_pTargetMouseMove)
	{
		m_fAngle[1] += D3DXToRadian(m_pTargetMouseMove[1] * 3.f) * fTimeDelta;
		D3DXMatrixRotationX(&matRotY, m_fAngle[1]);
		memcpy(&matRotY.m[3], &m_Camera_Desc.vAt, sizeof(_vec3));
		vDist = m_Camera_Desc.vEye - m_Camera_Desc.vAt;
		D3DXVec3TransformCoord(&m_Camera_Desc.vEye, &vDist, &matRotY);

		m_fAngle[0] += D3DXToRadian(m_pTargetMouseMove[0] * 3.f) * fTimeDelta;
		D3DXMatrixRotationY(&matRotY, m_fAngle[0]);
		memcpy(&matRotY.m[3], &m_Camera_Desc.vAt, sizeof(_vec3));
		vDist = m_Camera_Desc.vEye - m_Camera_Desc.vAt;
		D3DXVec3TransformCoord(&m_Camera_Desc.vEye, &vDist, &matRotY);

	}
	return _int();
}

void CCamera_Target::Render_GameObject()
{

}

HRESULT CCamera_Target::SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc)
{
	Set_CameraDesc(CamDesc);
	Set_ProjectionDesc(ProjDesc);

	SetUp_Matrix();

	return NOERROR;
}

HRESULT CCamera_Target::SetUp_Target(const CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_pTarget = pGameObject;

	m_pTargetWorldMatrix = ((CPlayer*)m_pTarget)->Get_RealMatrix();
	m_pTargetMouseMove = ((CPlayer*)m_pTarget)->Get_MouseMove();
	m_iStage = ((CPlayer*)m_pTarget)->Get_StageNum();

	if (0 == m_iStage)
	{
		m_fCameraEyeX = 0.f;
		m_fCameraEyeY = 3.f;
		m_fCameraEyeZ = 6.3f;
		m_fCameraAtX = 0.f;
		m_fCameraAtY = 5.f;
		m_fCameraAtZ = 0.f;
	}
	if (1 == m_iStage)
	{
		m_fCameraEyeX = 0.f;
		m_fCameraEyeY = -0.74f;
		m_fCameraEyeZ = 0.62f;
		m_fCameraAtX = 0.f;
		m_fCameraAtY = 2.54f;
		m_fCameraAtZ = 0.f;
	}
	return NOERROR;
}

_vec3 CCamera_Target::Get_CameraEye()
{
	return m_Camera_Desc.vEye;
}

HRESULT CCamera_Target::Ready_Component()
{
	CComponent_Manager*		pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Com_Frustum
	m_pFrustumCom = (CFrustum*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Frustum");
	if (nullptr == m_pFrustumCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Frustum", m_pFrustumCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

_bool CCamera_Target::Culling_ToFrustum(CTransform * pTransform, CVIBuffer * pBuffer, const _float & fRadius)
{
	if (nullptr == m_pFrustumCom)
		return false;

	return m_pFrustumCom->Culling_ToFrustum(pTransform, pBuffer, fRadius);
}

_bool CCamera_Target::Culling_ToQuadTree(CTransform * pTransform, CVIBuffer * pBuffer, const _float & fRadius)
{
	if (nullptr == m_pFrustumCom)
		return false;

	return m_pFrustumCom->Culling_ToQuadTree(pTransform, pBuffer, fRadius);
}

void CCamera_Target::MouseEvent()
{
	if (true == ((CPlayer*)m_pTarget)->Get_IsButtonDown(8))
	{
		if (m_fCameraEyeZ > 0.02f) // EYEY = -0.74 -> 0.22 0.96이동. 0.6이동 // AtY -> 2.54 -> 1.52 --> 1정도 이동. 비슷. // 30틱
		{
			m_fCameraEyeZ -= 0.03f;
			m_fCameraEyeY += 0.045f;
			m_fCameraAtY -= 0.034f;
		}
	}
	else
	{
		if (m_fCameraEyeZ < 0.62f)
		{
			m_fCameraEyeZ += 0.03f;
			m_fCameraEyeY -= 0.045f;
			m_fCameraAtY += 0.034f;
		}
	}

}

CCamera_Target * CCamera_Target::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Target*		pInstance = new CCamera_Target(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CCamera_Target Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCamera_Target::Clone_GameObject()
{
	CCamera_Target*		pInstance = new CCamera_Target(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CCamera_Target Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Target::Free()
{
	Safe_Release(m_pFrustumCom);
	CCamera::Free();
}

