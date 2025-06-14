
#pragma once

#include "CMyShapeManager.h"

// CChildView 창
class CChildView : public CWnd {
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	CMyShapeManager shapeManager;

// 작업입니다.
public:
	void drawBackground(CDC& dc, CRect& rect);
	void drawIndicators(CDC& dc, CRect& rect);

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDrawRectangle();
	afx_msg void OnDrawCircle();
	afx_msg void OnDrawCurve();
	afx_msg void OnUpdateDrawRectangle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawCircle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawCurve(CCmdUI* pCmdUI);
	afx_msg void OnActionSelect();
	afx_msg void OnUpdateActionSelect(CCmdUI* pCmdUI);
	afx_msg void OnDrawStar();
	afx_msg void OnUpdateDrawStar(CCmdUI* pCmdUI);
	afx_msg void OnAlignBringFront();
	afx_msg void OnUpdateAlignBringFront(CCmdUI* pCmdUI);
	afx_msg void OnAlignBringBack();
	afx_msg void OnUpdateAlignBringBack(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnActionGroup();
	afx_msg void OnUpdateActionGroup(CCmdUI* pCmdUI);
	afx_msg void OnActionUngroup();
	afx_msg void OnUpdateActionUngroup(CCmdUI* pCmdUI);
};
