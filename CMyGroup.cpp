#include "pch.h"
#include "CMyGroup.h"

CMyGroup::CMyGroup() {}

void CMyGroup::draw(CDC* dc) {
	for (CMyShape* shape : children) shape->draw(dc);
}

// drawingShape�� �ƴϹǷ� ȣ�� �� ���� ����
void CMyGroup::update(CPoint start, CPoint end) {}

void CMyGroup::moveBy(CPoint delta) {
	for (CMyShape* shape : children) shape->moveBy(delta);

	CRect boundingBox = getBoundingBox();
	boundingBox.OffsetRect(delta);

	setBoundingBox(boundingBox);
}

bool CMyGroup::contains(CPoint point) {
	for (CMyShape* shape : children) {
		if (shape->contains(point)) return true;
	}

	return false;
}

void CMyGroup::addChildren(CMyShape* shape) {
	TRACE("[CMyGroup] add children\n");
	children.push_back(shape);

	// ���ο� boundingBox ���
	CRect boundingBox = children[0]->getBoundingBox();

	for (int i = 1; i < children.size(); i++) {
		boundingBox.UnionRect(boundingBox, children[i]->getBoundingBox());
	}

	setBoundingBox(boundingBox);
}
