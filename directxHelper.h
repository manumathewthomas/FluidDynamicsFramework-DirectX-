#include<Windows.h>
#include<d3dx9.h>
#include <process.h>
#include "Game1.h"
#include <math.h>




IDirect3DDevice9 *d3ddev =getDev();
#define SHADE_FLAT 0
#define SHADE_GOURAUD 1
bool threadFinished = false;
class water;
HWND  hwnd;
IDirect3DDevice9 *device;
IDirect3DTexture9 *tex1[100];
static int texCount=0;
float startPt=-10,endPt=30,maxHeight=-10;
typedef int texture2D;

bool InitWndApp(HINSTANCE,int);
int MsgLoop(void);
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void d3dInit(UINT,UINT,D3DFORMAT,UINT,D3DMULTISAMPLE_TYPE,DWORD,D3DSWAPEFFECT,bool,HWND,bool,D3DFORMAT,DWORD,UINT,UINT);
void wavesPropogation(water);
void waterSplash(int,int,int,water);

struct Vertex
{
	float x,y,z,u,v;
	
	
	Vertex(float x,float y,float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

		Vertex(float x,float y,float z,float u,float v)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->u = u;
		this->v = v;
	}


};

struct VertexColor
{

	float x,y,z;
	D3DCOLOR color;

	VertexColor(float x,float y,float z,D3DCOLOR color)
	{
		
		this->x = x;
		this->y = y;
		this->z = z;
		this->color = color;
	}

};



bool InitWndApp(HINSTANCE hinstance,int show,char *windowTitle,UINT style=CS_HREDRAW|CS_VREDRAW,WNDPROC lpfnWndProc = WndProc,int cbClsExtra = 0,int cbWndExtra = 0,HICON hIcon = LoadIcon(0,IDI_APPLICATION),HCURSOR hCursor = LoadCursor(0,IDC_ARROW),HBRUSH hbrBackground =  (HBRUSH)GetStockObject(WHITE_BRUSH),LPCSTR lpszMenuName = 0,LPCSTR lpszClassName = "MyClass")
{
	WNDCLASS w;

	w.style = style;
	w.lpfnWndProc = lpfnWndProc;
	w.cbClsExtra = cbClsExtra;
	w.cbWndExtra = cbWndExtra;
	w.hIcon = hIcon;
	w.hCursor = hCursor;
	w.hbrBackground = hbrBackground;
	w.hInstance = hinstance;
	w.lpszMenuName = lpszMenuName;
	w.lpszClassName = lpszClassName;


	if(!RegisterClass(&w))
	{
		MessageBox(0,"Registeration Failed",0,0);
		return false;
	}

	hwnd = CreateWindow("myclass",windowTitle,WS_OVERLAPPEDWINDOW,0,0,800,600,0,0,hinstance,0);

	if(!hwnd)
	{
		MessageBox(0,"Creation Failed",0,0);
		return false;
	}

	ShowWindow(hwnd,show);
	UpdateWindow(hwnd);
	return true;

}

int MsgLoop()
{
	MSG msg;
	ZeroMemory(&msg,sizeof(MSG));
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return 0;
}


void d3dInit(UINT BackBufferWidth = 1024,UINT BackBufferHeight = 768,D3DFORMAT BackBufferFormat = D3DFMT_X8R8G8B8,UINT BackBufferCount = 1,D3DMULTISAMPLE_TYPE MultiSampleType = D3DMULTISAMPLE_NONE,DWORD MultiSampleQuality = 0,D3DSWAPEFFECT SwapEffect = D3DSWAPEFFECT_DISCARD,bool Windowed = true,HWND hDeviceWindow = hwnd,bool EnableAutoDepthStencil = true,D3DFORMAT AutoDepthStencilFormat = D3DFMT_D24S8,DWORD Flags=0,UINT FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT,UINT PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE)
{
	IDirect3D9 *d3dptr;
	d3dptr=Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 caps;
	int vp=0;

	d3dptr->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);

	if(caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;

	d3dpp.BackBufferWidth = BackBufferWidth;
	d3dpp.BackBufferHeight =BackBufferHeight;
	d3dpp.BackBufferFormat = BackBufferFormat;
	d3dpp.BackBufferCount = BackBufferCount;
	d3dpp.MultiSampleType = MultiSampleType;
	d3dpp.MultiSampleQuality = MultiSampleQuality;
	d3dpp.SwapEffect = SwapEffect;
	d3dpp.Windowed = Windowed;
	d3dpp.hDeviceWindow = hDeviceWindow;
	d3dpp.EnableAutoDepthStencil = EnableAutoDepthStencil;
	d3dpp.AutoDepthStencilFormat = AutoDepthStencilFormat;
	d3dpp.Flags=Flags;
	d3dpp.FullScreen_RefreshRateInHz = FullScreen_RefreshRateInHz;
	d3dpp.PresentationInterval = PresentationInterval;



	HRESULT res;

	res = d3dptr->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,vp,&d3dpp,&device);
	if(FAILED(res))
	{
		MessageBox(0,"Error Creating Directx Window",0,0);
	}
}

int loadTexture(char *texName)
{
	texCount++;
	 D3DXCreateTextureFromFileA(device,texName,&tex1[texCount]);
	return texCount;
}


