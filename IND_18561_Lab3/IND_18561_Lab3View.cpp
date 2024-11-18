
// IND_18561_Lab3View.cpp : implementation of the CIND18561Lab3View class
//

#include "pch.h"
#include "framework.h"
#include <math.h>
#include <string>
using namespace std;
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_18561_Lab3.h"
#endif

#include "IND_18561_Lab3Doc.h"
#include "IND_18561_Lab3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIND18561Lab3View

IMPLEMENT_DYNCREATE(CIND18561Lab3View, CView)

BEGIN_MESSAGE_MAP(CIND18561Lab3View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CIND18561Lab3View construction/destruction

CIND18561Lab3View::CIND18561Lab3View() noexcept
{
	// TODO: add construction code here
	
	for (int i = 0; i < 9; i++)
	{
		CString putanja = CString(string("res/" + to_string(i + 1) + ".bmp").c_str());
		delovi[i].slika.Load(putanja);
	}
}

CIND18561Lab3View::~CIND18561Lab3View()
{
}

BOOL CIND18561Lab3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND18561Lab3View drawing

void CIND18561Lab3View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM transformacija = { 1,0,0,1,dX,dY };
	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformacija, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transformacija, MWT_LEFTMULTIPLY);
}

void CIND18561Lab3View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM transformacija = { sX,0,0,sY,0,0 };
	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformacija, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transformacija, MWT_LEFTMULTIPLY);
}

void CIND18561Lab3View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float ugaoURadijanima = angle * (3.1415 / 180);
	XFORM transformacija = { cos(ugaoURadijanima),sin(ugaoURadijanima),-sin(ugaoURadijanima),cos(ugaoURadijanima),0,0};
	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformacija, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transformacija, MWT_LEFTMULTIPLY);
}

void CIND18561Lab3View::Mirror(CDC* pDC, bool mX, bool mY, bool rightMultiply)
{
	int x = mX ? -1 : 1;
	int y = mY ? -1 : 1;
	XFORM transformacija = { x,0,0,y,0,0 };
	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformacija, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transformacija, MWT_LEFTMULTIPLY);
}

