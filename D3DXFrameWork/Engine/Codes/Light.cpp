#include "..\Headers\Light.h"

CLight::CLight(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

HRESULT CLight::Ready_Light(const D3DLIGHT9 * pLightInfo)
{
	m_LightInfo = *pLightInfo;

	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(sizeof(VTXVIEWPORT) * 4, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	D3DVIEWPORT9			ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	VTXVIEWPORT*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec4(0.0f, 0.0f, 0.f, 1.f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPosition = _vec4(ViewPort.Width, 0.f, 0.f, 1.f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPosition = _vec4(ViewPort.Width, ViewPort.Height, 0.f, 1.f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPosition = _vec4(0.0f, ViewPort.Height, 0.f, 1.f);
	pVertices[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(sizeof(POLYGON16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	POLYGON16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

void CLight::Render_Light(LPD3DXEFFECT pEffect)
{
	pEffect->SetVector("g_vLightDir", &_vec4(m_LightInfo.Direction, 0.f));

	pEffect->CommitChanges(); // Begin 이후에 값이 변경 되었기 때문에 반드시 해줘야 한다.

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORT));
	m_pGraphic_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

CLight * CLight::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 * pLightInfo)
{
	CLight*		pInstance = new CLight(pGraphic_Device);

	if (FAILED(pInstance->Ready_Light(pLightInfo)))
	{
		_MSG_BOX("CLight Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	Safe_Release(m_pGraphic_Device);
}
