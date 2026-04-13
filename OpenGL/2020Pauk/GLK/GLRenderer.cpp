#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include <cmath>
#define PI 3.1415
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	xRot = 0;
	yRot = 0;
	zoom = -20;
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

	front = LoadTexture("assets/front.jpg");
	back = LoadTexture("assets/back.jpg");
	top = LoadTexture("assets/top.jpg");
	bot = LoadTexture("assets/bot.jpg");
	left = LoadTexture("assets/left.jpg");
	right = LoadTexture("assets/right.jpg");
	spider = LoadTexture("assets/spider.png");
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// ENV CUBE
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	{
		glTranslated(0.0, 0.0, zoom);
		glRotated(xRot, 1.0, 0.0, 0.0);
		glRotated(yRot, 0.0, 1.0, 0.0);
		DrawEnvCube(100.0);
	}
	glPopMatrix();

	// AXIS
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	{
		glTranslated(0.0, 0.0, zoom);
		glRotated(xRot, 1.0, 0.0, 0.0);
		glRotated(yRot, 0.0, 1.0, 0.0);
		DrawAxis();
	}
	glPopMatrix();

	// SPIDER
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	{
		glTranslated(0.0, 0.0, zoom);
		glRotated(xRot, 1.0, 0.0, 0.0);
		glRotated(yRot, 0.0, 1.0, 0.0);
		DrawSpider();
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

	glDeleteTextures(1, &front);
	glDeleteTextures(1, &back);
	glDeleteTextures(1, &top);
	glDeleteTextures(1, &bot);
	glDeleteTextures(1, &left);
	glDeleteTextures(1, &right);
	glDeleteTextures(1, &spider);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
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
	// FRONT
	glBindTexture(GL_TEXTURE_2D, front);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(-a / 2, a / 2, -a / 2);

		glTexCoord2d(0.0, 1.0);
		glVertex3d(-a / 2, -a / 2, -a / 2);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(a / 2, -a / 2, -a / 2);

		glTexCoord2d(1.0, 0.0);
		glVertex3d(a / 2, a / 2, -a / 2);
	}
	glEnd();

	// BACK
	glBindTexture(GL_TEXTURE_2D, back);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(a / 2, a / 2, a / 2);

		glTexCoord2d(0.0, 1.0);
		glVertex3d(a / 2, -a / 2, a / 2);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(-a / 2, -a / 2, a / 2);

		glTexCoord2d(1.0, 0.0);
		glVertex3d(-a / 2, a / 2, a / 2);
	}
	glEnd();

	// LEFT
	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(-a / 2, a / 2, a / 2);

		glTexCoord2d(0.0, 1.0);
		glVertex3d(-a / 2, -a / 2, a / 2);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(-a / 2, -a / 2, -a / 2);

		glTexCoord2d(1.0, 0.0);
		glVertex3d(-a / 2, a / 2, -a / 2);
	}
	glEnd();

	// RIGHT
	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(a / 2, a / 2, -a / 2);

		glTexCoord2d(0.0, 1.0);
		glVertex3d(a / 2, -a / 2, -a / 2);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(a / 2, -a / 2, a / 2);

		glTexCoord2d(1.0, 0.0);
		glVertex3d(a / 2, a / 2, a / 2);
	}
	glEnd();

	// TOP
	glBindTexture(GL_TEXTURE_2D, top);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(-a / 2, a / 2, a / 2);

		glTexCoord2d(0.0, 1.0);
		glVertex3d(-a / 2, a / 2, -a / 2);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(a / 2, a / 2, -a / 2);

		glTexCoord2d(1.0, 0.0);
		glVertex3d(a / 2, a / 2, a / 2);
	}
	glEnd();

	// BOT
	glBindTexture(GL_TEXTURE_2D, bot);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(-a / 2, -a / 2, -a / 2);

		glTexCoord2d(0.0, 1.0);
		glVertex3d(-a / 2, -a / 2, a / 2);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(a / 2, -a / 2, a / 2);

		glTexCoord2d(1.0, 0.0);
		glVertex3d(a / 2, -a / 2, -a / 2);
	}
	glEnd();
}

