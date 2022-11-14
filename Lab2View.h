
// Lab2View.h : interface of the CLab2View class
//

#pragma once


class CLab2View : public CView
{
protected: // create from serialization only
	CLab2View() noexcept;
	DECLARE_DYNCREATE(CLab2View)

// Attributes
public:
	CLab2Doc* GetDocument() const;
	bool grid;
	float ang1 , ang2 ;
	const double PI = atan(1)*4; // Pi konstanta
	const float angleStep = 5 * ((2 * PI) / 360); // Korak rotiranja
	HENHMETAFILE lgr, dgr;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawBase(CDC* pDC);
	void DrawBackgroud(CDC* pDC, CRect& rect);
	void DrawPot(CDC* pDC);
	void DrawCircle(CDC* pDC);
	void DrawDarkC(CDC* pDC);
	void DrawLightC(CDC* pDC);
	void Translate(CDC* pDC, float x, float y, bool rightMul);
	void Rotate(CDC* pDC, float angle, bool rightMul);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DefaultTransformation(CDC* pDC);
	void WriteText(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void DrawGrid(CDC* pDC);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLab2View();
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

#ifndef _DEBUG  // debug version in Lab2View.cpp
inline CLab2Doc* CLab2View::GetDocument() const
   { return reinterpret_cast<CLab2Doc*>(m_pDocument); }
#endif

