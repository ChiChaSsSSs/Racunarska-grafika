
// 2023RobotView.h : interface of the CMy2023RobotView class
//

#pragma once
#include "DImage.h"


class CMy2023RobotView : public CView
{
protected: // create from serialization only
	CMy2023RobotView() noexcept;
	DECLARE_DYNCREATE(CMy2023RobotView)

// Attributes
public:
	CMy2023RobotDoc* GetDocument() const;
	DImage glava;
	DImage nadkolenica;
	DImage nadlaktica;
	DImage podkolenica;
	DImage podlaktica;
	DImage saka;
	DImage stopalo;
	DImage telo;
	DImage pozadina;
	DImage kurcina;
	int rotNadlaktica;
	int rotPodlaktica;
	int rotSaka;
	int rotCeoRobot;
	int rotDick;
	float skaliranje;

// Operations
public:
	float degreesToRadians(float degAngle);
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawRobot(CDC* pDC);
	void DrawHalf(CDC* pDC);
	void DrawHead(CDC* pDC);
	void DrawDick(CDC* pDC);
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
	virtual ~CMy2023RobotView();
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

#ifndef _DEBUG  // debug version in 2023RobotView.cpp
inline CMy2023RobotDoc* CMy2023RobotView::GetDocument() const
   { return reinterpret_cast<CMy2023RobotDoc*>(m_pDocument); }
#endif

