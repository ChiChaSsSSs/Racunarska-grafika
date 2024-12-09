
// 2016BilijarView.cpp : implementation of the CMy2016BilijarView class
//

#include "pch.h"
#include "framework.h"
#include <math.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "2016Bilijar.h"
#endif

#include "2016BilijarDoc.h"
#include "2016BilijarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2016BilijarView

IMPLEMENT_DYNCREATE(CMy2016BilijarView, CView)

BEGIN_MESSAGE_MAP(CMy2016BilijarView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMy2016BilijarView construction/destruction

CMy2016BilijarView::CMy2016BilijarView() noexcept
{
	// TODO: add construction code here
	rotAngle = 0;
	stickPosition = 0;
	ballPosition = 0;
	background.Load(CString("res/felt2.jpg"));
	border.Load(CString("res/wood.jpg"));
}

CMy2016BilijarView::~CMy2016BilijarView()
{
}

BOOL CMy2016BilijarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy2016BilijarView drawing

float CMy2016BilijarView::degreesToRadians(float degAngle)
{
	return degAngle * 3.1415 / 180;
}

void CMy2016BilijarView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat = { 1,0,0,1,dX,dY };
	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CMy2016BilijarView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float radAngle = degreesToRadians(angle);
	XFORM mat = { cos(radAngle),sin(radAngle),-sin(radAngle),cos(radAngle),0,0 };
	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CMy2016BilijarView::DrawTable(CDC* pDC, CRect rect)
{
	int i, j;
	int w = rect.Width();
	int h = rect.Height();
	int wB = background.Width();
	int hB = background.Height();
	CRect rctImage(0, 0, wB, hB);

	for (i = 0; i < w - wB ; i += wB)
		for (j = 0; j < h - hB; j += hB)
			background.Draw(pDC, rctImage, CRect(rect.left + i, rect.top + j, rect.left + wB + i, rect.top + hB + j));
	for (i = 0; i < w - wB; i += wB)
		background.Draw(pDC, rctImage, CRect(rect.left + i, rect.top + h - hB, rect.left + wB + i, rect.top + h));
	for (i = 0; i < h - hB; i += hB)
		background.Draw(pDC, rctImage, CRect(rect.left + w - wB, rect.top + i, rect.left + w, rect.top + hB + i));
	background.Draw(pDC, rctImage, CRect(rect.left + w - wB, rect.top + h - hB, rect.left + w, rect.top + h));
}

void CMy2016BilijarView::DrawBorder(CDC* pDC, CRect rect, int w)
{
	CRect rctImage(0, 0, border.Width(), border.Height());
	border.Draw(pDC, rctImage, rect);

	CRect tableRect(rect.left + w, rect.top + w, rect.right - w, rect.bottom - w);
	DrawTable(pDC, tableRect);
	DrawHoles(pDC, rect, w);
}

void CMy2016BilijarView::DrawHoles(CDC* pDC, CRect rect, int size)
{
	CBrush newBrush(RGB(0, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&newBrush);

	int x1 = rect.left + size / 2;
	int x3 = rect.right - size - size / 2;
	int x2 = (x1 + x3) / 2;
	int y1 = rect.top + size / 2;
	int y2 = rect.bottom - size - size / 2;

	pDC->Ellipse(x1, y1, x1 + size, y1 + size);
	pDC->Ellipse(x2, y1, x2 + size, y1 + size);
	pDC->Ellipse(x3, y1, x3 + size, y1 + size);
	pDC->Ellipse(x1, y2, x1 + size, y2 + size);
	pDC->Ellipse(x2, y2, x2 + size, y2 + size);
	pDC->Ellipse(x3, y2, x3 + size, y2 + size);

	pDC->SelectObject(oldBrush);
}

void CMy2016BilijarView::DrawBall(CDC* pDC, int w)
{
	CBrush ballBrush(RGB(140, 0, 0));
	CBrush glareBrush(RGB(255, 255, 255));
	CPen borderPen(PS_SOLID, 2, RGB(80, 0, 0));

	CBrush* oldBrush = pDC->SelectObject(&ballBrush);
	CPen* oldPen = pDC->SelectObject(&borderPen);
	pDC->Ellipse(-w / 2, -w / 2, w / 2, w / 2);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(&glareBrush);
	Translate(pDC, 5, -5, false);
	int wg = w / 3;
	pDC->Ellipse(-wg / 2, -wg / 2, wg / 2, wg / 2);
	Translate(pDC, -5, 5, false);

	pDC->SelectObject(oldBrush);
	
}

void CMy2016BilijarView::DrawStick(CDC* pDC, int w)
{
	Translate(pDC, -w / 200, 15, false);
	CBrush ocher(RGB(220, 160, 100));
	CBrush brown(RGB(150, 75, 0));
	CPen newPen(PS_SOLID, 1, RGB(150, 75, 0));
	CPen whitePen(PS_SOLID, 1, RGB(255, 255, 255));

	CBrush* oldBrush = pDC->SelectObject(&ocher);
	CPen* oldPen = pDC->SelectObject(&newPen);
	pDC->MoveTo(0, 0);
	pDC->BeginPath();
	{
		pDC->LineTo(-w / 300, 2 * w / 3);
		pDC->LineTo((w / 100) + (w / 300), 2 * w / 3);
		pDC->LineTo(w / 100, 0);
		pDC->LineTo(0, 0);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(&brown);
	pDC->MoveTo(-w / 300, 2 * w / 3);
	pDC->BeginPath();
	{
		pDC->LineTo(-w / 200, w);
		pDC->LineTo((w / 100) + (w / 200), w);
		pDC->LineTo((w / 100) + (w / 300), 2 * w / 3);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(whitePen);
	pDC->MoveTo(2, 0);
	pDC->LineTo(-w / 200 + 2, w);

	pDC->SelectObject(oldPen);
	Translate(pDC, w / 200, -15, false);
}

void CMy2016BilijarView::OnDraw(CDC* pDC)
{
	CMy2016BilijarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	int oldMode = memDC.SetGraphicsMode(GM_ADVANCED);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC.SelectObject(bmp);

	DrawBorder(&memDC, rect, 30);

	Translate(&memDC, 600, 400, false);
	Rotate(&memDC, rotAngle, false);

	XFORM oldTransform;
	memDC.GetWorldTransform(&oldTransform);
	Translate(&memDC, 0, ballPosition, false);
	Rotate(&memDC, -rotAngle, false);
	DrawBall(&memDC, 30);
	
	memDC.SetWorldTransform(&oldTransform);
	Translate(&memDC, 0, stickPosition, false);
	DrawStick(&memDC, 600);
	Translate(&memDC, 0, -stickPosition, false);
	Rotate(&memDC, -rotAngle, false);
	Translate(&memDC, -600, -400, false);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SetGraphicsMode(oldMode);
	memDC.DeleteDC();
}


// CMy2016BilijarView printing

BOOL CMy2016BilijarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy2016BilijarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy2016BilijarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

// CMy2016BilijarView diagnostics

#ifdef _DEBUG
void CMy2016BilijarView::AssertValid() const
{
	CView::AssertValid();
}

void CMy2016BilijarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2016BilijarDoc* CMy2016BilijarView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2016BilijarDoc)));
	return (CMy2016BilijarDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy2016BilijarView message handlers


BOOL CMy2016BilijarView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CMy2016BilijarView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
	case VK_LEFT:
		rotAngle += 10;
		break;
	case VK_RIGHT:
		rotAngle -= 10;
		break;
	case VK_UP:
		if (!stickPosition)
			ballPosition -= 10;
		else
			stickPosition -= 10;
		break;
	case VK_DOWN:
		if (!ballPosition)
			stickPosition += 10;
		else
			ballPosition += 10;
		break;
	}

	Invalidate();
}
