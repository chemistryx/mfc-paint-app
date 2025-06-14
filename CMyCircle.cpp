#include "pch.h"
#include "CMyCircle.h"

CMyCircle::CMyCircle() {}

void CMyCircle::draw(CDC* dc) {
	CRect boundingBox = getBoundingBox();
	COLORREF fillColor = getFillColor();
	COLORREF outlineColor = getOutlineColor();

	CBrush brush(fillColor);
	CBrush* oldBrush = dc->SelectObject(&brush);

	CPen pen(PS_SOLID, 3, outlineColor);
	CPen* oldPen = dc->SelectObject(&pen);

	dc->Ellipse(boundingBox);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}

void CMyCircle::update(CPoint start, CPoint end) {
	int cx = start.x;
	int cy = start.y;

	int dx = end.x - cx;
	int dy = end.y - cy;

	int radius = sqrt(dx * dx + dy * dy);

	CRect boundingBox(cx - radius, cy - radius, cx + radius, cy + radius);

	setBoundingBox(boundingBox);
}

void CMyCircle::moveBy(CPoint delta) {
	CRect boundingBox = getBoundingBox();
	boundingBox.OffsetRect(delta);

	setBoundingBox(boundingBox);
}

bool CMyCircle::contains(CPoint point) {
	CRect boundingBox = getBoundingBox();
	CPoint center = boundingBox.CenterPoint();
	int radius = boundingBox.Width() / 2;

	double dist = sqrt((point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y));

	return dist <= radius;
}