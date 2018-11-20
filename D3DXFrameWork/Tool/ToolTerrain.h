#pragma once

#include "GameObject.h"
#include "Defines.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CBuffer_Terrain;
class CTexture;
class CShader;
class CNavigation;
class CCell;
_END

_BEGIN(Tool)

class CToolTerrain : public CGameObject
{
private:
	explicit CToolTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CToolTerrain(const CToolTerrain& rhs);
	virtual ~CToolTerrain() = default;

public:
	void Set_BrushRadius(_float fRadius);
	void Set_BrushPower(_float fPower);
	void Set_MouseState(MOUSE_STATE eState);
	void Set_TerrainSize(_uint iWidth, _uint iHeight) {
		m_iWidth = iWidth;
		m_iHeight = iHeight;
	}
	_vec3 Get_PickingPoint() {
		return m_vPickingPoint;
	}
	_vec3 Get_MousePoint();
	MOUSE_STATE Get_MouseState() {
		return m_eMouseState;
	}
	vector<_vec3>* Get_vecPoint();
public:
	HRESULT Save_HeightMap();
	HRESULT Load_SplattingMap();
	HRESULT Add_Cell(_vec3* vPosArray);
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CBuffer_Terrain*	m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	_float				m_fRadius = 0;

	LPDIRECT3DTEXTURE9	m_pFilterTexture = nullptr;
	LPDIRECT3DTEXTURE9	m_pSplattingTexture = nullptr;
	D3DMATERIAL9		m_MtrlInfo;

	MOUSE_STATE	m_eMouseState = STATE_ADD_OBJECT;
	_uint				m_iWidth = 0;
	_uint				m_iHeight = 0;

	_vec3				m_vOut;
	_vec3				m_vPickingPoint;

public:
	HRESULT Ready_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CToolTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END

