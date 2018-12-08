#include "..\Headers\Mesh_Dynamic.h"
#include "HierarchyLoader.h"
#include "AnimationCtrl.h"
CMesh_Dynamic::CMesh_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{
}

CMesh_Dynamic::CMesh_Dynamic(const CMesh_Dynamic & rhs)
	: CComponent(rhs)
	, m_pLoader(rhs.m_pLoader)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_matPivot(rhs.m_matPivot)
	, m_vecMeshContainerList(rhs.m_vecMeshContainerList)
	, m_pAniCtrl(CAnimationCtrl::Create(*rhs.m_pAniCtrl))
{
	m_pLoader->AddRef();
}

const _matrix * CMesh_Dynamic::Get_FrameMatrixByName(const char * pFrameName)
{
	D3DXFRAME_DERIVED*	pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);

	if (nullptr == pFrame)
		return nullptr;

	return &pFrame->CombinedTransformationMatrix;
}

HRESULT CMesh_Dynamic::Ready_Mesh(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar			szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(Get_Graphic_Device(), pFilePath);
	if (nullptr == m_pLoader)
		return E_FAIL;

	LPD3DXANIMATIONCONTROLLER pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, Get_Graphic_Device(), m_pLoader, nullptr, &m_pRootFrame,  &pAniCtrl)))
		return E_FAIL;

	m_pAniCtrl = CAnimationCtrl::Create(pAniCtrl);
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	Safe_Release(pAniCtrl);

	D3DXMatrixIdentity(&m_matPivot);
	D3DXMatrixRotationY(&m_matPivot, D3DXToRadian(270));

	Update_CombinedTransformationMatrix((D3DXFRAME_DERIVED*)m_pRootFrame, m_matPivot);

	SetUp_CombinedMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);
	return NOERROR;
}

void CMesh_Dynamic::Render_Mesh(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	for (auto& pMeshContainer : m_vecMeshContainerList)
	{
		for (size_t i = 0; i < pMeshContainer->dwNumBones; i++)
			pMeshContainer->pRenderingMatrices[i] = pMeshContainer->pOffsetMatrices[i] * *pMeshContainer->ppCombinedMatrices[i];

		void	*pSour, *pDest;

		pMeshContainer->pMesh_Original->LockVertexBuffer(0, &pSour);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDest);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrices, nullptr, pSour, pDest);

		pMeshContainer->pMesh_Original->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

		for (size_t i = 0; i < pMeshContainer->NumMaterials; i++)
		{
			if (nullptr != pMeshContainer->pTextures[i].pDiffuse)
				pEffect->SetTexture("g_DiffuseTexture", pMeshContainer->pTextures[i].pDiffuse);

			if (nullptr != pMeshContainer->pTextures[i].pNormal)
				pEffect->SetTexture("g_NormalTexture", pMeshContainer->pTextures[i].pNormal);

			if (nullptr != pMeshContainer->pTextures[i].pSpecular)
				pEffect->SetTexture("g_SpecularTexture", pMeshContainer->pTextures[i].pSpecular);

			pEffect->CommitChanges(); // 갱신 필수

			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}

	Safe_Release(pEffect);
}

HRESULT CMesh_Dynamic::Set_AnimationSet(const _uint & iIndex)
{
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	return m_pAniCtrl->Set_AnimationSet(iIndex);
}

_bool CMesh_Dynamic::Is_Finish()
{
	if (nullptr == m_pAniCtrl)
		return false;

	return m_pAniCtrl->Is_Finish();
}

void CMesh_Dynamic::Play_AnimationSet(const _float & fTimeDelta)
{
	if (nullptr == m_pAniCtrl)
		return;

	m_pAniCtrl->Play_AnimationSet(fTimeDelta);
	Update_CombinedTransformationMatrix((D3DXFRAME_DERIVED*)m_pRootFrame, m_matPivot);
}

