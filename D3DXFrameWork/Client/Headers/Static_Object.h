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
class CStatic_Object final : public CGameObject
{
private:
	explicit CStatic_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CStatic_Object(const CStatic_Object& rhs);
	virtual ~CStatic_Object() = default;
public:
	void Set_StateInfo(_vec3* pRight, _vec3* pUp, _vec3* vLook, _vec3* vPos, _bool isOffCulling, _bool isExplosion);
	void Set_StateInfo(_vec3* vPos, _vec3 vScale, _float fDegree ,_bool isOffCulling, _bool isExplosion);
	void Set_Stage(_uint iStage) {
		m_iStage = iStage;	}
	HRESULT SetUp_Radius();
public:
	virtual HRESULT Ready_GameObject_Prototype(const _tchar* pComponent_Tag);
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
	const _tchar*		m_pComponent_Tag = nullptr;
	// col
	_bool				m_isCol = false;
	_bool				m_isOffCulling = false;
	_bool				m_isExplosion = false;
	_float				m_fRadius = 0;
	_uint				m_iStage = 0;
private:
	const CGameObject*	m_pAirplane = nullptr;

private:
	LPD3DXMESH				m_pColliderMesh = nullptr;
	BOOL					m_Hit = FALSE ;
	_bool					m_isDamaged = false;
	_float					m_fDist =  0 ;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT isHitScan();
public:
	static CStatic_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pComponent_Tag);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END