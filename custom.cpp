#include<glut.h>
#include<algorithm>
#include "custom.h"
#include<iostream>
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

// Bresenham Line Algorithm
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

//inline GLint round(const float a) { return static_cast<GLint> (a + 0.5); }

// Midpoint Ellipse Algorithm
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

void BoundaryFill(const ColorRGB& fillColor, const ColorRGB& boundaryColor, GLint x, GLint y)
{
	//std::cout << "(" << x << ", " << y << ")" << '\n';
	if (x < 0 || y < 0 || x > 600 || y > 500)
	{
		return;
	}
	GLfloat* pixel{ new GLfloat[4] };
	//GLfloat pixel[4]{};
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixel);

	/*for (auto color : *pixel)
	{
		std::cout << color << "\n";
	}*/
	if ((
		pixel[0] == fillColor.r &&
		pixel[1] == fillColor.g &&
		pixel[2] == fillColor.b
		) || (
			pixel[0] == boundaryColor.r &&
			pixel[1] == boundaryColor.g &&
			pixel[2] == boundaryColor.b
			)
		)

	{
		delete[] pixel;
		pixel = nullptr;
		return;
	}
	else
	{
		glColor3f(fillColor.r, fillColor.g, fillColor.b);
		glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();
		glFlush();
		BoundaryFill(fillColor, boundaryColor, x - 1, y);
		//BoundaryFill(fillColor, boundaryColor, x + 1, y);
		BoundaryFill(fillColor, boundaryColor, x, y - 1);
		BoundaryFill(fillColor, boundaryColor, x, y + 1);

		delete[] pixel;
		pixel = nullptr;
		return;
	}
}

void BoundaryFillRemaining(const ColorRGB& fillColor, const ColorRGB& boundaryColor, GLint x, GLint y)
{
	//std::cout << "Hello from 2" << '\n';
	if (x < 0 || y < 0 || x > 600 || y > 500)
	{
		return;
	}
	GLfloat* pixel{ new GLfloat[4] };
	//GLfloat pixel[4]{};
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixel);

	/*for (auto color : *pixel)
	{
		std::cout << color << "\n";
	}*/
	if ((
		pixel[0] == fillColor.r &&
		pixel[1] == fillColor.g &&
		pixel[2] == fillColor.b
		) || (
			pixel[0] == boundaryColor.r &&
			pixel[1] == boundaryColor.g &&
			pixel[2] == boundaryColor.b
			)
		)

	{
		delete[] pixel;
		pixel = nullptr;
		return;
	}
	else
	{
		glColor3f(fillColor.r, fillColor.g, fillColor.b);
		glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();
		glFlush();
		//BoundaryFill(fillColor, boundaryColor, x - 1, y);
		BoundaryFillRemaining(fillColor, boundaryColor, x + 1, y);
		BoundaryFillRemaining(fillColor, boundaryColor, x, y - 1);
		BoundaryFillRemaining(fillColor, boundaryColor, x, y + 1);

		delete[] pixel;
		pixel = nullptr;
		return;
	}
}

void translatePolygon(std::vector<WCPoint>& vertList, GLfloat tx, GLfloat ty)
{
	for (auto& eachVert : vertList)
	{
		eachVert.x += tx;
		eachVert.y += ty;
	}
	glBegin(GL_POLYGON);
	for (auto& eachVert : vertList)
	{
		glVertex2f(eachVert.x, eachVert.y);
	}
	glEnd();
	glFlush();
}

void rotatePolygon(std::vector<WCPoint> vertList, WCPoint pivot, GLdouble theta)
{
	std::vector<WCPoint> vertsRotate{};
	for (auto& eachVert : vertList)
	{
		GLfloat xRotate = pivot.x + (eachVert.x - pivot.x) * cos(theta) - (eachVert.y - pivot.y) * sin(theta);
		GLfloat yRotate = pivot.x + (eachVert.x - pivot.x) * sin(theta) + (eachVert.y - pivot.y) * cos(theta);
		vertsRotate.push_back({ xRotate, yRotate });
	}

	glBegin(GL_POLYGON);
	for (auto& eachVert : vertsRotate)
	{
		glVertex2f(eachVert.x, eachVert.y);
	}
	glEnd();
	glFlush();
}

void scalePolygon(std::vector<WCPoint> vertList, WCPoint fixedPoint, GLfloat sx, GLfloat sy)
{
	std::vector<WCPoint> scaledVerts;

	for (auto& eachVert : vertList)
	{
		GLfloat scaledX = eachVert.x * sx + fixedPoint.x * (1 - sx);
		GLfloat scaledY = eachVert.y * sy + fixedPoint.y * (1 - sy);
	}

	glBegin(GL_POLYGON);
	for (auto& eachVert : scaledVerts)
	{
		glVertex2f(eachVert.x, eachVert.y);
	}
	glEnd();
	glFlush();
}