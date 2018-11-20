#include "..\Headers\Graphic_Device.h"

_IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
{
}

HRESULT CGraphic_Device::Ready_Graphic_Device(HWND hWnd, WINMODE eMode, const _uint& iBackCX, const _uint& iBackCY, LPDIRECT3DDEVICE9* ppGraphic_Device)
{
	// 장치를 조사하기위한 객체인 IDirect3D9객체를 생성한다.
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == m_pSDK)
		return E_FAIL;

	// 장치의 동작방식을 결정하기위해서는 장치의 지원범위를 알아야된다.
	
	// 장치의 지원범위를 얻어온다.
	D3DCAPS9			Devcaps;
	ZeroMemory(&Devcaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Devcaps)))
		return E_FAIL;

	_ulong			dwBehaviorFlags = 0;

	// 버텍스 프로세싱의 기능을 하드웨어 단계에서 지원가능한지?!
	// 버텍스 프로세싱(정점의 변환 + 정점의 빛 연산)
	if (Devcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
	else
		dwBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	dwBehaviorFlags |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = iBackCX;
	d3dpp.BackBufferHeight = iBackCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode;	
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// 장치를 생성한다.
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, dwBehaviorFlags, &d3dpp, &m_pGraphic_Device)))
		return E_FAIL;

	if (nullptr != ppGraphic_Device)
	{
		*ppGraphic_Device = m_pGraphic_Device;
		m_pGraphic_Device->AddRef();
	}


	return NOERROR;
}

void CGraphic_Device::Free()
{
	_ulong		dwRefCnt = 0;

	if (dwRefCnt = Safe_Release(m_pGraphic_Device))
		_MSG_BOX("IDirect3DDevice9 Release Failed");

	if (dwRefCnt = Safe_Release(m_pSDK))
		_MSG_BOX("IDirect3D9 Release Failed");

}
