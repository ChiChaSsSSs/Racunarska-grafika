#pragma once

class CGLRenderer
{
public:
	UINT texExcavator, texFront, texBack, texTop, texBottom, texLeft, texRight;
	double xRotation, yRotation, zoom;
	double bodyRotation, firstForkRotation, secondArmRotation, forkRotation;

	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawAxis();
	UINT LoadTexture(char* fileName);
	void DrawEnvCube(double a);
	void DrawPolygon(POINTF* points, POINTF* texCoords, int n);
	void DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b);
	void DrawBase();
	void DrawBody();
	void DrawArm(double zh);
	void DrawFork();
	void DrawExcavator();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
