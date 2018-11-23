#pragma once

#include "Defines.h"
#include "Camera.h"
_BEGIN(Engine)

class CFrustum;

_END

_BEGIN(Client)

class CCamera_Debug final : public CCamera
{
private:
	explicit CCamera_Debug(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Debug(const CCamera_Debug& rhs);
	virtual ~CCamera_Debug() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	_bool Culling_ToFrustum(CTransform* pTransform, CVIBuffer* pBuffer = nullptr, const _float& fRadius = 0.f);
	_bool Culling_ToQuadTree(CTransform* pTransform, CVIBuffer* pBuffer = nullptr, const _float& fRadius = 0.f);
	HRESULT Ready_Component();

public:
	HRESULT SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc);
private:
	_float			m_fCamSpeed = 0.f;
	CFrustum*		m_pFrustumCom = nullptr;

public:
	static CCamera_Debug* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();

};

_END