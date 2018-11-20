#include "..\Headers\HierarchyLoader.h"


CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

HRESULT CHierarchyLoader::Ready_HierarchyLoader(const _tchar * pFilePath)
{
	m_pFilePath = pFilePath;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED*		pNewFrame_Derived = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame_Derived, sizeof(D3DXFRAME_DERIVED));

	if (nullptr != Name)
	{
		if (FAILED(Allocate_Name(&pNewFrame_Derived->Name, Name)))
			return E_FAIL;
	}
	pNewFrame_Derived->CombinedTransformationMatrix = *D3DXMatrixIdentity(&pNewFrame_Derived->TransformationMatrix);
	*ppNewFrame = pNewFrame_Derived;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{ // -> �� �Լ��� ����Ǹ� ���ڷ� ���� �ֵ��� ��� ������ ���ش�.

	//D3DXMESHDATA
	//D3DXMATERIAL : �����
	//D3DMATERIAL9 ��¼�� : ����
	//pEffectInstances : �ʿ� ���� �Ⱦ���.
	//NumMaterials : �ﰢ�� ����
	//pAdjacency : ���� �ﰢ�� �迭
	//pSkinInfo : �޽� ���(�Ǻ�)�� �� �� �ִ� �ϵ��� �Լ���? --> ��Ű��(���� ���� ���Դϴ�.)�� �� �ÿ� ���� �Ǻθ� �����ش�. (�ִϸ��̼� �� �þ �Ǻθ� ä���ش�.)
	// ���� �پ�� �ϴ� �޽��鿡 ���� �߰����� ��ɵ�

	D3DXMESHCONTAINER_DERIVED* pNewMeshContainer_Derived = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pNewMeshContainer_Derived, sizeof(D3DXMESHCONTAINER_DERIVED));
	//*ppNewMeshContainer = pNewMeshContainer_Derived;

	if (nullptr != Name)
	{
		if (FAILED(Allocate_Name(&pNewMeshContainer_Derived->Name, Name)))
			return E_FAIL;
	}
	
	pNewMeshContainer_Derived->MeshData.Type = D3DXMESHTYPE_MESH; // �޽� Ÿ�� ���Ƿ� ����
	LPD3DXMESH pTempMesh = pMeshData->pMesh; // �޽������� ����
	pTempMesh->AddRef();

	pNewMeshContainer_Derived->pAdjacency = new _ulong[pTempMesh->GetNumFaces() * 3];
	memcpy(pNewMeshContainer_Derived->pAdjacency, pAdjacency, sizeof(_ulong) * pTempMesh->GetNumFaces() * 3);

	_ulong dwFVF = pTempMesh->GetFVF(); // ���� �� �޽��� �ִ� �������� FVF ������ �� ���´�.
	if (false == (dwFVF & D3DFVF_NORMAL)) // Normal�� �����ϴ�?
	{
		if (FAILED(pTempMesh->CloneMeshFVF(pTempMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphic_Device, &pNewMeshContainer_Derived->MeshData.pMesh)))
			return E_FAIL;
		// Normal�� ���� �޽��� NORMAL�� �߰��� ������ ��, ���� ��ü�� �������ش�.

		if (FAILED(D3DXComputeNormals(pNewMeshContainer_Derived->MeshData.pMesh, pNewMeshContainer_Derived->pAdjacency)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(pTempMesh->CloneMeshFVF(pTempMesh->GetOptions(), dwFVF , m_pGraphic_Device, &pNewMeshContainer_Derived->MeshData.pMesh)))
			return E_FAIL;
	}

	Safe_Release(pTempMesh);
	
	// ���� ����.
	pNewMeshContainer_Derived->NumMaterials = NumMaterials == 0 ? 1 : NumMaterials; // 0�϶� 1, �ƴϸ� NumMaterials
	pNewMeshContainer_Derived->pMaterials = new D3DXMATERIAL[pNewMeshContainer_Derived->NumMaterials];
	ZeroMemory(pNewMeshContainer_Derived->pMaterials, sizeof(D3DXMATERIAL)*pNewMeshContainer_Derived->NumMaterials);


	if (0 != NumMaterials)
	{
		memcpy(pNewMeshContainer_Derived->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer_Derived->NumMaterials);  

		pNewMeshContainer_Derived->pTextures = new MESHTEXTURE[pNewMeshContainer_Derived->NumMaterials];
		ZeroMemory(pNewMeshContainer_Derived->pTextures, sizeof(MESHTEXTURE)*pNewMeshContainer_Derived->NumMaterials);

		// NumMaterials�� 0�ϼ��� ������, pNewMeshCo...->NumMaterials�� �ּ� 1�̶� �̷��� ����.
		for (size_t i = 0; i < pNewMeshContainer_Derived->NumMaterials; ++i)
		{

			_tchar	szFileName[MAX_PATH] = L"";

			MultiByteToWideChar(CP_ACP, 0, pMaterials[i].pTextureFilename, strlen(pMaterials[i].pTextureFilename),
				szFileName, MAX_PATH);

			Load_Texture(szFileName, L"D", &pNewMeshContainer_Derived->pTextures[i].pDiffuse);
			Load_Texture(szFileName, L"N", &pNewMeshContainer_Derived->pTextures[i].pNormal);
			Load_Texture(szFileName, L"S", &pNewMeshContainer_Derived->pTextures[i].pSpecular);
		}
	} 
	else // ��Ƽ������ 1���� ���� ���.
	{
		pNewMeshContainer_Derived->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(1.f, 0.f, 1.f, 1.f);
		pNewMeshContainer_Derived->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(1.f, 0.f, 1.f, 1.f);
		pNewMeshContainer_Derived->pMaterials[0].MatD3D.Specular = D3DXCOLOR(1.f, 0.f, 1.f, 1.f);
		pNewMeshContainer_Derived->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(1.f, 0.f, 1.f, 1.f);
		pNewMeshContainer_Derived->pMaterials[0].MatD3D.Power = 0.f;
		pNewMeshContainer_Derived->pMaterials[0].pTextureFilename = nullptr;
	}
	pNewMeshContainer_Derived->pSkinInfo = pSkinInfo;
	pNewMeshContainer_Derived->pSkinInfo->AddRef();

	pNewMeshContainer_Derived->dwNumBones = pNewMeshContainer_Derived->pSkinInfo->GetNumBones();


	// �� �޽��� ������ ��ġ�� ������ Combined����� �ּҸ� ������.
	pNewMeshContainer_Derived->ppCombinedMatrices = new D3DXMATRIX*[pNewMeshContainer_Derived->dwNumBones];
	ZeroMemory(pNewMeshContainer_Derived->ppCombinedMatrices, sizeof(D3DXMATRIX*)*pNewMeshContainer_Derived->dwNumBones);

	pNewMeshContainer_Derived->pOffsetMatrices = new D3DXMATRIX[pNewMeshContainer_Derived->dwNumBones];
	pNewMeshContainer_Derived->pRenderingMatrices = new D3DXMATRIX[pNewMeshContainer_Derived->dwNumBones];

	for (size_t i = 0; i < pNewMeshContainer_Derived->dwNumBones; i++)
	{
		pNewMeshContainer_Derived->pOffsetMatrices[i] =	*pNewMeshContainer_Derived->pSkinInfo->GetBoneOffsetMatrix(i);
		D3DXMatrixIdentity(&pNewMeshContainer_Derived->pRenderingMatrices[i]);
	}
	
	// D3DVERTEXELEMENT9 : ������ FVF���� �ϳ��� ������� ����ü.
	D3DVERTEXELEMENT9			Element[MAX_FVF_DECL_SIZE];
	ZeroMemory(&Element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);
	pNewMeshContainer_Derived->MeshData.pMesh->GetDeclaration(Element);

	if (FAILED(pNewMeshContainer_Derived->MeshData.pMesh->CloneMesh(pNewMeshContainer_Derived->MeshData.pMesh->GetOptions(), Element, m_pGraphic_Device, &pNewMeshContainer_Derived->pMesh_Original)))
		return E_FAIL;

	*ppNewMeshContainer = pNewMeshContainer_Derived;
	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer); 


	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	Safe_Delete(pFrameToFree);
	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Release(pMeshContainer->MeshData.pMesh);
	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->pAdjacency);
	Safe_Release(pMeshContainer->pSkinInfo);

	for (size_t i = 0; i < pMeshContainer->NumMaterials; i++)
	{
		Safe_Release(pMeshContainer->pTextures[i].pDiffuse);
		Safe_Release(pMeshContainer->pTextures[i].pNormal);
		Safe_Release(pMeshContainer->pTextures[i].pSpecular);
	}

	Safe_Delete_Array(pMeshContainer->pTextures);
	Safe_Release(pMeshContainer->pMesh_Original);

	Safe_Delete_Array(pMeshContainer->ppCombinedMatrices);
	Safe_Delete_Array(pMeshContainer->pOffsetMatrices);
	Safe_Delete_Array(pMeshContainer->pRenderingMatrices);

	Safe_Delete(pMeshContainer);

	return NOERROR;
}

