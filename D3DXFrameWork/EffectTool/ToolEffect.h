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

class CToolEffect final : public CGameObject
{
private:
	explicit CToolEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CToolEffect(const CToolEffect& rhs);
	virtual ~CToolEffect() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	void Set_EffectInfo(CGameObject* pParent, _float fFrameSpeed, _float fFrameMax,_float fMoveSpeed, _float fSurviveTime,_float fDegreeRange, _vec3* vSetScale, _vec3* vSetPos, _vec3* vDir);
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CBuffer_RcTex*		m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	_float				m_fFrame = 0.f; // �ִϸ��̼��� ���� ������
	_float				m_fFrameSpeed = 0.f; // �ִϸ��̼� ������ �ӵ�
	_float				m_fFrameMax = 0.f; // �ִϸ��̼� �ִ� ������
	_float				m_fMoveSpeed = 0.f;
	_float				m_fSurviveTime = 0.f;
	_float				m_fDegreeRange = 0.f; // ȸ���Ǿ��ִ� ���� ( ���� )
	_float				m_fAlpha = 1.f; // �ִϸ��̼��� Alpha�� ( 0 ~ 1 ) ( 0 ~ m_fFrame / m_fFrameMax ) 
	_float				m_fTimeAcc = 0.f;
	_vec3				m_vSetScale; // ���� ũ��
	_vec3				m_vSetPos; // ���� ��ġ
	_vec3				m_vDir; // �����̴� ���� 

	CGameObject*		m_pParent = nullptr;
	
public:
	static CToolEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};