#pragma once

#include "Defines.h"
#include "Camera.h"

_BEGIN(Client)

class CCamera_Cinematic final : public CCamera
{
private:
	explicit CCamera_Cinematic(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Cinematic(const CCamera_Cinematic& rhs);
	virtual ~CCamera_Cinematic() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	virtual HRESULT SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc);
	void SetUp_CameraMove(vector<_vec3> vecEyePoints,_vec3 vLookAt, _float fTime);
public:
	_vec3 Bezier4(_vec3 vPoint_1, _vec3 vPoint_2, _vec3 vPoint_3, _vec3 vPoint_4, double Offset);
private:
	_float				m_fCamSpeed = 0.f;
	vector<_vec3>		m_vecEyePoints;
	_uint				m_iCurrentIndex = 0;
	_double				m_dlCurrentOffset = 0;
	_float				m_fTime = 0; // 걸릴 시간
public:
	static CCamera_Cinematic* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();

};

_END