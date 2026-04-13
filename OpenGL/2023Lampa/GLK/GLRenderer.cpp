#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include <math.h>
#define RAD(a) (3.1415*a/180)
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	yRotation = 0;
	xRotation = 0;
	zoom = -50;
	firstArmRot = 0;
	secondArmRot = 0;
	headRot = 0;
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

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	sideTex = LoadTexture("assets/side.jpg");
	bottomTex = LoadTexture("assets/bot.jpg");
	topTex = LoadTexture("assets/top.jpg");
	lampTex = LoadTexture("assets/lamp.jpg");
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslated(0, -20, zoom);
	glRotated(xRotation, 1.0, 0.0, 0.0);
	glRotated(yRotation, 0.0, 1.0, 0.0);

	// KOCKA
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glTranslated(0, 100, 0);
	DrawEnvCube(200);
	glTranslated(0, -100, 0);

	// KOORDINATNE OSE
	glDisable(GL_TEXTURE_2D);
	DrawAxis();

	// LAMPA
	glEnable(GL_TEXTURE_2D);
	DrawLamp();
	

	glFlush();
	SwapBuffers(pDC->m_hDC);
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures(1, &sideTex);
	glDeleteTextures(1, &bottomTex);
	glDeleteTextures(1, &topTex);
	glDeleteTextures(1, &lampTex);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::DrawEnvCube(double a)
{
	glPushMatrix();
	{
		// FRONT
		glBindTexture(GL_TEXTURE_2D, sideTex);
		glBegin(GL_QUADS);
		{
			glColor3f(1.0, 1.0, 1.0);

			glTexCoord2d(0.0, 0.0);
			glVertex3d(-a / 2, a / 2, -a / 2);

			glTexCoord2d(1.0, 0.0);
			glVertex3d(a / 2, a / 2, -a / 2);

			glTexCoord2d(1.0, 1.0);
			glVertex3d(a / 2, -a / 2, -a / 2);

			glTexCoord2d(0.0, 1.0);
			glVertex3d(-a / 2, -a / 2, -a / 2);
		}

		// LEFT
		glBegin(GL_QUADS);
		{
			glColor3f(1.0, 1.0, 1.0);

			glTexCoord2d(0.0, 0.0);
			glVertex3d(-a / 2, a / 2, a / 2);

			glTexCoord2d(1.0, 0.0);
			glVertex3d(-a / 2, a / 2, -a / 2);

			glTexCoord2d(1.0, 1.0);
			glVertex3d(-a / 2, -a / 2, -a / 2);

			glTexCoord2d(0.0, 1.0);
			glVertex3d(-a / 2, -a / 2, a / 2);
		}
		glEnd();

		// RIGHT
		glBegin(GL_QUADS);
		{
			glColor3f(1.0, 1.0, 1.0);

			glTexCoord2d(0.0, 0.0);
			glVertex3d(a / 2, a / 2, -a / 2);

			glTexCoord2d(1.0, 0.0);
			glVertex3d(a / 2, a / 2, a / 2);

			glTexCoord2d(1.0, 1.0);
			glVertex3d(a / 2, -a / 2, a / 2);

			glTexCoord2d(0.0, 1.0);
			glVertex3d(a / 2, -a / 2, -a / 2);
		}
		glEnd();

		// BACK
		glBegin(GL_QUADS);
		{
			glColor3f(1.0, 1.0, 1.0);

			glTexCoord2d(0.0, 0.0);
			glVertex3d(a / 2, a / 2, a / 2);

			glTexCoord2d(1.0, 0.0);
			glVertex3d(-a / 2, a / 2, a / 2);

			glTexCoord2d(1.0, 1.0);
			glVertex3d(-a / 2, -a / 2, a / 2);

			glTexCoord2d(0.0, 1.0);
			glVertex3d(a / 2, -a / 2, a / 2);
		}
		glEnd();

		// BOTTOM
		glBindTexture(GL_TEXTURE_2D, bottomTex);
		glBegin(GL_QUADS);
		{
			glColor3f(1.0, 1.0, 1.0);

			glTexCoord2d(0.0, 0.0);
			glVertex3d(-a / 2, -a / 2, -a / 2);

			glTexCoord2d(1.0, 0.0);
			glVertex3d(a / 2, -a / 2, -a / 2);

			glTexCoord2d(1.0, 1.0);
			glVertex3d(a / 2, -a / 2, a / 2);

			glTexCoord2d(0.0, 1.0);
			glVertex3d(-a / 2, -a / 2, a / 2);
		}
		glEnd();

		// TOP
		glBindTexture(GL_TEXTURE_2D, topTex);
		glBegin(GL_QUADS);
		{
			glColor3f(1.0, 1.0, 1.0);

			glTexCoord2d(0.0, 0.0);
			glVertex3d(-a / 2, a / 2, a / 2);

			glTexCoord2d(1.0, 0.0);
			glVertex3d(-a / 2, a / 2, -a / 2);

			glTexCoord2d(1.0, 1.0);
			glVertex3d(a / 2, a / 2, -a / 2);

			glTexCoord2d(0.0, 1.0);
			glVertex3d(a / 2, a / 2, a / 2);
		}
		glEnd();
	}
	glPopMatrix();
}

