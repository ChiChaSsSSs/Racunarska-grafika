
// IND_18561_Lab2View.h : interface of the CIND18561Lab2View class
//

#pragma once


class CIND18561Lab2View : public CView
{
protected: // create from serialization only
	CIND18561Lab2View() noexcept;
	DECLARE_DYNCREATE(CIND18561Lab2View)

// Attributes
public:
	CIND18561Lab2Doc* GetDocument() const;
	const int podeok = 25;
	bool nacrtanGrid;
	float velikiUgao;
	float maliUgao;
	HENHMETAFILE tamniKaktus;
	HENHMETAFILE svetliKaktus;
// Operations
public:
	void nacrtajGrid(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawFigure(CDC* pDC);
	void nacrtajSaksiju(CDC* pDC);
	void nacrtajKaktus(CDC* pDC);
	void nacrtajKrugove(CDC* pDC);
	void nacrtajBodlje(CDC* pDC);
	float stepeniURadijane(float s);
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
	virtual ~CIND18561Lab2View();
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

#ifndef _DEBUG  // debug version in IND_18561_Lab2View.cpp
inline CIND18561Lab2Doc* CIND18561Lab2View::GetDocument() const
   { return reinterpret_cast<CIND18561Lab2Doc*>(m_pDocument); }
#endif

