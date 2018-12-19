#pragma once
#include "afxwin.h"

#include "Engine_Defines.h"
#include "Defines.h"
// CPage_Effect 대화 상자입니다.
_BEGIN(Engine)
class CGameObject;
_END
class CPage_Effect : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Effect)

public:
	CPage_Effect();
	virtual ~CPage_Effect();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_EFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_TextureList;
	float m_fInitX;
	float m_fInitY;
	float m_fInitZ;
	bool m_isRandomPos;
	bool m_isSettingPos;
	float m_fScale;
	float m_fMoveSpeed;
	float m_fSurviveTime;
	bool m_isAnimationTexture;
	bool m_isOneTexture;
	float m_fFrameSpeed;
	float m_fFrameMax;
	float m_fRotDegree;
	float m_fDirX;
	float m_fDirY;
	float m_fDirZ;
	BOOL m_isContinueCreate;
	float m_fCreateTime;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeList1();
	_uint	m_iSelect = 0;

private:
	vector<CGameObject*>	m_vecObject;
public:
	CListBox m_AnimList;
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnBnClickedButton10();
};