void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	double ang1, ang2;
	double dAng1 = PI / (double)nSeg;
	double dAng2 = 2 * PI / (double)nSeg;
	ang1 = -PI / 2;

	for (int i = 0; i < nSeg; i++)
	{
		ang2 = 0;

		glBegin(GL_QUAD_STRIP);
		{
			glColor3f(1.0, 1.0, 1.0);

			for (int j = 0; j <= nSeg; j++)
			{
				double x1 = r * cos(ang1) * cos(ang2);
				double y1 = r * sin(ang1);
				double z1 = r * cos(ang1) * sin(ang2);

				double x2 = r * cos(ang1 + dAng1) * cos(ang2);
				double y2 = r * sin(ang1 + dAng1);
				double z2 = r * cos(ang1 + dAng1) * sin(ang2);

				double tx1 = x1 / r * texR + texU;
				double ty1 = z1 / r * texR + texV;

				double tx2 = x2 / r * texR + texU;
				double ty2 = z2 / r * texR + texV;

				glTexCoord2d(tx1, ty1);
				glVertex3d(x1, y1, z1);

				glTexCoord2d(tx2, ty2);
				glVertex3d(x2, y2, z2);

				ang2 += dAng2;
			}
		}
		glEnd();

		ang1 += dAng1;
	}
}

void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	double step = 2 * PI / (double)nSeg;

	glBegin(GL_TRIANGLE_FAN);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(texU, texV);
		glVertex3d(0.0, h, 0.0);

		for (double angle = 0; angle <= 2 * PI; angle += step)
		{
			double x = r * cos(angle);
			double z = r * sin(angle);

			double tx = x / r * texR + texU;
			double ty = z / r * texR + texV;

			glTexCoord2d(tx, ty);
			glVertex3d(x, 0, z);
		}
	}
	glEnd();
}

void CGLRenderer::DrawLegSegment(double r, double h, int nSeg)
{
	DrawSphere(r, nSeg, 0.25, 0.25, 0.25);
	DrawCone(r, h, nSeg, 0.75, 0.75, 0.25);
}

void CGLRenderer::DrawLeg()
{
	glPushMatrix();
	{
		glRotated(45.0, 0.0, 0.0, 1.0);

		glPushMatrix();
		{
			glTranslated(0.0, 10.0, 0.0);
			glRotated(95.0, 0.0, 0.0, 1.0);
			DrawLegSegment(1, 15, 5);
		}
		glPopMatrix();

		DrawLegSegment(1, 10, 5);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpiderBody()
{
	// CHEST
	glPushMatrix();
	{
		glScaled(1.0, 0.5, 1.0);
		DrawSphere(3.0, 10, 0.25, 0.25, 0.25);
	}
	glPopMatrix();

	// HEAD
	glPushMatrix();
	{
		glTranslated(-5.0, 0.0, 0.0);
		glScaled(1.0, 0.5, 1.0);
		DrawSphere(2.0, 10, 0.75, 0.25, 0.25);
	}
	glPopMatrix();

	// STOMACH
	glPushMatrix();
	{
		glTranslated(8.0, 0.0, 0.0);
		glScaled(1.0, 0.8, 1.0);
		DrawSphere(5.0, 10, 0.25, 0.25, 0.25);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpider()
{
	glBindTexture(GL_TEXTURE_2D, spider);

	DrawSpiderBody();

	glPushMatrix();
	{
		glRotated(45.0, 0.0, 1.0, 0.0);
		DrawLeg();

		for (int i = 1; i <= 3; i++)
		{
			glRotated(30.0, 0.0, 1.0, 0.0);
			DrawLeg();
		}
	}
	glPopMatrix();

	glPushMatrix();
	{
		glRotated(-45.0, 0.0, 1.0, 0.0);
		DrawLeg();

		for (int i = 1; i <= 3; i++)
		{
			glRotated(-30.0, 0.0, 1.0, 0.0);
			DrawLeg();
		}
	}
	glPopMatrix();
}
