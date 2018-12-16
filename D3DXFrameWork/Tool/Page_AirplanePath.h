#pragma once


// CPage_AirplanePath ��ȭ �����Դϴ�.
#include "Engine_Defines.h"
#include "Defines.h"
#include "afxwin.h"
_BEGIN(Engine)
class CGameObject;
_END

class CPage_AirplanePath : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_AirplanePath)

public:
	CPage_AirplanePath();
	virtual ~CPage_AirplanePath();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_AIRPLANEPATH };
#endif
public:
	void Add_CamPoint(_vec3 vPoint);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	float m_fValue;
	vector<CGameObject*>		m_vecCamPoints;

	virtual BOOL OnSetActive();
	CListBox m_PointList;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
