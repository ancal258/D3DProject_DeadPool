#pragma once

#include "Defines.h"
#include "GameObject.h"
_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Dynamic;
class CShader;
class CCollider;
class CNavigation;
_END

_BEGIN(Client)
class CBrawler abstract : public CGameObject
{
protected:
	explicit CBrawler(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBrawler(const CBrawler& rhs);
	virtual ~CBrawler() = default;
public:
	const _matrix& Get_HandMatrix(_uint iSide) const {
		return m_CombinedHandMatrix[iSide];
	}
public:
	void Set_Position(_vec3 vPos);
	void Set_Rotation(_float fDegree);
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

public:
	void CallBackFinish();
protected: // For. Component
	CTransform*         m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;
	CMesh_Dynamic*      m_pMeshCom = nullptr;
//protected: // For. Children;
	CGameObject*			m_pWeapon = nullptr;
protected: // For. Collider
	CCollider*			m_pColliderCom = nullptr;
	CCollider*			m_pColliderCom_Head = nullptr;
	CCollider*			m_pColliderCom_Body = nullptr;
	CCollider*			m_pColliderCom_Search = nullptr;
protected: // For. Player
	const CGameObject*	m_pPlayer[3] = { nullptr , nullptr, nullptr };

	_matrix             m_CombinedHandMatrix[2];
	const _matrix*      m_pHandMatrix[2] = { nullptr };
	_matrix            m_CombinedRootMatrix;
	const _matrix*      m_pRootMatrix = nullptr;
	_matrix				m_RealMatrix;
protected: // For. Animation
	_uint				m_iDeathIndex = 0;
	_uint				m_iIdleIndex = 0;
	_uint				m_iCurrentIndex = 0;
protected: // For. AttackCheck
	_bool			m_isDamaged = false;
	_bool			m_isLastDamaged = false;
	_bool			m_isSearch = false;
	_vec3			m_vPlayerDir;
	_vec3			m_vBrawlerLook;
	_bool			m_isCallDeathIdx = false;
	_float			m_fLength = 0;
	_float			m_fRadian = 0;
	_float			m_fDamegedTime = 0;

protected: // For. Attack to Player
	_bool			m_isAttack = false;
	_float			m_fAttackAcc = 0.f;
	_bool			m_isCompute = false;

protected: // For. Airplane Stage Move

protected: // For. StageNum
	_uint		m_iStageNum = 0;
protected:
	virtual HRESULT Ready_Component();
	HRESULT Update_HandMatrix();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void Compute_PlayerDir();
protected:
	virtual _int Update_Stage_Field(const _float& fTimeDelta);
	virtual _int LastUpdate_Stage_Field(const _float& fTimeDelta);

	virtual _int Update_Stage_Airplane(const _float& fTimeDelta);
	virtual _int LastUpdate_Stage_Airplane(const _float& fTimeDelta);
private:
	LPD3DXMESH				m_pColliderMesh[2] = { nullptr, nullptr };
	BOOL					m_Hit[2] = { FALSE };
	_float					m_fDist[2] = { 0 };
private:
	HRESULT isHitScan();
	HRESULT isHitScanAirplane();
	HRESULT CollisionCheck(_float fTimeDelta);
public:
	virtual CGameObject* Clone_GameObject() PURE;
protected:
	virtual void Free();
};

_END