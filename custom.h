#pragma once
#include<glut.h>
#include<algorithm>
void DDALine(GLint, GLint, GLint, GLint);
void BresenhamLine(GLint, GLint, GLint, GLint);
void MPCircle(GLint, GLint, GLint);
void MPEllipse(GLint, GLint, GLint, GLint);


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
