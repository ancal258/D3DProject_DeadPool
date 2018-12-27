#include "..\Headers\Mesh_Static.h"
#include "Picking.h"
#include "Transform.h"

CMesh_Static::CMesh_Static(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CMesh_Static::CMesh_Static(const CMesh_Static & rhs)
	: CComponent(rhs)
	//, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pMaterials(rhs.m_pMaterials)
	, m_dwNumSubset(rhs.m_dwNumSubset)
	, m_pSubsets(rhs.m_pSubsets)
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)
{
	m_pAdjacency->AddRef();
	m_pMaterials->AddRef();
	//m_pMesh->AddRef();

	// D3DVERTEXELEMENT9 : 정점의 FVF정보 하나를 담기위한 구조체.
	//D3DVERTEXELEMENT9			Element[MAX_FVF_DECL_SIZE];
	//ZeroMemory(&Element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);
	D3DVERTEXELEMENT9 Element[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		D3DDECL_END() // this macro is needed as the last item!
	};
	
	rhs.m_pMesh->CloneMesh(rhs.m_pMesh->GetOptions(), Element, Get_Graphic_Device(), &m_pMesh);

	LPDWORD pAdjacency = new DWORD[m_pMesh->GetNumFaces() * 3];

	m_pMesh->GenerateAdjacency(0.0001f, pAdjacency);
	D3DXComputeNormals(m_pMesh, pAdjacency);
	D3DXComputeTangent(m_pMesh, 0, 0, 0, 0, pAdjacency);

	//D3DXComputeTangentFrameEx(m_pMesh,
	//	D3DDECLUSAGE_POSITION, 0,
	//	D3DDECLUSAGE_NORMAL, 0,
	//	D3DDECLUSAGE_TEXCOORD, 0,
	//	D3DDECLUSAGE_TANGENT, 0,
	//	D3DXTANGENT_GENERATE_IN_PLACE,
	//	pAdjacency, 0.01f, 0.25f, 0.01f,
	//	NULL, NULL);


	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwNumSubset];
	memcpy(m_ppTextures, rhs.m_ppTextures, sizeof(LPDIRECT3DTEXTURE9) * m_dwNumSubset);
	for (size_t i = 0; i < m_dwNumSubset; ++i)
		m_ppTextures[i]->AddRef();

	if (nullptr != rhs.m_ppTexturesNorm)
	{
		m_ppTexturesNorm = new LPDIRECT3DTEXTURE9[m_dwNumSubset];
		memcpy(m_ppTexturesNorm, rhs.m_ppTexturesNorm, sizeof(LPDIRECT3DTEXTURE9) * m_dwNumSubset);

		for (size_t i = 0; i < m_dwNumSubset; ++i)
		{
			if(nullptr != m_ppTexturesNorm[i])
				m_ppTexturesNorm[i]->AddRef();
		}
	}
}


HRESULT CMesh_Static::Ready_Mesh(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar			szFullPath[MAX_PATH] = L"";
	m_pFilePath = pFilePath;
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
	
	m_ppTexturesNorm = new LPDIRECT3DTEXTURE9[m_dwNumSubset];
	ZeroMemory(m_ppTexturesNorm, sizeof(LPDIRECT3DTEXTURE9) * m_dwNumSubset);
	// 받아온 텍스쳐 이름으로 실제 사용할 수 있는 텍스쳐정보를 생성한다.
	for (size_t i = 0; i < m_dwNumSubset; ++i)
	{
		ZeroMemory(szFullPath, sizeof(_tchar) * MAX_PATH);

		_tchar	szFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_ACP, 0, m_pSubsets[i].pTextureFilename, strlen(m_pSubsets[i].pTextureFilename),
			szFileName, MAX_PATH);

		lstrcpy(szFullPath, pFilePath);
		lstrcat(szFullPath, szFileName);

		Load_Texture(szFileName, L"X", &m_ppTextures[i]);
		Load_Texture(szFileName, L"Y", &m_ppTexturesNorm[i]);

		//if (FAILED(D3DXCreateTextureFromFile(Get_Graphic_Device(), szFullPath, &m_ppTextures[i])))
		//	return E_FAIL;

		//if (FAILED(D3DXCreateTextureFromFile(Get_Graphic_Device(), szFullPath, &m_ppTexturesNorm[i])))
		//	return E_FAIL;
	}

	void*		pVertices = nullptr;

	m_pMesh->LockVertexBuffer(0, &pVertices);

	HRESULT hr = D3DXComputeBoundingBox((_vec3*)pVertices, m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_pMesh->GetFVF()), &m_vMin, &m_vMax);

	m_pMesh->UnlockVertexBuffer();

	if (FAILED(hr))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMesh_Static::Clone_Mesh()
{
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

HRESULT CMesh_Static::Set_TextureNorm_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, const _ulong & iSubset)
{
	if (nullptr == pEffect ||
		nullptr == m_ppTextures)
		return E_FAIL;

	if (m_dwNumSubset <= iSubset)
		return E_FAIL;

	pEffect->SetTexture(pConstantName, m_ppTexturesNorm[iSubset]);

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

		if(nullptr != m_ppTexturesNorm[i])
			pEffect->SetTexture("g_NormalTexture", m_ppTexturesNorm[i]);

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

HRESULT CMesh_Static::Load_Texture(const _tchar * pFileName, const _tchar * pTextureKey, LPDIRECT3DTEXTURE9 * ppTexture)
{
	_tchar			szFullPath[MAX_PATH] = L"";
	ZeroMemory(szFullPath, sizeof(_tchar) * MAX_PATH);

	_tchar			szFileName[MAX_PATH] = L"";
	lstrcpy(szFileName, pFileName);

	_int iLength = lstrlen(szFileName);

	for (_int i = 0; i < iLength + 1; i++)
	{
		if (szFileName[i] == L'.')
		{
			for (_int j = i; j >= 0; --j)
			{
				if (szFileName[j] == 'X')
				{
					szFileName[j] = *pTextureKey;

					break;
				}
			}
			break;
		}
	}

	lstrcpy(szFullPath, m_pFilePath);
	lstrcat(szFullPath, szFileName);

	D3DXCreateTextureFromFile(Get_Graphic_Device(), szFullPath, ppTexture);

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
	CMesh_Static*		pInstance = new CMesh_Static(*this);

	if (FAILED(pInstance->Clone_Mesh()))
	{
		_MSG_BOX("CMesh_Static Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh_Static::Free()
{
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pMaterials);
	Safe_Release(m_pMesh);

	for (size_t i = 0; i < m_dwNumSubset; ++i)
		Safe_Release(m_ppTextures[i]);

	Safe_Delete_Array(m_ppTextures);


	for (size_t i = 0; i < m_dwNumSubset; ++i)
		Safe_Release(m_ppTexturesNorm[i]);

	Safe_Delete_Array(m_ppTexturesNorm);

	if (false == is_Clone())
	{
		Safe_Delete_Array(m_pSubsets);

	}

	CComponent::Free();
}
