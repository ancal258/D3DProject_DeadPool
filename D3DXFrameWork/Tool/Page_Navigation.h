#pragma once
#include "afxwin.h"


// CPage_Navigation 대화 상자입니다.
#include "Engine_Defines.h"
#include "Defines.h"
class CPage_Navigation : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Navigation)

public:
	CPage_Navigation();
	virtual ~CPage_Navigation();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_NAVIGATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
void Set_VecPointNav(_vec3* pVecPoint);

public:
	float m_fNavPosX;
	float m_fNavPosY;
	float m_fNavPosZ;
	CListBox m_PointList;
	CListBox m_NavList;
	_vec3 m_vArray[3];
	vector<_vec3*> m_vecPoint;
	_uint m_iPointNum = 0;
	////////////////////////////
	LPDIRECT3DDEVICE9		 m_pGraphic_Device = nullptr;
	LPD3DXLINE				m_pLine = nullptr;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton10();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
};
