#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CBuffer_RcTex;
class CTexture;
class CShader;
_END

class CEffectT abstract : public CGameObject
{
protected:
	explicit CEffectT(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffectT(const CEffectT& rhs);
	virtual ~CEffectT() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	void Set_EffectInfo(CGameObject* pParent, _float fFrameSpeed, _float fFrameMax, _float fMoveSpeed, _float fSurviveTime, _float fDegreeRange, _vec3* vSetScale, _vec3* vSetPos, _vec3* vDir);
protected:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CBuffer_RcTex*		m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
protected:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
protected:
	_float				m_fFrame = 0.f; // 애니메이션의 현재 프레임
	_float				m_fFrameSpeed = 90.f; // 애니메이션 프레임 속도
	_float				m_fFrameMax = 90.f; // 애니메이션 최대 프레임
	_float				m_fMoveSpeed = 0.f;
	_float				m_fSurviveTime = 0.f;
	_float				m_fDegreeRange = 0.f; // 회전되어있는 각도 ( 범위 )
	_float				m_fAlpha = 1.f; // 애니메이션의 Alpha값 ( 0 ~ 1 ) ( 0 ~ m_fFrame / m_fFrameMax ) 
	_float				m_fTimeAcc = 0.f;
	_vec3				m_vSetScale; // 생성 크기
	_vec3				m_vSetPos; // 생성 위치
	_vec3				m_vDir; // 움직이는 방향 

	CGameObject*		m_pParent = nullptr;

protected:
	virtual void Free();
};