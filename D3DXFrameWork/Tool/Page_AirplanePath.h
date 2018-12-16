#pragma once


// CPage_AirplanePath 대화 상자입니다.
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

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_AIRPLANEPATH };
#endif
public:
	void Add_CamPoint(_vec3 vPoint);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_fValue;
	vector<CGameObject*>		m_vecCamPoints;

	virtual BOOL OnSetActive();
	CListBox m_PointList;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
