#include "..\Headers\Transform.h"

_USING(Engine)

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_matWorld(rhs.m_matWorld)
	, m_matParent(rhs.m_matParent)
	, m_CombinedMatrix(rhs.m_CombinedMatrix)
	, m_CombinedMatrix_NotRot(rhs.m_CombinedMatrix_NotRot)
{

}

_vec3 CTransform::Get_Scale() const
{
	return _vec3(D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_RIGHT][0]), 
		D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_UP][0]), 
		D3DXVec3Length((_vec3*)&m_matWorld.m[STATE_LOOK][0]));
}

HRESULT CTransform::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->SetMatrix(pConstantName, &m_CombinedMatrix);

	return NOERROR;
}

HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matParent);
	D3DXMatrixIdentity(&m_CombinedMatrix);
	D3DXMatrixIdentity(&m_CombinedMatrix_NotRot);

	return NOERROR;
}

void CTransform::Update_Matrix()
{
	m_CombinedMatrix_NotRot = m_CombinedMatrix = m_matWorld * m_matParent;

	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);


	vRight *= D3DXVec3Length(Get_StateInfo(CTransform::STATE_RIGHT));
	memcpy(&m_CombinedMatrix_NotRot.m[STATE_RIGHT][0], &vRight, sizeof(_vec3));

	vUp *= D3DXVec3Length(Get_StateInfo(CTransform::STATE_UP));
	memcpy(&m_CombinedMatrix_NotRot.m[STATE_UP][0], &vUp, sizeof(_vec3));

	vLook *= D3DXVec3Length(Get_StateInfo(CTransform::STATE_LOOK));
	memcpy(&m_CombinedMatrix_NotRot.m[STATE_LOOK][0], &vLook, sizeof(_vec3));
}

void CTransform::Scaling(const _vec3 & vScale)
{
	_vec3			vRight, vUp, vLook;

	vRight = *D3DXVec3Normalize(&vRight, Get_StateInfo(STATE_RIGHT)) * vScale.x;
	vUp = *D3DXVec3Normalize(&vUp, Get_StateInfo(STATE_UP)) * vScale.y;
	vLook = *D3DXVec3Normalize(&vLook, Get_StateInfo(STATE_LOOK)) * vScale.z;

	Set_StateInfo(STATE_RIGHT, &vRight);
	Set_StateInfo(STATE_UP, &vUp);
	Set_StateInfo(STATE_LOOK, &vLook);
}

void CTransform::Scaling(const _float & fX, const _float & fY, const _float & fZ)
{
	_vec3			vRight, vUp, vLook;

	vRight = *D3DXVec3Normalize(&vRight, Get_StateInfo(STATE_RIGHT)) * fX;
	vUp = *D3DXVec3Normalize(&vUp, Get_StateInfo(STATE_UP)) * fY;
	vLook = *D3DXVec3Normalize(&vLook, Get_StateInfo(STATE_LOOK)) * fZ;

	Set_StateInfo(STATE_RIGHT, &vRight);
	Set_StateInfo(STATE_UP, &vUp);
	Set_StateInfo(STATE_LOOK, &vLook);
}

void CTransform::Go_Straight(const _float & fSpeedPerSec, const _float& fTimeDelta)
{
	_vec3	vLook = *Get_StateInfo(STATE_LOOK);
	_vec3	vPosition = *Get_StateInfo(STATE_POSITION);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * fSpeedPerSec * fTimeDelta;

	Set_StateInfo(STATE_POSITION, &vPosition);
}

void CTransform::Set_AngleX(const _float & fRadian)
{
	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	vRight *= Get_Scale().x;
	vUp *= Get_Scale().y;
	vLook *= Get_Scale().z;

	_matrix		matRotX;
	D3DXMatrixRotationX(&matRotX, fRadian);

	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_RIGHT][0], &vRight, &matRotX);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_UP][0], &vUp, &matRotX);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_LOOK][0], &vLook, &matRotX);
}

void CTransform::Set_AngleY(const _float & fRadian)
{
	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	vRight *= Get_Scale().x;
	vUp *= Get_Scale().y;
	vLook *= Get_Scale().z;

	_matrix		matRotY;
	D3DXMatrixRotationY(&matRotY, fRadian);
	
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_RIGHT][0], &vRight, &matRotY);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_UP][0], &vUp, &matRotY);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_LOOK][0], &vLook, &matRotY);		
}

