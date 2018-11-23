#pragma once

#include "Component.h"

_BEGIN(Engine)

class CTransform;
class CVIBuffer;
class _ENGINE_DLL CFrustum final : public CComponent
{
private:
	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CFrustum() = default;
public:
	HRESULT Ready_Frustum();
	_int Update_Frustum(const _matrix* pViewMatrix, const _matrix* pProjMatrix);
	_bool Culling_ToFrustum(CTransform* pTransform, CVIBuffer* pBuffer = nullptr, const _float& fRadius = 0.f); // ���ý����̽� �浹�Ѵ� 
	_bool Culling_ToQuadTree(CTransform* pTransform, CVIBuffer* pBuffer = nullptr, const _float& fRadius = 0.f); // ���ý����̽� �浹�Ѵ� 
	_bool is_InFrustum(const D3DXPLANE* pPlanes, const _vec3* pPosition, _float fRadius);
private:
	_vec3			m_vOriPoint[8]; // ������ü ������ ���õǴ� ���������̽� ���� ����ü����.
	_vec3			m_vPoint[8];	// ī�޶� ������Ʈ���� ȣ���ؼ� ���彺���̽����� �Űܳ��� ����ü ����.
private:
	HRESULT Make_Plane(D3DXPLANE* pPlanes, const _vec3* pPoint);

public:
	static CFrustum* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END