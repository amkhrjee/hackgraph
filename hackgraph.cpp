#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<Windows.h>
#include<glut.h>
#include "custom.h"

GLsizei winWidth = 600, winHeight = 500;
GLint xRaster = 25, yRaster = 150;

GLubyte label[]{ 'J', 'a', 'n', 'F', 'e', 'b', 'M', 'a', 'r',
					'A', 'p', 'r', 'M', 'a', 'y', 'J', 'u', 'n',
					'J', 'u', 'l', 'A', 'u', 'g', 'S', 'e', 'p',
					'O', 'c', 't', 'N', 'o', 'v', 'D', 'e', 'c' };

GLint dataValue[]{ 420, 342, 324, 310, 262, 185,
					190, 196, 217, 240, 312, 438 };


void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}

void displayMonths()
{
	GLint month, k;
	glColor3f(0.0, 0.0, 0.0);
	xRaster = 20;
	for (month = 0; month < 12; month++)
	{
		glRasterPos2i(xRaster, yRaster);
		for (k = 3 * month; k < month * 3 + 3; k++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, label[k]);
		}
		xRaster += 50;
	}
}

void barChart()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	for (GLint k{ 0 }; k < 12; k++)
	{
		glRecti(20 + k * 50, 165, 40 + k * 50, dataValue[k]);
	}

	displayMonths();
	glFlush();
}

void lineSegment()
{
	GLint k;
	GLint x = 30;

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_STRIP);
	for (k = 0; k < 12; k++)
	{
		glVertex2i(x + k * 50, dataValue[k]);
	}
	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	for (k = 0; k < 12; k++)
	{
		glRasterPos2i(xRaster + k * 50, dataValue[k] - 4);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '*');
	}

	displayMonths();
	glFlush();
}

void displayChessboard()
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLint boxWidth = 50;
	GLint startingPointX = 100;
	GLint startingPointY = 50;
	for (GLint k{ 0 }; k < 8; k++)
	{
		GLint x1 = startingPointX + boxWidth * k;
		for (GLint j{ 0 }; j < 8; j++)
		{
			GLint y1 = startingPointY + boxWidth * j;
			if ((k + j) % 2)
			{
				glColor3f(0.0, 1.0, 1.0);
			}
			else
			{
				glColor3f(0.0, 0.0, 0.0);
			}
			glRecti(x1, y1, x1 + boxWidth, y1 + boxWidth);
		}
	}
	glFlush();
}

void displayCustom()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 1.0);
	//BresenhamLine(50, 50, 410, 410);
	//DDALine(10, 10, 210, 210);
	//MPCircle(300, 200, 100);
	//MPEllipse(100, 150, 300, 250);
	glBegin(GL_POLYGON);
	glVertex2f(200, 100);
	glVertex2f(250, 50);
	glVertex2f(150, 50);
	glEnd();
	glFlush();

	std::vector<WCPoint> vertList{ {200,100 },
									{250, 50},
									{150, 50} };

	scalePolygon(vertList, { 200, 75 }, 100, 100);
	//translatePolygon(vertList, 200, 200);
	rotatePolygon(vertList, { 200, 75 }, 180);
}

void boundaryFillAssignment()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);
	// Face
	MPCircle(300, 250, 150);
	//Left Eye
	glBegin(GL_POLYGON);
	glVertex2i(200, 300);
	glVertex2i(250, 300);
	glVertex2i(250, 350);
	glVertex2i(200, 350);
	glEnd();
	// Right Eye
	glBegin(GL_POLYGON);
	glVertex2i(350, 300);
	glVertex2i(400, 300);
	glVertex2i(400, 350);
	glVertex2i(350, 350);
	glEnd();
	// Nose
	glBegin(GL_POLYGON);
	glVertex2i(300, 240);
	glVertex2i(320, 200);
	glVertex2i(280, 200);
	glEnd();
	//Mouth
	glRecti(250, 120, 350, 140);
	glFlush();

	// Performing Boundary Fill
	ColorRGB boundaryColor{ 0.0, 1.0, 0.0 };
	ColorRGB fillColor{ 0.6, 0.6, 0.6 };
	BoundaryFillRemaining(fillColor, boundaryColor, 300, 250);
	BoundaryFill(fillColor, boundaryColor, 299, 250);
	BoundaryFillRemaining(fillColor, boundaryColor, 300, 180);
	BoundaryFill(fillColor, boundaryColor, 299, 180);
	BoundaryFillRemaining(fillColor, boundaryColor, 300, 119);
	BoundaryFill(fillColor, boundaryColor, 299, 119);

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Experiments");
	// Anti-aliasing
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	init();
	glutDisplayFunc(displayCustom);
	glutReshapeFunc([](GLint newWidth, GLint newHeight) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, newWidth, 0.0, newHeight);
		glClear(GL_COLOR_BUFFER_BIT);
		});
	glutMainLoop();
}