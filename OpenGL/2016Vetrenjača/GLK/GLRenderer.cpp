#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include <math.h>
//#pragma comment(lib, "GL\\glut32.lib")

#define RAD(a) (3.1415*a/180)

CGLRenderer::CGLRenderer(void)
{
	yRotation = 0;
	xRotation = 0;
	wheelRotation = 0;
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

	textEnv = LoadTexture("assets/env.png");
	textBrick = LoadTexture("assets/brick.png");
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST); // Iskljucujemo pogled u dubinu kako bi se stranice kocke iscrtale u beskonacnosti
	glDisable(GL_LIGHTING); // Osvetljenje iskljucujemo da ne bi uticalo negativno na teksture
	glTranslated(0, 0, -20);
	glPushMatrix();
	{
		glRotated(xRotation, 1, 0, 0);
		glRotated(yRotation, 0, 1, 0);
		DrawCube(100);
	}
	glPopMatrix();

	glEnable(GL_DEPTH_TEST); // Ukljucujemo pogled u dubinu jer crtamo objekte
	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT0);

	float ambientLight[] = { 1.0,1.0,1.0,1.0 };
	float diffuseLight[] = { 1.0,1.0,1.0,1.0 };
	float specularLight[] = { 1.0,1.0,1.0,1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	float lightPosition[] = { 1.0,1.0,1.0,0.0 }; // Definisemo poziciju izvora svetlosti, gde je poslednji argument oznaka da li je svetlost direkciona ili ne
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glPushMatrix();
	{
		glRotated(xRotation, 1, 0, 0);
		glRotated(yRotation, 0, 1, 0);

		DrawTube(2.5, 3.5, 10, 32);
		glTranslated(0, 5, 0);
		DrawCone(3.8, 2, 32);

		glPushMatrix();
		{
			glRotated(wheelRotation, 0, 0, 1);
			DrawPaddle(8, 1.5);
			glRotated(90, 0, 0, 1);
			DrawPaddle(8, 1.5);
			glRotated(90, 0, 0, 1);
			DrawPaddle(8, 1.5);
			glRotated(90, 0, 0, 1);
			DrawPaddle(8, 1.5);
		}
		glPopMatrix();
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
	double ascpect = (double)w / (double)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, ascpect, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures(1, &textEnv);
	glDeleteTextures(1, &textBrick);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT textID;
	DImage img;
	img.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Funkcija koja definise na koji nacin uzimam piksele iz slike. Ovde uzima po 4 bajta za svaki piksel (RGBA)
	glGenTextures(1, &textID); // Funkcija ce generisati jednu teksturu i smestiti je u promenljivu textID
	glBindTexture(GL_TEXTURE_2D, textID); // Postavlja trenutno aktivnu teksturu (aktivna tekstura je textID i sve dalje transformacije se primenjuju na nju)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Ova funkcija definise da u slucaju da tekstura horizontalno ne ispuni ceo objekat, ona se ponovo postavlja u nastavku dok se objekat ne ispuni
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Isto samo vertikalno
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Prilikom zumiranja slika ce linearno gubi kvalitet
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Pokusaj da saznas sam sta radi

	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Funkcija definise kako se boji tekstura kada je primenjujem. 
	// U ovom slucaju sa MODULATE vrednosti piksela teksture se kombinuju sa objektom na kojem se ona iscrtava, 
	// sto je neophodno zbog odrzavanja pravilnog osvetljenja objekta

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return textID;
}

void CGLRenderer::DrawCube(double a)
{
	double ySeg = 1.0 / 3;
	double xSeg = 1.0 / 4;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textEnv);

	// PREDNJA STRANA
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2d(2 * xSeg, 3 * ySeg);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2d(1 * xSeg, 3 * ySeg);
		glVertex3f(a / 2, a / 2, -a / 2);

		glTexCoord2d(1 * xSeg, 2 * ySeg);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2d(2 * xSeg, 2 * ySeg);
		glVertex3f(-a / 2, -a / 2, -a / 2);
	}
	glEnd();

	// DONJA STRANA
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2d(2 * xSeg, 2 * ySeg);
		glVertex3d(-a / 2, -a / 2, -a / 2);

		glTexCoord2d(1 * xSeg, 2 * ySeg);
		glVertex3d(a / 2, -a / 2, -a / 2);

		glTexCoord2d(1 * xSeg, 1 * ySeg);
		glVertex3d(a / 2, -a / 2, a / 2);

		glTexCoord2d(2 * xSeg, 1 * ySeg);
		glVertex3d(-a / 2, -a / 2, a / 2);
	}
	glEnd();

	// DESNA STRANA
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2d(1 * xSeg, 2 * ySeg);
		glVertex3d(a / 2, -a / 2, -a / 2);

		glTexCoord2d(0 * xSeg, 2 * ySeg);
		glVertex3d(a / 2, a / 2, -a / 2);

		glTexCoord2d(0 * xSeg, 1 * ySeg);
		glVertex3d(a / 2, a / 2, a / 2);

		glTexCoord2d(1 * xSeg, 1 * ySeg);
		glVertex3d(a / 2, -a / 2, a / 2);
	}
	glEnd();

	// LEVA STRANA
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2d(2 * xSeg, 2 * ySeg);
		glVertex3d(-a / 2, -a / 2, -a / 2);

		glTexCoord2d(3 * xSeg, 2 * ySeg);
		glVertex3d(-a / 2, a / 2, -a / 2);

		glTexCoord2d(3 * xSeg, 1 * ySeg);
		glVertex3d(-a / 2, a / 2, a / 2);

		glTexCoord2d(2 * xSeg, 1 * ySeg);
		glVertex3d(-a / 2, -a / 2, a / 2);
	}

	// ZADNJA STRANA
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2d(1 * xSeg, 1 * ySeg);
		glVertex3d(-a / 2, -a / 2, a / 2);

		glTexCoord2d(1 * xSeg, 0 * ySeg);
		glVertex3d(-a / 2, a / 2, a / 2);

		glTexCoord2d(2 * xSeg, 0 * ySeg);
		glVertex3d(a / 2, a / 2, a / 2);

		glTexCoord2d(2 * xSeg, 1 * ySeg);
		glVertex3d(a / 2, -a / 2, a / 2);
	}
	glEnd();

	// GORNJA STRANA
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2d(3 * xSeg, 1 * ySeg);
		glVertex3d(-a / 2, a / 2, a / 2);

		glTexCoord2d(4 * xSeg, 1 * ySeg);
		glVertex3d(a / 2, a / 2, a / 2);

		glTexCoord2d(4 * xSeg, 2 * ySeg);
		glVertex3d(a / 2, a / 2, -a / 2);

		glTexCoord2d(3 * xSeg, 2 * ySeg);
		glVertex3d(-a / 2, a / 2, -a / 2);
	}
	glEnd();
}

