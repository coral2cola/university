
// hw4_2018008531View.cpp : implementation of the Chw42018008531View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "hw4_2018008531.h"
#endif

#include "hw4_2018008531Doc.h"
#include "hw4_2018008531View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Chw42018008531View

IMPLEMENT_DYNCREATE(Chw42018008531View, CView)

BEGIN_MESSAGE_MAP(Chw42018008531View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Chw42018008531View construction/destruction

Chw42018008531View::Chw42018008531View() noexcept
{
	// TODO: add construction code here

}

Chw42018008531View::~Chw42018008531View()
{
}

BOOL Chw42018008531View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Chw42018008531View drawing

void Chw42018008531View::OnDraw(CDC* pDC)
{
	Chw42018008531Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here


	//제시된 세 구간에 대한 근 구하기
	print("The root (Bisection), r.e. = 10e-4              : ", rtbis(func, -320, -310, 10e-4), rtbis(func, 320, 330, 10e-4));
	print("The root (Bisection), r.e. = 10e-6              : ", rtbis(func, -320, -310, 10e-6), rtbis(func, 320, 330, 10e-6));
	printf("\n");

	print("The root (Linear interpolation), r.e. = 10e-4   : ", rtflsp(func, -320, -310, 10e-4), rtflsp(func, 320, 330, 10e-4));
	print("The root (Linear interpolation), r.e. = 10e-6   : ", rtflsp(func, -320, -310, 10e-6), rtflsp(func, 320, 330, 10e-6));
	printf("\n");

	print("The root (Secant), r.e. = 10e-4                 : ", rtsec(func, -320, -310, 10e-4), rtsec(func, 320, 330, 10e-4));
	print("The root (Secant), r.e. = 10e-6                 : ", rtsec(func, -320, -310, 10e-6), rtsec(func, 320, 330, 10e-6));
	printf("\n");

	print("The root (Newton-Raphson), r.e. = 10e-4         : ", rtnewt(dfunc, -320, -310, 10e-4), rtnewt(dfunc, 320, 330, 10e-4));
	print("The root (Newton-Raphson), r.e. = 10e-6         : ", rtnewt(dfunc, -320, -310, 10e-6), rtnewt(dfunc, 320, 330, 10e-6));
	printf("\n");

	print("The root (Newton with bracketing), r.e. = 10e-4 : ", rtsafe(dfunc, -320, -310, 10e-4), rtsafe(dfunc, 320, 330, 10e-4));
	print("The root (Newton with bracketing), r.e. = 10e-6 : ", rtsafe(dfunc, -320, -310, 10e-6), rtsafe(dfunc, 320, 330, 10e-6));
	printf("\n");

	print("The root (Muller), r.e. = 10e-4                 : ", muller(func, -320, -315, -310, 10e-4), muller(func, 320, 325, 330, 10e-4));
	print("The root (Muller), r.e. = 10e-6                 : ", muller(func, -320, -315, -310, 10e-6), muller(func, 320, 325, 330, 10e-6));
	printf("\n");



	// GRAPH
	CPen penAxis(PS_DOT, 2, RGB(0, 0, 255));
	pDC->SelectObject(&penAxis);

	pDC->MoveTo(50, 100);
	pDC->LineTo(650, 100);

	pDC->MoveTo(350, 20);
	pDC->LineTo(350, 200);

	
	int f;
	float i = -600;
	while (i <= 600)
	{
		f = -(int)(80 * func(2 * (double)i));
		pDC->SetPixel(i + 350, f + 100, RGB(255, 0, 0));
		i++;
	}
	
}


// Chw42018008531View printing

BOOL Chw42018008531View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Chw42018008531View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Chw42018008531View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Chw42018008531View diagnostics

#ifdef _DEBUG
void Chw42018008531View::AssertValid() const
{
	CView::AssertValid();
}

void Chw42018008531View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Chw42018008531Doc* Chw42018008531View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Chw42018008531Doc)));
	return (Chw42018008531Doc*)m_pDocument;
}
#endif //_DEBUG


// Chw42018008531View message handlers
