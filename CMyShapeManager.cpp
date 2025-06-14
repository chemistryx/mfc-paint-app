#include "pch.h"
#include "CMyShapeManager.h"

CMyShapeManager::CMyShapeManager() {
	drawingMode = DrawingMode::Shape;
	drawingType = DrawingType::Rectangle;
	drawingShape = nullptr;
	isSelecting = false;
	isDragging = false;
	isShiftPressed = false;
	srand(time(NULL));
}

CMyShapeManager::~CMyShapeManager() {
	removeShapes();
	delete drawingShape;
}

// ������ �߰��ϴ� �Լ�
void CMyShapeManager::addShape(CMyShape* shape) {
	if (shape) shapes.push_back(shape);
}

// �Ѱ��� ������ �����ϴ� �Լ�
void CMyShapeManager::removeShape(CMyShape* shape) {
	if (shape) {
		shape->setSelected(false); // ���� ����
		shapes.erase(remove(shapes.begin(), shapes.end(), shape), shapes.end()); // vector���� ����
		delete shape; // �޸� ����
	}
}

// ���� ��ü �Ǵ� ���õ� �������� �����ϴ� �Լ�
void CMyShapeManager::removeShapes(bool onlySelected) {
	std::vector<CMyShape*> toDelete;

	// ���� ��� ���� ����
	for (CMyShape* shape : shapes) {
		// onlySelected=true�� ���õ� ������ ����
		if (!onlySelected || shape->isSelected()) {
			toDelete.push_back(shape);
		}
	}

	// ������ ���� ����
	for (CMyShape* shape : toDelete) {
		removeShape(shape);
	}
}

// ���õ� ������ ������ ��ȯ�ϴ� �Լ�
int CMyShapeManager::getSelectedCount() {
	int count = 0;

	for (CMyShape* shape : shapes) {
		if (shape->isSelected()) count += 1;
	}

	return count;
}

// ��� ���� �׸��� �Լ�
void CMyShapeManager::drawShapes(CDC* dc) {
	for (CMyShape* shape : shapes) {
		shape->draw(dc); // ����� ���� �׸���

		// ���õ� ������ ���� ���� �׵θ� ǥ��
		if (shape->isSelected()) {
			CPen pen(PS_DOT, 1, RGB(255, 0, 0));
			CPen* oldPen = dc->SelectObject(&pen);
			CBrush* oldBrush = (CBrush*)dc->SelectStockObject(NULL_BRUSH);
			CRect rect = shape->getBoundingBox();

			rect.InflateRect(5, 5); // ���� �߰�
			dc->Rectangle(rect);

			dc->SelectObject(oldPen);
			dc->SelectObject(oldBrush);
		}
	}

	// ���콺�� ���� �׸��� �ִ� ���� �׸���
	if (drawingShape) drawingShape->draw(dc);

	// ���� ��忡�� �巡�� ���� ���� ǥ��
	if (drawingMode == DrawingMode::Select && isSelecting) {
		CPen pen(PS_DOT, 1, RGB(255, 0, 0));
		CPen* oldPen = dc->SelectObject(&pen);
		CBrush* oldBrush = (CBrush*)dc->SelectStockObject(NULL_BRUSH);
		CRect rect(drawStartPoint, drawEndPoint);

		dc->Rectangle(rect);

		dc->SelectObject(oldPen);
		dc->SelectObject(oldBrush);
	}
}

// ��ü ���� ���� ���� �Լ�
void CMyShapeManager::clearShapeSelection() {
	for (CMyShape* shape : shapes) shape->setSelected(false);
}

// �׸��� ��� ���� �Լ�
void CMyShapeManager::setDrawingMode(DrawingMode mode) {
	TRACE("[CMyShapeMananger] Transitioning drawingMode from %d to %d\n", drawingMode, mode);

	// ���� ��忡�� ����� ��� ��� ���� ���� ����
	if (drawingMode == DrawingMode::Select && mode != DrawingMode::Select) clearShapeSelection();
	drawingMode = mode;
}

// �׸��� Ÿ�� ���� �Լ�
void CMyShapeManager::setDrawingType(DrawingType type) {
	TRACE("[CMyShapeMananger] Transitioning drawingType from %d to %d\n", drawingType, type);
	drawingType = type;
}

// ���콺 ���� ��ư ���� ó�� �Լ�
void CMyShapeManager::handleMouseLeftDown(CPoint point) {
	TRACE("[CMyShapeMananger] handleMouseLeftDown\n");
	drawStartPoint = point;
	drawEndPoint = point;

	if (drawingMode == DrawingMode::Shape) {
		// �׸��� Ÿ�Կ� ���� �� ���� ��ü ����
		switch (drawingType) {
			case DrawingType::Rectangle: drawingShape = new CMyRectangle(); break;
			case DrawingType::Circle: drawingShape = new CMyCircle(); break;
			case DrawingType::Curve: drawingShape = new CMyCurve(); break;
			case DrawingType::Star: drawingShape = new CMyStar(); break;
		}
	} else if (drawingMode == DrawingMode::Select) {
		if (!isShiftPressed) clearShapeSelection(); // ShiftŰ�� ���� ���°� �ƴϸ� ���� �ʱ�ȭ (���� ���� ���)
		isSelecting = true;

		// ���� Ž���Ͽ� ���� ���� �ִ� ���� ����
		for (auto& it = shapes.rbegin(); it != shapes.rend(); it++) {
			CMyShape* shape = *it;

			if (shape->contains(point)) {
				shape->setSelected(true);
				isDragging = true;
				isSelecting = false;
				break;
			}
		}
	}

	// �ʱ� ���� ������Ʈ
	if (drawingShape) drawingShape->update(drawStartPoint, drawEndPoint);
}

