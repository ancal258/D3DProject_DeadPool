#pragma once

#include "GameObject.h"
#include "Input_Device.h"

_BEGIN(Engine)

class _ENGINE_DLL CCamera abstract : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;
public:
	void Set_CameraDesc(const CAMERADESC& Camera_Desc) {
		m_Camera_Desc = Camera_Desc; }
	void Set_ProjectionDesc(const PROJDESC& Projection_Desc) {
		m_Projection_Desc = Projection_Desc; }
	void Set_IsCameraOn(_bool isCameraOn) {
		m_isCameraOn = isCameraOn; }
public:
	_byte Get_DIKState(_ubyte byKeyID) {
		return m_pInput_Device->Get_DIKeyState(byKeyID); }
	_long Get_DIMouseMove(CInput_Device::MOUSEMOVE eMoveID) {
		return m_pInput_Device->Get_DIMouseMove(eMoveID); }
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
	virtual HRESULT SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc) PURE;
	virtual HRESULT SetUp_Target(const CGameObject* pGameObject) { return NOERROR; };
protected:
	HRESULT SetUp_Matrix();
public:
protected:
	_matrix				m_matWorld;
	_matrix				m_matView;
	CAMERADESC			m_Camera_Desc;
protected:
	_bool				m_isCameraOn = false;
protected:
	_matrix				m_matProj;
	PROJDESC			m_Projection_Desc;
private:
	CInput_Device*		m_pInput_Device = nullptr;

public:
	virtual CGameObject* Clone_GameObject() PURE;
protected:
	virtual void Free();
};

_END