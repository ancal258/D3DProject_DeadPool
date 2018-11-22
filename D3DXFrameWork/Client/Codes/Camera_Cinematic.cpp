#include "stdafx.h"
#include "..\Headers\Camera_Cinematic.h"
#include "Player.h"

_USING(Client)

CCamera_Cinematic::CCamera_Cinematic(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{


}
CCamera_Cinematic::CCamera_Cinematic(const CCamera_Cinematic& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Cinematic::Ready_GameObject_Prototype()
{
	if (FAILED(CCamera::Ready_GameObject_Prototype()))
		return E_FAIL;



	return NOERROR;
}

HRESULT CCamera_Cinematic::Ready_GameObject()
{
	m_fCamSpeed = 10.f;

	return NOERROR;
}

_int CCamera_Cinematic::Update_GameObject(const _float & fTimeDelta)
{
	if (false == m_isCameraOn)
		return 0;
	if (4 * m_iCurrentIndex >= m_vecEyePoints.size())
	{
		// 카메라 끝 시점
		m_dlCurrentOffset = 1;
		m_isFinish = true;
	}
	else
	{
		m_dlCurrentOffset += (fTimeDelta / m_fTime);
		m_Camera_Desc.vEye = Bezier4(m_vecEyePoints[0 + 4 * m_iCurrentIndex], m_vecEyePoints[1 + 4 * m_iCurrentIndex], m_vecEyePoints[2 + 4 * m_iCurrentIndex], m_vecEyePoints[3 + 4 * m_iCurrentIndex], m_dlCurrentOffset);
	}

	if (m_dlCurrentOffset >= 1)
	{
		m_dlCurrentOffset = 0;
		++m_iCurrentIndex;
	}


	return CCamera::Update_GameObject(fTimeDelta);
}

_int CCamera_Cinematic::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CCamera_Cinematic::Render_GameObject()
{

}

HRESULT CCamera_Cinematic::SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc)
{
	Set_CameraDesc(CamDesc);
	Set_ProjectionDesc(ProjDesc);

	SetUp_Matrix();

	return NOERROR;
}

void CCamera_Cinematic::SetUp_CameraMove(vector<_vec3> vecEyePoints, _vec3 vLookAt, _float fTime)
{
	m_Camera_Desc.vAt = vLookAt;
	m_vecEyePoints = vecEyePoints;
	m_Camera_Desc.vEye = m_vecEyePoints[0];
	m_fTime = fTime;
	m_dlCurrentOffset = 0;
	m_iCurrentIndex = 0;
}

_vec3 CCamera_Cinematic::Bezier4(_vec3 vPoint_1, _vec3 vPoint_2, _vec3 vPoint_3, _vec3 vPoint_4, double Offset)
{
	double OffsetA, OffsetB, OffsetC;

	_vec3 vResult;

	OffsetA = 1 - Offset;
	OffsetB = OffsetA * OffsetA * OffsetA;
	OffsetC = Offset * Offset * Offset;

	vResult.x = OffsetB*vPoint_1.x + 3 * Offset*OffsetA*OffsetA*vPoint_2.x + 3 * Offset*Offset*OffsetA*vPoint_3.x + OffsetC*vPoint_4.x;
	vResult.y = OffsetB*vPoint_1.y + 3 * Offset*OffsetA*OffsetA*vPoint_2.y + 3 * Offset*Offset*OffsetA*vPoint_3.y + OffsetC*vPoint_4.y;
	vResult.z = OffsetB*vPoint_1.z + 3 * Offset*OffsetA*OffsetA*vPoint_2.z + 3 * Offset*Offset*OffsetA*vPoint_3.z + OffsetC*vPoint_4.z;

	return(vResult);
}


CCamera_Cinematic * CCamera_Cinematic::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Cinematic*		pInstance = new CCamera_Cinematic(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CCamera_Cinematic Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCamera_Cinematic::Clone_GameObject()
{
	CCamera_Cinematic*		pInstance = new CCamera_Cinematic(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CCamera_Cinematic Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Cinematic::Free()
{
	CCamera::Free();
}

