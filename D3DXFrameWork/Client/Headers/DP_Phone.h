#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Static;
class CTexture;
class CShader;
class CCollider;
_END

_BEGIN(Client)

class CPlayer;
class CDP_Phone final : public CGameObject
{
private:
	explicit CDP_Phone(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CDP_Phone(const CDP_Phone& rhs);
	virtual ~CDP_Phone() = default;
public:
	_vec3 Get_BeginPoint() {
		return m_vPointBegin;
	}
	_vec3 Get_EndPoint() {
		return m_vPointEnd;
	}
public:
	virtual HRESULT Ready_GameObject_Prototype(_uint iSide);
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Static*		m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	CCollider*			m_pColliderCom_Sphere[5] = { nullptr , nullptr, nullptr, nullptr };
private:
	CPlayer*			m_pPlayer = nullptr;
	CGameObject*		m_pTrail = nullptr;
	_uint				m_iSide = 0; // 0 == ÁÂÃø , 1 == ¿ìÃø

	_vec3				m_vPointBegin;
	_vec3				m_vPointEnd;

private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT SetUp_PlayerPointer();

public:
	static CDP_Phone* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iSide);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END