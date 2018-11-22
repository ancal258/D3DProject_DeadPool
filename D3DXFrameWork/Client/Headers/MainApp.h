#pragma once

#include "Defines.h"
#include "Base.h"
#include "Graphic_Device.h"

_BEGIN(Engine)
class CManagement;
class CRenderer;
class CScene;
_END

_BEGIN(Client)


class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT Ready_MainApp();
	_int Update_MainApp(const _float& fTimeDelta);
	void Render_MainApp();
private:
	CManagement*				m_pManagement = nullptr;
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	CRenderer*					m_pRenderer = nullptr;
private: // Frame
	_ulong						m_dwRenderCnt = 0;
	_float						m_fTimeAcc = 0.f;
	_tchar						m_szFPS[128] = L"";
private:
	CScene*						m_pScene = nullptr;
	_bool						m_isFinish = false;

private:
	HRESULT Ready_Default_Setting(CGraphic_Device::WINMODE eMode, const _uint& iBackCX, const _uint& iBackCY);	
	HRESULT Ready_Static_Component();
	HRESULT Ready_Static_GameObject();
	HRESULT Ready_Start_Scene(SCENEID eID);

	HRESULT Ready_Gara();
public:
	static CMainApp* Create();
protected:
	virtual void Free();
};

_END