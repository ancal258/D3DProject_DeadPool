#include "..\Headers\Input_Device.h"
#include "Input_Device.h"
#include "Graphic_Device.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Mesh_Static.h"

_IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{

}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE hInst, HWND hWnd)
{
	m_hWnd = hWnd;

	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pSDK, nullptr)))
		return E_FAIL;

	if (FAILED(SetUp_KeyBoard(hWnd)))
		return E_FAIL;

	if (FAILED(SetUp_Mouse(hWnd)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CInput_Device::Inquire_Input_State()
{
	if (nullptr == m_pKeyBoard ||
		nullptr == m_pMouse)
		return E_FAIL;

	// m_byKeyState배열 중, 누른 키에 해당하는 인덱스배열에 음수를 채운다. 
	// 단, 눌리지않은 키들에대해서는 항상 0의 값이 담긴다.
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);

	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

	if (FAILED(SetUp_Picking()))
		return E_FAIL;


	return NOERROR;
}

_bool CInput_Device::Picking_ToBuffer(CVIBuffer * pBufferCom, CTransform * pTransformCom, _vec3 * pCollPos)
{
	return pBufferCom->Picking_ToBuffer(&m_vRayPos, D3DXVec3Normalize(&m_vRayDir, &m_vRayDir), pTransformCom, pCollPos);
}


_bool CInput_Device::Picking_ToBuffer(CVIBuffer * pBufferCom, CTransform * pTransformCom, CNavigation* pNavigation, _vec3 * pCollPos)
{
	return pBufferCom->Picking_ToBuffer(&m_vRayPos, D3DXVec3Normalize(&m_vRayDir, &m_vRayDir), pTransformCom, pNavigation, pCollPos);
}


HRESULT CInput_Device::Picking_ToMesh(CMesh_Static * pStaticMeshCom, CTransform * pTransformCom, BOOL* pHit)
{
	if (FAILED(pStaticMeshCom->Picking_ToMesh(&m_vRayPos, D3DXVec3Normalize(&m_vRayDir, &m_vRayDir), pTransformCom, pHit)))
		return E_FAIL;
	return NOERROR;
}

HRESULT CInput_Device::Picking_ToCollider(LPD3DXMESH pMesh, CTransform * pTransformCom, BOOL * pHit)
{
	_float		fU, fV, fDist;
	_vec3		vRayPos, vRayDir;
	_matrix     matWorld = *pTransformCom->Get_WorldMatrix();
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &m_vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &m_vRayDir, &matWorld);

	if (FAILED(D3DXIntersect(pMesh, &vRayPos, &vRayDir, (BOOL*)pHit, nullptr, &fU, &fV, &fDist, nullptr, nullptr)))
		return E_FAIL;
	return NOERROR;
}

HRESULT CInput_Device::SetUp_Picking()
{
	LPDIRECT3DDEVICE9 pGraphic_Device = CGraphic_Device::GetInstance()->Get_GraphicDev();
	if (nullptr == pGraphic_Device)
		return E_FAIL;

	pGraphic_Device->AddRef();

	// 마우스 위치 In.ViewPort Space
	POINT			ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	D3DVIEWPORT9	ViewPort;
	pGraphic_Device->GetViewport(&ViewPort);

	// In.Projection Space
	_vec3		vMousePos;

	// x : 0 -> -1, 800 -> 1, 400 -> 0
	// y : 0 -> 1, 600 -> -1, 300 -> 0
	vMousePos.x = (ptMouse.x / ((ViewPort.Width + 300) * 0.5f)) - 1.f; // mfc 300정도 보정
	vMousePos.y = (ptMouse.y / ((ViewPort.Height + 30) * -0.5f)) + 1.f; // mfc 30정도 보정
	vMousePos.z = 0.f;

	// In.View Space
	_matrix			matProj;
	pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	m_vRayPos = _vec3(0.f, 0.f, 0.f);

	m_vRayDir = vMousePos - m_vRayPos;

	// In.World Space
	_matrix			matView;
	pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);

	Safe_Release(pGraphic_Device);

	return S_OK;
}

HRESULT CInput_Device::SetUp_KeyBoard(HWND hWnd)
{
	if (nullptr == m_pSDK)
		return E_FAIL;

	if (FAILED(m_pSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	// 어떤 키보드 장치를 이용하는지.
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 키보드의 협조레벨을 설정한다.
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	// 키보드를 활성화합니다.
	m_pKeyBoard->Acquire();

	return NOERROR;
}

HRESULT CInput_Device::SetUp_Mouse(HWND hWnd)
{
	if (nullptr == m_pSDK)
		return E_FAIL;

	if (FAILED(m_pSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	// 어떤 키보드 장치를 이용하는지.
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 키보드의 협조레벨을 설정한다.
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 키보드를 활성화합니다.
	m_pMouse->Acquire();

	return NOERROR;
}

void CInput_Device::Free()
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pSDK);
}
