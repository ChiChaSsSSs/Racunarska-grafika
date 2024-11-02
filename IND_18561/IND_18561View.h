
// IND_18561View.h : interface of the CIND18561View class
//

#pragma once


class CIND18561View : public CView
{
protected: // create from serialization only
	CIND18561View() noexcept;
	DECLARE_DYNCREATE(CIND18561View)

// Attributes
public:
	CIND18561Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void nacrtajTrougao(CDC* pDC, struct Trougao t);
	void nacrtajCetvorougao(CDC* pDC, struct Cetvorougao c);
	void nacrtajGrid(CDC* pDC);
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIND18561View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_18561View.cpp
inline CIND18561Doc* CIND18561View::GetDocument() const
   { return reinterpret_cast<CIND18561Doc*>(m_pDocument); }
#endif

