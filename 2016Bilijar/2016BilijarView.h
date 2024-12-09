
// 2016BilijarView.h : interface of the CMy2016BilijarView class
//

#pragma once
#include "DImage.h"


class CMy2016BilijarView : public CView
{
protected: // create from serialization only
	CMy2016BilijarView() noexcept;
	DECLARE_DYNCREATE(CMy2016BilijarView)

// Attributes
public:
	CMy2016BilijarDoc* GetDocument() const;
	int rotAngle;
	int stickPosition;
	int ballPosition;
	DImage background;
	DImage border;
// Operations
public:
	float degreesToRadians(float degAngle);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawTable(CDC* pDC, CRect rect);
	void DrawBorder(CDC* pDC, CRect rect, int w);
	void DrawHoles(CDC* pDC, CRect rect, int size);
	void DrawBall(CDC* pDC, int w);
	void DrawStick(CDC* pDC, int w);
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
	virtual ~CMy2016BilijarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in 2016BilijarView.cpp
inline CMy2016BilijarDoc* CMy2016BilijarView::GetDocument() const
   { return reinterpret_cast<CMy2016BilijarDoc*>(m_pDocument); }
#endif

