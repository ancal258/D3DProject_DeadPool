#pragma once

#include "VIBuffer.h"

_BEGIN(Engine)

class CPicking;
class CTransform;
class CNavigation;
class _ENGINE_DLL CBuffer_Terrain final : public CVIBuffer
{
private:
	explicit CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuffer_Terrain(const CBuffer_Terrain& rhs);
	virtual ~CBuffer_Terrain() = default;
public:
	void Set_BrushRadius(_float fRadius) {
		m_fRadius = fRadius;	}
	void Set_BrushPower(_float fPower) {
		m_fPower = fPower;	}
	void Set_MouseState(_uint iMouseState) {
		m_iMouseState = iMouseState;	}
public:
	HRESULT Ready_VIBuffer(const _uint& iNumVertexX, const _uint& iNumVertexZ, const _float& fInterval);
	HRESULT Ready_VIBuffer(const _tchar* pHeightMapPath, const _float& fInterval);
	_vec3 SetHeight_OnTerrain(const _vec3* pPosition, const CTransform* pTransform_Terrain) const;
	virtual _bool Picking_ToBuffer(const _vec3* pRayPos, const _vec3* pRayDir, const CTransform* pTransCom, _vec3* pOut);
	virtual _bool Picking_ToBuffer(const _vec3* pRayPos, const _vec3* pRayDir, const CTransform* pTransCom, CNavigation* pNavigation , _vec3* pOut);
	virtual void Render_VIBuffer();
private:
	BITMAPFILEHEADER			m_fh;
	BITMAPINFOHEADER			m_ih;
	LPDIRECT3DTEXTURE9	m_pSplattingTexture = nullptr;

private:
	_float						m_fInterval = 0.f;
	_ulong*						m_pPixel = nullptr;
	_float						m_fRadius = 0;
	_float						m_fPower = 0;
	_uint						m_iMouseState = 0;
private:
	_bool*						m_pIsNavigation = nullptr;
private:
	void Draw_ToBuffer(_vec3* pOut);
	void Draw_ToTexture(_vec3* pOut);
	void Draw_ToNavigation(_vec3* pOut, CNavigation* pNavigation);
public:	
	static CBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint& iNumVertexX, const _uint& iNumVertexZ, const _float& fInterval = 1.f);
	static CBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMapPath, const _float& fInterval = 1.f);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END