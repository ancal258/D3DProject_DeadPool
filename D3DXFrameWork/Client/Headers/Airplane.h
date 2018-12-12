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
class CAirplane final : public CGameObject
{
private:
	explicit CAirplane(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CAirplane(const CAirplane& rhs);
	virtual ~CAirplane() = default;
public:
	void Set_StateInfo(_vec3* pRight, _vec3* pUp, _vec3* vLook, _vec3* vPos);
	const _matrix* Get_WorldMatrix() const;
public:
	virtual HRESULT Ready_GameObject_Prototype();
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

private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CAirplane* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END