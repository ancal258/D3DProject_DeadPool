#pragma once

#include "Page_MeshEffect.h"
#include "Page_Effect.h"


// CEffectSheet

class CEffectSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CEffectSheet)

public:
	CEffectSheet();
	CEffectSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CEffectSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CEffectSheet();
private:
	CPage_Effect		m_Page_Effect;
	CPage_MeshEffect	m_Page_MeshEffect;
protected:
	DECLARE_MESSAGE_MAP()
};


