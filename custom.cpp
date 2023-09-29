#include<glut.h>
#include<algorithm>
#include "custom.h"

// The DDA Line Algorithm
void DDALine(GLint x0, GLint y0, GLint x1, GLint y1)
{
	GLint dx{ x1 - x0 };
	GLint dy{ y1 - y0 };
	GLint steps{ std::max({abs(dx), abs(dy)}) };

	GLfloat xIncrement = static_cast<GLfloat> (dx / steps);
	GLfloat yIncrement = static_cast<GLfloat> (dy / steps);

	glBegin(GL_POINTS);
	GLfloat x = x0, y = y0;
	glVertex2i(x, y);
	for (int k{ 0 }; k < steps; k++)
	{
		x += xIncrement;
		y += yIncrement;
		glVertex2f(x, y);
	}
	glEnd();
}

void BresenhamLine(GLint x0, GLint y0, GLint x1, GLint y1)
{
	GLint dy{ abs(y1 - y0) };
	GLint dx{ abs(x1 - x0) };

	GLint p = 2 * dy - dx;
	GLint two_deltaY = 2 * dy;
	GLint twoDeltaY_minus_twoDeltaX = 2 * dy - 2 * dx;

	GLint x, y;
	if (x0 > x1)
	{
		x = x1;
		y = y1;
		x1 = x0;
	}
	else
	{
		x = x0;
		y = y0;
	}

	glBegin(GL_POINTS);
	glVertex2i(x, y);

	while (x < x1)
	{
		x++;
		if (p < 0)
		{
			p += two_deltaY;
		}
		else
		{
			y += 1;
			p += twoDeltaY_minus_twoDeltaX;
		}
		glVertex2i(x, y);
	}
	glEnd();
}

// Midpoint Circle Algorithm
void MPCircle(GLint xc, GLint yc, GLint r)
{
	GLint p = 1 - r;
	Point2D circPoint(0, r);
	void circPlotPoints(GLint, GLint, Point2D);
	circPlotPoints(xc, yc, circPoint);

	while (circPoint.getX() < circPoint.getY())
	{
		circPoint.incrementX();
		if (p < 0)
		{
			p += 2 * circPoint.getX() + 1;
		}
		else
		{
			circPoint.decrementY();
			p += 2 * (circPoint.getX() - circPoint.getY()) + 1;
		}
		circPlotPoints(xc, yc, circPoint);
	}
}

void circPlotPoints(GLint xc, GLint yc, Point2D point)
{
	glBegin(GL_POINTS);
	glVertex2i(xc + point.getX(), yc + point.getY());
	glVertex2i(xc - point.getX(), yc + point.getY());
	glVertex2i(xc + point.getX(), yc - point.getY());
	glVertex2i(xc - point.getX(), yc - point.getY());
	glVertex2i(xc + point.getY(), yc + point.getX());
	glVertex2i(xc - point.getY(), yc + point.getX());
	glVertex2i(xc + point.getY(), yc - point.getX());
	glVertex2i(xc - point.getY(), yc - point.getX());
	glEnd();
}

inline GLint round(const float a) { return static_cast<GLint> (a + 0.5); }

void MPEllipse(GLint rx, GLint ry, GLint xc, GLint yc)
{
	GLint x{ 0 };
	GLint y{ ry };

	GLint rx2{ rx * rx };
	GLint ry2{ ry * ry };
	GLint two_rx2{ 2 * rx2 };
	GLint two_ry2{ 2 * ry2 };

	GLint px{ 0 };
	GLint py{ two_rx2 * y };

	GLint p;

	void ellipsePlotPoints(GLint, GLint, GLint, GLint);
	ellipsePlotPoints(xc, yc, x, y);

	// Region 1
	p = round(ry2 - (rx2 * ry) + (0.25 * rx2));
	while (px < py)
	{
		x++;
		px += two_ry2;
		if (p < 0)
		{
			p += ry2 + px;
		}
		else
		{
			y--;
			py -= two_rx2;
			p += ry2 + px - py;
		}
		ellipsePlotPoints(xc, yc, x, y);
	}

	// Region 2
	p = round(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
	while (y > 0)
	{
		y--;
		py -= two_rx2;
		if (p > 0)
		{
			p += rx2 - py;
		}
		else
		{
			x++;
			px += two_ry2;
			p += rx2 - py + px;
		}
		ellipsePlotPoints(xc, yc, x, y);
	}
}

void ellipsePlotPoints(GLint xc, GLint yc, GLint x, GLint y)
{
	glBegin(GL_POINTS);
	glVertex2i(xc + x, yc + y);
	glVertex2i(xc - x, yc + y);
	glVertex2i(xc + x, yc - y);
	glVertex2i(xc - x, yc - y);
	glEnd();
}