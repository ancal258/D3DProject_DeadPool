
// EffectToolView.cpp : CEffectToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "EffectTool.h"
#endif

#include "EffectToolDoc.h"
#include "EffectToolView.h"

// My Include
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Graphic_Device.h"
#include "Input_Device.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define MY_TIMER 10


// CEffectToolView

IMPLEMENT_DYNCREATE(CEffectToolView, CView)

BEGIN_MESSAGE_MAP(CEffectToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CEffectToolView 생성/소멸

CEffectToolView::CEffectToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CEffectToolView::~CEffectToolView()
{
}

BOOL CEffectToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CEffectToolView 그리기

void CEffectToolView::OnDraw(CDC* /*pDC*/)
{
	CEffectToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CEffectToolView 인쇄

BOOL CEffectToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CEffectToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CEffectToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CEffectToolView 진단

#ifdef _DEBUG
void CEffectToolView::AssertValid() const
{
	CView::AssertValid();
}

void CEffectToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEffectToolDoc* CEffectToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEffectToolDoc)));
	return (CEffectToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CEffectToolView 메시지 처리기


void CEffectToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == MY_TIMER)
	{
		CObject_Manager::GetInstance()->Update_Object_Manager(0.f);
		CObject_Manager::GetInstance()->LastUpdate_Object_Manager(0.f);
		CInput_Device::GetInstance()->Inquire_Input_State();

		m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
		m_pGraphic_Device->BeginScene();

		if (nullptr != m_pRenderer)
			m_pRenderer->Render_Renderer();

		m_pGraphic_Device->EndScene();
		m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);
	}

	CView::OnTimer(nIDEvent);
}


void CEffectToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	SetTimer(MY_TIMER, 30, 0);

	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(m_hWnd, CGraphic_Device::MODE_WIN, g_iBackCX, g_iBackCY, &m_pGraphic_Device)))
		return;
	// For.Input_Device

	HINSTANCE hInst = AfxGetInstanceHandle();
	if (FAILED(CInput_Device::GetInstance()->Ready_Input_Device(hInst, m_hWnd)))
		return;

	CComponent_Manager::GetInstance()->Reserve_Component_Manager(1);
	CObject_Manager::GetInstance()->Reserve_Object_Manager(1);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