// ���콺 �̵� ó�� �Լ�
void CMyShapeManager::handleMouseMove(CPoint point) {
	if (drawingShape && drawingMode == DrawingMode::Shape) {
		// ���� ũ�� ������Ʈ
		drawingShape->update(drawStartPoint, point);
	} else if (drawingMode == DrawingMode::Select && isDragging) {
		CPoint offset = point - drawEndPoint;

		// ���õ� ������ offset��ŭ �̵�
		for (CMyShape* shape : shapes) {
			if (shape->isSelected()) shape->moveBy(offset);
		}
	}

	drawEndPoint = point;
}

// ���콺 ���� ��ư ���� ó�� �Լ�
void CMyShapeManager::handleMouseLeftUp(CPoint point) {
	TRACE("[CMyShapeMananger] handleMouseLeftUp\n");
	drawEndPoint = point;

	// �׸��� ����
	if (drawingShape && drawingMode == DrawingMode::Shape) {
		drawingShape->update(drawStartPoint, drawEndPoint);
		addShape(drawingShape); // ���� ��Ͽ� �߰�
		drawingShape = nullptr;
	} else if (drawingMode == DrawingMode::Select) {
		if (isSelecting) {
			isSelecting = false;

			CRect selectionRect(drawStartPoint, drawEndPoint);
			selectionRect.NormalizeRect(); // �巡�� ���� ����ȭ (��ǥ ���� ����)

			for (CMyShape* shape : shapes) {
				// ���� ���� �� ������ ���Ե� ������ ���� ó��
				if (selectionRect.PtInRect(shape->getBoundingBox().TopLeft()) && selectionRect.PtInRect(shape->getBoundingBox().BottomRight())) {
					shape->setSelected(true);
				}
			}
		}

		isDragging = false;
	}
}

// ���õ� �������� �� ������ �̵���Ű�� �Լ�
void CMyShapeManager::bringSelectedToFront() {
	std::vector<CMyShape*> selectedShapes;

	// ���õ� ���� ���� �� ����
	for (auto& it = shapes.begin(); it != shapes.end();) {
		CMyShape* shape = *it;

		if (shape->isSelected()) {
			selectedShapes.push_back(shape);
			it = shapes.erase(it);
		} else {
			it += 1;
		}
	}

	// ��� ���� �ڿ� �߰�(= ȭ��� �� ��)
	shapes.insert(shapes.end(), selectedShapes.begin(), selectedShapes.end());
}

// ���õ� �������� �� �ڷ� �̵���Ű�� �Լ�
void CMyShapeManager::bringSelectedToBack() {
	std::vector<CMyShape*> selectedShapes;

	// ���õ� ���� ���� �� ����
	for (auto& it = shapes.begin(); it != shapes.end();) {
		CMyShape* shape = *it;

		if (shape->isSelected()) {
			selectedShapes.push_back(shape);
			it = shapes.erase(it);
		} else {
			it += 1;
		}
	}

	// ��� ���� �տ� �߰�(= ȭ��� �� ��)
	shapes.insert(shapes.begin(), selectedShapes.begin(), selectedShapes.end());
}

// ���õ� �������� �׷����� ���� �Լ�
void CMyShapeManager::groupSelectedShapes() {
	CMyGroup* group = new CMyGroup();

	for (auto& it = shapes.begin(); it != shapes.end();) {
		CMyShape* shape = *it;

		if (shape->isSelected()) {
			group->addChildren(shape); // �׷쿡 ���� ���� �߰�
			it = shapes.erase(it); // ��ü ���� ��Ͽ��� ����
		} else {
			it += 1;
		}
	}

	group->setSelected(true);
	shapes.push_back(group); // �׷��� ���� ��Ͽ� �߰�
}

// ���õ� �����鿡 ���� �׷��� �����ϴ� �Լ�
void CMyShapeManager::ungroupSelectedShapes() {
	std::vector<CMyShape*> toUngroup;

	// ���õ� �׷� ����
	for (auto& it = shapes.begin(); it != shapes.end();) {
		CMyShape* shape = *it;
		CMyGroup* group = dynamic_cast<CMyGroup*>(shape); // dynamic_cast�� �׷����� Ȯ��

		if (group && group->isSelected()) {
			toUngroup.push_back(shape);
			it = shapes.erase(it); // �׷� ����
		} else {
			it += 1;
		}
	}

	// �ڽ� �������� ���� ���� ��Ͽ� �ٽ� �߰�
	for (CMyShape* shape : toUngroup) {
		CMyGroup* group = dynamic_cast<CMyGroup*>(shape);

		for (CMyShape* child : group->getChildren()) {
			child->setSelected(true);
			shapes.push_back(child);
		}

		delete group;
	}
}
