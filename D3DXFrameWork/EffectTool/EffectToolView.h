
// EffectToolView.h : CEffectToolView Ŭ������ �������̽�
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
protected: // serialization������ ��������ϴ�.
	CEffectToolView();
	DECLARE_DYNCREATE(CEffectToolView)

// Ư���Դϴ�.
public:
	CEffectToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
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
// ������ �޽��� �� �Լ�
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

#ifndef _DEBUG  // EffectToolView.cpp�� ����� ����
inline CEffectToolDoc* CEffectToolView::GetDocument() const
   { return reinterpret_cast<CEffectToolDoc*>(m_pDocument); }
#endif

