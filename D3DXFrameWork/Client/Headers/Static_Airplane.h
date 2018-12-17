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
class CStatic_Airplane final : public CGameObject
{
private:
	explicit CStatic_Airplane(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CStatic_Airplane(const CStatic_Airplane& rhs);
	virtual ~CStatic_Airplane() = default;
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
	_float				m_fValue = 0;
	_float				m_fTimeValue = 0;
	_float				m_fSpeed = 0.017f;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CStatic_Airplane* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END