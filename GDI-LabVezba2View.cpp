
// GDI-LabVezba2View.cpp : implementation of the CGDILabVezba2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI-LabVezba2.h"
#endif

#include "GDI-LabVezba2Doc.h"
#include "GDI-LabVezba2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGDILabVezba2View

IMPLEMENT_DYNCREATE(CGDILabVezba2View, CView)

BEGIN_MESSAGE_MAP(CGDILabVezba2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGDILabVezba2View construction/destruction


CGDILabVezba2View::CGDILabVezba2View() noexcept
{
	
	drawGrid = false;
	emf_cactus = GetEnhMetaFile(CString("cactus_part.emf")); //postavljamo slick
	emf_cactus_light = GetEnhMetaFile(CString("cactus_part_light.emf"));
	angle1 = 0; //postavljamo ugao. On ce se menjati kasnije
	angle2 = 0;
}

CGDILabVezba2View::~CGDILabVezba2View()
{
	DeleteEnhMetaFile(emf_cactus);
	DeleteEnhMetaFile(emf_cactus_light); //brisemo
}

BOOL CGDILabVezba2View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CGDILabVezba2View drawing


void CGDILabVezba2View::OnDraw(CDC* pDC)
{
	CGDILabVezba2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);
	//Kreiranje memorijskog DC-ja kompatibilnog sa DC-jem prozora i od sad radimo sa memDC, a ne sa pDC. PROVERI
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	//Inicijalizacija objekta sa bitmapom tako da je kompatibilan sa navedenim uređajem
	CBitmap img;
	img.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	memDC->SelectObject(img);

	//setuje region...
	CRgn region;
	region.CreateRectRgnIndirect(_SetRegion(memDC)); //ili CreateRectRgn(koordinate)

	//do ovde je bio sablon
	_DrawBackground(memDC); //pozadina
	_DrawCactus(memDC); //kaktus 
	_DrawPot(memDC);  //vaza
	_DrawName(memDC); //index ime prezime 

	if (drawGrid)
		_DrawGrid(memDC);

	memDC->SelectClipRgn(&region);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY); //sablon
	delete memDC;

}

//User methods

CRect* CGDILabVezba2View::_SetRegion(CDC* pDC)
{
	//pamti inicijalni region u result
	CRect result;
	pDC->GetClipBox(result); //pribavlja okvirni pravougaonik clip regiona 
	//
	CRgn region;
	region.CreateRectRgn(0, 0, 500, 500); //kreira novi region i selektuje ga. Nzm zasto je to bitno
	pDC->SelectClipRgn(&region);   
	return &result;
}

void CGDILabVezba2View::_DrawGrid(CDC* pDC)
{
	CPen* whiteBrush = new CPen(PS_SOLID,0,RGB(255, 255, 255));
	CPen* oldBrush = (CPen*)pDC->SelectObject(whiteBrush);
	for (int p = 25; p <= 500; p = p + 25)
	{
		pDC->MoveTo(p, 0);
		pDC->LineTo(p, 500);
		pDC->MoveTo(0, p);
		pDC->LineTo(500, p);
	}
	pDC->SelectObject(oldBrush);
}

void CGDILabVezba2View::_DrawPot(CDC* pDC) //vaza ez
{
	CBrush* brush = new CBrush(RGB(222, 148, 0));  //postavljanje cetke
	CBrush* oldBrush = pDC->SelectObject(brush);  //cuvanje stare cetke
	CPoint* trapez = new CPoint[4]();  //postavljanje tacaka za crtanje trapeza. 4.....
	trapez[0] = CPoint(200, 450);
	trapez[1] = CPoint(300, 450);
	trapez[2] = CPoint(290, 500);
	trapez[3] = CPoint(210, 500);
	pDC->Polygon(trapez, 4);  //crtaj od tacke od tacke i ispuini je cetkom 
	pDC->Rectangle(190, 430, 310, 450); //nacrtaj pravougaonik iznad, cetkom
	pDC->SelectObject(oldBrush); //vrati cetku
	delete[]trapez;
	delete brush;
}

