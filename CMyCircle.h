#ifndef __C_MYCIRCLE_H__
#define __C_MYCIRCLE_H__

#include "CMyShape.h"

class CMyCircle : public CMyShape {
public:
	CMyCircle();
	virtual ~CMyCircle() = default;

	virtual void draw(CDC* dc) override;
	virtual void update(CPoint start, CPoint end) override;
	virtual void moveBy(CPoint delta) override;
	virtual bool contains(CPoint point) override;
};

#endif