void CTransform::Set_AngleZ(const _float & fRadian)
{
	_vec3		vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	vRight *= Get_Scale().x;
	vUp *= Get_Scale().y;
	vLook *= Get_Scale().z;

	_matrix		matRotZ;
	D3DXMatrixRotationZ(&matRotZ, fRadian);

	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_RIGHT][0], &vRight, &matRotZ);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_UP][0], &vUp, &matRotZ);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_LOOK][0], &vLook, &matRotZ);
}

void CTransform::RotationX(const _float & fAnglePerSec, const _float & fTimeDelta)
{
	_matrix		matRotX;
	D3DXMatrixRotationX(&matRotX, fAnglePerSec * fTimeDelta);

	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_RIGHT][0], (_vec3*)&m_matWorld.m[STATE_RIGHT][0], &matRotX);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_UP][0], (_vec3*)&m_matWorld.m[STATE_UP][0], &matRotX);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_LOOK][0], (_vec3*)&m_matWorld.m[STATE_LOOK][0], &matRotX);
}
void CTransform::RotationY(const _float & fAnglePerSec, const _float & fTimeDelta)
{
	_matrix		matRotY;
	D3DXMatrixRotationY(&matRotY, fAnglePerSec * fTimeDelta);

	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_RIGHT][0], (_vec3*)&m_matWorld.m[STATE_RIGHT][0], &matRotY);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_UP][0], (_vec3*)&m_matWorld.m[STATE_UP][0], &matRotY);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_LOOK][0], (_vec3*)&m_matWorld.m[STATE_LOOK][0], &matRotY);
}
void CTransform::RotationZ(const _float & fAnglePerSec, const _float & fTimeDelta)
{
	_matrix		matRotZ;
	D3DXMatrixRotationZ(&matRotZ, fAnglePerSec * fTimeDelta);

	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_RIGHT][0], (_vec3*)&m_matWorld.m[STATE_RIGHT][0], &matRotZ);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_UP][0], (_vec3*)&m_matWorld.m[STATE_UP][0], &matRotZ);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_LOOK][0], (_vec3*)&m_matWorld.m[STATE_LOOK][0], &matRotZ);
}

void CTransform::RotationAxis(const _vec3 vAxis, const _float & fRadianPerSec, const _float & fTimeDelta)
{
	_matrix		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vAxis, fRadianPerSec* fTimeDelta);

	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_RIGHT][0], (_vec3*)&m_matWorld.m[STATE_RIGHT][0], &matRotAxis);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_UP][0], (_vec3*)&m_matWorld.m[STATE_UP][0], &matRotAxis);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[STATE_LOOK][0], (_vec3*)&m_matWorld.m[STATE_LOOK][0], &matRotAxis);
}

// 0 : Look  || 1 : Up || 2 : Right
void CTransform::Set_PlusPosition(_uint iType, _float & fValue)
{
	_vec3	vLook = *Get_StateInfo(STATE_LOOK);
	_vec3	vRight = *Get_StateInfo(STATE_RIGHT);
	_vec3	vUp = *Get_StateInfo(STATE_UP);
	_vec3	vPosition = *Get_StateInfo(STATE_POSITION);



	switch (iType)
	{
	//Foward Plus
	case 0:
		vPosition += *D3DXVec3Normalize(&vLook, &vLook) * fValue;
		break;
	//Up Plus
	case 1:
		vPosition += *D3DXVec3Normalize(&vUp, &vUp) * fValue;
		break;
	//Right Plus
	case 2:
		vPosition += *D3DXVec3Normalize(&vRight, &vRight) * fValue;
		break;
	}

	Set_StateInfo(STATE_POSITION, &vPosition);
}

void CTransform::Set_PlusPosition(_vec3 vDir, _float & fValue)
{
	_vec3	vPosition = *Get_StateInfo(STATE_POSITION);
	vPosition += vDir * fValue;


	Set_StateInfo(STATE_POSITION, &vPosition);
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*		pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Ready_Transform()))
	{
		_MSG_BOX("CTransform Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone_Component()
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	CComponent::Free();
}
