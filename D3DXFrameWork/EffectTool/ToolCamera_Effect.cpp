#include "stdafx.h"
#include "ToolCamera_Effect.h"
#include "MainFrm.h"

CToolCamera_Effect::CToolCamera_Effect(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCamera(pGraphic_Device)
{
}

CToolCamera_Effect::CToolCamera_Effect(const CToolCamera_Effect & rhs)
	: CCamera(rhs)
{
}

HRESULT CToolCamera_Effect::Ready_GameObject_Prototype()
{
	if (FAILED(CCamera::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CToolCamera_Effect::Ready_GameObject()
{
	m_fCamSpeed = 10.f;
	return NOERROR;
}

_int CToolCamera_Effect::Update_GameObject(const _float & fTimeDelta)
{
	LPDIRECT3DDEVICE9 pGraphic_Device = Get_Graphic_Device();
	if (nullptr == pGraphic_Device)
		return -1;
	pGraphic_Device->AddRef();

	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();

	//D3DVIEWPORT9		ViewPort;
	//pGraphic_Device->GetViewport(&ViewPort);

	//POINT			pt;
	//pt.x = ViewPort.Width * 0.5f;
	//pt.y = ViewPort.Height * 0.5f;

	//HWND hWnd = ((CWnd*)pMainFrame)->m_hWnd;
	//ClientToScreen(hWnd, &pt);

	//SetCursorPos(pt.x, pt.y);



	if (Get_DIKState(DIK_W) & 0x80)
	{
		_vec3		vLook = m_Camera_Desc.vAt - m_Camera_Desc.vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_Camera_Desc.vEye += vLook * m_fCamSpeed	* m_Move_Speed;// fTimeDelta;
		m_Camera_Desc.vAt += vLook * m_fCamSpeed	* m_Move_Speed;// fTimeDelta;
	}

	if (Get_DIKState(DIK_S) & 0x80)
	{

		_vec3		vLook = m_Camera_Desc.vEye - m_Camera_Desc.vAt;
		D3DXVec3Normalize(&vLook, &vLook);

		m_Camera_Desc.vEye += vLook * m_fCamSpeed	* m_Move_Speed;// fTimeDelta;
		m_Camera_Desc.vAt += vLook * m_fCamSpeed	* m_Move_Speed;// fTimeDelta;
	}

	if (Get_DIKState(DIK_A) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_Camera_Desc.vEye -= vRight * m_fCamSpeed	* m_Move_Speed;// fTimeDelta;
		m_Camera_Desc.vAt -= vRight * m_fCamSpeed	* m_Move_Speed;// fTimeDelta;
	}

	if (Get_DIKState(DIK_D) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_Camera_Desc.vEye += vRight * m_fCamSpeed	 * m_Move_Speed;// fTimeDelta;
		m_Camera_Desc.vAt += vRight * m_fCamSpeed	 * m_Move_Speed;// fTimeDelta;
	}

	_long		MouseMove = 0;
	_vec3		vLook = m_Camera_Desc.vAt - m_Camera_Desc.vEye;
	if (CInput_Device::GetInstance()->Get_DIMouseState(CInput_Device::DIM_RBUTTON) & 0x80)
	{
		if (MouseMove = Get_DIMouseMove(CInput_Device::DIMM_X))
		{
			_matrix		matRot;
			D3DXMatrixRotationY(&matRot, D3DXToRadian(MouseMove * m_fCamSpeed) * m_Rotate_Speed);

			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_Camera_Desc.vAt = m_Camera_Desc.vEye + vLook;
		}

		if (MouseMove = Get_DIMouseMove(CInput_Device::DIMM_Y))
		{
			_vec3		vRight;
			D3DXVec3Cross(&vRight, &m_Camera_Desc.vAxisY, &vLook);

			_matrix		matRot;
			D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(MouseMove * m_fCamSpeed)* m_Rotate_Speed);

			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_Camera_Desc.vAt = m_Camera_Desc.vEye + vLook;
		}
	}
	Safe_Release(pGraphic_Device);
	return CCamera::Update_GameObject(0.1f);
}

_int CToolCamera_Effect::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CToolCamera_Effect::Render_GameObject()
{
}

HRESULT CToolCamera_Effect::SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc)
{
	Set_CameraDesc(CamDesc);
	Set_ProjectionDesc(ProjDesc);

	SetUp_Matrix();

	return NOERROR;
}

CToolCamera_Effect * CToolCamera_Effect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CToolCamera_Effect*		pInstance = new CToolCamera_Effect(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CToolCamera_Effect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CToolCamera_Effect::Clone_GameObject()
{
	CToolCamera_Effect*		pInstance = new CToolCamera_Effect(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CToolCamera_Effect Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CToolCamera_Effect::Free()
{
}
