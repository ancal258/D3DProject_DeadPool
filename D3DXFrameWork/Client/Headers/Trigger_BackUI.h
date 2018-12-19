#pragma once

//#include "Engine_Defines.h"
#include "Defines.h"
#include "GameObject.h"
_BEGIN(Engine)
class CTransform;
class CRenderer;
class CCollider;
_END

_BEGIN(Client)
class CStaticBackUI;
_END

class CTrigger_BackUI final : public CGameObject
{

private:
	explicit CTrigger_BackUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTrigger_BackUI(const CTrigger_BackUI& rhs);
	virtual ~CTrigger_BackUI() = default;
public:
	_bool Get_ClickedObject() {
		return m_isCol;
	}
public:
	void Set_StateInfo(_vec3* vPos);
	void Add_String(_tchar* pString);
	void Add_ButtonString(_tchar* pString);
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
	CStaticBackUI*		m_pBackUI = nullptr;
	vector<_tchar*>		m_vevString;
	vector<_tchar*>		m_vecButtonString;
	_float				m_fOffsetY = 0; // ��ȭ���� Y ������
	_uint				m_iKind = 0; // 0 : ȭ��Ʈ // 1 : ������
private:
	_uint				m_iBoxNum = 0; // ��ȭ���� ��ȣ
	_float				m_fTimeAcc = 0.f;
private:
	HRESULT Ready_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void Create_BackUI();
public:
	static CTrigger_BackUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};
