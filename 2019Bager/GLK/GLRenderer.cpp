#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include <math.h>
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	xRotation = 0;
	yRotation = 0;
	zoom = -20;
	bodyRotation = 0;
	firstForkRotation = 0;
	secondArmRotation = 0;
	forkRotation = 0;
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

	texExcavator = LoadTexture("assets/excavator.png");
	texFront = LoadTexture("assets/front.jpg");
	texBack = LoadTexture("assets/back.jpg");
	texTop = LoadTexture("assets/top.jpg");
	texBottom = LoadTexture("assets/bot.jpg");
	texLeft = LoadTexture("assets/left.jpg");
	texRight = LoadTexture("assets/right.jpg");
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// CRTANJE KOCKE
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	{
		glTranslated(0.0, 0.0, zoom);
		glRotated(xRotation, 1.0, 0.0, 0.0);
		glRotated(yRotation, 0.0, 1.0, 0.0);
		DrawEnvCube(100.0);
	}
	glPopMatrix();


	// CRTANJE OSA
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	{
		glTranslated(0.0, 0.0, zoom);
		glRotated(xRotation, 1.0, 0.0, 0.0);
		glRotated(yRotation, 0.0, 1.0, 0.0);
		DrawAxis();
	}
	glPopMatrix();

	// CRTANJE BAGERA
	glPushMatrix();
	{
		glTranslated(0.0, 0.0, zoom);
		glRotated(xRotation, 1.0, 0.0, 0.0);
		glRotated(yRotation, 0.0, 1.0, 0.0);
		DrawExcavator();
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
	gluPerspective(55, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures(1, &texExcavator);
	glDeleteTextures(1, &texFront);
	glDeleteTextures(1, &texBack);
	glDeleteTextures(1, &texTop);
	glDeleteTextures(1, &texBottom);
	glDeleteTextures(1, &texLeft);
	glDeleteTextures(1, &texRight);

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
		glVertex3f(10.0, 0.0, 0.0);

		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);
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
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texFront);
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
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texLeft);
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

	glBindTexture(GL_TEXTURE_2D, texRight);
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

	glBindTexture(GL_TEXTURE_2D, texBack);
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

	glBindTexture(GL_TEXTURE_2D, texTop);
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

	glBindTexture(GL_TEXTURE_2D, texBottom);
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
}

void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n)
{
	glLineWidth(2.0);
	
	glBegin(GL_TRIANGLE_FAN);
	{
		glColor3f(1.0, 1.0, 1.0);

		for (int i = 0; i < n; i++)
		{
			glTexCoord2d(texCoords[i].x, texCoords[i].y);
			glVertex3d(points[i].x, points[i].y, 0.0);
		}
	}
	glEnd();
}

void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b)
{
	glPushMatrix();
	{
		glTranslated(0.0, 0.0, zh / 2);
		DrawPolygon(points, texCoords, n);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(0.0, 0.0, -zh / 2);
		DrawPolygon(points, texCoords, n);
	}
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUAD_STRIP);
	{
		for (int i = 0; i < n; i++)
		{
			glColor3f(r, g, b);

			if (i == 0)
			{
				glVertex3d(points[i].x, points[i].y, zh / 2);
				glVertex3d(points[i].x, points[i].y, -zh / 2);
			}
			else
			{
				glVertex3d(points[i].x, points[i].y, -zh / 2);
				glVertex3d(points[i].x, points[i].y, zh / 2);
			}
		}
		glVertex3d(points[0].x, points[0].y, -zh / 2);
		glVertex3d(points[0].x, points[0].y, zh / 2);
	}
	glEnd();
}

void CGLRenderer::DrawBase()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texExcavator);

	POINTF texCoords[] = { {0.0625,1.0},{0.0,0.9375},{0.0,0.8125},{0.0625,0.75},{0.9375,0.75},{1.0,0.8125},{1.0,0.9375},{0.9375,1.0} };
	POINTF realCoords[] = { {0.5,0.0},{0.0,0.5},{0.0,1.5},{0.5,2.0},{7.5,2.0},{8.0,1.5},{8.0,0.5},{7.5,0.0} };
	int n = 8;

	glTranslated(-4.0, 0.0, 0.0);
	DrawExtrudedPolygon(realCoords, texCoords, n, 5.0, 0.0, 0.0, 0.0);
	glTranslated(4.0, 0.0, 0.0);
}

