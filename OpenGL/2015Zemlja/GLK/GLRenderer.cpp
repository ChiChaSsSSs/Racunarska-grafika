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
	xRot = 0;
	yRot = 0;
	zoom = -20;
	moonRot = 0;
}

CGLRenderer::~CGLRenderer(void)
{
	lightOn = true;
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

	TSC0 = LoadTexture("assets/TSC0.jpg");
	TSC1 = LoadTexture("assets/TSC1.jpg");
	TSC2 = LoadTexture("assets/TSC2.jpg");
	TSC3 = LoadTexture("assets/TSC3.jpg");
	TSC4 = LoadTexture("assets/TSC4.jpg");
	TSC5 = LoadTexture("assets/TSC5.jpg");

	M0 = LoadTexture("assets/M0.jpg");
	M1 = LoadTexture("assets/M1.jpg");
	M2 = LoadTexture("assets/M2.jpg");
	M3 = LoadTexture("assets/M3.jpg");
	M4 = LoadTexture("assets/M4.jpg");
	M5 = LoadTexture("assets/M5.jpg");

	S0 = LoadTexture("assets/S0.jpg");
	S1 = LoadTexture("assets/S1.jpg");
	S2 = LoadTexture("assets/S2.jpg");
	S3 = LoadTexture("assets/S3.jpg");
	S4 = LoadTexture("assets/S4.jpg");
	S5 = LoadTexture("assets/S5.jpg");
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	{
		glRotated(xRot, 1.0, 0.0, 0.0);
		glRotated(yRot, 0.0, 1.0, 0.0);
		DrawSpace();
	}
	glPopMatrix();

	if (lightOn)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	float ambientLight[] = { 1.0,1.0,1.0,1.0 };
	float diffuseLight[] = { 1.0,1.0,1.0,1.0 };
	float specularLight[] = { 1.0,1.0,1.0,1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	float lightPosition[] = { 0.0,0.0,1.0,0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	{
		glTranslated(0.0, 0.0, zoom);
		glRotated(xRot, 1.0, 0.0, 0.0);
		glRotated(yRot, 0.0, 1.0, 0.0);
		DrawEarth(3.0, 20);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(50.0, 0.0, zoom);
		glRotated(xRot, 1.0, 0.0, 0.0);
		glRotated(yRot, 0.0, 1.0, 0.0);
		glRotated(moonRot, 0.0, 1.0, 0.0);
		DrawMoon();
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
	gluPerspective(40, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures(1, &TSC0);
	glDeleteTextures(1, &TSC1);
	glDeleteTextures(1, &TSC2);
	glDeleteTextures(1, &TSC3);
	glDeleteTextures(1, &TSC4);
	glDeleteTextures(1, &TSC5);

	glDeleteTextures(1, &M0);
	glDeleteTextures(1, &M1);
	glDeleteTextures(1, &M2);
	glDeleteTextures(1, &M3);
	glDeleteTextures(1, &M4);
	glDeleteTextures(1, &M5);

	glDeleteTextures(1, &S0);
	glDeleteTextures(1, &S1);
	glDeleteTextures(1, &S2);
	glDeleteTextures(1, &S3);
	glDeleteTextures(1, &S4);
	glDeleteTextures(1, &S5);

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

void CGLRenderer::DrawPatch(double R, int n)
{
	double delta = 2.0 / n; // Pomera s obzirom da se isecak crta u opsegu [-1, 1], sto je ukupne duzine 2
	double y = 1.0; // Krecemo iz gornjeg levog coska

	for (int i = 0; i < n; i++) // Petlja kretanja po y-osi
	{
		glBegin(GL_TRIANGLE_STRIP);
		{
			double x = -1.0; // Pocetno x

			for (int j = 0; j <= n; j++) // Petlja kretanja po x-osi
			{
				double phi, theta;
				phi = atan(x); // ugao pomeraja po x-osi
				theta = atan(y * cos(phi)); // ugao pomeraja po y-osi

				double xd, yd, zd;
				xd = R * cos(theta) * sin(phi); // Izracunvanje x-koordinate za trenutnu lokaciju u odnosu na uglove phi i theta
				yd = R * sin(theta); // Izracunvanje y-koordinate za trenutnu lokaciju u odnosu na uglove phi i theta
				zd = R * cos(theta) * cos(phi); // Izracunvanje z-koordinate za trenutnu lokaciju u odnosu na uglove phi i theta
				glNormal3f(xd / R, yd / R, zd / R);
				glTexCoord2d((x + 1.0) / 2, (-y + 1.0) / 2);
				glVertex3d(xd, yd, zd);

				theta = atan((y - delta) * cos(phi)); // Pomeranje po y-osi;
				xd = R * cos(theta) * sin(phi);
				yd = R * sin(theta);
				zd = R * cos(theta) * cos(phi);
				glNormal3f(xd / R, yd / R, zd / R);
				glTexCoord2d((x + 1.0) / 2, (-y + 1.0 + delta) / 2);
				glVertex3d(xd, yd, zd);

				x += delta;
			}
		}
		glEnd();
		
		y -= delta;
	}
}

void CGLRenderer::DrawEarth(double R, int tes)
{
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, TSC0);
		DrawPatch(R, tes);

		glRotated(90.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, TSC1);
		DrawPatch(R, tes);

		glRotated(90.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, TSC2);
		DrawPatch(R, tes);

		glRotated(90.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, TSC3);
		DrawPatch(R, tes);
	}

	glPushMatrix();
	{
		glRotated(-90.0, 1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, TSC4);
		DrawPatch(R, tes);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glRotated(90.0, 1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, TSC5);
		DrawPatch(R, tes);
	}
	glPopMatrix();
}

void CGLRenderer::DrawMoon()
{
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, M0);
		DrawPatch(1.0, 20);

		glRotated(90.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, M1);
		DrawPatch(1.0, 20);

		glRotated(90.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, M2);
		DrawPatch(1.0, 20);

		glRotated(90.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, M3);
		DrawPatch(1.0, 20);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glRotated(-90, 1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, M4);
		DrawPatch(1.0, 20);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glRotated(90, 1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, M5);
		DrawPatch(1.0, 20);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpace()
{
	glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, S0);
		DrawPatch(1.0, 20);

		glRotated(90.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, S1);
		DrawPatch(1.0, 20);

		glRotated(90.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, S2);
		DrawPatch(1.0, 20);

		glRotated(90.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, S3);
		DrawPatch(1.0, 20);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glRotated(-90, 1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, S4);
		DrawPatch(1.0, 20);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glRotated(90, 1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, S5);
		DrawPatch(1.0, 20);
	}
	glPopMatrix();
}
