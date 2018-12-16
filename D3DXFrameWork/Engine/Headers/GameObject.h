#pragma once

#include "Base.h"

// 원형객체를 우선생성.
// 복제본을 생성하여 실제 렌더링.


_BEGIN(Engine)

class CComponent;
class _ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	LPDIRECT3DDEVICE9 Get_Graphic_Device() const {
		return m_pGraphic_Device;}
	CComponent* Get_ComponentPointer(const _tchar* pComponentTag) const;
	HRESULT Get_Transform(D3DTRANSFORMSTATETYPE eType, _matrix* pMatrix) {
		return m_pGraphic_Device->GetTransform(eType, pMatrix); }	
	_float Get_ViewDepth() const {
		return m_fDepth;}
public:
	_bool Get_Lived() {
		return m_isLived; }
	_int Get_HP() {
		return m_iHP;}
	void Set_Lived(_bool isLived) {
		m_isLived = isLived;}
	void Set_Active(_bool isActive) {
		m_isActive = isActive;	}
public:	
	HRESULT Set_Transform(D3DTRANSFORMSTATETYPE eType, const _matrix* pMatrix) {
		return m_pGraphic_Device->SetTransform(eType, pMatrix); }
	HRESULT Add_Component(const _tchar*, CComponent* pComponent);

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
protected:
	void Compute_ViewDepth(const _vec3* pWorldPos);
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device;	
	_float						m_fDepth;

private:
	map<const _tchar*, CComponent*>			m_mapComponent;
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENT;
protected:
	_bool						m_isLived = true; // 살았니? 죽었니?
	_bool						m_isActive = true; // 활성화 되어있니? 안되어있니?
	_int						m_iHP = 1;
private:
	CComponent* Find_Component(const _tchar* pComponentTag) const;

public:
	virtual CGameObject* Clone_GameObject() PURE;
protected:
	virtual void Free();

};

_END