void CGLRenderer::DrawBody()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texExcavator);
	POINTF texCoords[] = { {0.5,0.5},{0.5,0.0},{0.75,0.0},{1.0,0.25}, {1.0,0.5} };
	POINTF realCoords[] = { {0.0,0.0},{0.0,4.0},{2.0,4.0},{4.0,2.0},{4.0,0.0} };
	int n = 5;

	glTranslated(-2.0, 0.0, 0.0);
	DrawExtrudedPolygon(realCoords, texCoords, n, 4.0, 0.96, 0.5, 0.12);
	glTranslated(2.0, 0.0, 0.0);
}

void CGLRenderer::DrawArm(double zh)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texExcavator);
	POINTF texCoords[] = { {0.0625,0.75},{0.0,0.6875},{0.0,0.5625},{0.0625,0.5},{0.25,0.5},{1.0,0.5625},{1.0,0.6875},{0.25,0.75} };
	POINTF realCoords[] = { {0.5,0.0},{0.0,0.5},{0.0,1.5},{0.5,2.0},{2.0,2.0},{8.0,1.5},{8.0,0.5},{2.0,0.0} };
	int n = 8;

	glPushMatrix();
	{
		glTranslated(-4.0, 0.0, 0.0);

		if (zh == 1.0)
		{
			glTranslated(1.0, 1.0, 0.0);
			glRotated(firstForkRotation, 0.0, 0.0, 1.0);
			glTranslated(-1.0, -1.0, 0.0);
		}
		
		else
		{
			glTranslated(1.0, -5.0, 0.0);
			glRotated(firstForkRotation, 0.0, 0.0, 1.0);
			glTranslated(-1.0, 5.0, 0.0);

			glTranslated(1.0, 1.0, 0.0);
			glRotated(secondArmRotation, 0.0, 0.0, 1.0);
			glTranslated(-1.0, -1.0, 0.0);
		}

		DrawExtrudedPolygon(realCoords, texCoords, n, zh, 0.96, 0.5, 0.12);
	}
	glPopMatrix();
}

void CGLRenderer::DrawFork()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texExcavator);
	POINTF texCoords[] = { {0.0,0.375},{0.0,0.0625},{0.0625,0.0},{0.4375,0.0}, {0.5,0.0625},{0.5,0.375} };
	POINTF realCoords[] = { {0.0,0.0},{0.0,2.5},{0.5,3.0},{3.5,3.0},{4.0,2.5},{4.0,0.0} };
	int n = 6;

	glPushMatrix();
	{
		glTranslated(-2.0, 0.0, 0.0);

		glTranslated(7.0, -5.5, 0.0);
		glRotated(firstForkRotation, 0.0, 0.0, 1.0);
		glTranslated(-7.0, 5.5, 0.0);

		glTranslated(1.0, -5.5, 0.0);
		glRotated(secondArmRotation, 0.0, 0.0, 1.0);
		glTranslated(-1.0, 5.5, 0.0);

		glTranslated(1.0, 1.0, 0.0);
		glRotated(forkRotation, 0.0, 0.0, 1.0);
		glTranslated(-1.0, -1.0, 0.0);

		DrawExtrudedPolygon(realCoords, texCoords, n, 1.0, 0.7, 0.7, 0.7);
	}
	glPopMatrix();
}

void CGLRenderer::DrawExcavator()
{
	glPushMatrix();
	{
		DrawBase();

		glTranslated(0.0, 2.0, 0.0);
		glRotated(bodyRotation, 0.0, 1.0, 0.0);
		DrawBody();

		// PRVA RUKA
		glPushMatrix();
		{
			glTranslated(2.5, 4.0, -2.5);
			glRotated(90.0, 0.0, 0.0, 1.0);
			DrawArm(1.0);
		}
		glPopMatrix();

		// DRUGA RUKA
		glPushMatrix();
		{
			glTranslated(4.5, 6.0, -2.5);
			DrawArm(1.5);
		}
		glPopMatrix();

		// VILJUSKA
		glPushMatrix();
		{
			glTranslated(7.0, 6.0, -2.5);
			glRotated(-90.0, 0.0, 0.0, 1.0);
			DrawFork();
		}
		glPopMatrix();
	}
	glPopMatrix();
}
