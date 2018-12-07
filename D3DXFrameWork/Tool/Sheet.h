#pragma once

#include "Page_Terrain.h"
#include "Page_Object.h"
#include "Page_Animation.h"
#include "Page_Navigation.h"
#include "Page_CinemaCam.h"

// CSheet

class CSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet)
public:
	void Set_PickingPoint(_vec3 vPickingPoint) {
		m_Page_Object.Set_PickingPoint(vPickingPoint);
	}
	void Set_VecPointNav(_vec3* pVecPoint) {
		m_Page_Navigation.Set_VecPointNav(pVecPoint);
	}
	void Add_CamPoint(_vec3 vPoint) {
		m_Page_CinemaCam.Add_CamPoint(vPoint);	}
public:
	CSheet();
	CSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheet();
private:
	CPage_Terrain			m_Page_Terrain;
	CPage_Object			m_Page_Object;
	CPage_Animation			m_Page_Animation;
	CPage_Navigation		m_Page_Navigation;
	CPage_CinemaCam			m_Page_CinemaCam;
protected:
	DECLARE_MESSAGE_MAP()
};


