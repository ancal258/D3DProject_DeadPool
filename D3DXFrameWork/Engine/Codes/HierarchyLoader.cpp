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
{ // -> 이 함수가 종료되면 인자로 들어온 애들을 모두 릴리즈 해준다.

	//D3DXMESHDATA
	//D3DXMATERIAL : 서브셋
	//D3DMATERIAL9 어쩌고 : 재질
	//pEffectInstances : 필요 없다 안쓴다.
	//NumMaterials : 삼각형 갯수
	//pAdjacency : 인접 삼각형 배열
	//pSkinInfo : 메쉬 덩어리(피부)로 할 수 있는 일들의 함수들? --> 스키닝(뼈에 살을 붙입니다.)을 할 시에 없는 피부를 입혀준다. (애니메이션 시 늘어난 피부를 채워준다.)
	// 뼈에 붙어야 하는 메쉬들에 대한 추가적인 기능들

	D3DXMESHCONTAINER_DERIVED* pNewMeshContainer_Derived = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pNewMeshContainer_Derived, sizeof(D3DXMESHCONTAINER_DERIVED));
	//*ppNewMeshContainer = pNewMeshContainer_Derived;

	if (nullptr != Name)
	{
		if (FAILED(Allocate_Name(&pNewMeshContainer_Derived->Name, Name)))
			return E_FAIL;
	}
	
	pNewMeshContainer_Derived->MeshData.Type = D3DXMESHTYPE_MESH; // 메쉬 타입 임의로 설정
	LPD3DXMESH pTempMesh = pMeshData->pMesh; // 메쉬데이터 저장
	pTempMesh->AddRef();

	pNewMeshContainer_Derived->pAdjacency = new _ulong[pTempMesh->GetNumFaces() * 3];
	memcpy(pNewMeshContainer_Derived->pAdjacency, pAdjacency, sizeof(_ulong) * pTempMesh->GetNumFaces() * 3);

	_ulong dwFVF = pTempMesh->GetFVF(); // 실제 이 메쉬에 있는 정점들의 FVF 정보를 싹 얻어온다.
	if (false == (dwFVF & D3DFVF_NORMAL)) // Normal이 존재하니?
	{
		if (FAILED(pTempMesh->CloneMeshFVF(pTempMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphic_Device, &pNewMeshContainer_Derived->MeshData.pMesh)))
			return E_FAIL;
		// Normal이 없는 메쉬에 NORMAL을 추가해 복사한 뒤, 원래 객체에 대입해준다.

		if (FAILED(D3DXComputeNormals(pNewMeshContainer_Derived->MeshData.pMesh, pNewMeshContainer_Derived->pAdjacency)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(pTempMesh->CloneMeshFVF(pTempMesh->GetOptions(), dwFVF , m_pGraphic_Device, &pNewMeshContainer_Derived->MeshData.pMesh)))
			return E_FAIL;
	}

	Safe_Release(pTempMesh);
	
	// 재질 대입.
	pNewMeshContainer_Derived->NumMaterials = NumMaterials == 0 ? 1 : NumMaterials; // 0일땐 1, 아니면 NumMaterials
	pNewMeshContainer_Derived->pMaterials = new D3DXMATERIAL[pNewMeshContainer_Derived->NumMaterials];
	ZeroMemory(pNewMeshContainer_Derived->pMaterials, sizeof(D3DXMATERIAL)*pNewMeshContainer_Derived->NumMaterials);


	if (0 != NumMaterials)
	{
		memcpy(pNewMeshContainer_Derived->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer_Derived->NumMaterials);  

		pNewMeshContainer_Derived->pTextures = new MESHTEXTURE[pNewMeshContainer_Derived->NumMaterials];
		ZeroMemory(pNewMeshContainer_Derived->pTextures, sizeof(MESHTEXTURE)*pNewMeshContainer_Derived->NumMaterials);

		// NumMaterials는 0일수도 있지만, pNewMeshCo...->NumMaterials는 최소 1이라서 이렇게 대입.
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
	else // 머티리얼이 1개도 없는 경우.
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


	// 이 메쉬에 영향을 미치는 뼈들이 Combined행렬의 주소를 가진다.
	pNewMeshContainer_Derived->ppCombinedMatrices = new D3DXMATRIX*[pNewMeshContainer_Derived->dwNumBones];
	ZeroMemory(pNewMeshContainer_Derived->ppCombinedMatrices, sizeof(D3DXMATRIX*)*pNewMeshContainer_Derived->dwNumBones);

	pNewMeshContainer_Derived->pOffsetMatrices = new D3DXMATRIX[pNewMeshContainer_Derived->dwNumBones];
	pNewMeshContainer_Derived->pRenderingMatrices = new D3DXMATRIX[pNewMeshContainer_Derived->dwNumBones];

	for (size_t i = 0; i < pNewMeshContainer_Derived->dwNumBones; i++)
	{
		pNewMeshContainer_Derived->pOffsetMatrices[i] =	*pNewMeshContainer_Derived->pSkinInfo->GetBoneOffsetMatrix(i);
		D3DXMatrixIdentity(&pNewMeshContainer_Derived->pRenderingMatrices[i]);
	}
	
	// D3DVERTEXELEMENT9 : 정점의 FVF정보 하나를 담기위한 구조체.
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
