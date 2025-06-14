#ifndef __C_MYGROUP_H__
#define __C_MYGROUP_H__

#include <vector>
#include "CMyShape.h"

class CMyGroup : public CMyShape {
	std::vector<CMyShape*> children;
public:
	CMyGroup();
	virtual ~CMyGroup() = default;

	virtual void draw(CDC* dc);
	virtual void update(CPoint start, CPoint end);
	virtual void moveBy(CPoint delta);
	virtual bool contains(CPoint point);
	std::vector<CMyShape*>& getChildren() { return children; };
	void addChildren(CMyShape* shape);
};

#endif


