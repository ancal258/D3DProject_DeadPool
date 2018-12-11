#pragma once


#include "Defines.h"
// CEffectForm 폼 뷰입니다.

class CEffectSheet;
class CEffectForm : public CFormView
{
	DECLARE_DYNCREATE(CEffectForm)

protected:
	CEffectForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CEffectForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	CEffectSheet*			m_pSheet = nullptr;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


