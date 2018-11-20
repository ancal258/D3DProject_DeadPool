#pragma once

#include "Base.h"

_BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCell() = default;
public:
	const _vec3* Get_Point(POINT ePoint) const {
		return &m_vPoint[ePoint];
	}

	void Set_Neighbor(NEIGHBOR eNeighborID, CCell* pCell) {
		m_pNeighbor[eNeighborID] = pCell;
	}
public:
	HRESULT Ready_Cell(const _vec3 * pPoints, const _ulong& dwIndex);
	_bool Compare_Point(const _vec3* pSourPoint, const _vec3* pDestPoint);
	_bool Is_Move(const _vec3* pPosition, _ulong* pIndex);
	_bool Is_Sliding(const _vec3* pPosition, _ulong* pIndex);
	_vec2 Sliding_Move(const _vec3* pLook, _ulong dwSlideIndex);
	void Render_Cell(LPD3DXLINE pLine);

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	_vec3					m_vPoint[POINT_END];
	CCell*					m_pNeighbor[NEIGHBOR_END];
	_ulong					m_dwIndex = 0;
	_vec2					m_vDir[LINE_END];
	_vec2					m_vNormal[LINE_END];

public:
	static CCell* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3* pPoints, const _ulong& dwIndex);
protected:
	virtual void Free();
};

_END