void CGDILabVezba2View::_DrawName(CDC* pDC)
{ 
	//1. tekst crna slova
	XFORM oldForm, form1;

	//pDC->MoveTo(0, 0);    //testiraj ovo da vidis kako radi rotacija
	//pDC->LineTo(400, 300);

	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED); //sablon mora
	pDC->GetWorldTransform(&oldForm); //moramo da cuvamo staro stanje uvek kad transformisemo na neki nacin
	//Postavljamo rotaciju za Pi/2
	_SetRotation(pDC, C_PI / 2.0,475, 25); //premesti na sistem na (475,25) i rotiraj za 90* (na dole)! X raste na dole, a Y na levo
	
	//CPen* olovka = new CPen(PS_SOLID, 0, RGB(255, 0, 0));
	//pDC->SelectObject(olovka);
	//pDC->MoveTo(0, 0);
	//DC->LineTo(700, 300); //smanjuj polako na (500,25) da vidis kako se menja...
	
	CString* ime = new CString("17774 TOP G");
	CFont* font = new CFont();
	                 //Font - 20 slajd
	font->CreateFontW(25, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));
	CFont* oldFont = pDC->SelectObject(font);
	CPoint textPosition(475, 25); //rucno koordiante
	pDC->SetBkMode(TRANSPARENT);

	COLORREF oldColor = pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOutW(textPosition.x +1, textPosition.y +1, *ime); //malo je iznad 
	
	//2. tekst zuta slova
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->TextOutW(textPosition.x, textPosition.y, *ime);
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldColor);
	pDC->SetBkMode(OPAQUE);
	delete font;
	delete ime;
	pDC->SetWorldTransform(&oldForm);
	pDC->SetGraphicsMode(oldMode);
}

void CGDILabVezba2View::_DrawBackground(CDC* pDC)
{
	CBrush* brush = new CBrush(RGB(135, 206, 235));
	CBrush* oldBrush = pDC->SelectObject(brush);
	CPen* pen = new CPen(PS_NULL, 0, RGB(0, 0, 0));
	CPen* oldPen = pDC->SelectObject(pen);
	CRect* rect = new CRect(0, 0, 501, 501);
	pDC->Rectangle(rect);
	delete rect;
	pDC->SelectObject(oldBrush);
	delete brush;
	pDC->SelectObject(oldPen);
	delete pen;

}
//dimenzije listova (duzina,sirina): zeleniKruzic=(25,25)  mali_tanak=(75,10) , najveci_debeli=(95,10)    

void CGDILabVezba2View::_DrawCactus(CDC* pDC)
{	
	
	CBrush* brush = new CBrush(RGB(51, 204, 51));
	CBrush* oldBrush = pDC->SelectObject(brush);
	XFORM oldForm; //ovo je objekat kompatabilan za postavljanje transformacija. On ima: M11,M12,M21,M22, Dx, Dy...

	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED); //ova linija mora i ona poslednja da se postave(graphic mode advanced) da bi radile te kao transformacije. Ax: ova fja samo u onDraw! Ali ne radi tako :(
	pDC->GetWorldTransform(&oldForm); //sacuvaj prethodne transformacije

	//Postavimo vecu promenjivu rotaciju 1
	_SetTransformation1(pDC); //omogucavamo rotiranjel, za sad samo za najveci list
	//  samo napisi pDC->m_hDC, uzimas svetlo zeleni list, i stavis ga u rectange( u zavisnosti od velicine rectangla drugaciji list, ali isti apsect ratio slike)
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus_light, CRect(250 - (30 / 1), 350 , 250 + (30 / 1), 425 )); //najveci list. Sada, mozemo da rotiramo najveci list, menjanjem angle-a na tasturi

