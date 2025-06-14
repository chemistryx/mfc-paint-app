#include "pch.h"
#include "CMyRectangle.h"

CMyRectangle::CMyRectangle() {}

void CMyRectangle::draw(CDC* dc) {
	CRect boundingBox = getBoundingBox();
	COLORREF fillColor = getFillColor();
	COLORREF outlineColor = getOutlineColor();

	CBrush brush(fillColor);
	CBrush* oldBrush = dc->SelectObject(&brush);

	CPen pen(PS_SOLID, 3, outlineColor);
	CPen* oldPen = dc->SelectObject(&pen);

	dc->Rectangle(boundingBox);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}

void CMyRectangle::update(CPoint start, CPoint end) {
	CRect boundingBox(start, end);

	setBoundingBox(boundingBox);
}

void CMyRectangle::moveBy(CPoint delta) {
	CRect boundingBox = getBoundingBox();
	boundingBox.OffsetRect(delta);

	setBoundingBox(boundingBox);
}

bool CMyRectangle::contains(CPoint point) {
	return getBoundingBox().PtInRect(point);
}