void CGLRenderer::DrawAxis()
{
	glLineWidth(2.0);
	glBegin(GL_LINES);
	{
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(50.0, 0.0, 0.0);

		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 50.0, 0.0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 50.0);
	}
	glEnd();
}

void CGLRenderer::DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen)
{
	double step = 360 / nSeg;
	double texStepX = 1.0 / nSeg;
	double texStepY = 0.5 / nSeg;
	double yStart;

	if (texMode == 0)
		yStart = 0.0;
	else
		yStart = 0.5;

	glBindTexture(GL_TEXTURE_2D, lampTex);

	if (!bIsOpen)
	{
		glBegin(GL_TRIANGLE_FAN);
		{
			glColor3f(1.0, 1.0, 1.0);

			glTexCoord2d(0.5, 0.25);
			glVertex3f(0.0, 0.0, 0.0);

			for (int i = 0; i <= nSeg; i++)
			{
				glTexCoord2d(0.5 + 0.5 * cos(RAD(i * step)), 0.25 + 0.25 * sin(RAD(i * step)));
				glVertex3f(r1 * cos(RAD(i * step)), 0, r1 * sin(RAD(i * step)));
			}
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		{
			glColor3f(1.0, 1.0, 1.0);

			glTexCoord2d(0.5, 0.25);
			glVertex3f(0.0, h, 0.0);

			for (int i = 0; i <= nSeg; i++)
			{
				glTexCoord2d(0.5 + 0.5 * cos(RAD(i * step)), 0.25 + 0.25 * sin(RAD(i * step)));
				glVertex3f(r2 * cos(RAD(i * step)), h, r2 * sin(RAD(i * step)));
			}
		}
		glEnd();
	}

	glBegin(GL_QUAD_STRIP);
	{
		glColor3f(1.0, 1.0, 1.0);

		for (int i = 0; i <= nSeg; i++)
		{
			glTexCoord2d(i * texStepX, yStart);
			glVertex3f(r2 * cos(RAD(i * step)), h, r2 * sin(RAD(i * step)));

			glTexCoord2d(i * texStepX, yStart + 0.5);
			glVertex3f(r1 * cos(RAD(i * step)), 0, r1 * sin(RAD(i * step)));
		}
	}
	glEnd();
}

void CGLRenderer::DrawLampBase()
{
	DrawCylinder(8.0, 7.0, 2.0, 36, 1, false);
}

void CGLRenderer::DrawLampArm()
{
	glPushMatrix();
	{
		glTranslated(0.0, 3.0, -1.0);
		glRotated(90, 1, 0, 0);
		DrawCylinder(3.0, 3.0, 2.0, 36, 1, false);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(0.0, 3.0, 0.0);
		DrawCylinder(1.0, 1.0, 15.0, 36, 1, true);
	}
	glPopMatrix();
}

void CGLRenderer::DrawLampHead()
{
	glPushMatrix();
	{
		glTranslated(0.0, 2.0, -1.0);
		glRotated(90, 1, 0, 0);
		DrawCylinder(2.0, 2.0, 2.0, 36, 1, false);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(2.5, 7.0, 0.0);
		glRotated(90, 0, 0, 1);
		DrawCylinder(3.0, 3.0, 5.0, 36, 1, false);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(-2.5, 7.0, 0.0);
		glRotated(90, 0, 0, 1);
		DrawCylinder(2.0, 2.0, 1.0, 36, 1, false);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(7.5, 7.0, 0.0);
		glRotated(90, 0, 0, 1);
		DrawCylinder(6.0, 3.0, 5.0, 36, 0, true);
	}
	glPopMatrix();
}

void CGLRenderer::DrawLamp()
{
	glPushMatrix();
	{
		DrawLampBase();


		glTranslated(0.0, -1.0, 0.0);
		glRotated(firstArmRot, 0.0, 0.0, 1.0);
		DrawLampArm();

		glTranslated(0.0, 17.0, 0.0);
		glRotated(secondArmRot, 0.0, 0.0, 1.0);
		DrawLampArm();

		glTranslated(0.0, 16.0, 0.0);
		glRotated(headRot, 0.0, 0.0, 1.0);
		DrawLampHead();
	}
	glPopMatrix();
}