//Posto smo prethodno postavili transformaciju1, svako dalje iscrtavanje figura bice podlozno toj transformaciji tj. u ovom slucaju bice zarotirane sve te grane
	_DrawRightArm(pDC);  
	//_DrawUpperKomsha(pDC);  //moze da posluzi srednji deo kaktusa
	_DrawLeftArm(pDC);   //levi deo kaktusa
	
	pDC->Ellipse(250 - 10, 350 - 10, 250 + 10, 350 + 10); //Ellipse() Crtamo tamno zelene krugove obicno overdraw-ujemo bodlji listova odnosno posle iscrtavanja listova
	pDC->Ellipse(250 - 10, 425 - 10, 250 + 10, 425 + 10); 

	pDC->SetWorldTransform(&oldForm); //vrati prvobitne transformacije
	pDC->SelectObject(oldBrush);
	delete brush;
	pDC->SetGraphicsMode(oldMode); //vrti prethodni graphics mode

}

void CGDILabVezba2View::_DrawRightArm(CDC* pDC)
{
	
	XFORM form,saveForm,oldForm;
	pDC->GetWorldTransform(&oldForm); //sacuvaj transformaciju na pocetku fje! Uvek!
	//Ovde smo pod uticajem samo prve transformacije tj. transformacije1 sto je ok. Zelimo da se i grane rotiraju isto "odozdo"
	
	
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 3), 275, 250 + (30 / 3), 350)); //list tacno na mid. Mrzi me posebna fja za upper arm
	
	const double angleStep4 = C_PI / 4;
	//Rotiramo prvo za Pi/4
	//_SetRotation ROTIRAMO CEO KOORDINATNI SISTEM I PREMESTAMO GA NA (250,350)
	_SetRotation(pDC, angleStep4, 250, 350); //250 i 350 centrira 2. zeleni kruzic. Od njega krecemo da crtamo figure pod razlicitim uglovima
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 3), 275 , 250 + (30 / 3), 350)); //uzeli smo 2. tamni list i za rotiran zbog linije iznad
	
	pDC->GetWorldTransform(&saveForm);
	
	////Na vec rotirano za Pi/4 zarotiramo jos za Pi/4
	//SAD KRECEMO SURIKEN DA CRTAMO
	_SetRotation(pDC, angleStep4, 250, 275); //Y raste levo. X raste dole
	
	//crtamo 'suriken' listova
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 2) - 5, 200 , 250 + (30 / 2) + 5, 275 )); 
	_SetRotation(pDC, -angleStep4, 250, 275); //Y RASTE PO 225*, X PO 325*
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(300 - (30 / 2) - 5, 150, 300 + (30 / 2) + 5, 225)); //gornji list surikena. 
	_SetRotation(pDC, -2*angleStep4, 250, 275); //Y RASTE PO 325*, X PO 45*. napomena: u ovom slucaju spustanje lista dole dijagonalno zahteva samo menjanje y ose. ali po dijagoani znaci sizeOfGrid * sqrt(2)... Tkd mora rucno 
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(300 - (30 / 2) - 5, 150+175, 300 + (30 / 2) + 5, 225+175));
	pDC->Ellipse(290, 315, 290 + 20, 315 + 20); //zeleni kruzic mora da prekrije bodlje. Nije bas u piksel sad, al ko ga jebe. Nek ostanu okrugle cifre (samo malo desno da se pomeri)
	
	pDC->SetWorldTransform(&oldForm);
	_SetTransformation2(pDC); //OVOM FJOM OMOGUCAVAMO ROTACIJU 2. SVETLOG LISTA. Tako sto stelujemo tacku Dx i Dy (grubo receno) 
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus_light, CRect(303 - (30 / 2) - 5, 222, 303 + (30 / 2) + 5, 297));
	
	//vracamo staru transformaciju tkd ne budemo podlozni _SetTransformation2 na dalje. Zelimo samo 2. zeleni list da bude podlozan toj transformaciji
	pDC->SetWorldTransform(&saveForm); 
	pDC->Ellipse(250 - 10, 275 - 10, 250 + 10, 275 + 10); //3. kruzic
	
	pDC->SetWorldTransform(&oldForm);
	//Moramo da pazimo sta i kako transformisemo i da imamo sliku koordinatnog sistema u glavi...
}

