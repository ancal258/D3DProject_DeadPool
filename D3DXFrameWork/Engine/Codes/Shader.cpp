#include "..\Headers\Shader.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)	
{
	m_pEffect->AddRef();

}

HRESULT CShader::Ready_Shader(const _tchar * pFileName)
{
	if (FAILED(D3DXCreateEffectFromFile(Get_Graphic_Device(), pFileName, nullptr, nullptr, 0, nullptr, &m_pEffect, nullptr)))
		return E_FAIL;

	return NOERROR;
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFileName)
{
	CShader*		pInstance = new CShader(pGraphic_Device);

	if (FAILED(pInstance->Ready_Shader(pFileName)))
	{
		_MSG_BOX("CShader Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CShader::Clone_Component()
{
	return new CShader(*this);
}

void CShader::Free()
{
	Safe_Release(m_pEffect);

	CComponent::Free();
}