void displayTexture(int texCode,IDirect3DDevice9 *device)
{
	
	device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE );

	device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT);

	device->SetRenderState(D3DRS_LIGHTING,false);
	
	device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_POINT);

	device->SetTexture(0,tex1[texCode]);
}
void colorVertexInit(int mode,IDirect3DDevice9 *device)
{

	device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);

	if(SHADE_GOURAUD)
	{
		device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	}
	else
	{
		device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	}

	device->SetRenderState(D3DRS_LIGHTING,FALSE);
}

void VertexInit(int mode,IDirect3DDevice9 *device)
{
	
	device->SetFVF(D3DFVF_XYZ);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	device->SetRenderState(D3DRS_LIGHTING,FALSE);
}


class water
{
public:
	IDirect3DVertexBuffer9 *vb1;
	VertexColor *vert1;

	IDirect3DIndexBuffer9 *ib1;
	WORD *ind1;
	int f;

	float targetHeight,height,heightPrev,speed,scale,tension,dampening,spread;

	water()
	{
		tension = 0.025;
		dampening = 0.025;
		spread = 0.1;
		f=0;
	}
	void initWater(int,float,float,float,float,IDirect3DDevice9 *device);
	void update(float ,float );

	
};


void water:: initWater(int waterCount,float waterHeight,float _height,float _targetHeight,float _speed,IDirect3DDevice9 *device)
{

	if(f==0)
	{

	
		device->CreateVertexBuffer(sizeof(VertexColor)*4,0,D3DFVF_XYZ|D3DFVF_DIFFUSE,D3DPOOL_MANAGED,&vb1,0);
		
		device->CreateIndexBuffer(sizeof(WORD)*6,0,D3DFMT_INDEX16,D3DPOOL_MANAGED,&ib1,0);

		height = _height,
		targetHeight = _targetHeight,
		speed =_speed;
		
		f=1;
	}
	
	vb1->Lock(0,0,(void**)&vert1,0);

   
	vert1[0] = VertexColor((waterCount-1),waterHeight,0.0f,D3DCOLOR_ARGB(255,117,216,250));
	vert1[1] = VertexColor((waterCount-1),heightPrev,0.0f,D3DCOLOR_ARGB(255,117,216,250));
	vert1[2] = VertexColor(  waterCount,height,0.0f,D3DCOLOR_ARGB(255,117,216,250));
	vert1[3] = VertexColor(waterCount,waterHeight,0.0f,D3DCOLOR_ARGB(255,117,216,250));

	vb1->Unlock();

	

	

	ib1->Lock(0,0,(void**)&ind1,0);
	ind1[0] = 0;
	ind1[1] = 1;
	ind1[2] = 2;
	ind1[3] = 2;
	ind1[4] = 3;
	ind1[5] = 0;

	ib1->Unlock();

	device->SetIndices(ib1);
}


void water :: update(float dampening,float tension)
{
		float x = targetHeight - height;
		speed += tension*x-speed*dampening;
		height +=speed;
}



class Object
{
	public:
	IDirect3DVertexBuffer9 *vb;
	Vertex *vert;

	IDirect3DIndexBuffer9 *ib;
	WORD *ind;
	int f;
	
	void initObject(float,float);
	void fall(int,int);

	Object()
	{
		f=0;
	}


};


void Object::initObject(float x1,float y1)
	{
		if(f==0)
		{
			d3ddev->CreateVertexBuffer(sizeof(Vertex)*4,0,D3DFVF_XYZ|D3DFVF_DIFFUSE,D3DPOOL_MANAGED,&vb,0);
			d3ddev->CreateIndexBuffer(sizeof(WORD)*6,0,D3DFMT_INDEX16,D3DPOOL_MANAGED,&ib,0);
		}
	
	vb->Lock(0,0,(void**)&vert,0);

   
	vert[0] = Vertex(x1,-y1,0,0.0f,1.0f);
	vert[1] = Vertex(x1,-y1+2,0,0.0f,0.0f);
	vert[2] = Vertex(x1+2,-y1+2,0.0,1.0f,0.0f);
	vert[3] = Vertex(x1+2,-y1,0.0,1.0f,1.0f);

	vb->Unlock();

	
	
	

	ib->Lock(0,0,(void**)&ind,0);
	ind[0] = 0;
	ind[1] = 1;
	ind[2] = 2;
	ind[3] = 2;
	ind[4] = 3;
	ind[5] = 0;

	ib->Unlock();

	device->SetIndices(ib);
	}


void waterSplash(int x,int speed,int direction,water * w)
{
	
	for(int i=x+10;i<x+12;i++)
		w[i].speed=speed*direction;
}

void wavesPropogation(water *w)
{
	for(int i =0;i<endPt;i++)
	{
		w[i].update(w[i].dampening,w[i].tension);
	}

	float lDeltas[30],rDeltas[30];

	for(int j=0;j<5;j++)
	{
		for(int i=0;i<endPt;i++)
		{
			if(i>0)
			{
				lDeltas[i] = w[i].spread*(w[i].height - w[i-1].height);
				w[i-1].speed +=lDeltas[i];
			}
			if(i<endPt-1)
			{
				rDeltas[i] = w[i].spread * (w[i].height - w[i+1].height);
				w[i+1].speed +=rDeltas[i];
			}
		}

		for(int i=0;i<endPt;i++)
		{
				if(i>0)
					w[i-1].height +=lDeltas[i];
				if(i<endPt-1)
					w[i+1].height +=rDeltas[i];
		}
	}

}

