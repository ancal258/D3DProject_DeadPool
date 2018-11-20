#pragma once

#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;
public:
	LPDIRECT3DDEVICE9 Get_Graphic_Device() const {
		return m_pGraphic_Device;}
	_bool is_Clone() const {
		return m_isClone; }
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	_bool						m_isClone = false;
public:
	virtual CComponent* Clone_Component() = 0;
protected:
	virtual void Free();


};

_END