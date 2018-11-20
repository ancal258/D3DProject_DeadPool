#pragma once

#include "GameObject.h"

_BEGIN(Engine)

class CLayer final : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;
public:
	const CGameObject* Get_ObjectPointer(const _uint& iIndex);
public:
	HRESULT Ready_Layer();
	HRESULT Add_ObjectToLayer(CGameObject* pGameObject);
	_int Update_Layer(const _float& fTimeDelta);
	_int LastUpdate_Layer(const _float& fTimeDelta);
private:
	list<CGameObject*>			m_ObjectList;
	typedef list<CGameObject*>	OBJECTLIST;
public:
	static CLayer* Create();
protected:
	virtual void Free();
};

_END