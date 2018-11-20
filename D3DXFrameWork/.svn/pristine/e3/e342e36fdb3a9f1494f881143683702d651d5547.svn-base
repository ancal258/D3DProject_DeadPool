#pragma once

#include "Component.h"

_BEGIN(Engine)

class CAnimationCtrl;
class CHierarchyLoader;
class _ENGINE_DLL CMesh_Dynamic final : public CComponent
{
private:
	explicit CMesh_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh_Dynamic(const CMesh_Dynamic& rhs);
	virtual ~CMesh_Dynamic() = default;
public:
	const _matrix* Get_FrameMatrixByName(const char* pFrameName);

public:
	HRESULT Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName);
	void Render_Mesh(LPD3DXEFFECT pEffect);
	HRESULT Set_AnimationSet(const _uint& iIndex);
	void Play_AnimationSet(const _float& fTimeDelta);
private:
	CHierarchyLoader*						m_pLoader = nullptr;
	D3DXFRAME*								m_pRootFrame = nullptr;
	D3DXMATRIX								m_matPivot;
	vector<D3DXMESHCONTAINER_DERIVED*>		m_vecMeshContainerList;
	CAnimationCtrl*							m_pAniCtrl = nullptr;
private:
	HRESULT Update_CombinedTransformationMatrix(D3DXFRAME_DERIVED* pFrame, D3DXMATRIX matParent);
	HRESULT SetUp_CombinedMatrixPointer(D3DXFRAME_DERIVED* pFrame);
public:
	static CMesh_Dynamic* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END