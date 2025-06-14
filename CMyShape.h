#ifndef __C_MYSHAPE_H__
#define __C_MYSHAPE_H__

class CMyShape {
	CRect boundingBox;
	COLORREF fillColor;
	COLORREF outlineColor;
	bool selected;
public:
	CMyShape() {
		fillColor = generateRandomColor();
		outlineColor = RGB(GetRValue(fillColor) * 0.5, GetGValue(fillColor) * 0.5, GetBValue(fillColor) * 0.5);
		selected = false;
	}

	virtual ~CMyShape() = default;

	virtual void draw(CDC* dc) = 0;
	virtual void update(CPoint start, CPoint end) = 0;
	virtual void moveBy(CPoint delta) = 0;
	virtual bool contains(CPoint point) = 0;

	CRect getBoundingBox() { return boundingBox; }
	void setBoundingBox(CRect box) { boundingBox = box; }

	COLORREF getFillColor() { return fillColor; }
	COLORREF getOutlineColor() { return outlineColor;}

	COLORREF generateRandomColor() {
		int r = 128 + rand() % 128;
		int g = 128 + rand() % 128;
		int b = 128 + rand() % 128;

		return RGB(r, g, b);
	}

	bool isSelected() { return selected; }
	void setSelected(bool value) { selected = value; }
};

#endif

