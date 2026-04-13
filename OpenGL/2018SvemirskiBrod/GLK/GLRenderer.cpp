#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include <cmath>
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	xRot = 0;
	yRot = 0;
	zoom = -20;
	lightEnabled = true;
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
	ship = LoadTexture("assets/ShipT1.png");

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	{
		glTranslated(0.0, 0.0, zoom);
		glRotated(xRot, 1.0, 0.0, 0.0);
		glRotated(yRot, 0.0, 1.0, 0.0);
		DrawSpaceCube(100.0);
	}
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	{
		glTranslated(0.0, 0.0, zoom);
		glRotated(xRot, 1.0, 0.0, 0.0);
		glRotated(yRot, 0.0, 1.0, 0.0);
		DrawAxis(10.0);
	}
	glPopMatrix();


	float posLight[] = { 0.0,0.0,1.0,0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, posLight);
	if (lightEnabled)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	glEnable(GL_TEXTURE_2D);
	{
		glTranslated(0.0, 0.0, zoom);
		glRotated(xRot, 1.0, 0.0, 0.0);
		glRotated(yRot, 0.0, 1.0, 0.0);

		double d = sqrt(5.8 * 5.8 + 2.15 * 2.15);
		glTranslatef(-d / 2, 0, 0);
		glRotatef(90, 1, 0, 0);

		DrawShip();
	}

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
	glDeleteTextures(1, &ship);


	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxis(double d)
{
	glLineWidth(2.0);

	glBegin(GL_LINES);
	{
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(d, 0.0, 0.0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, d, 0.0);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, d);
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

void CGLRenderer::DrawSpaceCube(double a)
{
	// FRONT
	glBindTexture(GL_TEXTURE_2D, front);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(-a / 2, -a / 2, -a / 2);

		glTexCoord2d(1.0, 0.0);
		glVertex3d(a / 2, -a / 2, -a / 2);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(a / 2, a / 2, -a / 2);

		glTexCoord2d(0.0, 1.0);
		glVertex3d(-a / 2, a / 2, -a / 2);
	}
	glEnd();

	// BACK
	glBindTexture(GL_TEXTURE_2D, back);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(a / 2, -a / 2, a / 2);

		glTexCoord2d(1.0, 0.0);
		glVertex3d(-a / 2, -a / 2, a / 2);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(-a / 2, a / 2, a / 2);

		glTexCoord2d(0.0, 1.0);
		glVertex3d(a / 2, a / 2, a / 2);
	}
	glEnd();

	// LEFT
	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(-a / 2, -a / 2, a / 2);

		glTexCoord2d(1.0, 0.0);
		glVertex3d(-a / 2, -a / 2, -a / 2);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(-a / 2, a / 2, -a / 2);

		glTexCoord2d(0.0, 1.0);
		glVertex3d(-a / 2, a / 2, a / 2);
	}
	glEnd();

	// RIGHT
	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(a / 2, -a / 2, -a / 2);

		glTexCoord2d(1.0, 0.0);
		glVertex3d(a / 2, -a / 2, a / 2);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(a / 2, a / 2, a / 2);

		glTexCoord2d(0.0, 1.0);
		glVertex3d(a / 2, a / 2, -a / 2);
	}
	glEnd();

	// TOP
	glBindTexture(GL_TEXTURE_2D, bot);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(-a / 2, a / 2, -a / 2);

		glTexCoord2d(1.0, 0.0);
		glVertex3d(a / 2, a / 2, -a / 2);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(a / 2, a / 2, a / 2);

		glTexCoord2d(0.0, 1.0);
		glVertex3d(-a / 2, a / 2, a / 2);
	}
	glEnd();

	// BOT
	glBindTexture(GL_TEXTURE_2D, top);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(-a / 2, -a / 2, a / 2);

		glTexCoord2d(1.0, 0.0);
		glVertex3d(a / 2, -a / 2, a / 2);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(a / 2, -a / 2, -a / 2);

		glTexCoord2d(0.0, 1.0);
		glVertex3d(-a / 2, -a / 2, -a / 2);
	}
	glEnd();
}

void CGLRenderer::DrawTriangle(float d1, float d2, float rep)
{
	double alpha = atan2(d2, d1);
	double d3 = sqrt(d1 * d1 + d2 * d2);

	double hor = d1 * cos(alpha) / d3;
	double ver = d1 * sin(alpha) / d3;

	glBegin(GL_TRIANGLES);
	{
		glColor3f(1.0, 1.0, 1.0);

		glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2d(0.5 * rep, 0.0);
		glVertex3f(0.0, 0.0, 0.0);

		glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2d((0.5 + ver) * rep, hor * rep);
		glVertex3f(d1, 0.0, 0.0);

		glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2d(0.5 * rep, 1.0 * rep);
		glVertex3f(d1, d2, 0.0);

	}
	glEnd();
}

void CGLRenderer::DrawShip()
{
	glBindTexture(GL_TEXTURE_2D, ship);

	double angle = atan2(2.15, 5.8);
	float rep = 3.0;

	glPushMatrix();
	{
		glRotated(13, 0.0, 1.0, 0.0);
		glRotated(-4.75, 1.0, 0.0, 0.0);
		glRotated(-toDeg(angle), 0.0, 0.0, 1.0);
		DrawTriangle(5.8, 2.15, rep);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glScaled(1.0, -1.0, 1.0);
		glRotated(13, 0.0, 1.0, 0.0);
		glRotated(-4.75, 1.0, 0.0, 0.0);
		glRotated(-toDeg(angle), 0.0, 0.0, 1.0);
		DrawTriangle(5.8, 2.15, rep);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glRotated(180.0, 1.0, 0.0, 0.0);
		glRotated(13, 0.0, 1.0, 0.0);
		glRotated(-4.75, 1.0, 0.0, 0.0);
		glRotated(-toDeg(angle), 0.0, 0.0, 1.0);
		DrawTriangle(5.8, 2.15, rep);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glRotated(180.0, 1.0, 0.0, 0.0);
		glScaled(1.0, -1.0, 1.0);
		glRotated(13, 0.0, 1.0, 0.0);
		glRotated(-4.75, 1.0, 0.0, 0.0);
		glRotated(-toDeg(angle), 0.0, 0.0, 1.0);
		DrawTriangle(5.8, 2.15, rep);
	}
	glPopMatrix();
}

double CGLRenderer::toDeg(double radAngle)
{
	return radAngle * 180 / 3.1415;
}
