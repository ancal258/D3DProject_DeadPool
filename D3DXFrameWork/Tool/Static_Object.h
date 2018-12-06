#pragma once

//#include "Engine_Defines.h"
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

class CStatic_Object final : public CGameObject
{

private:
	explicit CStatic_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CStatic_Object(const CStatic_Object& rhs);
	virtual ~CStatic_Object() = default;
public:
	STATIC_OBJECT_INFO Get_ObjectInfo();
	void Set_PrototypeTag(const _tchar* pPrototype_Tag) {
		m_pPrototype_Tag = pPrototype_Tag;
	}
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

private:
	const _tchar*		m_pComponent_Tag = nullptr;
	const _tchar*		m_pPrototype_Tag = nullptr;

private:
	_bool				m_isCol = false;
	_bool				m_isLButtonDown = false;
	_bool				m_isTransformMode = false;
public:
	_bool				m_isLock = false;
private:
	HRESULT Ready_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CStatic_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pComponent_Tag);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

