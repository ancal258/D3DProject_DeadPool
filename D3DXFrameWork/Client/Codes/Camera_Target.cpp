#include "stdafx.h"
#include "..\Headers\Camera_Target.h"
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
	m_fCamSpeed = 10.f;
	m_fAngle[0] = D3DXToRadian(48.f);
	return NOERROR;
}
_int CCamera_Target::Update_GameObject(const _float & fTimeDelta)
{
	if (false == m_isCameraOn)
		return 0;
	if (nullptr == m_pTarget &&
		nullptr == m_pTargetWorldMatrix &&
		nullptr == m_pTargetMouseMove)
		return 0;



	_vec3 vTargetPos = m_pTargetWorldMatrix->m[3];
	m_Camera_Desc.vEye = vTargetPos;
	//m_Camera_Desc.vEye.x += 3.f;
	m_Camera_Desc.vEye.y = vTargetPos.y - m_fCameraY;
	m_Camera_Desc.vEye.z = vTargetPos.z - m_fCameraZ;

	if (true == ((CPlayer*)m_pTarget)->Get_IsButtonDown(8))
	{
		if (m_fCameraZ > 9.f)
		{
			m_fCameraZ -= 0.3f;
			m_fCameraY += 0.1f;
		}
	}
	else
	{
		if (m_fCameraZ < 15.3f)
		{
			m_fCameraZ += 0.3f;
			m_fCameraY -= 0.1f;
		}
	}
	_vec3 vLook = vTargetPos - m_Camera_Desc.vEye;
	m_Camera_Desc.vAt = m_Camera_Desc.vEye + vLook;
	m_Camera_Desc.vAt.y = m_Camera_Desc.vEye.y + 5.f;
	m_Camera_Desc.vAt.x += m_fCameraX;
	


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
	return CCamera::Update_GameObject(fTimeDelta);
}

_int CCamera_Target::LastUpdate_GameObject(const _float & fTimeDelta)
{
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

	m_fCameraX = 0.f;
	m_fCameraY = -3.f;
	m_fCameraZ = 15.3f;

	return NOERROR;
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
	CCamera::Free();
}

