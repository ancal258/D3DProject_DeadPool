#include "stdafx.h"
#include "..\Headers\Camera_Minigun.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Minigun.h"
_USING(Client)

CCamera_Minigun::CCamera_Minigun(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{


}
CCamera_Minigun::CCamera_Minigun(const CCamera_Minigun& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Minigun::Ready_GameObject_Prototype()
{
	if (FAILED(CCamera::Ready_GameObject_Prototype()))
		return E_FAIL;



	return NOERROR;
}

HRESULT CCamera_Minigun::Ready_GameObject()
{
	m_fCamSpeed = 10.f;
	m_isCameraOn = true;

	// For.Com_Frustum
	m_pFrustumCom = (CFrustum*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Frustum");
	if (nullptr == m_pFrustumCom)
		return E_FAIL;
	if (FAILED(Add_Component(L"Com_Frustum", m_pFrustumCom)))
		return E_FAIL;

	m_pPlayer = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Minigun", 0);
	if (nullptr == m_pPlayer)
		return E_FAIL;

	m_fOffsetX = 0.143f;
	m_fOffsetY = 1.4f;
	m_fOffsetZ = 0.272f;

	return NOERROR;
}

_int CCamera_Minigun::Update_GameObject(const _float & fTimeDelta)
{
	if (false == m_isCameraOn)
		return 0;

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
	SetCursorPos(pt.x, pt.y);



	//if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_W) & 0x8000)
	//{
	//	m_fOffsetZ -= 0.001f;
	//}
	//if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_S) & 0x8000)
	//{
	//	m_fOffsetZ += 0.001f;
	//}
	//if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_A) & 0x8000)
	//{
	//	m_fOffsetX -= 0.001f;
	//}
	//if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_D) & 0x8000)
	//{
	//	m_fOffsetX += 0.001f;
	//}
	//if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_Q) & 0x8000)
	//{
	//	m_fOffsetY += 0.001f;
	//}
	//if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_E) & 0x8000)
	//{
	//	m_fOffsetY -= 0.001f;
	//}

	_long		MouseMove = 0;
	//m_Camera_Desc.vAt = _vec3(m_Camera_Desc.vEye.x - 0.073, m_Camera_Desc.vEye.y - 0.408, m_Camera_Desc.vEye.z + 16.122);
	_vec3		vLook = m_Camera_Desc.vAt - m_Camera_Desc.vEye;

	if (MouseMove = Get_DIMouseMove(CInput_Device::DIMM_X))
	{
		if (MouseMove < 0)
		{
			if (m_dwRotAcc[0] > -730)
			{
				m_dwRotAcc[0] += MouseMove;

				_matrix		matRot;
				D3DXMatrixRotationY(&matRot, D3DXToRadian(MouseMove * m_fCamSpeed) * fTimeDelta);
				D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
				m_Camera_Desc.vAt = m_Camera_Desc.vEye + vLook;
			}
		}
		else
		{
			if (m_dwRotAcc[0] < 553)
			{
				m_dwRotAcc[0] += MouseMove;

				_matrix		matRot;
				D3DXMatrixRotationY(&matRot, D3DXToRadian(MouseMove * m_fCamSpeed) * fTimeDelta);
				D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
				m_Camera_Desc.vAt = m_Camera_Desc.vEye + vLook;
			}
		}

	}
	// -400 + 580
	if (MouseMove = Get_DIMouseMove(CInput_Device::DIMM_Y))
	{
		if (MouseMove < 0)
		{
			if (m_dwRotAcc[1] > -400)
			{
				m_dwRotAcc[1] += MouseMove;

				_vec3		vRight;
				D3DXVec3Cross(&vRight, &m_Camera_Desc.vAxisY, &vLook);

				_matrix		matRot;
				D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(MouseMove * m_fCamSpeed) * fTimeDelta);

				D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

				m_Camera_Desc.vAt = m_Camera_Desc.vEye + vLook;
			}
		}
		else
		{
			if (m_dwRotAcc[1] < 580)
			{
				m_dwRotAcc[1] += MouseMove;

				_vec3		vRight;
				D3DXVec3Cross(&vRight, &m_Camera_Desc.vAxisY, &vLook);

				_matrix		matRot;
				D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(MouseMove * m_fCamSpeed) * fTimeDelta);

				D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

				m_Camera_Desc.vAt = m_Camera_Desc.vEye + vLook;
			}
		}


	}
	if (nullptr != m_pFrustumCom)
		m_pFrustumCom->Update_Frustum(&m_matView, &m_matProj);

	Safe_Release(pGraphic_Device);

	return CCamera::Update_GameObject(fTimeDelta);
}

_int CCamera_Minigun::LastUpdate_GameObject(const _float & fTimeDelta)
{
	const CTransform* pTransformCom = (const CTransform*)m_pPlayer->Get_ComponentPointer(L"Com_Transform");
	if (nullptr == pTransformCom)
	{
		return -1;
	}
	_matrix	matParent = *pTransformCom->Get_WorldMatrix();
	_vec3 vParentPos = (_vec3)matParent.m[3];
	m_Camera_Desc.vEye.x = vParentPos.x + m_fOffsetX;
	m_Camera_Desc.vEye.y = vParentPos.y + m_fOffsetY;
	m_Camera_Desc.vEye.z = vParentPos.z - m_fOffsetZ;
	

	if (nullptr != m_pFrustumCom)
		m_pFrustumCom->Update_Frustum(&m_matView, &m_matProj);
	//cout << "X : " << m_fOffsetX << "Y : " << m_fOffsetY << "Z : " << m_fOffsetZ << endl;
	//cout << "At : (" << m_Camera_Desc.vAt.x << "," << m_Camera_Desc.vAt.y << "," << m_Camera_Desc.vAt.z << ")" << endl;

	return _int();
}

void CCamera_Minigun::Render_GameObject()
{

}

_bool CCamera_Minigun::Culling_ToFrustum(CTransform * pTransform, CVIBuffer * pBuffer, const _float& fRadius)
{
	if (nullptr == m_pFrustumCom)
		return false;

	return m_pFrustumCom->Culling_ToFrustum(pTransform, pBuffer, fRadius);
}

_bool CCamera_Minigun::Culling_ToQuadTree(CTransform * pTransform, CVIBuffer * pBuffer, const _float & fRadius)
{
	if (nullptr == m_pFrustumCom)
		return false;

	return m_pFrustumCom->Culling_ToQuadTree(pTransform, pBuffer, fRadius);
}

HRESULT CCamera_Minigun::Ready_Component()
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

HRESULT CCamera_Minigun::SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc)
{ 

	CamDesc.vAt = _vec3(41.567f, 5.092f, 62.342f);

	Set_CameraDesc(CamDesc);
	Set_ProjectionDesc(ProjDesc);



	SetUp_Matrix();

	return NOERROR;
}

CCamera_Minigun * CCamera_Minigun::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Minigun*		pInstance = new CCamera_Minigun(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CCamera_Minigun Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCamera_Minigun::Clone_GameObject()
{
	CCamera_Minigun*		pInstance = new CCamera_Minigun(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CCamera_Minigun Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Minigun::Free()
{
	Safe_Release(m_pFrustumCom);
	CCamera::Free();
}

