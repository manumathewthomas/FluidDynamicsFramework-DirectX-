#pragma once
#include <d3dx9.h>

struct vertex
{
	float x,y,z;
	float u,v;
	D3DCOLOR col;
	vertex(float x1,float y1, float z1,float u1, float v1,D3DCOLOR col1)
	{
		x=x1;
		y=y1;
		z=z1;
		u=u1;
		v=v1;

		col = col1;
	}
};

class dxtexture
{
	//required for loading and storing texture
	
	
	IDirect3DVertexBuffer9 *vb;
	vertex *vert;

	IDirect3DIndexBuffer9 *ib;
	IDirect3DTexture9 *tex;
	WORD *ind;
	//for preventing memory leaks due to repeated declaration
public :
	bool bufferscreated,texturecreated;
public:
	//positon of texture to be loaded
	float x,y,z,fx,fy,fz,fxrot,fyrot,fzrot,fxmove,fymove,fzmove,fxr1,fyr1,fxr2,fyr2,fxr3,fyr3,fdiagnol,fangle,fxpos,fypos,fzpos,fAngle;
	float x0,y0,x1,y1,x2,y2,x3,y3;
	
	float width,height;
		//constrcutors
	dxtexture(void);
	dxtexture(float x,float y,float z,float width,float height);
public:
	
	//functions
	void createbuffers(IDirect3DDevice9 *device);

	//func for setting coordinates
	void setcoordinates(IDirect3DDevice9 *device);

	//setting source on screen
		void setstream(IDirect3DDevice9 *device);

		//loading texture
		void settexture(LPCSTR texf,IDirect3DDevice9 *device);

		
		//drawing to screen
		void drawobject(LPCSTR texf,IDirect3DDevice9 *device);

		
		//for rotating
		void rotate(float iangle);

		

		//clearing memeory
		void clearmem(IDirect3DDevice9 *device);

		//function for finding intersection
		

		//
		~dxtexture(void);



};