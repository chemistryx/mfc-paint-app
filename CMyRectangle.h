#ifndef __C_MYRECTANGLE_H__
#define __C_MYRECTANGLE_H__

#include "CMyShape.h"

class CMyRectangle : public CMyShape {
public:
	CMyRectangle();
	virtual ~CMyRectangle() = default;

	virtual void draw(CDC* dc) override;
	virtual void update(CPoint start, CPoint end) override;
	virtual void moveBy(CPoint delta) override;
	virtual bool contains(CPoint point) override;
};

#endif
