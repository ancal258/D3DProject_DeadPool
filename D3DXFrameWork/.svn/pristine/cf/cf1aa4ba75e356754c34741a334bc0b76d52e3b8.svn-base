#include "..\Headers\Picking.h"

CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

HRESULT CPicking::Ready_Picking()
{
	return NOERROR;
}

_bool CPicking::Picking_ToPolygon(HWND hWnd, const _vec3 * pPointA, const _vec3 * pPointB, const _vec3 * pPointC, const _matrix* pWorldMatrix, _vec3* pOut) const
{
	LPDIRECT3DDEVICE9 pGraphic_Device = Get_Graphic_Device();
	if (nullptr == pGraphic_Device)
		return false;

	pGraphic_Device->AddRef();

	// 마우스 위치 In.ViewPort Space
	POINT			ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);
	
	D3DVIEWPORT9	ViewPort;
	pGraphic_Device->GetViewport(&ViewPort);

	// In.Projection Space
	_vec3		vMousePos;

	// x : 0 -> -1, 800 -> 1, 400 -> 0
	// y : 0 -> 1, 600 -> -1, 300 -> 0
	vMousePos.x = (ptMouse.x / (ViewPort.Width * 0.5f)) - 1.f;
	vMousePos.y = (ptMouse.y / (ViewPort.Height * -0.5f)) + 1.f;
	vMousePos.z = 0.f;

	// In.View Space
	_matrix			matProj;
	pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayDir, vRayPos(0.0f, 0.0f, 0.0f);

	vRayDir = vMousePos - vRayPos;

	

	return _bool(false);
}

CPicking * CPicking::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPicking*		pInstance = new CPicking(pGraphic_Device);

	if (FAILED(pInstance->Ready_Picking()))
	{
		_MSG_BOX("CPicking Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CPicking::Clone_Component()
{
	AddRef();

	return this;
}

void CPicking::Free()
{
	CComponent::Free();
}
