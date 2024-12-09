
// 2023RobotView.cpp : implementation of the CMy2023RobotView class
//

#include "pch.h"
#include "framework.h"
#include <math.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "2023Robot.h"
#endif

#include "2023RobotDoc.h"
#include "2023RobotView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2023RobotView

IMPLEMENT_DYNCREATE(CMy2023RobotView, CView)

BEGIN_MESSAGE_MAP(CMy2023RobotView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMy2023RobotView construction/destruction

CMy2023RobotView::CMy2023RobotView() noexcept
{
	// TODO: add construction code here

	glava.Load(CString("res/glava.png"));
	nadkolenica.Load(CString("res/nadkolenica.png"));
	nadlaktica.Load(CString("res/nadlaktica.png"));
	podkolenica.Load(CString("res/podkolenica.png"));
	podlaktica.Load(CString("res/podlaktica.png"));
	saka.Load(CString("res/saka.png"));
	stopalo.Load(CString("res/stopalo.png"));
	telo.Load(CString("res/telo.png"));
	pozadina.Load(CString("res/pozadina.jpg"));
	kurcina.Load(CString("res/dick.png"));

	rotNadlaktica = 0;
	rotPodlaktica = 0;
	rotSaka = 0;
	rotCeoRobot = 0;
	rotDick = 0;
	skaliranje = 1;
}

CMy2023RobotView::~CMy2023RobotView()
{
	
}

BOOL CMy2023RobotView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy2023RobotView drawing

float CMy2023RobotView::degreesToRadians(float degAngle)
{
	return degAngle * 3.1415 / 180;
}

void CMy2023RobotView::DrawBackground(CDC* pDC)
{
	int w = pozadina.Width();
	int h = pozadina.Height();

	pozadina.Draw(pDC, CRect(0, 0, w, h), CRect(0, 0, w, h));
}

void CMy2023RobotView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	pImage->DrawTransparent(pDC, pImage);
}

void CMy2023RobotView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat = { 1,0,0,1,dX,dY };
	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CMy2023RobotView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float radAngle = degreesToRadians(angle);
	XFORM mat = { cos(radAngle),sin(radAngle),-sin(radAngle),cos(radAngle),0,0 };
	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CMy2023RobotView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM mat = { sX,0,0,sY,0,0 };
	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CMy2023RobotView::DrawRobot(CDC* pDC)
{
	XFORM oldTransform;
	pDC->GetWorldTransform(&oldTransform);


	Translate(pDC, pozadina.Width() / 2, pozadina.Height() / 2, false);
	Rotate(pDC, rotCeoRobot, false);
	Scale(pDC, skaliranje, skaliranje, false);
	Translate(pDC, -pozadina.Width() / 2, -pozadina.Height() / 2, false);
	Translate(pDC, pozadina.Width() / 2 - telo.Width(), pozadina.Height() / 2 - telo.Height() - 20, false);
	DrawHalf(pDC);
	pDC->SetWorldTransform(&oldTransform);

	Translate(pDC, pozadina.Width() / 2, pozadina.Height() / 2, false);
	Rotate(pDC, rotCeoRobot, false);
	Scale(pDC, skaliranje, skaliranje, false);
	Translate(pDC, -pozadina.Width() / 2, -pozadina.Height() / 2, false);
	Translate(pDC, pozadina.Width() / 2 + telo.Width(), pozadina.Height() / 2 - telo.Height() - 20, false);
	Scale(pDC, -1, 1, false);
	DrawHalf(pDC);
	pDC->SetWorldTransform(&oldTransform);

	Translate(pDC, pozadina.Width() / 2, pozadina.Height() / 2, false);
	Rotate(pDC, rotCeoRobot, false);
	Scale(pDC, skaliranje, skaliranje, false);
	Translate(pDC, -pozadina.Width() / 2, -pozadina.Height() / 2, false);
	Translate(pDC, pozadina.Width() / 2 - glava.Width() / 2, pozadina.Height() / 2 - telo.Height() - glava.Height(), false);
	DrawHead(pDC);
	pDC->SetWorldTransform(&oldTransform);

	Translate(pDC, pozadina.Width() / 2, pozadina.Height() / 2, false);
	Rotate(pDC, rotCeoRobot, false);
	Scale(pDC, skaliranje, skaliranje, false);
	Translate(pDC, -pozadina.Width() / 2, -pozadina.Height() / 2, false);
	Translate(pDC, pozadina.Width() / 2, pozadina.Height() / 2 - 50, false);
	Rotate(pDC, rotDick, false);
	Translate(pDC, -pozadina.Width() / 2, -pozadina.Height() / 2 + 50, false);
	Translate(pDC, pozadina.Width() / 2 - 50, pozadina.Height() / 2 - 50, false);
	Scale(pDC, 0.2, 0.5, false);
	DrawDick(pDC);
	pDC->SetWorldTransform(&oldTransform);
}

