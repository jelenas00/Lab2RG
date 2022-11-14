
// GrafikaLabView.cpp : implementation of the CGrafikaLabView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GrafikaLab.h"
#endif

#include "GrafikaLabDoc.h"
#include "GrafikaLabView.h"

#define _USE_MATH_DEFINES
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGrafikaLabView

IMPLEMENT_DYNCREATE(CGrafikaLabView, CView)

BEGIN_MESSAGE_MAP(CGrafikaLabView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGrafikaLabView construction/destruction

CGrafikaLabView::CGrafikaLabView() noexcept
{

	this->partDark = GetEnhMetaFile(CString("cactus_part.emf"));
	this->partLight = GetEnhMetaFile(CString("cactus_part_light.emf"));

	this->angle1 = 0;
	this->angle2 = M_PI/2;

	this->gridOn = false;
	// TODO: add construction code here

}

CGrafikaLabView::~CGrafikaLabView()
{
}

BOOL CGrafikaLabView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void Translate(CDC* pDC, float dx, float dy, bool rightMultiply) {
	XFORM transformation;
	transformation.eM11 = 1;
	transformation.eM12 = 0;
	transformation.eM21 = 0;
	transformation.eM22 = 1;
	transformation.eDx = dx;
	transformation.eDy = dy;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformation, MWT_RIGHTMULTIPLY);
	else pDC->ModifyWorldTransform(&transformation, MWT_LEFTMULTIPLY);
}

void Scale(CDC* pDC, float dx, float dy, bool rightMultiply) {
	XFORM transformation;
	transformation.eM11 = dx;
	transformation.eM12 = 0;
	transformation.eM21 = 0;
	transformation.eM22 = dy;
	transformation.eDx = 0;
	transformation.eDy = 0;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformation, MWT_RIGHTMULTIPLY);
	else pDC->ModifyWorldTransform(&transformation, MWT_LEFTMULTIPLY);
}

void Rotate(CDC* pDC, float angle, bool rightMultiply) {
	XFORM transformation;
	transformation.eM11 = cos(angle);
	transformation.eM12 = sin(angle);
	transformation.eM21 = -sin(angle);
	transformation.eM22 = cos(angle);
	transformation.eDx = 0;
	transformation.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformation, MWT_RIGHTMULTIPLY);
	else pDC->ModifyWorldTransform(&transformation, MWT_LEFTMULTIPLY);
}

// CGrafikaLabView drawing

void CGrafikaLabView::OnDraw(CDC* pDC)
{
	CGrafikaLabDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	XFORM old;
	GetWorldTransform(pDC->m_hDC, &old);

	//background
	CBrush backgroundBrush(RGB(135, 206, 235));
	CBrush* oldBrush = pDC->SelectObject(&backgroundBrush);

	CPen backgroundPen(PS_SOLID, 0, RGB(221, 221, 221));
	CPen* oldPen = pDC->SelectObject(&backgroundPen);

	pDC->Rectangle(0, 0, 500, 500);

	//cactus
	CBrush cactusBrush(RGB(0, 204, 0));
	oldBrush = pDC->SelectObject(&cactusBrush);

	CPen cactusPen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen = pDC->SelectObject(&cactusPen);

	DrawCactus(pDC, old);
	//pot
	DrawPot(oldBrush, pDC, oldPen);

	//text
	DrawText(pDC);

	//grid
	DrawGrid(oldPen, pDC);

	// TODO: add draw code for native data here
}

void CGrafikaLabView::DrawCactus(CDC* pDC, XFORM& old)
{
	Translate(pDC, 250, 75, false);
	//Rotate(pDC, this->angle2, false);
	pDC->PlayMetaFile(this->partDark, CRect(-30, 0, 30, 75));

	DrawBranches(pDC);
	SetWorldTransform(pDC->m_hDC, &old);

}

