#pragma once

// 충돌하기위한 객체.

#include "Component.h"

_BEGIN(Engine)

class CShader;
class _ENGINE_DLL CCollider final : public CComponent
{
private:
	typedef struct tagOBB
	{
		_vec3		vPoint[8];
		_vec3		vCenter;
		_vec3		vAlignAxis[3]; // 박스와 평행한 축.
		_vec3		vProjDir[3]; // 박스의 중심에서 면의 중심을 바라보는 벡터.
	}OBB;
public:
	enum TYPE { TYPE_BOUNDINGBOX, TYPE_SPHERE };
	enum BOXTYPE { BOXTYPE_AABB, BOXTYPE_OBB, BOXTYPE_NONE };
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;
public:
	const _vec3* Get_Min() const {
		return &m_vMin;
	}
	const _vec3* Get_Max() const {
		return &m_vMax;
	}
	const OBB* Get_OBBDesc() const {
		return &m_tOBB;
	}
	const _matrix* Get_WorldMatrixPointer() const {
		return m_pWorldMatrix;
	}
	const LPD3DXMESH Get_Mesh() {
		return m_pMesh;	}
public:
	HRESULT Ready_Collider(TYPE eType);

	// 복사된 콜라이더의 로컬상태에 대하여 추가적으로 설정한다.
	HRESULT SetUp_Collider(const _matrix* pWorldMatrixPointer, const _matrix* pLocalMatrix);
	HRESULT SetUp_Collider(const _vec3* pMin, const _vec3* pMax, const _matrix* pWorldMatrixPointer, const _vec3* pTransMore = nullptr);
	HRESULT SetUp_Collider(const _matrix* pWorldMatrixPointer, const _vec3* pScale, const _vec3* pAngle, const _vec3* pPosition);
	_bool Collision_AABB(const CCollider* pTargetCollider);
	_bool Collision_OBB(const CCollider* pTargetCollider);

	// 월드변환을 위한 추가 상태변환 행렬을 얻어온다.
	void Render_Collider();
private:
	LPD3DXMESH				m_pMesh = nullptr;
	LPD3DXBUFFER			m_pAdjacency = nullptr;
	TYPE					m_eColliderType = TYPE(0);
	CShader*				m_pShader = nullptr;
	_vec3					m_vMin, m_vMax;
	_bool					m_isColl = false;
	const _matrix*			m_pWorldMatrix = nullptr;
private:
	OBB						m_tOBB;
private:
	HRESULT Create_BoundingBox();
	HRESULT Create_Sphere();
	// 실제 메시의 정보를 받아오거나 or
	// 내가 정의한 사이즈의 정보를 받아오거나.
	// 받아온 정보를 바탕으로 콜라이더 메시를 만들었다.
	// 이 메시의 민과 맥스를 구하자.
	HRESULT Compute_MinMax();
	HRESULT SetUp_OBB(); // 
public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END