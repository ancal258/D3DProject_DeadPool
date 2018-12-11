#pragma once


#include "Defines.h"
// CEffectForm �� ���Դϴ�.

class CEffectSheet;
class CEffectForm : public CFormView
{
	DECLARE_DYNCREATE(CEffectForm)

protected:
	CEffectForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


