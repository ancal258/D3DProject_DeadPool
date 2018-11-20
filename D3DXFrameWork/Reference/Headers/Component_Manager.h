#pragma once

#include "Base.h"

// ���� ������Ʈ���� ��Ƽ� �����Ѵ�.
#include "Buffer_RcTex.h"
#include "Buffer_RcCol.h"
#include "Buffer_Collider.h"
#include "Renderer.h"
#include "Transform.h"
#include "Shader.h"
#include "Texture.h"
#include "Buffer_Terrain.h"
#include "Picking.h"
#include "Mesh_Static.h"
#include "Mesh_Dynamic.h"
#include "Collider.h"
#include "Navigation.h"
_BEGIN(Engine)

class _ENGINE_DLL CComponent_Manager final : public CBase
{
	_DECLARE_SINGLETON(CComponent_Manager)
private:
	explicit CComponent_Manager();
	virtual ~CComponent_Manager() = default;
public:
	HRESULT Reserve_Component_Manager(const _uint& iMaxScene);
	HRESULT Add_Component(const _uint& iSceneIdx, const _tchar* pComponentTag, CComponent* pComponent);
	CComponent* Clone_Component(const _uint& iSceneIdx, const _tchar* pComponentTag);
	HRESULT Clear_Component(const _uint& iSceneIdx);
private:
	map<const _tchar*, CComponent*>*		m_pMapComponents; // ������ü���� ������ �����̳ʴ�. 
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENT;
private:
	_uint									m_iMaxScene = 0;
private:
	CComponent* Find_Component(const _uint& iSceneIdx, const _tchar* pComponentTag);
protected:
	virtual void Free();


};

_END