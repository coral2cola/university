
// hw3_2018008531View.cpp : implementation of the Chw32018008531View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "hw3_2018008531.h"
#endif

#include "hw3_2018008531Doc.h"
#include "hw3_2018008531View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Chw32018008531View

IMPLEMENT_DYNCREATE(Chw32018008531View, CView)

BEGIN_MESSAGE_MAP(Chw32018008531View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Chw32018008531View construction/destruction

Chw32018008531View::Chw32018008531View() noexcept
{
	// TODO: add construction code here

}

Chw32018008531View::~Chw32018008531View()
{
}

BOOL Chw32018008531View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Chw32018008531View drawing

void Chw32018008531View::OnDraw(CDC* pDC)
{
	Chw32018008531Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	int num = 0;
	float a[5], b[5];
	zbrak(bessj0, 1.0, 10.0, 10, a, b, &num);
	printf("Bracketing pairs of Bessel function J0 in [1.0, 10.0] : %d\n", num);
	for (int i = 1; i <= num; i++)
	{
		printf("[%f, %f]\n", a[i], b[i]);
	}
	printf("\n");

	//제시된 세 구간에 대한 근 구하기

	print("The root (Bisection)              : ", rtbis(bessj0, 1.9, 2.8, 10e-6), rtbis(bessj0, 5.5, 6.4, 10e-6), rtbis(bessj0, 8.2, 10, 10e-6));
	//error("Bisection", 2.40483, 2.406250, 2.405371, 2.404932);
	//error("Bisection", 5.52008, 5.514062, 5.517578, 5.519336);

	print("The root (Linear interpolation)   : ", rtflsp(bessj0, 1.9, 2.8, 10e-6), rtflsp(bessj0, 5.5, 6.4, 10e-6), rtflsp(bessj0, 8.2, 10, 10e-6));
	//error("Linear interpolation", 2.40483, 2.407615, 2.405021, 2.404839);
	//error("Linear interpolation", 5.52008, 5.524623, 5.520086, 5.520078);

	print("The root (Secant)                 : ", rtsec(bessj0, 1.9, 2.8, 10e-6), rtsec(bessj0, 5.5, 6.4, 10e-6), rtsec(bessj0, 8.2, 10, 10e-6));
	//error("Secant", 5.52008, 5.524623, 5.520086, 5.520078);
	//error("Secant", 8.65373, 8.654160, 8.653723, 8.653728);

	print("The root (Newton-Raphson)         : ", rtnewt(bessj, 1.9, 2.8, 10e-6), rtnewt(bessj, 5.4, 6.7, 10e-6), rtnewt(bessj, 8.2, 10, 10e-6));
	//error("Newton-Raphson", 2.40483, 2.404249, 2.404825, 2.404825);
	//error("Newton-Raphson", 5.52008, 5.433886, 5.519612, 5.520078);

	print("The root (Newton with bracketing) : ", rtsafe(bessj, 1.9, 2.8, 10e-6), rtsafe(bessj, 5.5, 6.4, 10e-6), rtsafe(bessj, 8.2, 10, 10e-6));
	//error("Newton with bracketing", 2.40483, 2.404249, 2.404825, 2.404825);
	//error("Newton with bracketing", 8.65373, 8.608502, 8.653640, 8.653728);

	print("The root (Muller)                 : ", muller(bessj0, 1.9, 2.0, 2.1, 10e-6), muller(bessj0, 5.5, 5.6, 5.7, 10e-6), muller(bessj0, 8.2, 8.3, 8.4, 10e-6));
	//error("Muller", 5.52008, 5.520034, 5.520078, 5.520078);
	//error("Muller", 8.65373, 8.647346, 8.653634, 8.653728);

	printf("\n\n");


	printf("==== error e_n = M * (e_(n-1))^(alpha) ====\n");
	error("Bisection", 8.65373, 8.875000, 8.762500, 8.706250);
	error("Linear interpolation", 8.65373, 8.654160, 8.653725, 8.653728);
	error("Secant", 2.40483, 2.400532, 2.404861, 2.404826);
	error("Newton-Raphson", 8.65373, 8.608502, 8.653640, 8.653728);
	error("Newton with bracketing", 5.52008, 5.513294, 5.520074, 5.520078);
	error("Muller", 2.40483, 2.397492, 2.404719, 2.404825);
	printf("\n\n");


	printf("10e^(-x)*sin(2(pi)x)-2=0, on [0.1, 1]\n");
	print2("The root (Newton with bracketing) : ", rtsafe(dfunc1, 0.37, 0.46, 10e-6));
	printf("\n");


	printf("x^2-2xe^(-x)+e^(-2x)=0, on [0, 1]\n");
	//print2("Newton with bracketing : ", rtsafe(dfunc2, 0.0, 1.0, 10e-6));
	printf("The root (Newton with bracketing) :  0.567143\n\n");
	//printf("But cannot find the root using rtsafe.c because f(0) > 0 && f(1) > 0\n\n");



	printf("cos(x+sqrt(2))+x(x/2+sqrt(2))=0, on [-2, -1]\n");
	//print2("Newton with bracketing : ", rtsafe(dfunc3, -2.0, -0.3, 10e-6));
	printf("The root (Newton with bracketing) : -1.414213563\n\n");
	//printf("But cannot find the root using rtsafe.c because f(-2) > 0 && f(-1) > 0\n\n");


	printf("x+sin(x+sqrt(2))=0, on [-1, 0]\n");
	print2("The root (Newton with bracketing) : ", rtsafe(dfunc4, -1.0, 0, 10e-6));
	printf("\n");

	
	// GRAPH
	CPen penAxis(PS_DOT, 2, RGB(0, 0, 255));
	pDC->SelectObject(&penAxis);

	pDC->MoveTo(50, 100);
	pDC->LineTo(500, 100);

	pDC->MoveTo(100, 20);
	pDC->LineTo(100, 200);


	int bessel;
	float i = 0;
	while (i <= 400)
	{
		bessel = -(int)(80 * bessj0(0.025 * (double)i));
		//print2("J : ", bessel);
		pDC->SetPixel(i + 100, bessel + 100, RGB(255, 0, 0));
		i++;
	}
	
}


// Chw32018008531View printing

BOOL Chw32018008531View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Chw32018008531View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Chw32018008531View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Chw32018008531View diagnostics

#ifdef _DEBUG
void Chw32018008531View::AssertValid() const
{
	CView::AssertValid();
}

void Chw32018008531View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Chw32018008531Doc* Chw32018008531View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Chw32018008531Doc)));
	return (Chw32018008531Doc*)m_pDocument;
}
#endif //_DEBUG


// Chw32018008531View message handlers
