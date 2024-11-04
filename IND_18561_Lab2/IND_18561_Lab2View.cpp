
// IND_18561_Lab2View.cpp : implementation of the CIND18561Lab2View class
//

#include "pch.h"
#include "framework.h"
#include <math.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_18561_Lab2.h"
#endif

#include "IND_18561_Lab2Doc.h"
#include "IND_18561_Lab2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CIND18561Lab2View

IMPLEMENT_DYNCREATE(CIND18561Lab2View, CView)

BEGIN_MESSAGE_MAP(CIND18561Lab2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND18561Lab2View construction/destruction

CIND18561Lab2View::CIND18561Lab2View() noexcept
{
	// TODO: add construction code here

	nacrtanGrid = false;
	velikiUgao = 0;
	maliUgao = 0;
	CString tamni("D:\\cactus_part.emf");
	CString svetli("D:\\cactus_part_light.emf");
	tamniKaktus = GetEnhMetaFile(tamni);
	svetliKaktus = GetEnhMetaFile(svetli);
}

CIND18561Lab2View::~CIND18561Lab2View()
{
	DeleteEnhMetaFile(tamniKaktus);
	DeleteEnhMetaFile(svetliKaktus);
}

BOOL CIND18561Lab2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND18561Lab2View drawing

void CIND18561Lab2View::nacrtajGrid(CDC* pDC)
{
	CPen* olovka;
	olovka = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
	nacrtanGrid = true;
	CPen* staraOlovka = pDC->SelectObject(olovka);
	for (int i = 1; i < 20; i++)
	{
		pDC->MoveTo(i * podeok, 0);
		pDC->LineTo(i * podeok, 500);
		pDC->MoveTo(0, i * podeok);
		pDC->LineTo(500, i * podeok);
	}
	pDC->SelectObject(staraOlovka);
	delete olovka;
}

void CIND18561Lab2View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat = { 1,0,0,1,dX,dY };
	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CIND18561Lab2View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM mat = { sX,0,0,sY,0,0 };
	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CIND18561Lab2View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM mat = { cos(angle),sin(angle),-sin(angle),cos(angle),0,0 };
	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CIND18561Lab2View::DrawFigure(CDC* pDC)
{
	nacrtajKaktus(pDC);
	nacrtajSaksiju(pDC);
}

void CIND18561Lab2View::nacrtajSaksiju(CDC* pDC)
{
	CBrush saksija(RGB(222, 148, 0));
	CBrush* staraCetka = pDC->SelectObject(&saksija);
	pDC->Rectangle(7.6 * podeok, 2 * podeok, 12.4 * podeok, 2.9 * podeok);
	POINT temena[] = {
		{8.3 * podeok,0 * podeok},
		{11.7 * podeok,0 * podeok},
		{12 * podeok,2 * podeok},
		{8 * podeok,2 * podeok},
	};
	pDC->Polygon(temena, 4);
	pDC->SelectObject(staraCetka);
}

void CIND18561Lab2View::nacrtajKaktus(CDC* pDC)
{
	CBrush tamnoZelena(RGB(0, 204, 0));
	CBrush* staraCetka = pDC->SelectObject(&tamnoZelena);

	pDC->Ellipse(9.6 * podeok, 2.6 * podeok, 10.4 * podeok, 3.4 * podeok); // Centralni kruzic oko koga se vrsi velika rotacija

	XFORM prethodnaTransformacija;
	pDC->GetWorldTransform(&prethodnaTransformacija);
	Translate(pDC, 10 * podeok, 3 * podeok, false);
	Rotate(pDC, velikiUgao, false);
	Translate(pDC, -10 * podeok, -3 * podeok, false);

	nacrtajKrugove(pDC);
	nacrtajBodlje(pDC);

	pDC->SetWorldTransform(&prethodnaTransformacija);
}

void CIND18561Lab2View::nacrtajKrugove(CDC* pDC)
{
	CBrush tamnoZelena(RGB(0, 204, 0));
	CBrush* staraCetka = pDC->SelectObject(&tamnoZelena);

	pDC->Ellipse(9.6 * podeok, 5.6 * podeok, 10.4 * podeok, 6.4 * podeok);
	pDC->Ellipse(9.6 * podeok, 8.6 * podeok, 10.4 * podeok, 9.4 * podeok);
	pDC->Ellipse(9.6 * podeok, 11.6 * podeok, 10.4 * podeok, 12.4 * podeok);
	pDC->Ellipse(7.5 * podeok, 10.7 * podeok, 8.3 * podeok, 11.5 * podeok);

	pDC->SelectObject(&staraCetka);
}

void CIND18561Lab2View::nacrtajBodlje(CDC* pDC)
{	
	XFORM prethodnaTransformacija;
	pDC->GetWorldTransform(&prethodnaTransformacija);

	Translate(pDC, 10 * podeok, 3 * podeok, false);
	Scale(pDC, 1.2, 1, false);
	PlayEnhMetaFile(pDC->GetSafeHdc(), svetliKaktus, CRect(-1 * podeok, 0 * podeok, 1 * podeok, 3 * podeok));
	pDC->SetWorldTransform(&prethodnaTransformacija);

	Translate(pDC, 10 * podeok, 3 * podeok, false);
	Scale(pDC, 1.2, 1, false);
	PlayEnhMetaFile(pDC->GetSafeHdc(), tamniKaktus, CRect(-1 * podeok, 3 * podeok, 1 * podeok, 6 * podeok));
	pDC->SetWorldTransform(&prethodnaTransformacija);

	Translate(pDC, 10 * podeok, 9 * podeok, false);
	Rotate(pDC, stepeniURadijane(45), false);
	Scale(pDC, 0.4, 1, false);
	PlayEnhMetaFile(pDC->GetSafeHdc(), tamniKaktus, CRect(-1 * podeok, 0 * podeok, 1 * podeok, 3 * podeok));
	pDC->SetWorldTransform(&prethodnaTransformacija);

	Translate(pDC, 10 * podeok, 3 * podeok, false);
	Scale(pDC, 0.4, 1, false);
	PlayEnhMetaFile(pDC->GetSafeHdc(), tamniKaktus, CRect(-1 * podeok, 6 * podeok, 1 * podeok, 9 * podeok));
	pDC->SetWorldTransform(&prethodnaTransformacija);

	Translate(pDC, 10 * podeok, 9 * podeok, false);
	Rotate(pDC, stepeniURadijane(-45) + maliUgao, false);
	Scale(pDC, 0.4, 1, false);
	PlayEnhMetaFile(pDC->GetSafeHdc(), svetliKaktus, CRect(-1 * podeok, 0 * podeok, 1 * podeok, 3 * podeok));
	pDC->SetWorldTransform(&prethodnaTransformacija);

	Translate(pDC, 10 * podeok, 12 * podeok, false);
	Rotate(pDC, stepeniURadijane(45), false);
	Scale(pDC, 0.4, 1, false);
	PlayEnhMetaFile(pDC->GetSafeHdc(), tamniKaktus, CRect(-1 * podeok, 0 * podeok, 1 * podeok, 3 * podeok));
	pDC->SetWorldTransform(&prethodnaTransformacija);

	Translate(pDC, 10 * podeok, 12 * podeok, false);
	Rotate(pDC, stepeniURadijane(-45), false);
	Scale(pDC, 0.4, 1, false);
	PlayEnhMetaFile(pDC->GetSafeHdc(), tamniKaktus, CRect(-1 * podeok, 0 * podeok, 1 * podeok, 3 * podeok));
	pDC->SetWorldTransform(&prethodnaTransformacija);

	Translate(pDC, 10 * podeok, 3 * podeok, false);
	Scale(pDC, 0.4, 1, false);
	PlayEnhMetaFile(pDC->GetSafeHdc(), tamniKaktus, CRect(-1 * podeok, 9 * podeok, 1 * podeok, 12 * podeok));
	pDC->SetWorldTransform(&prethodnaTransformacija);

	Translate(pDC, 7.9 * podeok, 11.1 * podeok, false);
	Scale(pDC, 0.4, 1, false);
	PlayEnhMetaFile(pDC->GetSafeHdc(), tamniKaktus, CRect(-1 * podeok, 0 * podeok, 1 * podeok, 3 * podeok));
	pDC->SetWorldTransform(&prethodnaTransformacija);

	Translate(pDC, 7.9 * podeok, 11.1 * podeok, false);
	Rotate(pDC, stepeniURadijane(45), false);
	Scale(pDC, 0.4, 1, false);
	PlayEnhMetaFile(pDC->GetSafeHdc(), tamniKaktus, CRect(-1 * podeok, 0 * podeok, 1 * podeok, 3 * podeok));
	pDC->SetWorldTransform(&prethodnaTransformacija);

	Translate(pDC, 7.9 * podeok, 11.1 * podeok, false);
	Rotate(pDC, stepeniURadijane(90), false);
	Scale(pDC, 0.4, 1, false);
	PlayEnhMetaFile(pDC->GetSafeHdc(), tamniKaktus, CRect(-1 * podeok, 0 * podeok, 1 * podeok, 3 * podeok));
	pDC->SetWorldTransform(&prethodnaTransformacija);
}

float CIND18561Lab2View::stepeniURadijane(float s)
{
	return s * (3.14 / 180);
}

void CIND18561Lab2View::OnDraw(CDC* pDC)
{
	CIND18561Lab2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect deoZaCrtanje(0, 0, 500, 500);
	CRgn region;
	region.CreateRectRgnIndirect(&deoZaCrtanje);
	pDC->SelectClipRgn(&region);
	CBrush pozadina(RGB(135, 206, 235));
	CBrush* staraCetka = pDC->SelectObject(&pozadina);
	pDC->Rectangle(0, 0, 500, 500);
	pDC->SelectObject(staraCetka);
	if (nacrtanGrid)
		nacrtajGrid(pDC);

	int stariMod = pDC->SetGraphicsMode(GM_ADVANCED);
	DrawFigure(pDC);
	pDC->SetGraphicsMode(stariMod);
	pDC->SelectClipRgn(nullptr);
}


// CIND18561Lab2View printing

BOOL CIND18561Lab2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND18561Lab2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND18561Lab2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND18561Lab2View diagnostics

#ifdef _DEBUG
void CIND18561Lab2View::AssertValid() const
{
	CView::AssertValid();
}

void CIND18561Lab2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND18561Lab2Doc* CIND18561Lab2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND18561Lab2Doc)));
	return (CIND18561Lab2Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND18561Lab2View message handlers


void CIND18561Lab2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
		case VK_SPACE:
		{
			if (!nacrtanGrid)
				nacrtajGrid(GetDC());
			else
			{
				nacrtanGrid = false;
				Invalidate();
			}
			break;
		}
		case VK_LEFT:
		case 'A':
		{
			(nChar == VK_LEFT ? velikiUgao : maliUgao) -= stepeniURadijane(30);
			break;
		}
		case VK_RIGHT:
		case 'D':
		{
			(nChar == VK_RIGHT ? velikiUgao : maliUgao) += stepeniURadijane(30);
			break;
		}
	}

	Invalidate();
}
