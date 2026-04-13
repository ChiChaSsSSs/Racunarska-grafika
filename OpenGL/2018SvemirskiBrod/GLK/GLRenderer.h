#pragma once

class CGLRenderer
{
public:
	UINT front, back, top, bot, left, right, ship;
	double yRot, xRot, zoom;
	bool lightEnabled;

	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawAxis(double d);
	UINT LoadTexture(char* fileName);
	void DrawSpaceCube(double a);
	void DrawTriangle(float d1, float d2, float rep);
	void DrawShip();
	double toDeg(double radAngle);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
