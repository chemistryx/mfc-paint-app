#ifndef __C_MYSHAPEMANAGER_H__
#define __C_MYSHAPEMANAGER_H__

#include <vector>
#include "CMyRectangle.h"
#include "CMyCircle.h"
#include "CMyCurve.h"
#include "CMyStar.h"
#include "CMyGroup.h"

enum class DrawingMode { Shape, Select };
enum class DrawingType { None, Rectangle, Circle, Curve, Star };

class CMyShapeManager {
	std::vector<CMyShape*> shapes;
	DrawingMode drawingMode;
	DrawingType drawingType;

	CPoint drawStartPoint;
	CPoint drawEndPoint;

	CMyShape* drawingShape;
	bool isSelecting;
	bool isDragging;
	bool isShiftPressed;
public:
	CMyShapeManager();
	~CMyShapeManager();

	void addShape(CMyShape* shape);
	void removeShape(CMyShape* shape);
	void removeShapes(bool onlySelected = false);

	int getShapesCount() { return shapes.size(); }

	int getSelectedCount();

	void drawShapes(CDC* dc);

	void clearShapeSelection();

	DrawingMode getDrawingMode() { return drawingMode; }
	void setDrawingMode(DrawingMode mode);

	DrawingType getDrawingType() { return drawingType; }
	void setDrawingType(DrawingType type);

	void handleMouseLeftDown(CPoint point);
	void handleMouseMove(CPoint point);
	void handleMouseLeftUp(CPoint point);

	void setShiftPressed(bool value) { isShiftPressed = value; }

	void bringSelectedToFront();
	void bringSelectedToBack();

	void groupSelectedShapes();
	void ungroupSelectedShapes();
};

#endif
