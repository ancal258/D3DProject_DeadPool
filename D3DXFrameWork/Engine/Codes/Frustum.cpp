#include "..\Headers\Frustum.h"
#include "Transform.h"
#include "VIBuffer.h"

CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

HRESULT CFrustum::Ready_Frustum()
{
	// ���� �����̽��� �����ϴ� ����ü�� ��������.
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

	// ���������̽� -> �佺���̽� -> ���彺���̽�
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

	// ���彺���̽� -> ���ý����̽�
	for (size_t i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &m_vPoint[i], &matWorldInv);
	}

	if (FAILED(Make_Plane(Plane, vPoint)))
		return false;



	// ax + by + cz + d > 0 => ����� ���ʿ� �����Ѵ�.
	// ax + by + cz + d == 0 => ���� �ִ�.
	// ax + by + cz + d < 0 => ����� ���ʿ� �����Ѵ�.
	if (nullptr == pBuffer)
	{
		_vec3		vPosition;
		vPosition = *pTransform->Get_StateInfo(CTransform::STATE_POSITION);

		D3DXVec3TransformCoord(&vPosition, &vPosition, &matWorldInv);

		if (false == is_InFrustum(Plane, &vPosition, fRadius))
			return true;
	}

	else // ���۰� ��ȿ�ϰ� ������ ���������� ����ü�� �ø��ϰ����Ѵ�.
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

	// ���彺���̽� -> ���ý����̽�
	for (size_t i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &m_vPoint[i], &matWorldInv);
	}

	if (FAILED(Make_Plane(Plane, vPoint)))
		return false;



	// ax + by + cz + d > 0 => ����� ���ʿ� �����Ѵ�.
	// ax + by + cz + d == 0 => ���� �ִ�.
	// ax + by + cz + d < 0 => ����� ���ʿ� �����Ѵ�.
	if (nullptr == pBuffer)
	{
		_vec3		vPosition;
		vPosition = *pTransform->Get_StateInfo(CTransform::STATE_POSITION);

		D3DXVec3TransformCoord(&vPosition, &vPosition, &matWorldInv);

		if (false == is_InFrustum(Plane, &vPosition, fRadius))
			return true;
	}

	else // ���۰� ��ȿ�ϰ� ������ ���������� ����ü�� �ø��ϰ����Ѵ�.
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
		// D3DXPlaneDotCoord(&pPlanes[i], pPosition) : ���� ���� �Ÿ�
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
