#pragma once


// CPage_AirplanePath 대화 상자입니다.
#include "Engine_Defines.h"
#include "Defines.h"
#include "afxwin.h"
_BEGIN(Engine)
class CGameObject;
_END
class CTriggerCube;
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
	float m_fX;
	float m_fY;
	float m_fZ;
	afx_msg void OnBnClickedButton6();
	CListBox m_StringList;
	CListBox m_ButtonList;
	CListBox m_TriggerList;

	vector<CString> m_vecSaveString;
	vector<CString> m_vecSaveButton;
	vector<CTriggerCube*> m_vecTrigger;

	_uint			m_iIndex = 0;

	CString m_szButton;
	CString m_szString;
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton19();
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnLbnSelchangeList3();
	afx_msg void OnBnClickedButton9();
};
