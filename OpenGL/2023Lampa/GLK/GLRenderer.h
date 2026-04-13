#pragma once

class CGLRenderer
{
public:
	UINT sideTex, bottomTex, topTex, lampTex;
	double yRotation, xRotation, zoom;
	double firstArmRot, secondArmRot, headRot;

	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	UINT LoadTexture(char* fileName);
	void DrawEnvCube(double a);
	void DrawAxis();
	void DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen);
	void DrawLampBase();
	void DrawLampArm();
	void DrawLampHead();
	void DrawLamp();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
