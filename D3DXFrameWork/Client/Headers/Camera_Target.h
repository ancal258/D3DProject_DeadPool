#pragma once

#include "Defines.h"
#include "Camera.h"
_BEGIN(Engine)

class CFrustum;

_END
_BEGIN(Client)

class CCamera_Target final : public CCamera
{
private:
	explicit CCamera_Target(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Target(const CCamera_Target& rhs);
	virtual ~CCamera_Target() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	virtual HRESULT SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc);
	virtual HRESULT SetUp_Target(const CGameObject* pGameObject);
public:
	HRESULT Ready_Component();
	_bool Culling_ToFrustum(CTransform* pTransform, CVIBuffer* pBuffer = nullptr, const _float& fRadius = 0.f);
	_bool Culling_ToQuadTree(CTransform* pTransform, CVIBuffer* pBuffer = nullptr, const _float& fRadius = 0.f);
private:
	CFrustum*		m_pFrustumCom = nullptr;

private:
	_float				m_fCamSpeed = 0.f;
	const CGameObject*	m_pTarget = nullptr;
	const _matrix*		m_pTargetWorldMatrix = nullptr;
	const _long*		m_pTargetMouseMove = nullptr;
	_float				m_fAngle[2] = { 0 };
private:
	_float				m_fCameraEyeX = 0.f;
	_float				m_fCameraEyeY = 0.f;
	_float				m_fCameraEyeZ = 0.f;
	_float				m_fCameraAtX = 0.f;
	_float				m_fCameraAtY = 0.f;
	_float				m_fCameraAtZ = 0.f;
	_uint				m_iStage = 0;
private:
	void MouseEvent();
public:
	static CCamera_Target* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();

};

_END