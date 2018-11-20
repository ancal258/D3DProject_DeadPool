
// ToolView.h : CToolView 클래스의 인터페이스
//
#include "Engine_Defines.h"
#include "Defines.h"
#include "ToolDoc.h"

#pragma once
_BEGIN(Engine)
class CRenderer;
class CGameObject;
_END

class CToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

	// 특성입니다.
public:
	CToolDoc* GetDocument() const;

	// 작업입니다.
public:


	// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	// 구현입니다.
public:
	virtual ~CToolView();

private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	CRenderer*					m_pRenderer = nullptr;
public:
	HRESULT Ready_Layer_Object(const _tchar * pPrototypeTag, const _tchar* pLayerTag, CGameObject** ppGameObject);
private:
	HRESULT Ready_Componet_Prototype();
	HRESULT Ready_Componet_Prototype_Static();
	HRESULT Ready_Componet_Prototype_SceneAPT();
	HRESULT Ready_Componet_Prototype_SceneMap();
	HRESULT Ready_Componet_Prototype_SceneStatic();

	HRESULT Ready_GameObject_Prototype();
	HRESULT Ready_GameObject_Prototype_Static();
	HRESULT Ready_GameObject_Prototype_SceneAPT();
	HRESULT Ready_GameObject_Prototype_SceneMap();
	HRESULT Ready_GameObject_Prototype_SceneStatic();

	HRESULT Ready_Camera_Info();
	HRESULT Ready_LightInfo();



#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
{
	return reinterpret_cast<CToolDoc*>(m_pDocument);
}
#endif

