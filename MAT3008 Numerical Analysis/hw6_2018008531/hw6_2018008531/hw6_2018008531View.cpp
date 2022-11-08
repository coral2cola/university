
// hw6_2018008531View.cpp : implementation of the Chw62018008531View class
//

#include "pch.h"
#include "nr.h"
#include <string>
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "hw6_2018008531.h"
#endif

#include "hw6_2018008531Doc.h"
#include "hw6_2018008531View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Chw62018008531View

IMPLEMENT_DYNCREATE(Chw62018008531View, CView)

BEGIN_MESSAGE_MAP(Chw62018008531View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Chw62018008531View construction/destruction

Chw62018008531View::Chw62018008531View() noexcept
{
	// TODO: add construction code here

}

Chw62018008531View::~Chw62018008531View()
{
}

BOOL Chw62018008531View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void Chw62018008531View::DrawSolidRectangle(int originx, int originy, int x, int width, int height)
{
	CClientDC pDC(this);
	pDC.FillSolidRect(originx + x, originy - height, width, height, BAR_COLOR);
	pDC.SetBkColor(RGB(255, 255, 255));
}

// Chw62018008531View drawing

void Chw62018008531View::OnDraw(CDC* pDC)
{
	Chw62018008531Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// (0,200), width 50, height 80, red
	// pDC->FillSolidRect(0, 200, 50, 80, RGB(255, 0, 0));

	//CClientDC pDC(this);
	pDC->MoveTo(ORIGIN_X, ORIGIN_Y); // 원점으로 이동
	pDC->LineTo(ORIGIN_X + X_AXIS_LENGTH, ORIGIN_Y); // X축 선 그리기
	pDC->MoveTo(ORIGIN_X, ORIGIN_Y); // 원점으로 이동
	pDC->LineTo(ORIGIN_X, ORIGIN_Y - Y_AXIS_LENGTH); // Y축 선 그리기

	int cnt[110];
	long idum = -1.234;
	float g;
	for (int i = 0; i <= 100; i++)
		cnt[i] = 0;
	for (int i = 0; i <= 100; i++)
	{
		g = gasdev(&idum);
		cnt[int(g / 0.05) + 50] += 1;
	}

	CString str;
	for (int i = 0; i <= 100; i++)
	{
		DrawSolidRectangle(ORIGIN_X, ORIGIN_Y, BAR_WIDTH * i, BAR_WIDTH - 1, BAR_HEIGHT * cnt[i]);
	}

	pDC->MoveTo(ORIGIN_X, ORIGIN_Y + 200); // 원점으로 이동
	pDC->LineTo(ORIGIN_X + X_AXIS_LENGTH, ORIGIN_Y + 200); // X축 선 그리기
	pDC->MoveTo(ORIGIN_X, ORIGIN_Y + 200); // 원점으로 이동
	pDC->LineTo(ORIGIN_X, ORIGIN_Y + 200 - Y_AXIS_LENGTH); // Y축 선 그리기
	
	idum = -1.234;
	for (int i = 0; i <= 100; i++)
		cnt[i] = 0;
	float f;
	for (int i = 0; i < 100; i++)
	{
		f = gasdev(&idum);
		cnt[int(f / 0.05) + 50] += 1;
	}
	for (int i = 0; i < 100; i++)
	{
		f = gasdev(&idum);
		cnt[int(f / 0.05) + 50] += 1;
	}

	
	for (int i = 0; i < 100; i++)
	{
		DrawSolidRectangle(ORIGIN_X, ORIGIN_Y+200, BAR_WIDTH * i, BAR_WIDTH - 1, BAR_HEIGHT * cnt[i]);
	}

	pDC->MoveTo(ORIGIN_X, ORIGIN_Y + 400); // 원점으로 이동
	pDC->LineTo(ORIGIN_X + X_AXIS_LENGTH, ORIGIN_Y + 400); // X축 선 그리기
	pDC->MoveTo(ORIGIN_X, ORIGIN_Y + 400); // 원점으로 이동
	pDC->LineTo(ORIGIN_X, ORIGIN_Y + 400 - Y_AXIS_LENGTH); // Y축 선 그리기

	idum = -1.234;
	for (int i = 0; i <= 100; i++)
		cnt[i] = 0;
	float h;
	for (int i = 0; i < 100; i++)
	{
		h = gasdev(&idum);
		cnt[int(h / 0.05) + 50] += 1;
	}
	for (int i = 0; i < 100; i++)
	{
		h = gasdev(&idum);
		cnt[int(h / 0.05) + 50] += 1;
	}
	for (int i = 0; i < 100; i++)
	{
		h = gasdev(&idum);
		cnt[int(h / 0.05) + 50] += 1;
	}
	
	for (int i = 0; i < 100; i++)
	{
		DrawSolidRectangle(ORIGIN_X, ORIGIN_Y + 400, BAR_WIDTH * i, BAR_WIDTH - 1, BAR_HEIGHT * cnt[i]);
	}

}


// Chw62018008531View printing

BOOL Chw62018008531View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Chw62018008531View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Chw62018008531View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Chw62018008531View diagnostics

#ifdef _DEBUG
void Chw62018008531View::AssertValid() const
{
	CView::AssertValid();
}

void Chw62018008531View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Chw62018008531Doc* Chw62018008531View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Chw62018008531Doc)));
	return (Chw62018008531Doc*)m_pDocument;
}
#endif //_DEBUG


// Chw62018008531View message handlers
