#pragma once
#include "dxtexture.h"
IDirect3DDevice9 *getDev();
class Game1
{
	

//header file of game .h

	//Camera 
	D3DXVECTOR3 pos;
	D3DXVECTOR3 tgt;
	D3DXVECTOR3 up;

	D3DXMATRIX v;
	D3DXMATRIX proj;

public:
	Game1(void);
	void UpdateGame(IDirect3DDevice9 *device);
	void DrawGame(IDirect3DDevice9 *device,float screenwidth,float screenheight);
	void SetCamera(IDirect3DDevice9 *device,float screenwidth,float screenheight);
	~Game1(void);
};