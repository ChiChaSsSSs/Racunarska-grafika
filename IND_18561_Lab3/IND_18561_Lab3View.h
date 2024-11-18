
// IND_18561_Lab3View.h : interface of the CIND18561Lab3View class
//

#pragma once
#include "../DImage/DImage.h"

struct deoSlagalice {
	DImage slika;
	int transX;
	int transY;
	int ugaoRotacije;
	bool refX;
	bool refY;
	bool siva;
};

class CIND18561Lab3View : public CView
{
protected: // create from serialization only
	CIND18561Lab3View() noexcept;
	DECLARE_DYNCREATE(CIND18561Lab3View)

// Attributes
public:
	CIND18561Lab3Doc* GetDocument() const;
	deoSlagalice delovi[9] = {
		{DImage(), -280, 120, -105, true, false, false},
		{DImage(), -262, 271, 55, true, false, true},
		{DImage(), -117, -20, 131, true, false, true},
		{DImage(), 20, 275, -136, true, false, true},
		{DImage(), -275, -34, -53, true, false, true},
		{DImage(), 20, -23, -137, true, false, true},
		{DImage(), -113, 120, 54, true, false, true},
		{DImage(), -114, 268, 32, true, false, true},
		{DImage(), 33, 115, 18, true, false, true},
	};
	const int podeok = 25;
// Operations
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void Mirror(CDC* pDC, bool mX, bool mY, bool rightMultiply = false);
	void nacrtajGrid(CDC* pDC);
	void ucitajPuzle(CDC* pDC);
	void nacrtajDeo(CDC* pDC, DImage& puzla, bool jeSiva);
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
	virtual ~CIND18561Lab3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in IND_18561_Lab3View.cpp
inline CIND18561Lab3Doc* CIND18561Lab3View::GetDocument() const
   { return reinterpret_cast<CIND18561Lab3Doc*>(m_pDocument); }
#endif