void CMy2023RobotView::DrawHalf(CDC* pDC)
{
	XFORM oldTransform;
	pDC->GetWorldTransform(&oldTransform);

	// Telo
	DrawImgTransparent(pDC, &telo);

	// Nadlaktica
	Translate(pDC, -10, 30, false);
	Translate(pDC, 35, 35, false);
	Rotate(pDC, rotNadlaktica, false);
	Translate(pDC, -35, -35, false);
	DrawImgTransparent(pDC, &nadlaktica);

	// Podlaktica
	Translate(pDC, -8, 134, false);
	Translate(pDC, 30, 33, false);
	Rotate(pDC, rotPodlaktica, false);
	Translate(pDC, -30, -33, false);
	DrawImgTransparent(pDC, &podlaktica);

	// Saka
	Translate(pDC, 5, 137, false);
	Translate(pDC, 25, 3, false);
	Rotate(pDC, rotSaka, false);
	Translate(pDC, -25, -3, false);
	DrawImgTransparent(pDC, &saka);

	// Nadkolenica
	pDC->SetWorldTransform(&oldTransform);
	Translate(pDC, 32, 242, false);
	DrawImgTransparent(pDC, &nadkolenica);
	
	// Podkolenica
	Translate(pDC, 5, 147, false);
	DrawImgTransparent(pDC, &podkolenica);

	// Stopalo
	Translate(pDC, 5, 232, false);
	DrawImgTransparent(pDC, &stopalo);

	pDC->SetWorldTransform(&oldTransform);
}

void CMy2023RobotView::DrawHead(CDC* pDC)
{
	DrawImgTransparent(pDC, &glava);
}

void CMy2023RobotView::DrawDick(CDC* pDC)
{
	DrawImgTransparent(pDC, &kurcina);
}

void CMy2023RobotView::OnDraw(CDC* pDC)
{
	CMy2023RobotDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC.SelectObject(bmp);
	int oldMode = memDC.SetGraphicsMode(GM_ADVANCED);

	DrawBackground(&memDC);
	DrawRobot(&memDC);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SetGraphicsMode(oldMode);
	memDC.DeleteDC();
}


// CMy2023RobotView printing

BOOL CMy2023RobotView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy2023RobotView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy2023RobotView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMy2023RobotView diagnostics

#ifdef _DEBUG
void CMy2023RobotView::AssertValid() const
{
	CView::AssertValid();
}

void CMy2023RobotView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2023RobotDoc* CMy2023RobotView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2023RobotDoc)));
	return (CMy2023RobotDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy2023RobotView message handlers


BOOL CMy2023RobotView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CMy2023RobotView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
	case ('A'):
		if (rotSaka < 30)
			rotSaka += 10;
		break;
	case ('S'):
		if (rotSaka > -10)
			rotSaka -= 10;
		break;
	case ('D'):
		if (rotPodlaktica < 80)
			rotPodlaktica += 10;
		break;
	case ('F'):
		if (rotPodlaktica > - 10)
			rotPodlaktica -= 10;
		break;
	case ('G'):
		if (rotNadlaktica < 90)
			rotNadlaktica += 10;
		break;
	case ('H'):
		if (rotNadlaktica > -10)
			rotNadlaktica -= 10;
		break;
	case ('1'):
		rotCeoRobot -= 20;
		if (skaliranje > 0.2)
			skaliranje -= 0.05;
		break;
	case ('2'):
		rotCeoRobot += 20;
		if (skaliranje < 1.8)
			skaliranje += 0.05;
		break;
	case (VK_LEFT):
		rotDick -= 10;
		break;
	case (VK_RIGHT):
		rotDick += 10;
		break;
	}

	Invalidate();
}
