#pragma once


#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CBuffer_RcTex;
class CTexture;
class CShader;
_END

class CParentEffect final : public CGameObject
{
public:
	explicit CParentEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParentEffect(const CParentEffect& rhs);
	virtual ~CParentEffect() = default;

public:
	const _matrix& Get_ParentMatridx() const {
		return m_matParentMatrix;
	}
public:
	virtual HRESULT Ready_GameObject_Prototype(const _tchar*  pProtoTag);
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

public:
	void Set_EffectInfo(_float fFrameSpeed, _float fFrameMax, _float fMoveSpeed, _float fSurviveTime, _float fDegreeRange, _vec3* vSetScale, _vec3* vSetPos, _vec3* vDir, _bool isSettingPos,_bool isRandomPos,_float fCreateTime);
private:
	CTransform*			m_pTransformCom = nullptr;

private:
	HRESULT Ready_Component();

private:
	_float				m_fTimeAcc = 0.f;
	_float				m_fFrameSpeed = 0.f; // 애니메이션 프레임 속도
	_float				m_fFrameMax = 0.f; // 애니메이션 최대 프레임
	_float				m_fMoveSpeed = 0.f;
	_float				m_fSurviveTime = 0.f;
	_float				m_fDegreeRange = 0.f; // 회전되어있는 각도 ( 범위 )
	_vec3				m_vSetScale; // 생성 크기
	_vec3				m_vSetPos; // 생성 위치
	_vec3				m_vDir; // 움직이는 방향 
	_matrix				m_matParentMatrix;

	_float				m_fCreateTime = 0.f;
	_bool				m_isSettingPos = false;
	_bool				m_isRandomPos = false;

	const _tchar*		m_pProtoTag = nullptr;

public:
	static CParentEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar*  pProtoTag);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

