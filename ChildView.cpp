#include "pch.h"
#include "framework.h"
#include "CPP-HomeworkFinal.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView() {}

CChildView::~CChildView() {}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_DRAW_RECTANGLE, &CChildView::OnDrawRectangle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECTANGLE, &CChildView::OnUpdateDrawRectangle)
	ON_COMMAND(ID_DRAW_CIRCLE, &CChildView::OnDrawCircle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CIRCLE, &CChildView::OnUpdateDrawCircle)
	ON_COMMAND(ID_DRAW_CURVE, &CChildView::OnDrawCurve)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CURVE, &CChildView::OnUpdateDrawCurve)
	ON_COMMAND(ID_DRAW_STAR, &CChildView::OnDrawStar)
	ON_UPDATE_COMMAND_UI(ID_DRAW_STAR, &CChildView::OnUpdateDrawStar)
	ON_COMMAND(ID_ACTION_SELECT, &CChildView::OnActionSelect)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SELECT, &CChildView::OnUpdateActionSelect)
	ON_COMMAND(ID_ALIGN_BRINGFRONT, &CChildView::OnAlignBringFront)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_BRINGFRONT, &CChildView::OnUpdateAlignBringFront)
	ON_COMMAND(ID_ALIGN_BRINGBACK, &CChildView::OnAlignBringBack)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_BRINGBACK, &CChildView::OnUpdateAlignBringBack)
	ON_COMMAND(ID_ACTION_GROUP, &CChildView::OnActionGroup)
	ON_UPDATE_COMMAND_UI(ID_ACTION_GROUP, &CChildView::OnUpdateActionGroup)
	ON_COMMAND(ID_ACTION_UNGROUP, &CChildView::OnActionUngroup)
	ON_UPDATE_COMMAND_UI(ID_ACTION_UNGROUP, &CChildView::OnUpdateActionUngroup)
	ON_WM_CONTEXTMENU()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CChildView 메시지 처리기
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) {
	if (!CWnd::PreCreateWindow(cs)) return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, ::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

// 배경을 그리는 함수
void CChildView::drawBackground(CDC& dc, CRect& rect) {
	dc.FillSolidRect(rect, RGB(255, 255, 255)); // 흰색 배경 설정

	CFont font;
	font.CreatePointFont(400, _T("Times New Roman"));
	CFont* oldFont = dc.SelectObject(&font);
	dc.SelectObject(&font);
	dc.SetTextColor(RGB(255, 200, 200));

	// 배경 텍스트 출력
	dc.DrawTextW(CString(_T("C++ Homework Final")), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	dc.SelectObject(oldFont);
}

// 도형 개수를 화면에 표시하는 함수
void CChildView::drawIndicators(CDC& dc, CRect& rect) {
	CString shapesCountString;
	shapesCountString.Format(_T("Number of Shapes: %d"), shapeManager.getShapesCount());

	CString selectedCountString;
	selectedCountString.Format(_T("Number of Selected: %d"), shapeManager.getSelectedCount());

	dc.SetTextColor(RGB(0, 0, 0));
	dc.SetBkMode(TRANSPARENT); // 배경 투명 설정

	dc.TextOutW(0, 0, shapesCountString);
	dc.TextOutW(0, 20, selectedCountString);
}

void CChildView::OnPaint() {
	CPaintDC dc(this);
	
	CRect rect;
	GetClientRect(rect); // 화면 전체 영역

	// Double Buffering 구현
	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CBitmap* oldBitmap = memDC.SelectObject(&bitmap);

	// 배경 -> 도형 -> 인디케이터 순서대로 그리기
	drawBackground(memDC, rect);
	shapeManager.drawShapes(&memDC);
	drawIndicators(memDC, rect);

	// memDC에서 DC로 한번에 복사
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(oldBitmap);
}

// 배경 깜빡임 방지
BOOL CChildView::OnEraseBkgnd(CDC* pDC) { return TRUE; }

// 마우스 왼쪽 버튼 누를 때 호출되는 함수
void CChildView::OnLButtonDown(UINT nFlags, CPoint point) {
	SetCapture();
	shapeManager.handleMouseLeftDown(point);
	Invalidate(FALSE);

	CWnd::OnLButtonDown(nFlags, point);
}

// 마우스 이동 시 호출되는 함수
void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetCapture() == this) {
		shapeManager.handleMouseMove(point);
		Invalidate(FALSE);
	}

	CWnd::OnMouseMove(nFlags, point);
}

