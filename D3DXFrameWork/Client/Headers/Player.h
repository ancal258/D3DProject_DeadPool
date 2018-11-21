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
class CInput_Device;
_END

_BEGIN(Client)
class CCamera_Cinematic;
class CCamera_Debug;
class CCamera_Target;
class CPlayer final : public CGameObject
{
private:
	enum STATE_CAMERA {CAMERA_DEBUG, CAMERA_TARGET, CAMERA_CINEMATIC};
private:
	enum ANIM_APT {
		SIT_IDLE_BREATH, SIT_IDLE_HANDMOVE, SIT_GETUP, SIT_SITDOWN,
		NOGUN_IDLE00, NOGUN_IDLE01, NOGUN_IDLE02, NOGUN_IDLE03, NOGUN_IDLE04, NOGUN_IDLE05, NOGUN_IDLE06,
		NOGUN_WALK_F, NOGUN_WALK_FL, NOGUN_WALK_FR, NOGUN_WALK_L, NOGUN_WALK_R, NOGUN_WALK_B
	};
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
public:
	const _matrix& Get_HandMatrix(_uint iSide) const {
		return m_CombinedHandMatrix[iSide];
	}
	const _matrix* Get_WorldMatrix() const;
	const _long* Get_MouseMove() const {
		return m_dwMouseMove;
	}
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	void Render_Axis();
	HRESULT SetUp_Camera();
	HRESULT SetUp_CameraMove();
private:
	CTransform*         m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Dynamic*      m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;
	CInput_Device*		m_pInput_Device = nullptr;
	//Mouse
private:
	_float            m_fMouseSence = 0;
	_long            m_dwMouseMove[2] = { 0 };

	//Camera
private:
	CCamera_Cinematic* m_pCamera_Cinematic = nullptr;
	CCamera_Debug* m_pCamera_Debug = nullptr;
	CCamera_Target* m_pCamera_Target = nullptr;
	STATE_CAMERA	m_Camera_State = CAMERA_DEBUG;
	//Matrix
private:
	_matrix            m_CombinedHandMatrix[2];
	const _matrix*      m_pHandMatrix[2];
	_matrix            m_CombinedRootMatrix;
	const _matrix*      m_pRootMatrix;
private:
	HRESULT Ready_Component();
	HRESULT   SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT Update_HandMatrix();
	HRESULT Load_CamData(const _tchar * pFileName);
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END