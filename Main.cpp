
#include <Windows.h>
#include <d3dx9.h>
#include "Game1.h"


IDirect3DDevice9 *d3ddev1;

HWND hWnd;
Game1 g1;


//Function Declarations
bool initWnd(HINSTANCE,int);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void initD3D(void);
int MsgLoop(void);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int ShowCmd)
{
	if(!initWnd(hInstance,ShowCmd))
	{
		MessageBox(0,"Creation Failed",0,0);
		return 0;
	}

	initD3D();
	return MsgLoop();
	

}


//Function Definitions
int MsgLoop(void)
{
		
	MSG msg;
	ZeroMemory(&msg,sizeof(MSG));

	while(TRUE)
	{
    // Check to see if any messages are waiting in the queue
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
			break;

    // Run game code here
		//update and draw func are called continously
		g1.UpdateGame(d3ddev1);
		g1.DrawGame(d3ddev1,800,600);

	}


    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}
bool initWnd(HINSTANCE hInstance,int ShowCmd)
{
	WNDCLASS w;

	ZeroMemory(&w, sizeof(WNDCLASS));

	//w.cbSize = sizeof(WNDCLASS)
	w.style = CS_HREDRAW|CS_VREDRAW;
	w.lpfnWndProc = WndProc;
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;
	w.hIcon = LoadIcon(0,IDI_APPLICATION);
	w.hCursor = LoadCursor(0,IDC_ARROW);
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	w.hInstance = hInstance;
	w.lpszMenuName = 0;
	w.lpszClassName = "WindowClass1";

	if(!RegisterClass(&w))
	{
		MessageBox(0,"Registration Failed",0,0);
		return false;
	}

	hWnd = CreateWindow("WindowClass1","waveprojectileframework",WS_OVERLAPPEDWINDOW,0,0,800,600,0,0,hInstance,0);

	if(!hWnd)
	{
		MessageBox(0,"Creation Failed",0,0);
		return false;
	}

	ShowWindow(hWnd,ShowCmd);
	//UpdateWindow(hWnd);
	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch(message)
    {
        // this message is read when the window is closed
        case WM_DESTROY:
            {
                // close the application entirely
                PostQuitMessage(0);
                return 0;
            } break;
		case WM_KEYDOWN:
			{
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWnd);
					return 0;
				}
				
			}
    }
    // Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, message, wParam, lParam);
}

void initD3D()
{
	IDirect3D9 *d3d;
	d3d= Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 caps;
	
	d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,&caps);
	int vp=0;
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp=D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.BackBufferWidth=800;
	d3dpp.BackBufferHeight=600;
	d3dpp.BackBufferFormat=D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount=1;
	d3dpp.MultiSampleType=D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality=0;
	d3dpp.SwapEffect =D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed=true;
	d3dpp.hDeviceWindow=hWnd;
	d3dpp.EnableAutoDepthStencil=true;
	d3dpp.AutoDepthStencilFormat=D3DFMT_D24S8;
	d3dpp.Flags=0;
	d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
	HRESULT res;
	res=d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &d3ddev1);
	if(FAILED(res))
	{
		MessageBox(0,"Device Creation Failed",0,0);
	}
}