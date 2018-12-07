#pragma once

//#include "Engine_Defines.h"
#include "Defines.h"
#include "GameObject.h"
_BEGIN(Engine)
class CTransform;
class CRenderer;
class CCollider;
_END

class CNavPoint final : public CGameObject
{

private:
	explicit CNavPoint(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CNavPoint(const CNavPoint& rhs);
	virtual ~CNavPoint() = default;
public:
	_vec3 Get_ObjectInfo();
	_bool Get_ClickedObject() {
		return m_isCol;
	}
	void Set_TransformMode(_bool isTransformMode) {
		m_isTransformMode = isTransformMode;
	}
public:
	void Set_StateInfo(_vec3* pRight, _vec3* pUp, _vec3* vLook, _vec3* vPos);
	void Set_Position(_vec3* pPosition) const;
	void Set_Scale(_vec3* pScale) const;
	void Set_PlusPosition(_uint iType, _float & fValue) const;
	void RotationY(const _float & fRadian) const;
	void Scaling(_float fX, _float fY, _float fZ) const;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

	LPD3DXMESH				m_pMesh = nullptr;
private:
	BOOL				m_isCol = false;
	_bool				m_isLButtonDown = false;
	_bool				m_isTransformMode = false;
public:
	_vec3				m_vWorldPosition;
private:
	HRESULT Ready_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CNavPoint* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

