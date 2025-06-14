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

// 도형을 추가하는 함수
void CMyShapeManager::addShape(CMyShape* shape) {
	if (shape) shapes.push_back(shape);
}

// 한개의 도형을 삭제하는 함수
void CMyShapeManager::removeShape(CMyShape* shape) {
	if (shape) {
		shape->setSelected(false); // 선택 해제
		shapes.erase(remove(shapes.begin(), shapes.end(), shape), shapes.end()); // vector에서 제거
		delete shape; // 메모리 해제
	}
}

// 도형 전체 또는 선택된 도형만을 삭제하는 함수
void CMyShapeManager::removeShapes(bool onlySelected) {
	std::vector<CMyShape*> toDelete;

	// 삭제 대상 도형 수집
	for (CMyShape* shape : shapes) {
		// onlySelected=true면 선택된 도형만 삭제
		if (!onlySelected || shape->isSelected()) {
			toDelete.push_back(shape);
		}
	}

	// 수집된 도형 삭제
	for (CMyShape* shape : toDelete) {
		removeShape(shape);
	}
}

// 선택된 도형의 개수를 반환하는 함수
int CMyShapeManager::getSelectedCount() {
	int count = 0;

	for (CMyShape* shape : shapes) {
		if (shape->isSelected()) count += 1;
	}

	return count;
}

