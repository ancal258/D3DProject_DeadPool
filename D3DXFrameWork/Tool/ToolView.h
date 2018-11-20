
// ToolView.h : CToolView Ŭ������ �������̽�
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
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

	// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

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

	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
{
	return reinterpret_cast<CToolDoc*>(m_pDocument);
}
#endif

