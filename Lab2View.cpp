
// Lab2View.cpp : implementation of the CLab2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab2.h"
#endif

#include "Lab2Doc.h"
#include "Lab2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab2View

IMPLEMENT_DYNCREATE(CLab2View, CView)

BEGIN_MESSAGE_MAP(CLab2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLab2View construction/destruction

CLab2View::CLab2View() noexcept
{
	grid = false;
	dgr = GetEnhMetaFileW(CString("cactus_part.emf"));
	lgr = GetEnhMetaFileW(CString("cactus_part_light.emf"));
	ang1 = 0;
	ang2 = 0;
}

CLab2View::~CLab2View()
{
	DeleteEnhMetaFile(dgr);
	DeleteEnhMetaFile(lgr);
}

BOOL CLab2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CLab2View::DrawGrid(CDC* pDC)
{
	LOGBRUSH logBrush2;
	logBrush2.lbStyle = BS_SOLID;
	logBrush2.lbColor = RGB(208, 206, 206);
	CPen* pPen2 = new CPen(PS_GEOMETRIC | PS_SOLID |
		PS_ENDCAP_ROUND | PS_JOIN_ROUND, 2, &logBrush2);
	CPen* oldPen2 = pDC->SelectObject(pPen2);

	for (int i = 0;i <= 500;i += 25)
	{
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 500);
	}
	for (int i = 0;i <= 500;i += 25)
	{
		pDC->MoveTo(0, i);
		pDC->LineTo(500, i);
	}

	pDC->SelectObject(oldPen2);
	delete pPen2;

	return;
}

// CLab2View drawing

void CLab2View::OnDraw(CDC* pDC)
{
	CLab2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	DrawBackgroud(pDC, rect);

	/*XFORM transOld;
	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	pDC->GetWorldTransform(&transOld);
	CRect* rect1 = new CRect(0, 0, 50, 50);
	//Scale(pDC, 0.6, 1, false);
	//Translate(pDC, 0, , true);
	Rotate(pDC, 30, false);
	Scale(pDC, 0.6, 1, false);
	//Translate(pDC, 0, 350, true);
	DrawDarkC(pDC, rect1);

	pDC->SetWorldTransform(&transOld);
	pDC->SetGraphicsMode(prevMode);*/

	

	
	DrawBase(pDC);
	DrawPot(pDC);
	if (grid)
	{
		DrawGrid(pDC);
	}
}

void CLab2View::DrawBase(CDC* pDC)
{
	XFORM transOld;
	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	pDC->GetWorldTransform(&transOld);
	

	Translate(pDC,250 ,385 , false);
	Rotate(pDC, 0, false);
	Scale(pDC, 1.1, 1, false);
	DrawLightC(pDC);
	DefaultTransformation(pDC);

	Translate(pDC, 250, 315, false);
	Rotate(pDC, 0, false);
	Scale(pDC, 0.35, 1.1, false);
	DrawDarkC(pDC);//sredina
	DefaultTransformation(pDC);

	/*Translate(pDC, 225, 325, false);
	Rotate(pDC, -45, false);
	Scale(pDC, 0.35, 1.1, false);
	DrawDarkC(pDC);// leva grana
	DefaultTransformation(pDC);*/

	Rotate(pDC, ang1, false);
	Translate(pDC, 275, 325, false);
	Rotate(pDC, 45, false);
	Scale(pDC, 0.35, 1.1, false);
	DrawDarkC(pDC);// desna
	DefaultTransformation(pDC);

	Translate(pDC, 250, 350, false);
	DrawCircle(pDC);
	DefaultTransformation(pDC);

	Translate(pDC, 250, 420, false);
	DrawCircle(pDC);
	DefaultTransformation(pDC);

	pDC->SetWorldTransform(&transOld);
	pDC->SetGraphicsMode(prevMode);


}

void CLab2View::DrawBackgroud(CDC* pDC, CRect& rect) {

	CPen* pen= new CPen(PS_SOLID, 5, RGB(135, 206, 235));
	CPen* oldPen=pDC->SelectObject(pen);
	CBrush* brush= new CBrush(RGB(135, 206, 235));
	CBrush* oldBrush=pDC->SelectObject(brush);

	pDC->Rectangle(0, 0, 500, 500);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	delete(pen);
	delete(brush);
}