_ulong CHierarchyLoader::AddRef()
{
	return _ulong(++m_dwRefCnt);
}

_ulong CHierarchyLoader::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
		return _ulong(m_dwRefCnt--);
}


HRESULT CHierarchyLoader::Allocate_Name(char ** ppSourName, const char * pDestName)
{
	_uint iLength = strlen(pDestName);

	if (nullptr != *ppSourName)
		return E_FAIL;
	  
	*ppSourName = new char[iLength + 1];
	ZeroMemory(*ppSourName, sizeof(char) * (iLength + 1));
	strcpy(*ppSourName, pDestName);


	return NOERROR;
}

HRESULT CHierarchyLoader::Load_Texture(const _tchar* pFileName, const _tchar * pTextureKey, LPDIRECT3DTEXTURE9 * ppTexture)
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
				if (szFileName[j] == 'D')
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

	D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, ppTexture);

	return NOERROR;
}

CHierarchyLoader * CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath)
{
	CHierarchyLoader*		pInstance = new CHierarchyLoader(pGraphic_Device);

	if (FAILED(pInstance->Ready_HierarchyLoader(pFilePath)))
	{
		_MSG_BOX("CHierarchyLoader Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHierarchyLoader::Free()
{
	Safe_Release(m_pGraphic_Device);
}
