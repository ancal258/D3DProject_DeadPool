#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Dynamic;
class CShader;
class CCollider;
_END

_BEGIN(Client)
class CDog final : public CGameObject
{
private:
	enum ANIM_DOG {
		Dog_Idle_01,
		Dog_Idle_02,
		Dog_Idle_03,
		Dog_Idle_04,
		Dog_Idle_05,
		Dog_Idle_06,
		Dog_InteratctDP,
		Dog_End
	};
private:
	explicit CDog(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CDog(const CDog& rhs);
	virtual ~CDog() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

private:
	CTransform*         m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Dynamic*      m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
private:
	_uint				m_iIndex = 0;
private:
	HRESULT Ready_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CDog* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END