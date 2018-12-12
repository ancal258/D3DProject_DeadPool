
// EffectToolView.h : CEffectToolView 클래스의 인터페이스
//

#include "Engine_Defines.h"
#include "Defines.h"
#include "EffectToolDoc.h"

#pragma once
_BEGIN(Engine)
class CRenderer;
class CGameObject;
_END

class CEffectToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CEffectToolView();
	DECLARE_DYNCREATE(CEffectToolView)

// 특성입니다.
public:
	CEffectToolDoc* GetDocument() const;

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
	virtual ~CEffectToolView();


private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	CRenderer*					m_pRenderer = nullptr;


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
private:
	HRESULT			Ready_Layer_Object(const _tchar * pPrototypeTag, const _tchar * pLayerTag, CGameObject** ppGameObject);
	HRESULT			Ready_Component();
	HRESULT			Ready_Prototype();
	HRESULT			Ready_Layer_Camera();
	HRESULT			Layer_Object();
};

#ifndef _DEBUG  // EffectToolView.cpp의 디버그 버전
inline CEffectToolDoc* CEffectToolView::GetDocument() const
   { return reinterpret_cast<CEffectToolDoc*>(m_pDocument); }
#endif

