#pragma once

#include "Base.h"
#include "Brawler.h"

class CBrawler_Manager final : public CBase
{
	_DECLARE_SINGLETON(CBrawler_Manager)
private:
	explicit CBrawler_Manager();
	virtual ~CBrawler_Manager() = default;

public:
	HRESULT Add_Object(CGameObject* pGameObject);
	HRESULT Clear_Object(CGameObject* pGameObject);
	list<CGameObject*>* Get_BrawlerList() {
		return &m_BrawlerList;	}
	_bool Collision_Chech(CBrawler* pBrawler, _float *pLength);

private:
	list<CGameObject*>		m_BrawlerList;
protected:
	virtual void Free();
};

