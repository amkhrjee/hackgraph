#include<glut.h>
#include<bitset>
#include <string>
#include<sstream>
#include <fstream>
#include<algorithm>
#include<fmt/format.h>
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
	glVertex2f(x, y);
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

std::bitset<4> encode(WCPoint point, WCPoint  winMin, WCPoint winMax)
{
	std::bitset<4> code{ 0b0000 };

	if (point.x < winMin.x)
		code.set(0);
	if (point.x > winMax.x)
		code.set(1);
	if (point.y < winMin.y)
		code.set(2);
	if (point.y > winMax.y)
		code.set(3);

	return code;
}

WCPoint getClippedEndPoint(WCPoint point, WCPoint  winMin, WCPoint winMax, GLfloat slope, GLfloat intercept)
{
	using namespace std;
	bitset<4> code{ encode(point, winMin, winMax) };
	if (code == bitset<4>{0b0000})
	{
		return point;
	}
	else if (code == bitset<4>{ 0b1000 })
	{
		//  y = winMax.y
		GLfloat x{ (winMax.y - intercept) / slope };
		return { x, winMax.y };
	}
	else if (code == bitset<4> {0b0100})
	{
		// y = winMin.y
		GLfloat x{ (winMin.y - intercept) / slope };
		return { x, winMin.y };
	}
	else if (code == bitset<4>{ 0b0010 })
	{
		// x = winMax.x
		GLfloat y{ slope * winMax.x + intercept };
		return { winMax.x, y };
	}
	else if (code == bitset<4> {0b0001})
	{
		// x = winMin.x
		GLfloat y{ slope * winMin.x + intercept };
		return { winMin.x, y };
	}
	else if (code == bitset<4> {0b1001})
	{
		GLfloat x{ (winMax.y - intercept) / slope };
		if (x <= winMax.x && x >= winMin.x)
			return { x, winMax.y };
		else
		{
			GLfloat y{ slope * winMin.x + intercept };
			if (y <= winMax.y && y >= winMin.y)
				return { winMin.x, y };
		}
	}
	else if (code == bitset<4> {0b1010})
	{
		GLfloat x{ (winMax.y - intercept) / slope };
		if (x <= winMax.x && x >= winMin.x)
			return { x, winMax.y };
		else
		{
			GLfloat y{ slope * winMax.x + intercept };
			if (y <= winMax.y && y >= winMin.y)
				return { winMax.x, y };
		}
	}
	else if (code == bitset<4> {0b0101})
	{
		GLfloat x{ (winMin.y - intercept) / slope };
		if (x <= winMax.x && x >= winMin.x)
			return { x, winMin.y };
		else
		{
			GLfloat y{ slope * winMin.x + intercept };
			if (y <= winMax.y && y >= winMin.y)
				return { winMin.x, y };
		}
	}
	else if (code == bitset<4> {0b0110})
	{
		GLfloat x{ (winMin.y - intercept) / slope };
		if (x <= winMax.x && x >= winMin.x)
			return { x, winMin.y };
		else
		{
			GLfloat y{ slope * winMax.x + intercept };
			if (y <= winMax.y && y >= winMin.y)
				return { winMax.x, y };
		}
	}
	else
	{
		return point;
	}
}

// Cohen Sutherland Line Clipping Algorithm
void csLineClip(WCPoint winMin, WCPoint winMax, WCPoint point_1, WCPoint point_2)
{
	GLfloat slope{ (point_2.y - point_1.y) / (point_2.x - point_1.x) };
	fmt::print("The slope is {}\n", slope, point_2.y, point_1.y, point_2.x, point_1.x);
	GLfloat intercept{ point_1.y - (slope * point_1.x) };
	fmt::print("The intercept is {}\n", intercept);


	std::bitset<4> code_1{ encode(point_1, winMin, winMax) };
	fmt::print("Code for ({}, {}): {}\n", point_1.x, point_1.y, code_1.to_string());
	std::bitset<4> code_2{ encode(point_2, winMin, winMax) };
	fmt::print("Code for ({}, {}): {}\n", point_2.x, point_2.y, code_2.to_string());

	if (code_1 == std::bitset<4> {0b0000}&& code_2 == std::bitset<4> {0b0000})
	{
		fmt::print("Retaining entire line\n");
		// retain line
		glBegin(GL_LINE_STRIP);
		glVertex2f(point_1.x, point_1.y);
		glVertex2f(point_2.x, point_2.y);
		glEnd();
		glFlush();
	}
	else if ((code_1 & code_2) != std::bitset<4> {0b0000})
	{
		// discard line
		return;
	}
	else
	{
		fmt::print("Performing Clipping\n");
		// perform line clipping
		WCPoint clipped_point_1{ getClippedEndPoint(point_1, winMin, winMax, slope, intercept) };
		WCPoint clipped_point_2{ getClippedEndPoint(point_2, winMin, winMax, slope, intercept) };
		glBegin(GL_LINE_STRIP);
		glVertex2f(clipped_point_1.x, clipped_point_1.y);
		glVertex2f(clipped_point_2.x, clipped_point_2.y);
		glEnd();
		glFlush();
	}
}