void CGLRenderer::DrawTube(double r1, double r2, double h, int n)
{
	double step = 2 * 3.1415 / n;

	glBindTexture(GL_TEXTURE_2D, textBrick);
	glBegin(GL_QUAD_STRIP);
	{
		glColor3f(1.0, 1.0, 1.0);
		for (double i = 0; i <= n; i++)
		{
			glNormal3d(cos(i * step), 0, sin(i * step));
			glTexCoord2d(i / n, 0);
			glVertex3d(r1 * cos(i * step), h / 2, r1 * sin(i * step));

			glNormal3d(cos(i * step), 0, sin(i * step));
			glTexCoord2d(i / n, 1);
			glVertex3d(r2 * cos(i * step), -h / 2, r2 * sin(i * step));

			// Ove 2 linije ne treba jer on ponavlja 2 ista temena svakim prolaskom kroz for petlju
			//glTexCoord2d((i + 1) / n, 1);
			//glVertex3d(r2 * cos((i + 1) * step), -h / 2, r2 * sin((i + 1) * step));

			//glTexCoord2d((i + 1) / n, 0);
			//glVertex3d(r1 * cos((i + 1) * step), h / 2, r1 * sin((i + 1) * step));
		}
	}
	glEnd();
}

void CGLRenderer::DrawCone(double r, double h, int n)
{
	double step = 2 * 3.1415 / n;

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_FAN);
	{
		glColor3f(1.0, 1.0, 1.0);
		glVertex3d(0.0, 0.0, 0.0);
		for (double i = 0; i <= n; i++)
		{
			glNormal3d(cos(i * step), 0, sin(i * step));
			glVertex3d(r * cos(i * step), 0, r * sin(i * step));
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	{
		glColor3f(1.0, 1.0, 1.0);
		glNormal3d(0.0, 1.0, 0.0);
		glVertex3d(0.0, h, 0.0);
		for (double i = 0; i <= n; i++)
		{
			glVertex3d(r * cos(i * step), 0, r * sin(i * step));
		}
	}
	glEnd();
}

void CGLRenderer::DrawPaddle(double length, double width)
{
	glBegin(GL_POLYGON);
	{
		glColor3f(1.0, 1.0, 1.0);

		glNormal3d(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, 3.8);

		glNormal3d(0.0, 0.0, 1.0);
		glVertex3d(length, 0.0, 3.8);

		glNormal3d(0.0, 0.0, 1.0);
		glVertex3d(length, -width, 3.8);

		glNormal3d(0.0, 0.0, 1.0);
		glVertex3d(length / 8 * 2, -width, 3.8);

		glNormal3d(0.0, 0.0, 1.0);
		glVertex3d(length / 8, -width / 8, 3.8);

		glNormal3d(0.0, 0.0, 1.0);
		glVertex3d(0, -width / 8, 3.8);
	}
	glEnd();
}
