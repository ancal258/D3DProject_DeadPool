#include "stdafx.h"
#include "Texture_Bmp.h"

CTexture_Bmp::CTexture_Bmp(LPDIRECT3DDEVICE9 pGraphic_Device)
{

}

HRESULT CTexture_Bmp::Ready_Texture(const _tchar * pFilePath, const _uint & iCnt)
{
	m_vecTextures.reserve(iCnt);

	for (size_t i = 0; i < iCnt; ++i)
	{
		HBITMAP				hTexture = 0;
		_tchar				szFileName[128] = L"";

		wsprintf(szFileName, pFilePath, i);

		hTexture = (HBITMAP)LoadImage(0, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if (0 == hTexture)
			return E_FAIL;

		m_vecTextures.push_back(hTexture);
	}

	return NOERROR;
}

HRESULT CTexture_Bmp::SetUp_OnControl(CStatic * pControl, const _uint & iIndex)
{
	if (nullptr == pControl)
		return E_FAIL;

	if (m_vecTextures.size() <= iIndex)
		return E_FAIL;

	pControl->SetBitmap(m_vecTextures[iIndex]);

	return NOERROR;
}



CTexture_Bmp * CTexture_Bmp::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _uint & iCnt)
{
	CTexture_Bmp*		pInstance = new CTexture_Bmp(pGraphic_Device);

	if (FAILED(pInstance->Ready_Texture(pFilePath, iCnt)))
	{
		_MSG_BOX("CTexture_Bmp Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}
void CTexture_Bmp::Free()
{
	for (auto& pTexture : m_vecTextures)
		DeleteObject(pTexture);
	m_vecTextures.clear();
}
