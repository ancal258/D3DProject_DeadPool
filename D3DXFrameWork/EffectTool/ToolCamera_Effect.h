#pragma once

#include "Engine_Defines.h"
#include "Camera.h"

class CToolCamera_Effect final : public CCamera
{
private:
	explicit CToolCamera_Effect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CToolCamera_Effect(const CToolCamera_Effect& rhs);
	virtual ~CToolCamera_Effect() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	HRESULT SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc);
public:
	_float			m_fCamSpeed = 0.f;
	_float			m_Rotate_Speed = 0.02f;
	_float			m_Move_Speed = 0.05f;

public:
	static CToolCamera_Effect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};