// 마우스 왼쪽 버튼 뗄 때 호출되는 함수
void CChildView::OnLButtonUp(UINT nFlags, CPoint point) {
	if (GetCapture() == this) {
		shapeManager.handleMouseLeftUp(point);
		ReleaseCapture();
		Invalidate(FALSE);
	}

	CWnd::OnLButtonUp(nFlags, point);
}

/*
 * 메뉴 처리 함수
 */

// 사각형
void CChildView::OnDrawRectangle() {
	if (shapeManager.getDrawingMode() != DrawingMode::Shape) {
		shapeManager.setDrawingMode(DrawingMode::Shape);
		Invalidate(FALSE);
	}

	shapeManager.setDrawingType(DrawingType::Rectangle);
}

void CChildView::OnUpdateDrawRectangle(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(shapeManager.getDrawingType() == DrawingType::Rectangle);
}

// 원
void CChildView::OnDrawCircle() {
	if (shapeManager.getDrawingMode() != DrawingMode::Shape) {
		shapeManager.setDrawingMode(DrawingMode::Shape);
		Invalidate(FALSE);
	}

	shapeManager.setDrawingType(DrawingType::Circle);
}

void CChildView::OnUpdateDrawCircle(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(shapeManager.getDrawingType() == DrawingType::Circle);
}

// 곡선
void CChildView::OnDrawCurve() {
	if (shapeManager.getDrawingMode() != DrawingMode::Shape) {
		shapeManager.setDrawingMode(DrawingMode::Shape);
		Invalidate(FALSE);
	}

	shapeManager.setDrawingType(DrawingType::Curve);
}

void CChildView::OnUpdateDrawCurve(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(shapeManager.getDrawingType() == DrawingType::Curve);
}

// 별
void CChildView::OnDrawStar() {
	if (shapeManager.getDrawingMode() != DrawingMode::Shape) {
		shapeManager.setDrawingMode(DrawingMode::Shape);
		Invalidate(FALSE);
	}

	shapeManager.setDrawingType(DrawingType::Star);
}

void CChildView::OnUpdateDrawStar(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(shapeManager.getDrawingType() == DrawingType::Star);
}

// 선택 모드
void CChildView::OnActionSelect() {
	if (shapeManager.getDrawingMode() != DrawingMode::Select) shapeManager.setDrawingMode(DrawingMode::Select);
	shapeManager.setDrawingType(DrawingType::None);
}

void CChildView::OnUpdateActionSelect(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(shapeManager.getDrawingMode() == DrawingMode::Select);
}

// 도형 순서 (맨 앞으로)
void CChildView::OnAlignBringFront() {
	shapeManager.bringSelectedToFront();
	Invalidate(FALSE);
}

void CChildView::OnUpdateAlignBringFront(CCmdUI* pCmdUI) {
	pCmdUI->Enable(shapeManager.getSelectedCount() > 0);
}

// 도형 순서 (맨 뒤로)
void CChildView::OnAlignBringBack() {
	shapeManager.bringSelectedToBack();
	Invalidate(FALSE);
}

void CChildView::OnUpdateAlignBringBack(CCmdUI* pCmdUI) {
	pCmdUI->Enable(shapeManager.getSelectedCount() > 0);
}

// 그룹 생성
void CChildView::OnActionGroup() {
	shapeManager.groupSelectedShapes();
	Invalidate(FALSE);
}

void CChildView::OnUpdateActionGroup(CCmdUI* pCmdUI) {
	pCmdUI->Enable(shapeManager.getSelectedCount() >= 2);
}

// 그룹 해제
void CChildView::OnActionUngroup() {
	shapeManager.ungroupSelectedShapes();
	Invalidate(FALSE);
}

void CChildView::OnUpdateActionUngroup(CCmdUI* pCmdUI) {
	pCmdUI->Enable(shapeManager.getSelectedCount() > 0);
}

// 컨텍스트 메뉴
void CChildView::OnContextMenu(CWnd* pWnd, CPoint point) {
	CMenu menu;
	menu.LoadMenuW(IDR_MAINFRAME);

	CMenu* pMenu = menu.GetSubMenu(3);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
}

/*
 * 키보드 입력 처리 함수
 */

// 키보드 키 눌렀을 때 호출되는 함수
void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar == VK_SHIFT) shapeManager.setShiftPressed(true); // Shift 감지

	if (nChar == VK_DELETE) {
		shapeManager.removeShapes(true); // 선택된 도형 삭제
		Invalidate(FALSE);
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

// 키보드 키 뗐을 때 호출되는 함수
void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar == VK_SHIFT) shapeManager.setShiftPressed(false); // Shift 해제

	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}
