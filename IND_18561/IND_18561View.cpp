
// IND_18561View.cpp : implementation of the CIND18561View class
//

#include "pch.h"
#include "framework.h"
#include <string>
#include <cmath>
using namespace std;
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_18561.h"
#endif

#include "IND_18561Doc.h"
#include "IND_18561View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define YELLOW RGB(255,255,0)
#define GRAY RGB(220,220,220)
#define PI 3.14

struct Trougao {
	string boja;
	int r;
	int g;
	int b;
	POINT koordinate[3];
	int mnogougao;
	int poluprecnik;
};

struct Cetvorougao {
	string boja;
	int r;
	int g;
	int b;
	POINT koordinate[4];
};

const int podeok = 25;
bool nacrtanGrid = false;

const struct Trougao trouglovi[] = {
	{"YELLOW", 255, 255, 0, {{podeok * 14.5, podeok * 7}, {podeok * 11.5, podeok * 10}, {podeok * 14.5, podeok * 13}}, 0, 0},
	{"RED", 255, 0, 0, {{podeok * 14.5, podeok * 1}, {podeok * 8.5, podeok * 7}, {podeok * 14.5, podeok * 7}}, 5, 25},
	{"PINK", 255, 192, 203, {{podeok * 8.5,podeok * 7}, {podeok * 5.5,podeok * 10}, {podeok * 9.5,podeok * 10}}, 7, 12},
	{"WHITE_BLUE", 0, 0, 255, {{podeok * 8.5,podeok * 10}, {podeok * 11.5,podeok * 10}, {podeok * 11.5,podeok * 13}}, 8, 12},
	{"ORANGE", 255, 165, 0, {{podeok * 8.5,podeok * 13}, {podeok * 14.5,podeok * 13}, {podeok * 14.5,podeok * 19}}, 4, 25},
};

const struct Cetvorougao cetvorouglovi[] = {
	{"PURPLE", 189, 48, 207, {{podeok * 5.5, podeok * 10}, {podeok * 8.5, podeok * 10}, {podeok * 11.5, podeok * 13}, {podeok * 8.5, podeok * 13}}},
	{"GREEN", 0, 200, 0, {{podeok * 8.5, podeok * 7}, {podeok * 11.5, podeok * 7}, {podeok * 11.5, podeok * 10}, {podeok * 8.5, podeok * 10}}},
};

// CIND18561View

IMPLEMENT_DYNCREATE(CIND18561View, CView)

BEGIN_MESSAGE_MAP(CIND18561View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND18561View construction/destruction

CIND18561View::CIND18561View() noexcept
{
	// TODO: add construction code here

}

CIND18561View::~CIND18561View()
{
}

BOOL CIND18561View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CIND18561View::nacrtajTrougao(CDC* pDC, Trougao t)
{
	CPen olovka(PS_SOLID, 5, YELLOW);
	CBrush* cetka;
	if (t.boja == "WHITE_BLUE")
		cetka = new CBrush(HS_VERTICAL, RGB(t.r, t.g, t.b));
	else
		cetka = new CBrush(RGB(t.r, t.g, t.b));
	CPen* staraOlovka = pDC->SelectObject(&olovka);
	CBrush* staraCetka = pDC->SelectObject(cetka);

	pDC->Polygon(t.koordinate, 3);

	pDC->SelectObject(staraOlovka);

	float cx = (t.koordinate[0].x + t.koordinate[1].x + t.koordinate[2].x) / 3;
	float cy = (t.koordinate[0].y + t.koordinate[1].y + t.koordinate[2].y) / 3;
	
	DrawRegularPolygon(pDC, cx, cy, t.poluprecnik, t.mnogougao, 0);
	pDC->SelectObject(staraCetka);
}

void CIND18561View::nacrtajCetvorougao(CDC* pDC, Cetvorougao c)
{
	CPen olovka(PS_SOLID, 5, YELLOW);
	CBrush cetka(RGB(c.r, c.g, c.b));
	CPen* staraOlovka = pDC->SelectObject(&olovka);
	CBrush* staraCetka = pDC->SelectObject(&cetka);

	pDC->Polygon(c.koordinate, 4);

	pDC->SelectObject(staraOlovka);
	pDC->SelectObject(staraCetka);
}

void CIND18561View::nacrtajGrid(CDC* pDC)
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

void CIND18561View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
	CPen olovka(PS_SOLID, 3, YELLOW);
	CPen* staraOlovka = pDC->SelectObject(&olovka);
	POINT* temena = new POINT[n];
	for (int i = 0; i < n; i++)
	{
		temena[i].x = cx + r * cos(2 * PI * i / n + (rotAngle * (PI / 180)));
		temena[i].y = cy + r * sin(2 * PI * i / n + (rotAngle * (PI / 180)));
	}
	pDC->Polygon(temena, n);
	pDC->SelectObject(staraOlovka);
	delete[] temena;
}

// CIND18561View drawing

void CIND18561View::OnDraw(CDC* pDC)
{
	CIND18561Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CBrush boja_pozadine(GRAY);
	CBrush* staraCetka = pDC->SelectObject(&boja_pozadine);
	pDC->Rectangle(0, 0, 500, 500);
	pDC->SelectObject(staraCetka);

	for (int i = 0; i < sizeof(trouglovi) / sizeof(trouglovi[0]); i++)
		nacrtajTrougao(pDC, trouglovi[i]);
	for (int i = 0; i < sizeof(cetvorouglovi) / sizeof(cetvorouglovi[0]); i++)
		nacrtajCetvorougao(pDC, cetvorouglovi[i]);
}


// CIND18561View printing

BOOL CIND18561View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND18561View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND18561View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND18561View diagnostics

#ifdef _DEBUG
void CIND18561View::AssertValid() const
{
	CView::AssertValid();
}

void CIND18561View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND18561Doc* CIND18561View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND18561Doc)));
	return (CIND18561Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND18561View message handlers


void CIND18561View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nChar == VK_SPACE && !nacrtanGrid)
		nacrtajGrid(GetDC());
	else
	{
		nacrtanGrid = false;
		Invalidate();
	}
}