void CGDILabVezba2View::_DrawLeftArm(CDC* pDC)
{
	//NA POCETKU FJE: X RASTE PO (0*) DESNO, A Y PO 270*(DOLE). Kao i obicno
	XFORM oldForm, form;
	pDC->GetWorldTransform(&oldForm);
	
	//Postavlja se na rotaciju -Pi/4 koja je pod uticajem samo vece promenjive rotacije
	const double angleStep4 = C_PI / 4;
	_SetRotation(pDC, -angleStep4, 250, 350); //centrira 2. zeleni kruzic pravo u centar..., x raste na 45*, y na 325*  
	                                            //nzm kako dam vam objasnim ovo stelovanje koordinata...
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 3), 275,       250 + (30 / 3), 350));

	_SetRotation(pDC, -angleStep4, 240, 265); //x=y i y=x 
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(220, 200, 260, 275));

	_SetRotation(pDC, 2*angleStep4, 240, 265); //sad je normalni koordinatni sistem u toj tacki???
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(232, 190, 272, 265));
	pDC->Ellipse(252 - 10, 267 - 10, 252 + 10, 267 + 10);

	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(222, 115, 282, 190));
	pDC->Ellipse(252 - 10, 270 - 10 - 80, 252 + 10, 270 + 10 - 80);
	pDC->SetWorldTransform(&oldForm);
}

void CGDILabVezba2View::_SetTransformation1(CDC* pDC)
{
	XFORM  form1; //prosto objekat koji je neophodan za fje transformacija  

	form1 = {                 
		cosf(angle1),  //rot
		sinf(angle1),  //rot
		-sinf(angle1), //rot
		cosf(angle1),  //rot
		250,    // prvo se izvrsava rotiranje pa onda premestanje
		425    //x i y coordinate za premestanje kaktusa            //U sustini od ove tacke ti vazi rotacija svih figura koji su iscrtane pod 'dejstvom' ove fje
	};
	pDC->SetWorldTransform(&form1);  //fja za prvi put izvrsanje transformacija

	form1 = {
		1,  //mora ovde 1 jer mnozenje matrica...
		0,
		0,
		1,
		-250, //premesta ga za ovolko
		-425                            //GLOBAL <==> LEFTMULTIPLY <==> ODOZDO NA GORE CITANJE. PROVERI TAKO STO CES DA ZAMENIS VREDNOSTI FORME i MWT_LEFTMULTIPLY<->MWT_RIGHTMULTIPLY. A bice i 2 slicke kao dokaz u zip
	};                                  //LEFTMULTIPLY je citanje transformacija(koda) odozdo na gore 
	pDC->ModifyWorldTransform(&form1, MWT_LEFTMULTIPLY);  //fja za svaki sledeci put izvrsavanje transformacija. modifikujemo svaku sl transformaciju...
	
	//citava poenta ovog kurca: transliras(prebacis) figuru na koordinatni pocetak, rotitas je i vratis ponovo na isto mesto. Slajd 44
}

void CGDILabVezba2View::_SetTransformation2(CDC* pDC)
{
	XFORM form2;
	form2 = {
		cosf(angle2),   //angle2 != angle1 
		sinf(angle2),
		-sinf(angle2),
		cosf(angle2),
		302,    //od ove tacke radi rotacija. (2. gornji svetli list(samo taj list)) 
		297
	};
	pDC->ModifyWorldTransform(&form2, MWT_LEFTMULTIPLY);
	form2 = {
		1,
		0,
		0,
		1,
		-302,
		-297
	};
	pDC->ModifyWorldTransform(&form2, MWT_LEFTMULTIPLY);
}

