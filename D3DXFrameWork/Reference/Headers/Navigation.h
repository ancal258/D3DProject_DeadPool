#pragma once

#include "Component.h"

_BEGIN(Engine)

class CCell;
class CTransform;
class _ENGINE_DLL CNavigation final : public CComponent
{
private:
	explicit CNavigation(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;
public:
	HRESULT Ready_Navigation(const _tchar* pFilePath);
	HRESULT SetUp_Navigation(const _uint & iNaviIndex);
	void Compute_Animation(_matrix* pRealMatrix, CTransform * pTransformCom, const _float & fSpeedPerSec, const _float & fTimeDelta);
	void Update_LastMatrix(_matrix* pLastMatrix) {
		m_LastMatrix = *pLastMatrix;	}
	_bool Move_OnNavigation(CTransform* pTransformCom, const _float& fSpeedPerSec, const _float& fTimeDelta);
	HRESULT Add_Cell(_vec3* vPosArray);
	vector<_vec3>* Get_vecPoint();
	void Render_Navigation();
private:
	vector<CCell*>			m_vecCell;
	typedef vector<CCell*>	VECCELL;
	vector<_vec3> m_vecPoint;
private:
	_ulong					m_dwCurrentIdx = 0;
	_ulong					m_dwNumCell = 0;
	LPD3DXLINE				m_pLine = nullptr;
private:
	_matrix				m_LastMatrix;
private:
	HRESULT SetUp_Neighbor();

public:
	static CNavigation* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END