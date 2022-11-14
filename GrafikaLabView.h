
// GrafikaLabView.h : interface of the CGrafikaLabView class
//

#pragma once


class CGrafikaLabView : public CView
{
protected: // create from serialization only
	CGrafikaLabView() noexcept;
	DECLARE_DYNCREATE(CGrafikaLabView)

// Attributes
public:
	CGrafikaLabDoc* GetDocument() const;

	HENHMETAFILE partDark;
	HENHMETAFILE partLight;

	float angle1;
	float angle2;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawCactus(CDC* pDC, XFORM& old);
	void DrawBranches(CDC* pDC);
	void DrawPot(CBrush*& oldBrush, CDC* pDC, CPen*& oldPen);
	void DrawText(CDC* pDC);
	void DrawGrid(CPen*& oldPen, CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGrafikaLabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL gridOn;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in GrafikaLabView.cpp
inline CGrafikaLabDoc* CGrafikaLabView::GetDocument() const
   { return reinterpret_cast<CGrafikaLabDoc*>(m_pDocument); }
#endif

