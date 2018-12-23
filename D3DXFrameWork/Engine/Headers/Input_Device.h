#pragma once

#include "Base.h"

_BEGIN(Engine)

class CVIBuffer;
class CMesh_Static;
class CCollider;
class CTransform;
class CNavigation;
class _ENGINE_DLL CInput_Device final : public CBase
{
	_DECLARE_SINGLETON(CInput_Device)
public:
	enum MOUSEBUTTON { DIM_LBUTTON, DIM_RBUTTON, DIM_HBUTTON, DIM_XBUTTON, DIM_END };
	enum MOUSEMOVE { DIMM_X, DIMM_Y, DIMM_H, DIMM_END };
private:
	explicit CInput_Device();
	virtual ~CInput_Device() = default;
public:
	_byte Get_DIKeyState(_ubyte byKeyID) const {
		return m_byKeyState[byKeyID];
	}
	_byte Get_DIMouseState(MOUSEBUTTON eMouseBtID) const {
		return m_MouseState.rgbButtons[eMouseBtID];
	}
	_long Get_DIMouseMove(MOUSEMOVE eMouseMoveID) const {
		return *((_long*)&m_MouseState + eMouseMoveID);
	}

	_bool Get_SlowMotion() const {
		return m_isSlowMotion;	}
	void Set_SlowMotion(_bool isSlowMotion) {
		m_isSlowMotion = isSlowMotion;	}

public:
	HRESULT Ready_Input_Device(HINSTANCE hInst, HWND hWnd);
	HRESULT Inquire_Input_State();
	HRESULT Update_MinDist();
	_bool Picking_ToBuffer(CVIBuffer* pBufferCom, CTransform* pTransformCom, _vec3* pCollPos);
	_bool Picking_ToBuffer(CVIBuffer* pBufferCom, CTransform* pTransformCom, CNavigation* pNavigation, _vec3* pCollPos);
	HRESULT Picking_ToMesh(CMesh_Static * pStaticMeshCom, CTransform * pTransformCom, BOOL* pHit);
	HRESULT Picking_ToCollider(LPD3DXMESH pMesh, CTransform * pTransformCom, BOOL* pHit);
	HRESULT Picking_ToCollider(LPD3DXMESH pMesh, CTransform * pTransformCom, BOOL* pHit, _float* fDist);
	_bool Is_MinDist(_float fDist);
	HRESULT SetUp_Picking();
private:
	HWND							m_hWnd = 0;
	_vec3							m_vRayDir;
	_vec3							m_vRayPos;
	list<_float>					DistList;
	_float							m_fMinDist = 0;
private:
	LPDIRECTINPUT8					m_pSDK = nullptr;
	LPDIRECTINPUTDEVICE8			m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8			m_pMouse = nullptr;
private:
	_byte							m_byKeyState[256] = { 0 };
	DIMOUSESTATE					m_MouseState;
private:
	_bool							m_isSlowMotion = false;
private:
	HRESULT SetUp_KeyBoard(HWND hWnd);
	HRESULT SetUp_Mouse(HWND hWnd);
protected:
	virtual void Free();

};


_END