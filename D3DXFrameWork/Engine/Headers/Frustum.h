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
	_bool Culling_ToFrustum(CTransform* pTransform, CVIBuffer* pBuffer = nullptr, const _float& fRadius = 0.f); // 로컬스페이스 충돌한다 
	_bool Culling_ToQuadTree(CTransform* pTransform, CVIBuffer* pBuffer = nullptr, const _float& fRadius = 0.f); // 로컬스페이스 충돌한다 
	_bool is_InFrustum(const D3DXPLANE* pPlanes, const _vec3* pPosition, _float fRadius);
private:
	_vec3			m_vOriPoint[8]; // 원형객체 생성시 셋팅되는 투영스페이스 상의 절두체정보.
	_vec3			m_vPoint[8];	// 카메라 업데이트에서 호출해서 월드스페이스까지 옮겨놓은 절두체 정보.
private:
	HRESULT Make_Plane(D3DXPLANE* pPlanes, const _vec3* pPoint);

public:
	static CFrustum* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END