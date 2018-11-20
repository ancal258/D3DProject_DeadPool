#pragma once
#include "Engine_Defines.h"
#include "Defines.h"
#include "afxwin.h"

_BEGIN(Engine)
class CGameObject;
_END

// CPage_Terrain 대화 상자입니다.
class CTexture_Bmp;
class CPage_Terrain : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Terrain)

public:
	CPage_Terrain();
	virtual ~CPage_Terrain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_TERRAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	CGameObject*	m_pTerrain = nullptr;
	MOUSE_STATE	m_eMouseState = STATE_DRAWUP;
	int m_iTerrainX;
	int m_iTerrainZ;
	float m_fKeyboardSpeed;
	float m_fMouseSpeed;
	float m_fRadius;
	float m_fPower;

	// Texture
	CStatic m_Picture1;
	CStatic m_Picture2;

	CTexture_Bmp*	m_pBmp = nullptr;
	CStatic m_Picture3;
	CStatic m_Picture4;
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
};
