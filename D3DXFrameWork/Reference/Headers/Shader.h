#pragma once

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL CShader final : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;
public:
	LPD3DXEFFECT Get_EffectHandle() const {
		return m_pEffect; }

public:
	HRESULT Ready_Shader(const _tchar* pFileName);
private:
	LPD3DXEFFECT				m_pEffect = nullptr;
public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFileName);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END