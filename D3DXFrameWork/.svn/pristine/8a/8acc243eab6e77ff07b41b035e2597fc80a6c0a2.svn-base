#pragma once

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL CPicking final : public CComponent
{
private:
	explicit CPicking(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CPicking() = default;
public:
	HRESULT Ready_Picking();
	_bool Picking_ToPolygon(HWND hWnd, const _vec3 * pPointA, const _vec3 * pPointB, const _vec3 * pPointC, const _matrix* pWorldMatrix, _vec3* pOut) const;
public:
	static CPicking* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END