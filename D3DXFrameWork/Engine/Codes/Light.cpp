#include "..\Headers\Light.h"

CLight::CLight(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

void CLight::Set_PointInfo(_uint eValue, const _vec3* vPosition)
{
	if (eValue == STATE_POSITION)
	{
		_vec3 vTmp = _vec3(vPosition->x, vPosition->y + 7, vPosition->z);
		m_LightInfo.Position = vTmp;
	}
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

	pVertices[0].vPosition = _vec4(0.0f - 0.5f, 0.0f - 0.5f, 0.f, 1.f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPosition = _vec4(ViewPort.Width - 0.5f, 0.f - 0.5f, 0.f, 1.f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPosition = _vec4(ViewPort.Width - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPosition = _vec4(0.0f - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
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
	if (false == m_isRender)
		return;

	if (true == m_isUpDown)
	{

	}


	_uint			iPassIdx = 0;

	if (D3DLIGHT_DIRECTIONAL == m_LightInfo.Type)
	{
		iPassIdx = 0;
		pEffect->SetVector("g_vLightDir", &_vec4(m_LightInfo.Direction, 0.f));
	}
	else if (D3DLIGHT_POINT == m_LightInfo.Type)
	{
		iPassIdx = 1;
		pEffect->SetVector("g_vLightPos", &_vec4(m_LightInfo.Position, 1.f));
		pEffect->SetFloat("g_fRange", m_LightInfo.Range);
	}
	// 빛 세팅
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&m_LightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&m_LightInfo.Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&m_LightInfo.Specular);
	// View Proj행렬 받아서 역행렬을 인자로 던져줌 ( z나누기 위해 )
	_matrix		matView, matProj;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);
	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	pEffect->CommitChanges(); // Begin 이후에 값이 변경 되었기 때문에 반드시 해줘야 한다.

	pEffect->BeginPass(iPassIdx);

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORT));
	m_pGraphic_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
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
