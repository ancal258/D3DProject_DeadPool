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
class CPlayer;
class CBikiniGirl final : public CGameObject
{
private:
	enum ANIM_BIKINI {
		STATE_IDLE,
		STATE_HELP,
		STATE_END
	};
private:
	explicit CBikiniGirl(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBikiniGirl(const CBikiniGirl& rhs);
	virtual ~CBikiniGirl() = default;
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
	_bool				m_isCol = false;
	CPlayer*			m_pPlayer = nullptr;
private:
	HRESULT Ready_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CBikiniGirl* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END