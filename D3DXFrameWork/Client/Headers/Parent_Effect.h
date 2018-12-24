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

class CParent_Effect final : public CGameObject
{
public:
	explicit CParent_Effect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParent_Effect(const CParent_Effect& rhs);
	virtual ~CParent_Effect() = default;

public:
	const _matrix& Get_ParentMatridx() const {
		return m_matParentMatrix;	}
	void DeleteChild();
public:
	virtual HRESULT Ready_GameObject_Prototype(const _tchar*  pProtoTag);
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

public:
	void Set_EffectInfo(_float fFrameSpeed, _float fFrameMax, _float fMoveSpeed, _float fSurviveTime, _float fDegreeRange, _vec3* vSetScale, _vec3* vSetPos, _vec3 * vSetPosRange, _vec3* vDir, _bool isSettingPos,_bool isRandomPos,_float fCreateTime, _int iCreateCnt, _float fLiveTime);
private:
	CTransform*			m_pTransformCom = nullptr;

private:
	HRESULT Ready_Component();
public:
	_ubyte				m_iType = 0;
	_float				m_fLiveAcc = 0.f;

private:
	_float				m_fTimeAcc = 0.f;
	_float				m_fFrameSpeed = 0.f; // �ִϸ��̼� ������ �ӵ�
	_float				m_fFrameMax = 0.f; // �ִϸ��̼� �ִ� ������
	_float				m_fMoveSpeed = 0.f;
	_float				m_fSurviveTime = 0.f;
	_float				m_fDegreeRange = 0.f; // ȸ���Ǿ��ִ� ���� ( ���� )
	_vec3				m_vSetScale; // ���� ũ��
	_vec3				m_vSetPos; // ���� ��ġ
	_vec3				m_vSetPosRange; // ���� ����
	_vec3				m_vDir; // �����̴� ���� 
	_matrix				m_matParentMatrix;
	_int				m_iCreateCnt = 0;
	_float				m_fCreateTime = 0.f;
	_bool				m_isSettingPos = false;
	_bool				m_isRandomPos = false;
	_float				m_fLiveTime = 0.f;
	list<CGameObject*>  m_EffectList;
	const _tchar*		m_pProtoTag = nullptr;

public:
	static CParent_Effect* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar*  pProtoTag);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

