
// MainFrm.h : CMainFrame 클래스의 인터페이스
//
#include "Engine_Defines.h"

#pragma once
class CToolView;
class CForm;
class CMainFrame : public CFrameWnd
{

protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// 특성입니다.
public:

	// 작업입니다.
public:

	// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// 구현입니다.
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
protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CSplitterWnd	  m_Splitter;
public:
	CToolView*		  m_pToolView;
	CForm*			  m_pFormView;

	// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnDestroy();
};


