#pragma once

#include "Base.h"

// 그래픽카드 장치를 조사한다.
// 그래픽카드 장치를 대표하는 객체를 생성하낟.

_BEGIN(Engine)

class _ENGINE_DLL CGraphic_Device final : public CBase
{
	_DECLARE_SINGLETON(CGraphic_Device)
public:
	enum WINMODE { MODE_FULL, MODE_WIN };
public:
	explicit CGraphic_Device();
	virtual ~CGraphic_Device() = default;
public:
	LPDIRECT3DDEVICE9 Get_GraphicDev() const {
		return m_pGraphic_Device; }
public:
	// 그래픽카드 장치를 조사한다.
	// 그래픽카드 장치를 대표하는 객체를 생성하낟.
	HRESULT Ready_Graphic_Device(HWND hWnd, WINMODE eMode, const _uint& iBackCX, const _uint& iBackCY, LPDIRECT3DDEVICE9* ppGraphic_Device);
private:
	LPDIRECT3D9				m_pSDK = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;		
protected:
	virtual void Free();
};

_END