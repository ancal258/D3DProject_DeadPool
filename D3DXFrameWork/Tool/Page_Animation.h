#pragma once
#include "Engine_Defines.h"
#include "Defines.h"
#include "afxcmn.h"

// CPage_Object ��ȭ �����Դϴ�.

_BEGIN(Engine)
class CGameObject;
_END

class CPage_Animation : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Animation)

public:
	CPage_Animation();
	virtual ~CPage_Animation();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_ANIMATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	CGameObject*	m_pMesh = nullptr;
public:
	CSliderCtrl m_SliderCtrl;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton10();
	int m_iAnimationIndex;
	float m_fSpeed;
	float m_fArrived;
	float m_fTimeDelta;
	float m_fFrame;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
