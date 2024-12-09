#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include <math.h>
//#pragma comment(lib, "GL\\glut32.lib")

#define RAD(a) (3.14159265358979323846*a/180)

CGLRenderer::CGLRenderer(void)
{
	m_hrc = NULL;
	rWorld = 7;
	trenX = 0;
	trenY = 0;
	calculateView(trenX, trenY);
	centerx = 0;
	centery = 0;
	centerz = 0;
	upx = 0;
	upy = 1;
	upz = 0;
	rotAngle = 45;
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glClearColor(0.47, 0, 0, 1.0);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	glTranslated(0, -2, 0);
	DrawAxis(10);
	DrawGrid(5, 5, 10, 10);
	glPushMatrix();
	{
		DrawFigure(rotAngle);
	}
	glPopMatrix();
	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glViewport(0, 0, w, h);
	double aspect = (double)w / double(h);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::calculateView(int x, int y)
{
	trenY += x * 0.2;
	trenX += y * 0.2;
	double ugaoX = trenX * 3.1415 / 180;
	double ugaoY = trenY * 3.1415 / 180;

	eyex = rWorld * cos(ugaoX) * cos(ugaoY);
	eyey = rWorld * sin(ugaoX);
	eyez = rWorld * cos(ugaoX) * sin(ugaoY);
}

void CGLRenderer::changeRotAngle(double x)
{
	rotAngle += x;
}

void CGLRenderer::changeLook(double x)
{
	if (x > 0 && rWorld < 10)
	{
		rWorld += x;
		calculateView(0, 0);
	}
	else if (x < 0 && rWorld > 4)
	{
		rWorld += x;
		calculateView(0, 0);
	}
}

void CGLRenderer::DrawAxis(double width)
{
	glLineWidth(2);
	glPointSize(10);
	glBegin(GL_LINES);
	{
		glColor3f(1.0, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);

		glColor3f(0, 1.0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, width, 0);

		glColor3f(0, 0, 1.0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, width);
	}
	glEnd();
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	float a = 360 / nSegAlpha;
	float b = 360 / nSegBeta;
	float x1, y1, z1;
	float x2, y2, z2;

	glBegin(GL_QUAD_STRIP);
	{
		for (float beta = b; beta <= 180; beta += b)
		{
			for (float alpha = 0; alpha <= 360; alpha += a)
			{
				x1 = r * cos(RAD(alpha)) * cos(RAD(beta));
				y1 = r * sin(RAD(alpha));
				z1 = r * cos(RAD(alpha)) * sin(RAD(beta));

				x2 = r * cos(RAD(alpha)) * cos(RAD(beta) + RAD(b));
				y2 = r * sin(RAD(alpha));
				z2 = r * cos(RAD(alpha)) * sin(RAD(beta) + RAD(b));
				glVertex3f(x1, y1, z1);
				glVertex3d(x2, y2, z2);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{
	int angleStep = 360 / nSeg;
	double x, z;
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3d(0, 0, 0);
		for (int alpha = 0; alpha <= 360; alpha += angleStep)
		{
			x = r1 * cos(RAD(alpha));
			z = r1 * sin(RAD(alpha));
			glVertex3d(x, 0, z);
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3d(0, h, 0);
		for (int alpha = 0; alpha <= 360; alpha += angleStep)
		{
			x = r2 * cos(RAD(alpha));
			z = r2 * sin(RAD(alpha));
			glVertex3d(x, h, z);
		}
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	{
		glVertex3d(0, h, 0);
		for (int alpha = 0; alpha <= 360; alpha += angleStep)
		{
			x = r1 * cos(RAD(alpha));
			z = r1 * sin(RAD(alpha));
			glVertex3d(x, 0, z);
			x = r2 * cos(RAD(alpha));
			z = r2 * sin(RAD(alpha));
			glVertex3d(x, h, z);
		}
	}
	glEnd();
}

void CGLRenderer::DrawCone(double h, double r, int nSeg)
{
	int angleStep = 360 / nSeg;
	double x, z;
	glBegin(GL_TRIANGLE_FAN);
	{
		for (int alpha = 0; alpha <= 360; alpha += angleStep)
		{
			x = r * cos(RAD(alpha));
			z = r * sin(RAD(alpha));
			glVertex3d(x, 0, z);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3d(0, h, 0);
		for (int alpha = 0; alpha <= 360; alpha += angleStep)
		{
			x = r * cos(RAD(alpha));
			z = r * sin(RAD(alpha));
			glVertex3d(x, 0, z);
		}
	}
	glEnd();
}

void CGLRenderer::DrawGrid(double width, double height, int nSegW, int nSegH)
{
	double widthStep = width / nSegW;
	double heightStep = height / nSegH;

	glBegin(GL_LINES);
	{
		glColor3d(1, 1, 1);
		for (double i = -height / 2; i <= height / 2; i += heightStep)
		{
			glVertex3d(i, 0, -width / 2);
			glVertex3d(i, 0, width / 2);
		}
		for (double i = -width / 2; i <= width / 2; i += widthStep)
		{
			glVertex3d(-height / 2, 0, i);
			glVertex3d(height / 2, 0, i);
		}
	}
	glEnd();
}

void CGLRenderer::DrawFigure(double angle)
{
	// Crtanje vaze
	glColor3d(0, 0.8, 0);
	DrawCylinder(0.8, 0.5, 0.6, 100);
	glTranslated(0, 0.8, 0);
	DrawCylinder(0.25, 0.7, 0.7, 100);

	// Prva kupa kaktusa
	glColor3d(1, 0.6, 0);
	glTranslated(0, 0.25, 0);
	DrawCone(0.7, 0.2, 100);

	// Prva lopta kaktusa
	glColor3d(1, 1, 1);
	glTranslated(0, 0.8, 0);
	DrawSphere(0.1, 100, 100);

	// Prva desno rotirana strana
	glPushMatrix();
	{
		glRotated(-45, 1, 0, 0);
		glTranslated(0, 0.1, 0);
		glColor3d(1, 0.6, 0);
		DrawCylinder(0.7, 0.2, 0.2, 100);
		glTranslated(0, 0.8, 0);
		glColor3d(1, 1, 1);
		DrawSphere(0.1, 100, 100);
	}
	glPopMatrix();

	// Prva levo zarotirana strana 
	glPushMatrix();
	{
		glRotated(rotAngle, 1, 0, 0);
		glTranslated(0, 0.1, 0);
		glColor3d(1, 1, 0);
		DrawCone(0.7, 0.2, 100);
		glTranslated(0, 0.8, 0);
		glColor3d(1, 1, 1);
		DrawSphere(0.1, 100, 100);

		// Desno
		glPushMatrix();
		{
			glRotated(-45, 1, 0, 0);
			glTranslated(0, 0.1, 0);
			glColor3d(1, 0.6, 0);
			DrawCone(0.7, 0.2, 100);
			glTranslated(0, 0.8, 0);
			glColor3d(1, 1, 1);
			DrawSphere(0.1, 100, 100);
		}
		glPopMatrix();

		// Levo
		glPushMatrix();
		{
			glRotated(45, 1, 0, 0);
			glTranslated(0, 0.1, 0);
			glColor3d(1, 0.6, 0);
			DrawCylinder(0.7, 0.2, 0.2, 100);
			glTranslated(0, 0.8, 0);
			glColor3d(1, 1, 1);
			DrawSphere(0.1, 100, 100);
			glTranslated(0, 0.1, 0);
			glColor3d(1, 0.6, 0);
			DrawCylinder(0.7, 0.2, 0.2, 100);
			glTranslated(0, 0.8, 0);
			glColor3d(1, 1, 1);
			DrawSphere(0.1, 100, 100);
		}
		glPopMatrix();

		// Gore
		glTranslated(0, 0.1, 0);
		glColor3d(1, 0.6, 0);
		DrawCylinder(0.7, 0.2, 0.2, 100);
		glTranslated(0, 0.8, 0);
		glColor3d(1, 1, 1);
		DrawSphere(0.1, 100, 100);
		glTranslated(0, 0.1, 0);
		glColor3d(1, 0.6, 0);
		DrawCylinder(0.7, 0.2, 0.2, 100);
		glTranslated(0, 0.8, 0);
		glColor3d(1, 1, 1);
		DrawSphere(0.1, 100, 100);
	}
	glPopMatrix();

	glTranslated(0, 0.1, 0);
	glColor3d(1, 0.6, 0);
	DrawCylinder(0.7, 0.2, 0.2, 100);
	glTranslated(0, 0.8, 0);
	glColor3d(1, 1, 1);
	DrawSphere(0.1, 100, 100);

	// Druge zarotirane strane
	for (int angle = -45; angle <= 45; angle += 90)
	{
		glPushMatrix();
		{
			glRotated(angle, 1, 0, 0);
			glTranslated(0, 0.1, 0);
			glColor3d(1, 0.6, 0);
			DrawCone(0.7, 0.2, 100);
			glTranslated(0, 0.8, 0);
			glColor3d(1, 1, 1);
			DrawSphere(0.1, 100, 100);
		}
		glPopMatrix();
	}
}