void CGDILabVezba2View::_SetRotation(CDC* pDC, float angle, float x, float y)  //ROTIRAJ KOORDINATNN SISTEM ZA (PI=180*) I PREMESTI GA NA (X,Y) TACKU!!!!! 
{
	XFORM form;
	form = {
		cosf(angle), 
		sinf(angle),
		-sinf(angle),
		cosf(angle),
		x,
		y
	};
	pDC->ModifyWorldTransform(&form, MWT_LEFTMULTIPLY);
	form = {
		1,
		0,
		0,
		1,
		-x,
		-y
	};
	pDC->ModifyWorldTransform(&form, MWT_LEFTMULTIPLY);
}

void CGDILabVezba2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	// Povecavaj odgovarajuce uglove na odgovaruje tastere
	switch (nChar)
	{
	case VK_SPACE:
		drawGrid = !drawGrid;
		Invalidate();
		break;

	case VK_LEFT:
		angle1 -= angleStep;
		Invalidate();
		break;

	case VK_RIGHT:
		angle1 += angleStep;
		Invalidate();
		break;

	case VK_UP:
		angle2 += angleStep;
		Invalidate();
		break;

	case VK_DOWN:
		angle2 -= angleStep;
		Invalidate();
		break;

	default:
		break;
	}


	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


//void CGDILabVezba2View::_DrawUpperKomsha(CDC* pDC) //OVO KREIRA UPPER ARM OD KOMSHINOG LABA.  
//{
//	XFORM saveForm, form,oldForm;
//	pDC->GetWorldTransform(&oldForm);
//	//Postavljamo manju promenjivu rotaciju 
//	_SetTransformation2(pDC);
//
//
//	PlayEnhMetaFile(pDC->m_hDC, emf_cactus_light, CRect(250 - (30 / 3), 275 , 250 + (30 / 3), 350 ));
//	pDC->GetWorldTransform(&saveForm);
//	//Ovde smo pod uticajem obe promenjive transformacije
//	
//	//Prvo rotiramo za Pi/4
//	const double angleStep4 = C_PI / 4;
//
//	_SetRotation(pDC, angleStep4, 250, 275);
//
//	//Gornji desni deo
//	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 3), 200 , 250 + (30 / 3), 275 ));
//	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 1), 125 , 250 + (30 / 1), 200 ));
//	pDC->Ellipse(250 - 10, 200 - 10, 250 + 10, 200 + 10);
//	
//	//Vracamo uticaj na samo promenjive transformacije
//	pDC->SetWorldTransform(&saveForm);
//	//Crtamo gornji deo
//	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 3), 200 , 250 + (30 / 3), 275 ));
//	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 1), 125 , 250 + (30 / 1), 200 ));
//	pDC->Ellipse(250 - 10, 200 - 10, 250 + 10, 200 + 10);
//
//	//Rotiramo za -Pi/4, a pod uticajem smo obe promenjive transformacije
//	
//	_SetRotation(pDC, -angleStep4, 250, 275);
//
//	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 3), 200 , 250 + (30 / 3), 275 ));
//
//	//Vracamo uticaj na samo promenjive transformacije
//	pDC->SetWorldTransform(&saveForm);
//	//Crtamo krugove koji su ispred kaktusa i koji su pod uticajem obe promenjive transformacije
//	pDC->Ellipse(250 - 10, 275 - 10, 250 + 10, 275 + 10);
//	pDC->Ellipse(250 - 10, 200 - 10, 250 + 10, 200 + 10);
//
//	pDC->SetWorldTransform(&oldForm);
//}



// NIJE BITNO OVO NA DOLE !!!!!!

BOOL CGDILabVezba2View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


// CGDILabVezba2View printing

BOOL CGDILabVezba2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDILabVezba2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDILabVezba2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDILabVezba2View diagnostics

#ifdef _DEBUG
void CGDILabVezba2View::AssertValid() const
{
	CView::AssertValid();
}

void CGDILabVezba2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDILabVezba2Doc* CGDILabVezba2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDILabVezba2Doc)));
	return (CGDILabVezba2Doc*)m_pDocument;
}
#endif //_DEBUG