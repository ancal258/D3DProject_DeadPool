#include "stdafx.h"
#include "ToolTerrain.h"

#include "Component_Manager.h"
#include "Input_Device.h"
#include "Light_Manager.h"
#include "NavPoint.h"

_USING(Tool)


CToolTerrain::CToolTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CToolTerrain::CToolTerrain(const CToolTerrain & rhs)
	: CGameObject(rhs)
{
}

void CToolTerrain::Set_BrushRadius(_float fRadius)
{
	m_pBufferCom->Set_BrushRadius(fRadius);
	m_fRadius = fRadius;
}

void CToolTerrain::Set_BrushPower(_float fPower)
{
	m_pBufferCom->Set_BrushPower(fPower);
}

void CToolTerrain::Set_MouseState(MOUSE_STATE eState)
{
	m_eMouseState = eState;
	m_pBufferCom->Set_MouseState(m_eMouseState);

}

_vec3 CToolTerrain::Get_MousePoint()
{
	_vec3 vOut;
	ZeroMemory(vOut, sizeof(_vec3));
	if (true == CInput_Device::GetInstance()->Picking_ToBuffer(m_pBufferCom, m_pTransformCom, &vOut))
		return vOut;

	return _vec3(-1,-1,-1);
}

vector<_vec3>* CToolTerrain::Get_vecPoint()
{
	return m_pNavigationCom->Get_vecPoint();
}


HRESULT CToolTerrain::Save_HeightMap()
{
	if (FAILED(D3DXCreateTexture(Get_Graphic_Device(), m_iWidth, m_iHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pFilterTexture)))
		return E_FAIL;

	D3DLOCKED_RECT		LockRect;
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	m_pFilterTexture->LockRect(0, &LockRect, nullptr, 0);
	for (size_t i = 0; i < m_iHeight; i++)
	{
		for (size_t j = 0; j < m_iWidth; j++)
		{
			size_t	iIndex = i * m_iWidth + j;
			_float fHeight = m_pBufferCom->Get_VerticesPos()[iIndex].y;
			fHeight = (fHeight / 255) * 5;
			((_ulong*)LockRect.pBits)[iIndex] = D3DXCOLOR(fHeight, fHeight, fHeight, fHeight);
		}
	}

	m_pFilterTexture->UnlockRect(0);

	D3DXSaveTextureToFile(L"../Client/Bin/Resources/Textures/Terrain/Height2.bmp", D3DXIFF_BMP, m_pFilterTexture, nullptr);
	return NOERROR;
}

HRESULT CToolTerrain::Load_SplattingMap()
{
	D3DXCreateTextureFromFileEx(Get_Graphic_Device(), L"../Client/Bin/Splatting.bmp", m_iWidth, m_iHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &m_pSplattingTexture);
	return NOERROR;
}

HRESULT CToolTerrain::Add_Cell(_vec3* vPosArray)
{
	if (FAILED(m_pNavigationCom->Add_Cell(vPosArray)))
		return E_FAIL;



	return NOERROR;
}

HRESULT CToolTerrain::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CToolTerrain::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	Load_SplattingMap();
	m_MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	m_MtrlInfo.Emissive = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_MtrlInfo.Power = 40.0f;

	if (FAILED(D3DXCreateLine(Get_Graphic_Device(), &m_pLine)))
		return E_FAIL;
	return NOERROR;
}

_int CToolTerrain::Update_GameObject(const _float & fTimeDelta)
{
	if (CInput_Device::GetInstance()->Get_DIMouseState(CInput_Device::DIM_LBUTTON) & 0x80)
	{
		//if (STATE_ADD_NAV != m_eMouseState)
		//{
			if (true == CInput_Device::GetInstance()->Picking_ToBuffer(m_pBufferCom, m_pTransformCom, &m_vOut))
				m_vPickingPoint = m_vOut;
			if (STATE_TILE1 <= m_eMouseState && m_eMouseState <= STATE_TILE4)
				Load_SplattingMap();
		//}
		//else
		//{
		//	if (true == CInput_Device::GetInstance()->Picking_ToBuffer(m_pBufferCom, m_pTransformCom, m_pNavigationCom, &m_vOut))
		//		m_vPickingPoint = m_vOut;
		//}
	}
	else
	{
		m_vOut = _vec3(0, -100, 0);
	}

	return _int();
}

_int CToolTerrain::LastUpdate_GameObject(const _float & fTimeDelta)
{
	m_pTransformCom->Update_Matrix();

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this);

	return _int();
}

void CToolTerrain::Render_GameObject()
{
	if (nullptr == m_pBufferCom ||
		nullptr == m_pShaderCom)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Render_NavPoint();
	//m_pNavigationCom->Render_Navigation();
	//Safe_Release(pEffect);
}

