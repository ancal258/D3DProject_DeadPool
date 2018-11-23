#pragma once


// ����ü �ø��� ������ �����ϱ����� �ʿ��� Ŭ����.
// �ø��� ������ �����ϱ����� �ʿ��� �߰����� ������ ������.
// ������ ����Ʈ�� ��ü�� ��ü������ �߰����� ������ �ǹ��Ѵ�.

#include "Base.h"

_BEGIN(Engine)

class CFrustum;
class _ENGINE_DLL CQuadTree final : public CBase
{
public:
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_RB, CHILD_LB, CHILD_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_RB, CORNER_LB, CORNER_END };
	enum NEIGHBOR { NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END };
private:
	explicit CQuadTree(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CQuadTree() = default;
public:
	HRESULT Ready_QuadTree(const _vec3* pVerticesPos, const _uint& iNumVerticesX, const _uint& iNumVerticesZ);
	HRESULT Ready_QuadTree(const _vec3* pVerticesPos, const _uint& iLT, const _uint& iRT, const _uint& iRB, const _uint& iLB);
	void Make_ChildQuadTree(const _vec3* pVerticesPos);
	void Culling(CFrustum* pFrustum, D3DXPLANE* pPlanes, POLYGON32* pIndices, _ulong* pNumPolygons);
	void Set_Neighbor();
	_bool LevelofDetail();
private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	_uint					m_iCorner[CORNER_END] = { 0 }; // ����Ʈ���� �𼭸� �װ� ������ �ε���.
	_uint					m_iCenter = 0;
	_float					m_fRadius = 0.f; // ����Ʈ���� �����ϴ� ���� �������̴�.
	CQuadTree*				m_pChild[CHILD_END] = { nullptr };
	CQuadTree*				m_pNeighbor[NEIGHBOR_END] = { nullptr };
	const _vec3*			m_pVerticesPos = nullptr;
public:
	static CQuadTree* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3* pVerticesPos, const _uint& iNumVerticesX, const _uint& iNumVerticesZ);
	static CQuadTree* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3* pVerticesPos, const _uint& iLT, const _uint& iRT, const _uint& iRB, const _uint& iLB);
protected:
	virtual void Free();
};

_END