
// hw4_2018008531View.h : interface of the Chw42018008531View class
//

#pragma once


class Chw42018008531View : public CView
{
protected: // create from serialization only
	Chw42018008531View() noexcept;
	DECLARE_DYNCREATE(Chw42018008531View)

// Attributes
public:
	Chw42018008531Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Chw42018008531View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in hw4_2018008531View.cpp
inline Chw42018008531Doc* Chw42018008531View::GetDocument() const
   { return reinterpret_cast<Chw42018008531Doc*>(m_pDocument); }
#endif

