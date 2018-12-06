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
	_bool Is_Finish();
	void Play_AnimationSet(const _float& fTimeDelta);
	void Set_TrackPosition(DOUBLE dlPosition);
	DOUBLE Get_TrackPeriod();
	_bool Get_ChangeMatrix();
	void RegistCallbackFunc(function<void(void)> callbackFunc);
	void RegistCallbackCheckPair(function<_bool(_uint, _uint)> callbackCheckPair);
	void RegistCallbackCheckComboPair(function<_bool(_uint)> callbackCheckComboPair);
	void RegistCallbackCheckComboTime(function<_uint(_uint)> callbackCheckComboTime);
private:
	CHierarchyLoader*						m_pLoader = nullptr;
	D3DXFRAME*								m_pRootFrame = nullptr;
	D3DXMATRIX								m_matPivot;
	vector<D3DXMESHCONTAINER_DERIVED*>		m_vecMeshContainerList;
	CAnimationCtrl*							m_pAniCtrl = nullptr;

	_tchar							m_pFilePath[MAX_PATH] = L"";
	_tchar							m_pFileName[MAX_PATH] = L"";
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