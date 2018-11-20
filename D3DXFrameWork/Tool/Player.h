#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Dynamic;
class CShader;
_END

class CPlayer final : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
public:
	void Set_AnimationInfo(_float fTimeDelta, _float fSpeed, _float fArrived, _uint iAnimationIndex) {
		m_fTimeDelta = fTimeDelta;
		m_fSpeed = fSpeed;
		m_fArrived = fArrived;
		m_iAnimationIndex = iAnimationIndex;}
	void Set_Play() {
		m_isPlay = true;}
	void Set_Stop() {
		m_isPlay = false;}
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

	_float				m_fTimeDelta = 0;
	_float				m_fSpeed = 0;
	_float				m_fArrived = 0;
	_uint				m_iAnimationIndex = 0;
	_bool				m_isPlay = false;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};
