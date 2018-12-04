#pragma once

#include "Base.h"
#include "Defines.h"

_BEGIN(Engine)
class CInput_Device;
class CMesh_Dynamic;
class CNavigation;
class CTransform;
_END


_BEGIN(Client)

class CAnimator final : public CBase
{
private:
	enum ANIM_APT {
		SIT_IDLE_BREATH, SIT_IDLE_HANDMOVE, SIT_GETUP, SIT_SITDOWN,
		NOGUN_IDLE00, NOGUN_IDLE01, NOGUN_IDLE02, NOGUN_IDLE03, NOGUN_IDLE04, NOGUN_IDLE05, NOGUN_IDLE06,
		NOGUN_WALK_F, NOGUN_WALK_FL, NOGUN_WALK_FR, NOGUN_WALK_L, NOGUN_WALK_R, NOGUN_WALK_B, INTERACT_DOG,
		ANIM_END
	};
	enum ANIM_FIELD {
		SWORD_IDLE, SWORLD_LIGHT_01, SWORLD_LIGHT_02, SWORLD_LIGHT_03, SWORLD_HEAVY_01, SWORLD_HEAVY_02, SWORLD_HEAVY_03,
		SWORD_AIR_01,SWORD_AIL_02,SWORD_AIR_03, AIM_F, AIM_FL, AIM_FR, AIM_L, AIM_R, AIM_B, AIM_BL, AIM_BR, AIM_IDLE,
		SWORD_RUN_FORWARD, SWORD_JUMP, SWORD_DOUBLEJUMP, SWORD_JUMPLAND, RUN_JUMP, RUN_DOUBLEJUMP, RUN_JUMPLAND,
		FIELD_END
	};

	enum STATE {
		STATE_SWORD, STATE_AIM, STATE_RUN, STATE_NORMAL, STATE_END
	};

	enum KEY_STATE {
		Q,W,E,R,A,S,D,LBUTTON,RBUTTON, KEY_END
	};
private:
	explicit CAnimator(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CAnimator() = default;
public:
	HRESULT Ready_Animator(CMesh_Dynamic* pMeshCom, CTransform* pTransformCom, CNavigation* pNavigationCom, _uint iSceneNum);
	void Ready_Pair();
public:
	void SetUp_RootMatrix(_matrix* pRootMatrix) {
		m_CombinedRootMatrix = pRootMatrix;
	}
	_bool Get_IsButtonDown(_uint iIndex) {
		return m_isKeyDown[iIndex];}
	_bool Get_IsReservation();
	void Set_MouseRotate(_float *fRotate) {
		m_fRotate = fRotate;
	}
	
public:
	void Update_Animation(const _float & fTimeDelta);
	void Update_Animation_FIELD(const _float & fTimeDelta);
	void Last_Update_Animation_FIELD(const _float & fTimeDelta);
	void Input_Push_Back(_uint iIndex);
	//HRESULT SetUp_OneButtonAnimation()
private: // CallBack
	void AnimFinish();
	_bool CheckPair(_uint iFirst, _uint iSecond);
	_bool CheckCombo(_uint iCurrentIdx);
	_uint CheckComboTime(_uint iCurrentIdx);
private:
	_uint CheckSwordLight();
	_uint CheckSwordHeavy();
private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;
	CInput_Device* m_pInput_Device = nullptr;
private:
	CMesh_Dynamic*      m_pMeshCom = nullptr;
	CTransform*         m_pTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;
	_bool				m_ArrayAnimState[ANIM_END] = { 0 };
	_matrix*            m_CombinedRootMatrix = nullptr;
	_uint				m_iSceneNum = false;
	_uint				m_iState = STATE_SWORD;
	_uint				m_iLastState = STATE_SWORD;
	vector<pair<_uint, _uint>>		m_vecBlendPair;
	vector<pair<_uint, _uint>>		m_vecComboPair;
	vector<pair<_uint, _uint>>		m_vecComboTime;
	

	_bool				m_isKeyDown[KEY_END] = { false };
private:
	list<_uint>			m_ReservationList;

	//사용 변수
private:
	_uint				m_iSit = SIT_IDLE_BREATH;
	_float				*m_fRotate = nullptr;
public:
	static CAnimator* Create(LPDIRECT3DDEVICE9 pGraphic_Device,CMesh_Dynamic* pMeshCom, CTransform* pTransformCom, CNavigation* pNavigationCom,_uint iSceneNum);
protected:
	virtual void Free();
};

_END