// 모든 도형 그리기 함수
void CMyShapeManager::drawShapes(CDC* dc) {
	for (CMyShape* shape : shapes) {
		shape->draw(dc); // 저장된 도형 그리기

		// 선택된 도형에 대해 점선 테두리 표시
		if (shape->isSelected()) {
			CPen pen(PS_DOT, 1, RGB(255, 0, 0));
			CPen* oldPen = dc->SelectObject(&pen);
			CBrush* oldBrush = (CBrush*)dc->SelectStockObject(NULL_BRUSH);
			CRect rect = shape->getBoundingBox();

			rect.InflateRect(5, 5); // 여백 추가
			dc->Rectangle(rect);

			dc->SelectObject(oldPen);
			dc->SelectObject(oldBrush);
		}
	}

	// 마우스로 현재 그리고 있는 도형 그리기
	if (drawingShape) drawingShape->draw(dc);

	// 선택 모드에서 드래그 중인 영역 표시
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

// 전체 도형 선택 해제 함수
void CMyShapeManager::clearShapeSelection() {
	for (CMyShape* shape : shapes) shape->setSelected(false);
}

// 그리기 모드 변경 함수
void CMyShapeManager::setDrawingMode(DrawingMode mode) {
	TRACE("[CMyShapeMananger] Transitioning drawingMode from %d to %d\n", drawingMode, mode);

	// 선택 모드에서 벗어나는 경우 모든 도형 선택 해제
	if (drawingMode == DrawingMode::Select && mode != DrawingMode::Select) clearShapeSelection();
	drawingMode = mode;
}

// 그리기 타입 변경 함수
void CMyShapeManager::setDrawingType(DrawingType type) {
	TRACE("[CMyShapeMananger] Transitioning drawingType from %d to %d\n", drawingType, type);
	drawingType = type;
}

// 마우스 왼쪽 버튼 누름 처리 함수
void CMyShapeManager::handleMouseLeftDown(CPoint point) {
	TRACE("[CMyShapeMananger] handleMouseLeftDown\n");
	drawStartPoint = point;
	drawEndPoint = point;

	if (drawingMode == DrawingMode::Shape) {
		// 그리기 타입에 따른 새 도형 객체 생성
		switch (drawingType) {
			case DrawingType::Rectangle: drawingShape = new CMyRectangle(); break;
			case DrawingType::Circle: drawingShape = new CMyCircle(); break;
			case DrawingType::Curve: drawingShape = new CMyCurve(); break;
			case DrawingType::Star: drawingShape = new CMyStar(); break;
		}
	} else if (drawingMode == DrawingMode::Select) {
		if (!isShiftPressed) clearShapeSelection(); // Shift키가 눌린 상태가 아니면 선택 초기화 (다중 선택 기능)
		isSelecting = true;

		// 역순 탐색하여 가장 위에 있는 도형 선택
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

	// 초기 상태 업데이트
	if (drawingShape) drawingShape->update(drawStartPoint, drawEndPoint);
}

// 마우스 이동 처리 함수
void CMyShapeManager::handleMouseMove(CPoint point) {
	if (drawingShape && drawingMode == DrawingMode::Shape) {
		// 도형 크기 업데이트
		drawingShape->update(drawStartPoint, point);
	} else if (drawingMode == DrawingMode::Select && isDragging) {
		CPoint offset = point - drawEndPoint;

		// 선택된 도형들 offset만큼 이동
		for (CMyShape* shape : shapes) {
			if (shape->isSelected()) shape->moveBy(offset);
		}
	}

	drawEndPoint = point;
}

// 마우스 왼쪽 버튼 떼기 처리 함수
void CMyShapeManager::handleMouseLeftUp(CPoint point) {
	TRACE("[CMyShapeMananger] handleMouseLeftUp\n");
	drawEndPoint = point;

	// 그리기 종료
	if (drawingShape && drawingMode == DrawingMode::Shape) {
		drawingShape->update(drawStartPoint, drawEndPoint);
		addShape(drawingShape); // 도형 목록에 추가
		drawingShape = nullptr;
	} else if (drawingMode == DrawingMode::Select) {
		if (isSelecting) {
			isSelecting = false;

			CRect selectionRect(drawStartPoint, drawEndPoint);
			selectionRect.NormalizeRect(); // 드래그 영역 정규화 (좌표 꼬임 방지)

			for (CMyShape* shape : shapes) {
				// 선택 영역 내 완전히 포함된 도형만 선택 처리
				if (selectionRect.PtInRect(shape->getBoundingBox().TopLeft()) && selectionRect.PtInRect(shape->getBoundingBox().BottomRight())) {
					shape->setSelected(true);
				}
			}
		}

		isDragging = false;
	}
}

// 선택된 도형들을 맨 앞으로 이동시키는 함수
void CMyShapeManager::bringSelectedToFront() {
	std::vector<CMyShape*> selectedShapes;

	// 선택된 도형 추출 및 제거
	for (auto& it = shapes.begin(); it != shapes.end();) {
		CMyShape* shape = *it;

		if (shape->isSelected()) {
			selectedShapes.push_back(shape);
			it = shapes.erase(it);
		} else {
			it += 1;
		}
	}

	// 목록 제일 뒤에 추가(= 화면상 맨 앞)
	shapes.insert(shapes.end(), selectedShapes.begin(), selectedShapes.end());
}

// 선택된 도형들을 맨 뒤로 이동시키는 함수
void CMyShapeManager::bringSelectedToBack() {
	std::vector<CMyShape*> selectedShapes;

	// 선택된 도형 추출 및 제거
	for (auto& it = shapes.begin(); it != shapes.end();) {
		CMyShape* shape = *it;

		if (shape->isSelected()) {
			selectedShapes.push_back(shape);
			it = shapes.erase(it);
		} else {
			it += 1;
		}
	}

	// 목록 제일 앞에 추가(= 화면상 맨 뒤)
	shapes.insert(shapes.begin(), selectedShapes.begin(), selectedShapes.end());
}

// 선택된 도형들을 그룹으로 묶는 함수
void CMyShapeManager::groupSelectedShapes() {
	CMyGroup* group = new CMyGroup();

	for (auto& it = shapes.begin(); it != shapes.end();) {
		CMyShape* shape = *it;

		if (shape->isSelected()) {
			group->addChildren(shape); // 그룹에 현재 도형 추가
			it = shapes.erase(it); // 전체 도형 목록에서 제거
		} else {
			it += 1;
		}
	}

	group->setSelected(true);
	shapes.push_back(group); // 그룹을 도형 목록에 추가
}

// 선택된 도형들에 대한 그룹을 해제하는 함수
void CMyShapeManager::ungroupSelectedShapes() {
	std::vector<CMyShape*> toUngroup;

	// 선택된 그룹 추출
	for (auto& it = shapes.begin(); it != shapes.end();) {
		CMyShape* shape = *it;
		CMyGroup* group = dynamic_cast<CMyGroup*>(shape); // dynamic_cast로 그룹인지 확인

		if (group && group->isSelected()) {
			toUngroup.push_back(shape);
			it = shapes.erase(it); // 그룹 삭제
		} else {
			it += 1;
		}
	}

	// 자식 도형들을 원래 도형 목록에 다시 추가
	for (CMyShape* shape : toUngroup) {
		CMyGroup* group = dynamic_cast<CMyGroup*>(shape);

		for (CMyShape* child : group->getChildren()) {
			child->setSelected(true);
			shapes.push_back(child);
		}

		delete group;
	}
}
