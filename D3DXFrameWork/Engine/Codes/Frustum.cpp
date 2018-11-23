#include "..\Headers\Frustum.h"
#include "Transform.h"
#include "VIBuffer.h"

CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

HRESULT CFrustum::Ready_Frustum()
{
	// 투영 스페이스상에 존재하는 절두체를 구성하자.
	m_vOriPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vOriPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vOriPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vOriPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_vOriPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vOriPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vOriPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vOriPoint[7] = _vec3(-1.f, -1.f, 1.f);

	return NOERROR;
}

_int CFrustum::Update_Frustum(const _matrix * pViewMatrix, const _matrix * pProjMatrix)
{
	_matrix			matViewInv, matProjInv;

	D3DXMatrixInverse(&matProjInv, nullptr, pProjMatrix);
	D3DXMatrixInverse(&matViewInv, nullptr, pViewMatrix);

	// 투영스페이스 -> 뷰스페이스 -> 월드스페이스
	for (size_t i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vOriPoint[i], &matProjInv);
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matViewInv);
	}

	return _int();
}

_bool CFrustum::Culling_ToFrustum(CTransform * pTransform, CVIBuffer* pBuffer, const _float& fRadius)
{
	_vec3			vPoint[8];
	D3DXPLANE		Plane[6];

	_matrix			matWorldInv;

	D3DXMatrixInverse(&matWorldInv, nullptr, pTransform->Get_WorldMatrix());

	// 월드스페이스 -> 로컬스페이스
	for (size_t i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &m_vPoint[i], &matWorldInv);
	}

	if (FAILED(Make_Plane(Plane, vPoint)))
		return false;



	// ax + by + cz + d > 0 => 평면의 앞쪽에 존재한다.
	// ax + by + cz + d == 0 => 평면상에 있다.
	// ax + by + cz + d < 0 => 평면의 뒤쪽에 존재한다.
	if (nullptr == pBuffer)
	{
		_vec3		vPosition;
		vPosition = *pTransform->Get_StateInfo(CTransform::STATE_POSITION);

		D3DXVec3TransformCoord(&vPosition, &vPosition, &matWorldInv);

		if (false == is_InFrustum(Plane, &vPosition, fRadius))
			return true;
	}

	else // 버퍼가 유효하고 버퍼의 정점정보와 절두체를 컬링하고자한다.
	{
		pBuffer->Culling_ToFrustum(this, Plane);
	}


	return _bool();
}

_bool CFrustum::Culling_ToQuadTree(CTransform * pTransform, CVIBuffer * pBuffer, const _float & fRadius)
{
	_vec3			vPoint[8];
	D3DXPLANE		Plane[6];

	_matrix			matWorldInv;

	D3DXMatrixInverse(&matWorldInv, nullptr, pTransform->Get_WorldMatrix());

	// 월드스페이스 -> 로컬스페이스
	for (size_t i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &m_vPoint[i], &matWorldInv);
	}

	if (FAILED(Make_Plane(Plane, vPoint)))
		return false;



	// ax + by + cz + d > 0 => 평면의 앞쪽에 존재한다.
	// ax + by + cz + d == 0 => 평면상에 있다.
	// ax + by + cz + d < 0 => 평면의 뒤쪽에 존재한다.
	if (nullptr == pBuffer)
	{
		_vec3		vPosition;
		vPosition = *pTransform->Get_StateInfo(CTransform::STATE_POSITION);

		D3DXVec3TransformCoord(&vPosition, &vPosition, &matWorldInv);

		if (false == is_InFrustum(Plane, &vPosition, fRadius))
			return true;
	}

	else // 버퍼가 유효하고 버퍼의 정점정보와 절두체를 컬링하고자한다.
	{
		pBuffer->Culling_ToQuadTree(this, Plane);
	}


	return _bool();
}

HRESULT CFrustum::Make_Plane(D3DXPLANE * pPlanes, const _vec3 * pPoint)
{
	// +x
	D3DXPlaneFromPoints(&pPlanes[0], &pPoint[1], &pPoint[5], &pPoint[6]);
	// -x
	D3DXPlaneFromPoints(&pPlanes[1], &pPoint[4], &pPoint[0], &pPoint[3]);

	// +y
	D3DXPlaneFromPoints(&pPlanes[2], &pPoint[4], &pPoint[5], &pPoint[1]);
	// -y
	D3DXPlaneFromPoints(&pPlanes[3], &pPoint[3], &pPoint[2], &pPoint[6]);

	// +z
	D3DXPlaneFromPoints(&pPlanes[4], &pPoint[5], &pPoint[4], &pPoint[7]);
	// -z
	D3DXPlaneFromPoints(&pPlanes[5], &pPoint[0], &pPoint[1], &pPoint[2]);

	return NOERROR;
}

_bool CFrustum::is_InFrustum(const D3DXPLANE * pPlanes, const _vec3 * pPosition, _float fRadius)
{
	for (size_t i = 0; i < 6; i++)
	{
		// D3DXPlaneDotCoord(&pPlanes[i], pPosition) : 평면과 점의 거리
		if (fRadius <= D3DXPlaneDotCoord(&pPlanes[i], pPosition))
			return false;
	}

	return _bool(true);
}


CFrustum * CFrustum::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFrustum*		pInstance = new CFrustum(pGraphic_Device);

	if (FAILED(pInstance->Ready_Frustum()))
	{
		_MSG_BOX("CFrustum Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CFrustum::Clone_Component()
{
	AddRef();

	return this;
}

void CFrustum::Free()
{
	CComponent::Free();
}
