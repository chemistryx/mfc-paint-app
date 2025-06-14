#include "pch.h"
#include "CMyStar.h"
#define PI 3.141592

CMyStar::CMyStar() {}

void CMyStar::draw(CDC* dc) {
	COLORREF fillColor = getFillColor();
	COLORREF outlineColor = getOutlineColor();

	CBrush brush(fillColor);
	CBrush* oldBrush = dc->SelectObject(&brush);

	CPen pen(PS_SOLID, 3, outlineColor);
	CPen* oldPen = dc->SelectObject(&pen);

	dc->Polygon(points.data(), points.size());

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}

void CMyStar::update(CPoint start, CPoint end) {
	points.clear();
	CPoint center(start.x, start.y);

	double dx = end.x - start.x;
	double dy = end.y - start.y;

	double outerRadius = sqrt(dx * dx + dy * dy);
	double innerRadius = outerRadius * 0.5;
	int numPoints = 5;

	for (int i = 0; i < numPoints * 2; i++) {
		double angle = i * PI / numPoints - PI / 2;
		double radius = (i % 2 == 0) ? outerRadius : innerRadius;

		int x = center.x + radius * cos(angle);
		int y = center.y + radius * sin(angle);

		points.push_back(CPoint(x, y));
	}

	points.push_back(points[0]);

	int minX = points[0].x, minY = points[0].y;
	int maxX = points[0].x, maxY = points[0].y;

	for (CPoint& point : points) {
		if (point.x < minX) minX = point.x;
		if (point.y < minY) minY = point.y;
		if (point.x > maxX) maxX = point.x;
		if (point.y > maxY) maxY = point.y;
	}

	CRect boundingBox(minX, minY, maxX, maxY);

	setBoundingBox(boundingBox);
}

void CMyStar::moveBy(CPoint delta) {
	for (CPoint& point : points) {
		point += delta;
	}

	CRect boundingBox = getBoundingBox();
	boundingBox.OffsetRect(delta);
	
	setBoundingBox(boundingBox);
}

// Ray Casting 알고리즘
bool CMyStar::contains(CPoint point) {
	int count = 0;
	int n = points.size();

	for (int i = 0; i < n; i++) {
		CPoint a = points[i];
		CPoint b = points[(i + 1) % n];

		if ((a.y > point.y) != (b.y > point.y)) {
			double x = a.x + (double) (b.x - a.x) * (point.y - a.y) / (b.y - a.y);
			if (point.x < x) count++;
		}
	}

	return (count % 2) == 1;
}