void CGrafikaLabView::DrawBranches(CDC* pDC)
{
	pDC->Ellipse(10, 10, -10, -10);
	Translate(pDC, 0, 75, false);
	Rotate(pDC, M_PI/4+M_PI/2, false);
	pDC->PlayMetaFile(this->partDark, CRect(-20, 0, 20, -75));
	Rotate(pDC, 2*M_PI / 4, false);
	Rotate(pDC, angle1, false);
	pDC->PlayMetaFile(this->partLight, CRect(-20, 0, 20, -75));
	pDC->Ellipse(-10, -10, 10, 10);
	Translate(pDC, 0, -75, false);
	Rotate(pDC, M_PI / 4, false);
	pDC->PlayMetaFile(this->partDark, CRect(-12, 0, 12, -75));
	Translate(pDC, 0, -75, false);
	pDC->PlayMetaFile(this->partDark, CRect(-12, 0, 12, -75));
	Rotate(pDC, M_PI / 4, false);
	pDC->PlayMetaFile(this->partDark, CRect(-12, 0, 12, -75));
	Rotate(pDC, -M_PI / 2, false);
	pDC->PlayMetaFile(this->partDark, CRect(-12, 0, 12, -75));
	pDC->Ellipse(-10, -10, 10, 10);
	Rotate(pDC, M_PI / 4, false);
	Translate(pDC, 0, 75, false);

	Rotate(pDC, -M_PI / 2, false);
	pDC->PlayMetaFile(this->partDark, CRect(-12, 0, 12, -75));
	Translate(pDC, -75, 0, false);
	pDC->PlayMetaFile(this->partDark, CRect(30, 0, 30, -75));
	Translate(pDC, 75, 0, false);

	Rotate(pDC, M_PI / 4, false);
	pDC->PlayMetaFile(this->partDark, CRect(-12, 0, 12, -75));
	pDC->Ellipse(-10, -10, 10, 10);
	Translate(pDC, 0, -75, false);
	Rotate(pDC, angle2 - M_PI / 2, false);
	pDC->PlayMetaFile(this->partLight, CRect(-30, 0, 30, -75));
	pDC->Ellipse(-10, -10, 10, 10);

}


void CGrafikaLabView::DrawPot(CBrush*& oldBrush, CDC* pDC, CPen*& oldPen)
{
	CBrush potBrush(RGB(222, 148, 0));
	oldBrush = pDC->SelectObject(&potBrush);

	CPen potPen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen = pDC->SelectObject(&potPen);

	POINT potBasePoints[] = { 200, 65, 300, 65, 288, 1, 213, 1 };
	pDC->Polygon(potBasePoints, 4);

	pDC->Rectangle(312, 72, 187, 50);
}

void CGrafikaLabView::DrawText(CDC* pDC)
{
	pDC->SetBkMode(TRANSPARENT);
	CFont font;
	font.CreateFontW(30, 0, -2700, 1800, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));
	CFont* oldFont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOutW(23, 473, CString("BROJ IME PREZIME"));
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->TextOutW(25, 475, CString("BROJ IME PREZIME"));
}

void CGrafikaLabView::DrawGrid(CPen*& oldPen, CDC* pDC)
{
	CPen gridPen(PS_SOLID, 1, RGB(255, 255, 255));
	oldPen = pDC->SelectObject(&gridPen);

	//grid is toggled on click
	if (this->gridOn) {
		int pos = 0;
		for (int i = 0; i <= 20; i++)
		{
			//column
			pDC->MoveTo(pos, 0);
			pDC->LineTo(pos, 500);
			//row
			pDC->MoveTo(0, pos);
			pDC->LineTo(500, pos);
			pos += 25;
		}
	}
}


// CGrafikaLabView printing

BOOL CGrafikaLabView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGrafikaLabView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGrafikaLabView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGrafikaLabView diagnostics

#ifdef _DEBUG
void CGrafikaLabView::AssertValid() const
{
	CView::AssertValid();
}

void CGrafikaLabView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGrafikaLabDoc* CGrafikaLabView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGrafikaLabDoc)));
	return (CGrafikaLabDoc*)m_pDocument;
}
#endif //_DEBUG


// CGrafikaLabView message handlers


void CGrafikaLabView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	this->gridOn = !this->gridOn;
	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}


void CGrafikaLabView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (nChar == 'W')
	{
		this->angle1 += M_PI / 12;

		Invalidate();
	}

	if (nChar == 'Q')
	{
		this->angle1 -= M_PI / 12;

		Invalidate();
	}

	if (nChar == 'T')
	{
		this->angle2 += M_PI / 12;

		Invalidate();
	}

	if (nChar == 'R')
	{
		this->angle2 -= M_PI / 12;

		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
