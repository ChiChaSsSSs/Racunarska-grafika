#pragma once

class CGLRenderer
{
public:
	UINT TSC0, TSC1, TSC2, TSC3, TSC4, TSC5;
	UINT M0, M1, M2, M3, M4, M5;
	UINT S0, S1, S2, S3, S4, S5;
	double yRot, xRot, zoom, moonRot;
	bool lightOn;

	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	UINT LoadTexture(char* fileName);
	void DrawPatch(double R, int n);
	void DrawEarth(double R, int tes);
	void DrawMoon();
	void DrawSpace();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
