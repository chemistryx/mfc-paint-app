#ifndef __C_MYCURVE_H__
#define __C_MYCURVE_H__

#include <vector>
#include "CMyShape.h"

class CMyCurve : public CMyShape {
	std::vector<CPoint> points;
	bool isDrawing = false;
public:
	CMyCurve();
	virtual ~CMyCurve() = default;

	virtual void draw(CDC* dc) override;
	virtual void update(CPoint start, CPoint end) override;
	virtual void moveBy(CPoint delta) override;
	virtual bool contains(CPoint point) override;
private:
	int getPointToSegmentDistance(CPoint point, CPoint a, CPoint b);
	double distance(CPoint p1, CPoint p2);
};

#endif
