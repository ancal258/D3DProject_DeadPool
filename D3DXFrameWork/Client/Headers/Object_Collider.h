#pragma once
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CBuffer_Collider;
class CShader;
_END

_BEGIN(Client)
class CObject_Collider : public CGameObject
{
private:
	explicit CObject_Collider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CObject_Collider(const CObject_Collider& rhs);
	virtual ~CObject_Collider() = default;
public:
	const void Set_Position(_vec3 pPosition) {
		m_vPosition = pPosition;}
	void Set_Scale(_vec3 vScale);
	COLLIDER Get_ObjectCollider() {
		return m_tagCollider;}
public:
	bool IsCollision(COLLIDER tagDestCollider);
	void Update_ColliderInfo();
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CBuffer_Collider*	m_pBufferCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

	_vec3				m_vPosition;
	_vec3				m_vScale;
	COLLIDER			m_tagCollider;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CObject_Collider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END
