#pragma once

#include "Base.h"
#include "Layer.h"

_BEGIN(Engine)

class _ENGINE_DLL CObject_Manager final : public CBase
{
	_DECLARE_SINGLETON(CObject_Manager)
private:
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;
public: // 
	const CGameObject* Get_ObjectPointer(const _uint& iSceneID, const _tchar* pLayerTag, const _uint& iIndex);

public:
	HRESULT Reserve_Object_Manager(const _uint& iMaxScene);
	HRESULT Add_Object_Prototype(const _uint& iSceneID, const _tchar* pProtoTag, CGameObject* pPrototype);
	HRESULT Add_Object(const _uint& iPSceneID, const _tchar* pProtoTag, const _uint& iSceneID, const _tchar* pLayerTag, CGameObject** ppGameObject);
	HRESULT Clear_Object(const _uint& iSceneID);
public:
	_int Update_Object_Manager(const _float& fTimeDelta);
	_int LastUpdate_Object_Manager(const _float& fTimeDelta);
private:
	_uint										m_iMaxScene = 0;
	map<const _tchar*, CGameObject*>*			m_pPrototype = nullptr;
	typedef map<const _tchar*, CGameObject*>	MAPPROTOTYPE;
	map<const _tchar*, CLayer*>*				m_pLayer = nullptr;
	typedef map<const _tchar*, CLayer*>			MAPLAYER;
private:
	CGameObject* Find_Prototype(const _uint& iPSceneID, const _tchar* pProtoTag);
	CLayer* Find_Layer(const _uint& iSceneID, const _tchar* pLayerTag);
protected:
	virtual void Free();
};

_END