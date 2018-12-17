#pragma once

//#include "Engine_Defines.h"
#include "Defines.h"
#include "GameObject.h"
_BEGIN(Engine)
class CTransform;
class CRenderer;
class CCollider;
_END

class CTriggerCube final : public CGameObject
{

private:
	explicit CTriggerCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTriggerCube(const CTriggerCube& rhs);
	virtual ~CTriggerCube() = default;
public:
	_vec3 Get_ObjectInfo();
	_bool Get_ClickedObject() {
		return m_isCol;	}
	void Set_TransformMode(_bool isTransformMode) {
		m_isTransformMode = isTransformMode;	}

public:
	void Add_String(CString pString);
	void Add_ButtonString(CString pString);
public:
	void Set_StateInfo(_vec3* vPos);
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
public:
	vector<CString>		m_vString;
	vector<CString>		m_vButtonString;

private:
	BOOL				m_isCol = false;
	_bool				m_isLButtonDown = false;
	_bool				m_isTransformMode = false;
private:
	HRESULT Ready_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CTriggerCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