void CLab2View::DrawPot(CDC* pDC) {

	CPen* pen=new CPen(PS_SOLID, 1, RGB(25, 25, 25));
	CPen* oldPen = pDC->SelectObject(pen);
	CBrush* brush= new CBrush(RGB(222, 148, 0));
	CBrush* oldBrush = pDC->SelectObject(brush);

	CPoint Pt[] = { CPoint(200, 450), CPoint(300, 450), CPoint(290, 500), CPoint(210, 500) };

	pDC->Rectangle(190, 427, 310, 450);
	pDC->Polygon(Pt, 4);
	
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	delete(pen);
	delete(brush);

}

void CLab2View::DrawCircle(CDC* pDC)
{
	CPen* pen = new CPen(PS_SOLID, 1, RGB(25, 25, 25));
	CPen* oldPen = pDC->SelectObject(pen);
	CBrush* brushC = new CBrush(RGB(0, 220, 0));
	CBrush* oldBrush = pDC->SelectObject(brushC);
	pDC->Ellipse(-10, -10, 10, 10);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	delete(pen);
	delete(brushC);
}

void CLab2View::DrawDarkC(CDC* pDC)
{
	CRect* rect = new CRect(-25, -34, 25, 34);
	pDC->PlayMetaFile(dgr, rect);
}

void CLab2View::DrawLightC(CDC* pDC)
{
	CRect* rect = new CRect(-25, -34, 25, 34);
	pDC->PlayMetaFile(lgr, rect);
}

void CLab2View::WriteText(CDC* pDC) 
{

}

void CLab2View::Translate(CDC* pDC, float x, float y, bool rightMul)
{
	XFORM f;
	f.eM11 = 1;
	f.eM12 = 0;
	f.eM21 = 0;
	f.eM22 = 1;
	f.eDx = x;
	f.eDy = y;

	if (rightMul)
		pDC->ModifyWorldTransform(&f, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&f, MWT_LEFTMULTIPLY);
}

void CLab2View::Rotate(CDC* pDC, float angle, bool rightMul)
{
	float pi = 3.14159;
	XFORM f;
	f.eM11 = cos(angle * pi / 180);
	f.eM12 = sin(angle * pi / 180);
	f.eM21 = -sin(angle * pi / 180);
	f.eM22 = cos(angle * pi / 180);
	f.eDx = 0;
	f.eDy = 0;

	if (rightMul)
		pDC->ModifyWorldTransform(&f, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&f, MWT_LEFTMULTIPLY);
}

void CLab2View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply) {

	XFORM transformacija;
	///*
	transformacija.eM11 = sX;
	transformacija.eM12 = 0;
	transformacija.eM21 = 0;
	transformacija.eM22 = sY;
	//*/
	transformacija.eDx = 0;
	transformacija.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformacija, MWT_RIGHTMULTIPLY);
	else pDC->ModifyWorldTransform(&transformacija, MWT_LEFTMULTIPLY);
}

void CLab2View::DefaultTransformation(CDC* pDC)
{
	XFORM f;
	f.eM11 = 1;
	f.eM12 = 0;
	f.eM21 = 0;
	f.eM22 = 1;
	f.eDx = 0;
	f.eDy = 0;
	pDC->SetWorldTransform(&f);
}


// CLab2View printing

BOOL CLab2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab2View diagnostics

#ifdef _DEBUG
void CLab2View::AssertValid() const
{
	CView::AssertValid();
}

void CLab2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab2Doc* CLab2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab2Doc)));
	return (CLab2Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab2View message handlers


void CLab2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G' || nChar == 'g')
	{
		grid = !grid;
		Invalidate();
	}

	if (nChar == 'H' || nChar == 'h')  //ceo levo
	{
		ang1 -= angleStep;
		Invalidate();
	}
	else if (nChar == 'J' || nChar == 'j') //ceo desno
	{
		ang1 += angleStep;
		Invalidate();
	}

	else if (nChar == 'K' || nChar == 'k')	//mali u levo
	{
		ang2 -= angleStep;
		Invalidate();
	}
	else if (nChar == 'L' || nChar == 'l')		//mali u desno
	{
		ang2 += angleStep;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