HRESULT CToolTerrain::Ready_Component()
{
	CComponent_Manager*			pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Com_Transform
	m_pTransformCom = (CTransform*)pComponent_Manager->Clone_Component(0, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	m_pRendererCom = (CRenderer*)pComponent_Manager->Clone_Component(0, L"Component_Renderer");
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	// For.Com_Buffer
	m_pBufferCom = (CBuffer_Terrain*)pComponent_Manager->Clone_Component(0, L"Component_Buffer_Terrain");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(0, L"Component_Shader_Terrain");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(0, L"Component_Texture_Terrain");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	m_pNavigationCom = (CNavigation*)pComponent_Manager->Clone_Component(0, L"Component_Navigation");
	if (FAILED(Add_Component(L"Com_Navigation", m_pNavigationCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CToolTerrain::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");
	_matrix			matView, matProj;
	Get_Transform(D3DTS_VIEW, &matView);
	Get_Transform(D3DTS_PROJECTION, &matProj);



	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetVector("g_vMouse", &_vec4(m_vOut, 1));
	pEffect->SetVector("g_vPickingPoint", &_vec4(m_vPickingPoint, 1));
	pEffect->SetFloat("g_fRadius", m_fRadius);
	//m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", 0);

	// For.LightInfo
	const D3DLIGHT9* pLightInfo = CLight_Manager::GetInstance()->Get_LightInfo(0);
	if (nullptr == pLightInfo)
		return E_FAIL;

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&m_MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&m_MtrlInfo.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&m_MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", m_MtrlInfo.Power);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	//pEffect->SetVector("g_vBrushPosition", &_vec4(20.f, 0.f, 10.f, 1.f));
	//pEffect->SetFloat("g_fRadius", 5.0f);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_SourTexture", 0);
	m_pTextureCom->SetUp_OnShader(pEffect, "g_DestTexture1", 1);
	m_pTextureCom->SetUp_OnShader(pEffect, "g_DestTexture2", 2);
	m_pTextureCom->SetUp_OnShader(pEffect, "g_DestTexture3", 3);
	//m_pFilterCom->SetUp_OnShader(pEffect, "g_FilterTexture");
	// m_pFilterCom->SetUp_OnShader(pEffect, "g_FilterTexture");
	pEffect->SetTexture("g_FilterTexture", m_pSplattingTexture);

	//m_pBrushCom->SetUp_OnShader(pEffect, "g_BrushTexture");


	Safe_Release(pEffect);


	return NOERROR;
}

void CToolTerrain::Render_NavPoint()
{
	if (m_vecNavPoint.size() < 2)
		return;

	_matrix		matView, matProj;

	Get_Graphic_Device()->GetTransform(D3DTS_VIEW, &matView);
	Get_Graphic_Device()->GetTransform(D3DTS_PROJECTION, &matProj);

	_matrix		matTransform;
	D3DXMatrixIdentity(&matTransform);

	// 3으로 나누어 떨어질 때 마다 포인트를 + 1 해야된다.
	// 즉 몫 만큼 Size +가 되어야 함.

	//if (m_vecNavPoint.size() % 3 == 0)
	//{
	_uint		iSizeUp = m_vecNavPoint.size() / 3;

	_vec3*		pPointArray = new _vec3[m_vecNavPoint.size() + iSizeUp];

	_uint		iIndex = 0;
	for (size_t i = 0; i < m_vecNavPoint.size() + iSizeUp; ++i)
	{
		if ((i + 1) % 4 == 0 && i != 0)
		{
			pPointArray[i] = pPointArray[i - 3];
			continue;
		}
		pPointArray[i] = m_vecNavPoint[iIndex++]->m_vWorldPosition;
	}

	for (int j = 0; j < iSizeUp; ++j)
	{
		for (size_t i = j * 4; i < j * 4 + 4; ++i)
		{
			D3DXVec3TransformCoord(&pPointArray[i], &pPointArray[i], &matView);
			if (pPointArray[i].z < 0.0f)
				pPointArray[i].z = 0.f;

			D3DXVec3TransformCoord(&pPointArray[i], &pPointArray[i], &matProj);
		}
		m_pLine->SetWidth(2.0f);

		m_pLine->Begin();

		m_pLine->DrawTransform(&pPointArray[j*4], 4, &matTransform, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

		m_pLine->End();
	}

}

CToolTerrain * CToolTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CToolTerrain*		pInstance = new CToolTerrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CTerrain Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CToolTerrain::Clone_GameObject()
{
	CToolTerrain*		pInstance = new CToolTerrain(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CTerrain Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CToolTerrain::Free()
{
	CGameObject::Free();
}
