#include "pch.h"
#include "CMyCurve.h"

CMyCurve::CMyCurve() {}

void CMyCurve::draw(CDC* dc) {
	COLORREF fillColor = getFillColor();

	CPen pen(PS_SOLID, 3, fillColor);
	CPen* oldPen = dc->SelectObject(&pen);

	for (int i = 1; i < points.size(); i++) {
		dc->MoveTo(points[i - 1]);
		dc->LineTo(points[i]);
	}

	dc->SelectObject(oldPen);
}

void CMyCurve::update(CPoint start, CPoint end) {
	CPoint lastPoint = points.size() ? points.back() : end;

	if (!isDrawing) {
		TRACE("[CMyCurve] draw start\n");
		points.push_back(start);
		points.push_back(end);

		isDrawing = true;
	} else {
		// dragging
		if (end != lastPoint) {
			points.push_back(end);
		} else {
			TRACE("[CMyCurve] draw stop\n");
			isDrawing = false;
		}
	}

	// bounding box calculation
	if (points.empty()) return;

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

void CMyCurve::moveBy(CPoint delta) {
	for (CPoint& point : points) {
		point += delta;
	}

	CRect boundingBox = getBoundingBox();
	boundingBox.OffsetRect(delta);

	setBoundingBox(boundingBox);
}

bool CMyCurve::contains(CPoint point) {
	int threshold = 5;

	for (int i = 1; i < points.size(); i++) {
		if (getPointToSegmentDistance(point, points[i - 1], points[i]) <= threshold) return true;
	}

	return false;
}

// 점과 선분의 거리 계산
int CMyCurve::getPointToSegmentDistance(CPoint point, CPoint start, CPoint end) {
	CPoint lineVec(end.x - start.x, end.y - start.y);
	CPoint pointVec(point.x - start.x, point.y - start.y);

	double lineLength2 = (lineVec.x * lineVec.x) + (lineVec.y * lineVec.y);

	double t = (pointVec.x * lineVec.x + pointVec.y * lineVec.y) / lineLength2;

	// 점이 선분 외부에 있는 경우
	if (t < 0.0) {
		return distance(point, start);
	} else if (t > 1.0) {
		return distance(point, end);
	}

	// 점이 선분 내부에 있는 경우
	CPoint proj(start.x + t * lineVec.x, start.y + t * lineVec.y);

	return distance(point, proj);
}

double CMyCurve::distance(CPoint p1, CPoint p2) {
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}