void scatterPlot(std::string filename, WCPoint winMin, WCPoint winMax, int padding)
{
	std::vector<int> dates{};
	std::vector<float> prices{};
	std::string line;
	std::ifstream data{ filename };
	if (!data)
	{
		// Print an error and exit
		fmt::print("Uh oh, data could not be opened for reading!\n");
		return;
	}
	std::getline(data, line);

	while (std::getline(data, line))
	{
		int date;
		float price;
		std::stringstream dataline{ line };
		dataline >> date >> price;
		dates.push_back(date);
		prices.push_back(price);
	}

	// There is always 30 days
	float dategap{ ((winMax.x - winMin.x) - 2 * padding) / 30 };
	float pricegap{ ((winMax.y - winMin.y) - 2 * padding) / prices.size() };

	glPointSize(4);
	glBegin(GL_POINTS);
	for (int k{ 0 }; k < dates.size(); k++)
	{
		glVertex2f(padding + k * dategap, prices.at(k));
	}
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(padding, padding);
	glVertex2i(padding, winMax.y - padding);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(padding, padding);
	glVertex2i(winMax.x - padding, padding);
	glEnd();
	for (int k{ 0 }; k < 30; k++)
	{
		glBegin(GL_LINE_STRIP);
		glVertex2i(padding + k * dategap, padding);
		glVertex2i(padding + k * dategap, padding - 10);
		glEnd();
	}
	for (int k{ 0 }; k < prices.size(); k++)
	{
		glBegin(GL_LINE_STRIP);
		glVertex2i(padding, padding + k * pricegap);
		glVertex2i(padding - 10, padding + k * pricegap);
		glEnd();
	}

	std::vector<int> xLabels{};
	for (auto date : dates)
	{
		if (date <= 9)
		{
			xLabels.push_back(0);
			xLabels.push_back(48 + date);
		}
		else
		{
			xLabels.push_back(48 + (date / 10));
			xLabels.push_back(48 + (date % 10));
		}
	}

	for (int k{ 0 }; k < prices.size(); k++)
	{
		glRasterPos2i(padding + k * dategap, padding - 20);
		for (int j{ 0 }; j < 2; j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, xLabels.at(2 * k + j));
	}

	std::vector<int> yLabelSteps{};
	int yLabel{ 0 };
	for (int k{ 0 }; k < prices.size(); k++)
	{
		yLabelSteps.push_back(yLabel);
		fmt::print("Y Label Step: {}\n", yLabel);
		yLabel += pricegap;
	}

	std::vector<int> yLabels{};
	for (auto yLabelStep : yLabelSteps)
	{
		if (yLabelStep <= 9)
		{
			yLabels.push_back(0);
			yLabels.push_back(0);
			yLabels.push_back(48 + yLabelStep);
		}
		else if (yLabelStep > 9 && yLabelStep <= 99)
		{
			yLabels.push_back(0);
			yLabels.push_back(48 + (yLabelStep / 10));
			yLabels.push_back(48 + (yLabelStep % 10));
		}
		else
		{
			yLabels.push_back(48 + (yLabelStep / 100));
			yLabelStep = yLabelStep % 100;
			yLabels.push_back(48 + (yLabelStep / 10));
			yLabels.push_back(48 + (yLabelStep % 10));
		}
	}
	fmt::print("Ylabels size: {}", yLabels.size());
	for (int k{ 0 }; k < yLabelSteps.size(); k++)
	{
		glRasterPos2i(padding - 40, (k * pricegap) + padding);
		for (int j{ 0 }; j < 3; j++)
		{
			fmt::print("Ylabels Index: {}\n", 3 * k + j);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, yLabels.at((3 * k) + j));

		}
	}

	std::vector<GLubyte> label{ 'R', 'e', 'l', 'i', 'a', 'n', 'c', 'e', ' ', 'S', 't', 'o', 'c', 'k', ' ', 'P', 'r', 'i', 'c', 'e', 's' };
	glRasterPos2i(250, 450);

	for (auto labelChar : label)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, labelChar);
	}
	glFlush();
	return;
}