#ifndef __C_MYSTAR_H__
#define __C_MYSTAR_H__

#include <vector>
#include "CMyShape.h"

class CMyStar : public CMyShape {
	std::vector<CPoint> points;
public:
	CMyStar();
	virtual ~CMyStar() = default;

	virtual void draw(CDC* dc) override;
	virtual void update(CPoint start, CPoint end) override;
	virtual void moveBy(CPoint delta) override;
	virtual bool contains(CPoint point) override;
};

#endif
