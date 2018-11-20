#pragma once

#include "Base.h"
#include "Object_Manager.h"
#include "Input_Device.h"

_BEGIN(Engine)

class _ENGINE_DLL CScene abstract : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene() = default;
public:
	LPDIRECT3DDEVICE9 Get_Graphic_Device() const {
		return m_pGraphic_Device;}

	CInput_Device* Get_Input_Device() const {
		return m_pInput_Device; }
	HRESULT Add_Object_Prototype(const _uint& iSceneID, const _tchar* pProtoTag, CGameObject* pPrototype);
	HRESULT Add_Object(const _uint& iPSceneID, const _tchar* pProtoTag, const _uint& iSceneID, const _tchar* pLayerTag, CGameObject** ppGameObject = nullptr);
public:
	virtual HRESULT Ready_Scene() PURE;
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual _int LastUpdate_Scene(const _float& fTimeDelta);
	virtual void Render_Scene()PURE;
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	CInput_Device*				m_pInput_Device = nullptr;
private:
	CObject_Manager*			m_pObject_Manager = nullptr;
protected:
	virtual void Free();
};

_END