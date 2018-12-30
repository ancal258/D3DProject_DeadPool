#include "..\Headers\Target.h"



CTarget::CTarget(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

HRESULT CTarget::Ready_Target(const _uint & iWidth, const _uint & iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor)
{
	// 텍스쳐를 생성.
	if (FAILED(D3DXCreateTexture(m_pGraphic_Device, iWidth, iHeight, 1, D3DUSAGE_RENDERTARGET, eFormat, D3DPOOL_DEFAULT, &m_pTarget_Texture)))
		return E_FAIL;

	// 생성한 텍스쳐로부터 렌더타겟의 정보를 얻어온다.
	if (FAILED(m_pTarget_Texture->GetSurfaceLevel(0, &m_pTarget_Surface)))
		return E_FAIL;
	
	m_ClearColor = ClearColor;

	return NOERROR;
}

HRESULT CTarget::SetUp_OnGraphicDev(const _uint & iIndex)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pTarget_Surface)
		return E_FAIL;

	//현재 렌더타겟을 Old에 저장한다.
	m_pGraphic_Device->GetRenderTarget(iIndex, &m_pOld_Surface);

	//내가 지정한 인덱스에 지정한 렌더타겟을 넣는다.
	m_pGraphic_Device->SetRenderTarget(iIndex, m_pTarget_Surface);

	return NOERROR;
}

HRESULT CTarget::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName)
{
	if (nullptr == pEffect ||
		nullptr == m_pTarget_Texture)
		return E_FAIL;

	pEffect->SetTexture(pConstantName, m_pTarget_Texture);

	return NOERROR;
}

HRESULT CTarget::Release_OnGraphicDev(const _uint & iIndex)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pTarget_Surface)
		return E_FAIL;

	m_pGraphic_Device->SetRenderTarget(iIndex, m_pOld_Surface);

	Safe_Release(m_pOld_Surface);

	return NOERROR;
}

HRESULT CTarget::Clear_Target()
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pTarget_Surface)
		return E_FAIL;

	// 특정 타겟을 특정 색상으로 지워주기 위해 하는 작업
	// Clear를 하면 장치에 Set되어있는 모든 타겟을 지우기 때문에.

	m_pGraphic_Device->GetRenderTarget(0, &m_pOld_Surface);

	// 내가 지정한 인덱스에 지정한 렌더타겟을 셋해놓는다.
	m_pGraphic_Device->SetRenderTarget(0, m_pTarget_Surface);

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0);

	m_pGraphic_Device->SetRenderTarget(0, m_pOld_Surface);

	Safe_Release(m_pOld_Surface);

	return NOERROR;
}


HRESULT CTarget::Ready_DebugBuffer(const _float & fStartX, const _float & fStartY, const _float & fSizeX, const _float & fSizeY)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(sizeof(VTXVIEWPORT) * 4, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXVIEWPORT*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec4(fStartX, fStartY, 0.f, 1.f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPosition = _vec4(fStartX + fSizeX, fStartY, 0.f, 1.f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPosition = _vec4(fStartX + fSizeX, fStartY + fSizeY, 0.f, 1.f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPosition = _vec4(fStartX, fStartY + fSizeY, 0.f, 1.f);
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

void CTarget::Render_DebugBuffer()
{
	m_pGraphic_Device->SetTexture(0, m_pTarget_Texture);

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORT));
	m_pGraphic_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

CTarget * CTarget::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint & iWidth, const _uint & iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor)
{
	CTarget*		pInstance = new CTarget(pGraphic_Device);

	if (FAILED(pInstance->Ready_Target(iWidth, iHeight, eFormat, ClearColor)))
	{
		_MSG_BOX("CTarget Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CTarget::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	Safe_Release(m_pTarget_Surface);
	Safe_Release(m_pTarget_Texture);

	Safe_Release(m_pGraphic_Device);
}
