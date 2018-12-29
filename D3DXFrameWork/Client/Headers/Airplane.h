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
class CCamera_Debug;
class CCamera_Minigun;
class CAirplane final : public CGameObject
{
private:
	explicit CAirplane(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CAirplane(const CAirplane& rhs);
	virtual ~CAirplane() = default;
public:
	void Set_StateInfo(_vec3* pRight, _vec3* pUp, _vec3* vLook, _vec3* vPos);
	const _matrix* Get_WorldMatrix() const;
	_uint Get_DestroyPoint() {
		return m_iDestroyPoint;	}
	void Plus_DestroyPoint() {
		m_iDestroyPoint += 100;	}
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
	CCamera_Debug*		m_pCamera_Debug = nullptr;
	CCamera_Minigun*	m_pCamera_Minigun = nullptr;
private:
	vector<_vec3> 		m_vecPath;
	_uint				m_iCurrentIndex = 0;
	_double				m_dlCurrentOffset = 0;
	_bool				m_isFinish = false;
	_float				m_fSpeed = 0; // 속도
	_uint				m_iDestroyPoint = 0;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT SetUp_Camera();
	HRESULT Load_Path(_tchar* pFilePath); // 경로 로드하는 함수
	_vec3 Bezier4(_vec3 vPoint_1, _vec3 vPoint_2, _vec3 vPoint_3, _vec3 vPoint_4, double dlOffset);
	_int Update_Rotation(const _float& fTimeDelta);
public:
	static CAirplane* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END