void CMesh_Dynamic::Set_TrackPosition(DOUBLE dlPosition)
{
	if (nullptr == m_pAniCtrl)
		return;
	m_pAniCtrl->Set_TrackPosition(dlPosition);
}

DOUBLE CMesh_Dynamic::Get_TrackPeriod()
{
	if (nullptr == m_pAniCtrl)
		return 0;
	return m_pAniCtrl->Get_TrackPeriod();
}

_bool CMesh_Dynamic::Get_ChangeMatrix()
{
	if (nullptr == m_pAniCtrl)
		return false;
	return false;
}

void CMesh_Dynamic::RegistCallbackFunc(function<void(void)> callbackFunc)
{
	m_pAniCtrl->RegistCallbackFunc(callbackFunc);
}

void CMesh_Dynamic::RegistCallbackCheckPair(function<_bool(_uint, _uint)> callbackCheckPair)
{
	m_pAniCtrl->RegistCallbackCheckPair(callbackCheckPair);
}

void CMesh_Dynamic::RegistCallbackCheckComboPair(function<_bool(_uint)> callbackCheckComboPair)
{
	m_pAniCtrl->RegistCallbackCheckComboPair(callbackCheckComboPair);
}

void CMesh_Dynamic::RegistCallbackCheckComboTime(function<_uint(_uint)> callbackCheckComboTime)
{
	m_pAniCtrl->RegistCallbackCheckComboTime(callbackCheckComboTime);
}


HRESULT CMesh_Dynamic::Update_CombinedTransformationMatrix(D3DXFRAME_DERIVED * pFrame, D3DXMATRIX matParent)
{
	// 자기 자신 구조체에 부모 구조체를 곱해서 저장.
	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * matParent;
	// 첫번째 자식 구조체에 자신을 부모로 하는 행렬을 곱해 저장.
	if (nullptr != pFrame->pFrameFirstChild)
		Update_CombinedTransformationMatrix((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, pFrame->CombinedTransformationMatrix);

	// 형제 구조체에겐 자신과 같은 부모로 하는 행렬곱
	if (nullptr != pFrame->pFrameSibling)
		Update_CombinedTransformationMatrix((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, matParent);

	return NOERROR;
}

HRESULT CMesh_Dynamic::SetUp_CombinedMatrixPointer(D3DXFRAME_DERIVED * pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		m_vecMeshContainerList.push_back(pMeshContainer);

		for (size_t i = 0; i < pMeshContainer->dwNumBones; i++)
		{
			// 뼈의 이름을 받는다.
			const char* pBoneName = pMeshContainer->pSkinInfo->GetBoneName(i);
			if (nullptr == pBoneName)
				return E_FAIL;

			// 뼈를 찾는다. (프레임마다 호출하면 위험)
			D3DXFRAME_DERIVED* pThisFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
			if (nullptr == pFrame)
				return E_FAIL;

			pMeshContainer->ppCombinedMatrices[i] = &pThisFrame->CombinedTransformationMatrix;
		}
	}


	if (nullptr != pFrame->pFrameFirstChild)
	{
		SetUp_CombinedMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
	}
	if(nullptr != pFrame->pFrameSibling)
		SetUp_CombinedMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	return NOERROR;
}

CMesh_Dynamic * CMesh_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName)
{
	CMesh_Dynamic*		pInstance = new CMesh_Dynamic(pGraphic_Device);

	if (FAILED(pInstance->Ready_Mesh(pFilePath, pFileName)))
	{
		_MSG_BOX("CMesh_Dynamic Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMesh_Dynamic::Clone_Component()
{
	return new CMesh_Dynamic(*this);
}
void CMesh_Dynamic::Free()
{
	//m_pLoader->DestroyFrame();
	if (false == is_Clone())
	{
		m_vecMeshContainerList.clear();
		m_pLoader->DestroyFrame(m_pRootFrame);
	}
	Safe_Release(m_pAniCtrl);

	Safe_Release(m_pLoader);
	CComponent::Free();
}