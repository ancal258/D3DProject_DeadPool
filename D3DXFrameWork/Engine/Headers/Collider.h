#pragma once

// �浹�ϱ����� ��ü.

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
		_vec3		vAlignAxis[3]; // �ڽ��� ������ ��.
		_vec3		vProjDir[3]; // �ڽ��� �߽ɿ��� ���� �߽��� �ٶ󺸴� ����.
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

	// ����� �ݶ��̴��� ���û��¿� ���Ͽ� �߰������� �����Ѵ�.
	HRESULT SetUp_Collider(const _matrix* pWorldMatrixPointer, const _matrix* pLocalMatrix);
	HRESULT SetUp_Collider(const _vec3* pMin, const _vec3* pMax, const _matrix* pWorldMatrixPointer, const _vec3* pTransMore = nullptr);
	HRESULT SetUp_Collider(const _matrix* pWorldMatrixPointer, const _vec3* pScale, const _vec3* pAngle, const _vec3* pPosition);
	_bool Collision_AABB(const CCollider* pTargetCollider);
	_bool Collision_OBB(const CCollider* pTargetCollider);

	// ���庯ȯ�� ���� �߰� ���º�ȯ ����� ���´�.
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
	// ���� �޽��� ������ �޾ƿ��ų� or
	// ���� ������ �������� ������ �޾ƿ��ų�.
	// �޾ƿ� ������ �������� �ݶ��̴� �޽ø� �������.
	// �� �޽��� �ΰ� �ƽ��� ������.
	HRESULT Compute_MinMax();
	HRESULT SetUp_OBB(); // 
public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END