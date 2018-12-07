
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//
#include "Engine_Defines.h"

#pragma once
class CToolView;
class CForm;
class CMainFrame : public CFrameWnd
{

protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// Ư���Դϴ�.
public:

	// �۾��Դϴ�.
public:

	// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	void Set_PickingPoint(_vec3 vPickingPoint);
	void Set_VecPointNav(_vec3* pVecPoint);
	void Add_CamPoint(_vec3 vPoint);
protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CSplitterWnd	  m_Splitter;
public:
	CToolView*		  m_pToolView;
	CForm*			  m_pFormView;

	// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnDestroy();
};