void CIND18561Lab3View::nacrtajGrid(CDC* pDC)
{
	CPen olovka(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* staraOlovka = pDC->SelectObject(&olovka);
	for (int i = 0; i <= 20; i++)
	{
		pDC->MoveTo(i * podeok, 0);
		pDC->LineTo(i * podeok, 500);
		pDC->MoveTo(0, i * podeok);
		pDC->LineTo(500, i * podeok);
	}
	pDC->SelectObject(staraOlovka);
}

void CIND18561Lab3View::ucitajPuzle(CDC* pDC)
{
	XFORM staraTransformacija;

	for (int i = 0; i < 9; i++)
	{
		float sirina = delovi[i].slika.Width();
		float visina = delovi[i].slika.Height();
		pDC->GetWorldTransform(&staraTransformacija);

		Translate(pDC, delovi[i].refX ? sirina / 2 : 0, delovi[i].refY ? visina / 2 : 0);
		Mirror(pDC, delovi[i].refX, delovi[i].refY);
		Translate(pDC, delovi[i].refX ? -sirina / 2 : 0, -delovi[i].refY ? -visina / 2 : 0);

		Translate(pDC, delovi[i].transX, delovi[i].transY);

		Translate(pDC, sirina / 2, visina / 2);
		Rotate(pDC, delovi[i].ugaoRotacije);
		Translate(pDC, -sirina / 2, -visina / 2);
		nacrtajDeo(pDC, delovi[i].slika, delovi[i].siva);

		pDC->SetWorldTransform(&staraTransformacija);
	}
}

void CIND18561Lab3View::nacrtajDeo(CDC* pDC, DImage& puzla, bool jeSiva)
{
	// Pravimo DC-jeve za ucitanu sliku i za masku
	CDC* srcDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);
	CDC* maskaDC = new CDC();
	maskaDC->CreateCompatibleDC(pDC);

	// Bitmapa za sliku koju ucitavamo
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, puzla.Width(), puzla.Height());
	bmp.SetBitmapBits(puzla.Width() * puzla.Height() * puzla.BPP(), puzla.GetDIBBits());
	// Bitmapa za masku
	CBitmap maska;
	maska.CreateBitmap(puzla.Width(), puzla.Height(), 1, 1, NULL);
	CBitmap* staraBmpSRC = srcDC->SelectObject(&bmp);
	CBitmap* staraBmpMaska = maskaDC->SelectObject(&maska);

	// Kopiranje konture iz slike u masku, bez green screen-a
	COLORREF pozadinskiPiksel = srcDC->GetPixel(0, 0);
	COLORREF bkBoja = srcDC->SetBkColor(pozadinskiPiksel);
	maskaDC->BitBlt(0, 0, puzla.Width(), puzla.Height(), srcDC, 0, 0, SRCCOPY);

	long ukupnoBitova = puzla.Width() * puzla.Height() * puzla.BPP();
	BYTE* nizBitova = new BYTE[ukupnoBitova];
	GetBitmapBits(bmp, ukupnoBitova, nizBitova);
	for (int y = 0; y < puzla.Height(); y++)
	{
		for (int x = 0; x < puzla.Width(); x++)
		{
			long indeks = (y * puzla.Width() * puzla.BPP()) + (x * 4);

			if (jeSiva)
			{
				BYTE b = nizBitova[indeks];
				BYTE g = nizBitova[indeks + 1];
				BYTE r = nizBitova[indeks + 2];
				BYTE a = nizBitova[indeks + 3];
				BYTE gr = (BYTE)(64 + (r + g + b) / 3);

				if (gr > 255)
					gr = 255;
				nizBitova[indeks] = gr;
				nizBitova[indeks + 1] = gr;
				nizBitova[indeks + 2] = gr;
				nizBitova[indeks + 3] = gr;
			}
			else
			{
				nizBitova[indeks] = 0;
				nizBitova[indeks + 1] = 0;
				nizBitova[indeks + 2] += 15;
				nizBitova[indeks + 3] = 0;
			}
		}
	}
	bmp.SetBitmapBits(puzla.Width() * puzla.Height() * puzla.BPP(), nizBitova);
	delete[] nizBitova;

	// Izdvajanje samo konture iz slike pomocu AND operacije, pozadina postaje crna
	COLORREF textBoja = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->BitBlt(0, 0, puzla.Width(), puzla.Height(), maskaDC, 0, 0, SRCAND);

	srcDC->SetTextColor(textBoja);
	srcDC->SetBkColor(bkBoja);
	srcDC->SelectObject(staraBmpSRC);
	maskaDC->SelectObject(staraBmpMaska);
	srcDC->DeleteDC();
	maskaDC->DeleteDC();

	// Kreiranje transparentne maske
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap* staraMapa = memDC->SelectObject(&maska);
	pDC->BitBlt(0, 0, puzla.Width(), puzla.Height(), memDC, 0, 0, SRCAND);

	// ILI operacija izmedju transparentne maske i bitmape sa izdvojenom konturom
	memDC->SelectObject(&bmp);
	pDC->BitBlt(0, 0, puzla.Width(), puzla.Height(), memDC, 0, 0, SRCPAINT);
	
	memDC->SelectObject(staraMapa);
	memDC->DeleteDC();
}

void CIND18561Lab3View::OnDraw(CDC* pDC)
{
	CIND18561Lab3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, 520, 520);
	int stariMod = memDC->SetGraphicsMode(GM_ADVANCED);
	CBitmap* staraMapa = memDC->SelectObject(&bmp);
	memDC->FillSolidRect(0, 0, 520, 520, RGB(255, 255, 255));

	nacrtajGrid(memDC);
	ucitajPuzle(memDC);

	pDC->BitBlt(0, 0, 520, 520, memDC, 0, 0, SRCCOPY);
	memDC->SetGraphicsMode(stariMod);
	memDC->SelectObject(staraMapa);
	memDC->DeleteDC();
}


// CIND18561Lab3View printing

BOOL CIND18561Lab3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND18561Lab3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND18561Lab3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND18561Lab3View diagnostics

#ifdef _DEBUG
void CIND18561Lab3View::AssertValid() const
{
	CView::AssertValid();
}

void CIND18561Lab3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND18561Lab3Doc* CIND18561Lab3View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND18561Lab3Doc)));
	return (CIND18561Lab3Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND18561Lab3View message handlers
