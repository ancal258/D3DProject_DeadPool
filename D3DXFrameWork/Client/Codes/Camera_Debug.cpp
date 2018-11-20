#include "stdafx.h"
#include "..\Headers\Camera_Debug.h"

_USING(Client)

CCamera_Debug::CCamera_Debug(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{


}
CCamera_Debug::CCamera_Debug(const CCamera_Debug& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Debug::Ready_GameObject_Prototype()
{
	if (FAILED(CCamera::Ready_GameObject_Prototype()))
		return E_FAIL;



	return NOERROR;
}

HRESULT CCamera_Debug::Ready_GameObject()
{
	m_fCamSpeed = 10.f;

	return NOERROR;
}

_int CCamera_Debug::Update_GameObject(const _float & fTimeDelta)
{
	LPDIRECT3DDEVICE9 pGraphic_Device = Get_Graphic_Device();
	if (nullptr == pGraphic_Device)
		return -1;
	pGraphic_Device->AddRef();

	D3DVIEWPORT9		ViewPort;
	pGraphic_Device->GetViewport(&ViewPort);

	POINT			pt;
	pt.x = ViewPort.Width * 0.5f;
	pt.y = ViewPort.Height * 0.5f;

	ClientToScreen(g_hWnd, &pt);


	//SetCursorPos(pt.x, pt.y);

	if (Get_DIKState(DIK_W) & 0x80)
	{
		_vec3		vLook = m_Camera_Desc.vAt - m_Camera_Desc.vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_Camera_Desc.vEye += vLook * m_fCamSpeed * fTimeDelta;
		m_Camera_Desc.vAt += vLook * m_fCamSpeed * fTimeDelta;
	}

	if (Get_DIKState(DIK_S) & 0x80)
	{
		_vec3		vLook = m_Camera_Desc.vEye - m_Camera_Desc.vAt;
		D3DXVec3Normalize(&vLook, &vLook);

		m_Camera_Desc.vEye += vLook * m_fCamSpeed * fTimeDelta;
		m_Camera_Desc.vAt += vLook * m_fCamSpeed * fTimeDelta;
	}

	if (Get_DIKState(DIK_A) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_Camera_Desc.vEye -= vRight * m_fCamSpeed * fTimeDelta;
		m_Camera_Desc.vAt -= vRight * m_fCamSpeed * fTimeDelta;
	}

	if (Get_DIKState(DIK_D) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_Camera_Desc.vEye += vRight * m_fCamSpeed * fTimeDelta;
		m_Camera_Desc.vAt += vRight * m_fCamSpeed * fTimeDelta;
	}

	_long		MouseMove = 0;
	_vec3		vLook = m_Camera_Desc.vAt - m_Camera_Desc.vEye;	

	if (MouseMove = Get_DIMouseMove(CInput_Device::DIMM_X))
	{
		_matrix		matRot;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(MouseMove * m_fCamSpeed) * fTimeDelta);

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_Camera_Desc.vAt = m_Camera_Desc.vEye + vLook;
	}

	if (MouseMove = Get_DIMouseMove(CInput_Device::DIMM_Y))
	{
		_vec3		vRight;
		D3DXVec3Cross(&vRight, &m_Camera_Desc.vAxisY, &vLook);

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(MouseMove * m_fCamSpeed) * fTimeDelta);

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_Camera_Desc.vAt = m_Camera_Desc.vEye + vLook;
	}

	Safe_Release(pGraphic_Device);

	return CCamera::Update_GameObject(fTimeDelta);
}

_int CCamera_Debug::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CCamera_Debug::Render_GameObject()
{

}

HRESULT CCamera_Debug::SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc)
{
	Set_CameraDesc		(CamDesc);
	Set_ProjectionDesc(ProjDesc);

	SetUp_Matrix();

	return NOERROR;
}

CCamera_Debug * CCamera_Debug::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Debug*		pInstance = new CCamera_Debug(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CCamera_Debug Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCamera_Debug::Clone_GameObject()
{
	CCamera_Debug*		pInstance = new CCamera_Debug(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CCamera_Debug Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Debug::Free()
{
	CCamera::Free();
}

