#pragma once

#include "Defines.h"
#include "Scene.h"

_BEGIN(Client)
class CLoading;

class CScene_Intro final : public CScene
{
private:
	explicit CScene_Intro(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Intro() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual _int LastUpdate_Scene(const _float& fTimeDelta);
	virtual void Render_Scene();
	_bool Is_Finish() {
		return m_isFinish;	}
private:
	HWND				m_hVideo = 0;
	_bool				m_isFinish = false;
public:
	static CScene_Intro* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

_END