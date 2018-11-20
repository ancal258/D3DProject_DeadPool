#include "..\Headers\Camera.h"


CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, m_matView(rhs.m_matView)
	, m_Camera_Desc(rhs.m_Camera_Desc)
	, m_matProj(rhs.m_matProj)
	, m_Projection_Desc(rhs.m_Projection_Desc)
	, m_pInput_Device(rhs.m_pInput_Device)
{
	m_pInput_Device->AddRef();
}

HRESULT CCamera::Ready_GameObject_Prototype()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	ZeroMemory(&m_Camera_Desc, sizeof(CAMERADESC));
	ZeroMemory(&m_Projection_Desc, sizeof(PROJDESC));

	return NOERROR;
}

HRESULT CCamera::Ready_GameObject()
{
	return NOERROR;
}

_int CCamera::Update_GameObject(const _float & fTimeDelta)
{



	SetUp_Matrix();

	return _int();
}

_int CCamera::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CCamera::Render_GameObject()
{
}

HRESULT CCamera::SetUp_Matrix()
{
	//D3DXMatrixLookAtLH()

	// ViewSpace로의 변환행렬을 만들자.
	_vec3		vRight, vUp, vLook, vPosition;

	vPosition = m_Camera_Desc.vEye;
	vLook = m_Camera_Desc.vAt - m_Camera_Desc.vEye;
	D3DXVec3Cross(&vRight, &m_Camera_Desc.vAxisY, &vLook);
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	D3DXMatrixIdentity(&m_matWorld);

	memcpy(&m_matWorld.m[0][0], D3DXVec3Normalize(&vRight, &vRight), sizeof(_vec3));
	memcpy(&m_matWorld.m[1][0], D3DXVec3Normalize(&vUp, &vUp), sizeof(_vec3));
	memcpy(&m_matWorld.m[2][0], D3DXVec3Normalize(&vLook, &vLook), sizeof(_vec3));
	memcpy(&m_matWorld.m[3][0], &vPosition, sizeof(_vec3));

	D3DXMatrixInverse(&m_matView, nullptr, &m_matWorld);

	Set_Transform(D3DTS_VIEW, &m_matView);

	


	// Proj행렬을 만들자.
	// D3DXMatrixPerspectiveFovLH();

	D3DXMatrixIdentity(&m_matProj);

	m_matProj._11 = (1.f / tanf(m_Projection_Desc.fFovy * 0.5f)) / m_Projection_Desc.fAspect;
	m_matProj._22 = 1.f / tanf(m_Projection_Desc.fFovy * 0.5f);

	m_matProj._33 = m_Projection_Desc.fFar / (m_Projection_Desc.fFar - m_Projection_Desc.fNear);
	m_matProj._43 = (m_Projection_Desc.fFar * m_Projection_Desc.fNear) / (m_Projection_Desc.fFar - m_Projection_Desc.fNear) * -1.f;

	m_matProj._34 = 1.f;
	m_matProj._44 = 0.f;

	Set_Transform(D3DTS_PROJECTION, &m_matProj);

	return NOERROR;
}

void CCamera::Free()
{
	Safe_Release(m_pInput_Device);
	CGameObject::Free();
}
