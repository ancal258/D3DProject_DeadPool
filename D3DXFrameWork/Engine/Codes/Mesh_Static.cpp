#include "..\Headers\Mesh_Static.h"
#include "Picking.h"
#include "Transform.h"

CMesh_Static::CMesh_Static(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CMesh_Static::CMesh_Static(const CMesh_Static & rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pMaterials(rhs.m_pMaterials)
	, m_dwNumSubset(rhs.m_dwNumSubset)
	, m_pSubsets(rhs.m_pSubsets)
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)
{
	m_pAdjacency->AddRef();
	m_pMaterials->AddRef();
	m_pMesh->AddRef();

	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwNumSubset];
	memcpy(m_ppTextures, rhs.m_ppTextures, sizeof(LPDIRECT3DTEXTURE9) * m_dwNumSubset);

	for (size_t i = 0; i < m_dwNumSubset; ++i)
		m_ppTextures[i]->AddRef();

}


HRESULT CMesh_Static::Ready_Mesh(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar			szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	// m_pSubset : D3DXMATERIAL 배열
	// 정점버퍼생성 정점값 대입. 
	// 인덱스버퍼생성 인덱스값 대입. 
	if (FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, Get_Graphic_Device(), &m_pAdjacency, &m_pMaterials, nullptr, &m_dwNumSubset, &m_pMesh)))
		return E_FAIL;



	// X파일 내에 서브셋마다  저장되어잉ㅆ었떤 재질정보와 텍스쳐 이름을 받아온다. 저장한다.
	m_pSubsets = new D3DXMATERIAL[m_dwNumSubset];
	ZeroMemory(m_pSubsets, sizeof(D3DXMATERIAL) * m_dwNumSubset);
	memcpy(m_pSubsets, m_pMaterials->GetBufferPointer(), sizeof(D3DXMATERIAL) * m_dwNumSubset);

	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwNumSubset];
	ZeroMemory(m_ppTextures, sizeof(LPDIRECT3DTEXTURE9) * m_dwNumSubset);

	// 받아온 텍스쳐 이름으로 실제 사용할 수 있는 텍스쳐정보를 생성한다.
	for (size_t i = 0; i < m_dwNumSubset; ++i)
	{
		ZeroMemory(szFullPath, sizeof(_tchar) * MAX_PATH);

		_tchar	szFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_ACP, 0, m_pSubsets[i].pTextureFilename, strlen(m_pSubsets[i].pTextureFilename),
			szFileName, MAX_PATH);

		lstrcpy(szFullPath, pFilePath);
		lstrcat(szFullPath, szFileName);

		if (FAILED(D3DXCreateTextureFromFile(Get_Graphic_Device(), szFullPath, &m_ppTextures[i])))
			return E_FAIL;
	}

	void*		pVertices = nullptr;

	m_pMesh->LockVertexBuffer(0, &pVertices);

	HRESULT hr = D3DXComputeBoundingBox((_vec3*)pVertices, m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_pMesh->GetFVF()), &m_vMin, &m_vMax);

	m_pMesh->UnlockVertexBuffer();

	if (FAILED(hr))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMesh_Static::Set_Texture_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, const _ulong & iSubset)
{
	if (nullptr == pEffect ||
		nullptr == m_ppTextures)
		return E_FAIL;

	if (m_dwNumSubset <= iSubset)
		return E_FAIL;

	pEffect->SetTexture(pConstantName, m_ppTextures[iSubset]);

	return NOERROR;
}

HRESULT CMesh_Static::Set_Diffuse_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, const _ulong & iSubset)
{
	if (nullptr == pEffect ||
		nullptr == m_pSubsets)
		return E_FAIL;

	if (m_dwNumSubset <= iSubset)
		return E_FAIL;

	pEffect->SetVector(pConstantName, (_vec4*)&m_pSubsets[iSubset].MatD3D.Diffuse);

	return NOERROR;
}

HRESULT CMesh_Static::Set_Ambient_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, const _ulong & iSubset)
{
	if (nullptr == pEffect ||
		nullptr == m_pSubsets)
		return E_FAIL;

	if (m_dwNumSubset <= iSubset)
		return E_FAIL;

	pEffect->SetVector(pConstantName, (_vec4*)&m_pSubsets[iSubset].MatD3D.Ambient);

	return NOERROR;
}

HRESULT CMesh_Static::Set_Specular_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, const _ulong & iSubset)
{
	if (nullptr == pEffect ||
		nullptr == m_pSubsets)
		return E_FAIL;

	if (m_dwNumSubset <= iSubset)
		return E_FAIL;

	pEffect->SetVector(pConstantName, (_vec4*)&m_pSubsets[iSubset].MatD3D.Specular);

	return NOERROR;
}

HRESULT CMesh_Static::Set_Power_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, const _ulong & iSubset)
{
	if (nullptr == pEffect ||
		nullptr == m_pSubsets)
		return E_FAIL;

	if (m_dwNumSubset <= iSubset)
		return E_FAIL;

	pEffect->SetFloat(pConstantName, m_pSubsets[iSubset].MatD3D.Power);

	return NOERROR;
}

void CMesh_Static::Render_Mesh(LPD3DXEFFECT pEffect)
{
	if (nullptr == m_pMesh)
		return;

	for (size_t i = 0; i < m_dwNumSubset; i++)
	{
		pEffect->SetTexture("g_DiffuseTexture", m_ppTextures[i]);

		pEffect->CommitChanges();

		m_pMesh->DrawSubset(i);
	}


}

HRESULT CMesh_Static::Picking_ToMesh(const _vec3 * pRayPos, const _vec3 * pRayDir, const CTransform * pTransCom, BOOL* pHit)
{
	_float		fU, fV, fDist;
	_vec3		vRayPos, vRayDir;
	_matrix     matWorld = *pTransCom->Get_WorldMatrix();
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, pRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, pRayDir, &matWorld);

	if (FAILED(D3DXIntersect(m_pMesh, &vRayPos, &vRayDir, (BOOL*)pHit, nullptr, &fU, &fV, &fDist, nullptr, nullptr)))
		return E_FAIL;
	return NOERROR;
}

CMesh_Static * CMesh_Static::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName)
{
	CMesh_Static*		pInstance = new CMesh_Static(pGraphic_Device);

	if (FAILED(pInstance->Ready_Mesh(pFilePath, pFileName)))
	{
		_MSG_BOX("CMesh_Static Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMesh_Static::Clone_Component()
{
	return new CMesh_Static(*this);
}

void CMesh_Static::Free()
{
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pMaterials);
	Safe_Release(m_pMesh);

	for (size_t i = 0; i < m_dwNumSubset; ++i)
		Safe_Release(m_ppTextures[i]);

	Safe_Delete_Array(m_ppTextures);

	if (false == is_Clone())
	{
		Safe_Delete_Array(m_pSubsets);

	}

	CComponent::Free();
}
