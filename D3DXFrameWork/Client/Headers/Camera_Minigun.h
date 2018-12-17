#pragma once

#include "Defines.h"
#include "Camera.h"
_BEGIN(Engine)

class CFrustum;

_END

_BEGIN(Client)

class CCamera_Minigun final : public CCamera
{
private:
	explicit CCamera_Minigun(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Minigun(const CCamera_Minigun& rhs);
	virtual ~CCamera_Minigun() = default;
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
	CFrustum*		m_pFrustumCom = nullptr;
private:
	_float			m_fCamSpeed = 0.f;

	_long			m_dwRotAcc[2] = { 0 };
	_float			m_fOffsetX = 0.f;
	_float			m_fOffsetY = 0.f;
	_float			m_fOffsetZ = 0.f;
	_vec3			m_vTmpAt;
private:
	const CGameObject*	m_pPlayer = nullptr;

public:
	static CCamera_Minigun* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();

};

_END