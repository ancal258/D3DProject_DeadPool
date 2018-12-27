#pragma once

#include "Component.h"

_BEGIN(Engine)
class CTransform;

class _ENGINE_DLL CMesh_Static final : public CComponent
{
private:
	explicit CMesh_Static(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh_Static(const CMesh_Static& rhs);
	virtual ~CMesh_Static() = default;
public:
	const _ulong& Get_NumSubset() const {
		return m_dwNumSubset;
	}
	const _vec3* Get_MinPoint() const {
		return &m_vMin;
	}
	const _vec3* Get_MaxPoint() const {
		return &m_vMax;
	}
public:
	HRESULT Set_Texture_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, const _ulong& iSubset);
	HRESULT Set_TextureNorm_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, const _ulong& iSubset);
	HRESULT Set_Diffuse_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, const _ulong& iSubset);
	HRESULT Set_Ambient_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, const _ulong& iSubset);
	HRESULT Set_Specular_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, const _ulong& iSubset);
	HRESULT Set_Power_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, const _ulong& iSubset);
public:
	HRESULT Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName);
	HRESULT Clone_Mesh();
	HRESULT Picking_ToMesh(const _vec3 * pRayPos, const _vec3 * pRayDir, const CTransform * pTransCom, BOOL* pHit);
	void Render_Mesh(LPD3DXEFFECT pEffect);
private:
	LPD3DXMESH					m_pMesh = nullptr;
	LPD3DXBUFFER				m_pAdjacency = nullptr;
	LPD3DXBUFFER				m_pMaterials = nullptr;
	D3DXMATERIAL*				m_pSubsets = nullptr;
	LPDIRECT3DTEXTURE9*			m_ppTextures = nullptr;
	LPDIRECT3DTEXTURE9*			m_ppTexturesNorm = nullptr;
	const _tchar*				m_pFilePath = nullptr;

	_ulong						m_dwNumSubset = 0;
private:
	_vec3						m_vMin;
	_vec3						m_vMax;
private:
	HRESULT Load_Texture(const _tchar* pFileName, const _tchar* pTextureKey, LPDIRECT3DTEXTURE9* ppTexture);
public:
	static CMesh_Static* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END