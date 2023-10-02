#pragma once
#include<glut.h>
#include<algorithm>
#include<vector>

struct ColorRGB;
struct WCPoint;

void DDALine(GLint, GLint, GLint, GLint);
void BresenhamLine(GLint, GLint, GLint, GLint);
void MPCircle(GLint, GLint, GLint);
void MPEllipse(GLint, GLint, GLint, GLint);
void BoundaryFill(const ColorRGB&, const ColorRGB&, GLint, GLint);
void BoundaryFillRemaining(const ColorRGB&, const ColorRGB&, GLint, GLint);
void getPolygonPoints(GLfloat, GLfloat, const ColorRGB&, std::vector<WCPoint>&);
void getPolygonPointsRest(GLfloat, GLfloat, const ColorRGB&, std::vector<WCPoint>&);
void translatePolygon(std::vector<WCPoint>&, GLfloat, GLfloat);
void rotatePolygon(std::vector<WCPoint>, WCPoint, GLdouble);
void scalePolygon(std::vector<WCPoint>, WCPoint, GLfloat, GLfloat);
void scatterPlot(std::string filename, WCPoint winMin, WCPoint winMax, int padding);
void csLineClip(WCPoint winMin, WCPoint winMax, WCPoint point_1, WCPoint point_2);


struct WCPoint
{
	GLfloat x, y;
};
class Point2D
{
private:
	GLint m_x, m_y;
public:
	Point2D(GLint x = 0, GLint y = 0) : m_x{ x }, m_y{ y } { };
	GLint getX() { return m_x; }
	GLint getY() { return m_y; }
	void incrementX() { m_x++; }
	void decrementY() { m_y--; }
};

struct ColorRGB
{
	GLfloat r, g, b;
};