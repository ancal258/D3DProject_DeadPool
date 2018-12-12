#pragma once

#include "Defines.h"
#include "Scene.h"

_BEGIN(Client)

class CScene_AirPlane final : public CScene
{
private:
	explicit CScene_AirPlane(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_AirPlane() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual _int LastUpdate_Scene(const _float& fTimeDelta);
	virtual void Render_Scene();

private: // Frame
	_ulong						m_dwRenderCnt = 0;
	_float						m_fTimeAcc = 0.f;
	_tchar						m_szFPS[128] = L"";
public:
	static CScene_AirPlane* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

_END