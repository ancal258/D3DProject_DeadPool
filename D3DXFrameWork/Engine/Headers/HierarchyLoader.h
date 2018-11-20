#pragma once

#include "Engine_Defines.h"

_BEGIN(Engine)

class CHierarchyLoader final : public ID3DXAllocateHierarchy
{
private:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CHierarchyLoader() = default;
public:
	HRESULT Ready_HierarchyLoader(const _tchar* pFilePath);
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame) override;
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer) override;
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
public:
	_ulong AddRef();
	_ulong Release();
protected:
	_ulong			m_dwRefCnt = 0;
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	const _tchar*				m_pFilePath = nullptr;
private:
	HRESULT Allocate_Name(char** pSourName, const char* pDestName);
	HRESULT Load_Texture(const _tchar* pFileName, const _tchar* pTextureKey, LPDIRECT3DTEXTURE9* ppTexture);
public:
	static CHierarchyLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath);
protected:
	virtual void Free();
};

_END