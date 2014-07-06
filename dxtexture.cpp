#include "dxtexture.h"
#include <Math.h>
//constrcutors definitions
dxtexture::dxtexture(void)
{

}

dxtexture::dxtexture(float x1,float y1,float z1,float width1,float height1)
{
	x= x1;
	y=y1;
	z=z1;
	fxpos=x1;
	fypos=y1;
	fzpos=z1;
	width = width1;
	height = height1;
	bufferscreated = false;
	texturecreated = false;
	fxrot=0;
	fyrot=0;
	fzrot=0;
	fxmove=0;fymove=0;fzmove=0;
	fx=0;fy=0;
	fxr2=0,fyr2=0;fdiagnol=0;
	fxr1=0,fyr1=0;
	fxr3=0,fyr3=0;

	fxpos=x+fxrot+fxmove+fx;
	fypos=y+fyrot+fymove+fy;
		fAngle=0;
			x0=0;x1=0;x2=0;x3=0;
	y0=0;y1=0;y2=0;y3=0;

}
void dxtexture::rotate(float iangle)
{//for rotation around a point
	fAngle=iangle;
fdiagnol=sqrt((width*width)+(height*height));
fdiagnol/=2;




fx=0;
fy=0;	




/*workin points*/

fx=fdiagnol*cos(iangle*180/3.14);
fy=fdiagnol*sin(iangle*180/3.14);	


fxr1=fdiagnol*cos(iangle*180/3.14);
fyr1=fdiagnol*sin(iangle*180/3.14);

fxr2=fdiagnol*cos(iangle*180/3.14);
fyr2=fdiagnol*sin(iangle*180/3.14);


fxr3=fdiagnol*cos(iangle*180/3.14);
fyr3=fdiagnol*sin(iangle*180/3.14);






/*final psotion of x and y*/
fxpos=x+fxrot+fxmove+fx;
	fypos=y+fyrot+fymove+fy;


}

void dxtexture::createbuffers(IDirect3DDevice9 *device)
{
	if(bufferscreated == false)
	{
		device->CreateVertexBuffer(sizeof(vertex)*4,0,D3DFVF_XYZ|D3DFVF_TEX1,D3DPOOL_MANAGED,&vb,0);
		device->CreateIndexBuffer(sizeof(WORD)*6,0,D3DFMT_INDEX16,D3DPOOL_MANAGED,&ib,0);
		bufferscreated = true;
	}
}

void dxtexture::setcoordinates(IDirect3DDevice9 *device)
{
	//create buffers
	createbuffers(device);

	//assigning vertex value
	vb->Lock(0,0,(void**)&vert,0);
	//all the values of points x and y are added and vertex are created
	vert[0]=vertex(x+fxrot+fxmove+fx,y+fyrot+fymove+fy,z+fzrot+fzmove,0.0f,0.0f,D3DCOLOR_XRGB(256,0,0));
	vert[1]=vertex(x+width+fxrot+fxmove+fxr1,y-height+fyrot+fymove+fyr1,z+fzrot+fzmove,1.0f,1.0f,D3DCOLOR_XRGB(256,0,0));
	vert[2]=vertex(x+fxrot+fxmove+fxr2,y-height+fyrot+fymove+fyr2,z+fzrot+fzmove,0.0f,1.0f,D3DCOLOR_XRGB(256,0,0));
	vert[3]=vertex(x+width+fxrot+fxmove+fxr3,y+fyrot+fymove+fyr3,z+fzrot+fzmove,1.0f,0.0f,D3DCOLOR_XRGB(256,0,0));


	//testing launch code
	/*x0=x+fxrot+fxmove+fx;
	y0=y+fyrot+fymove+fy;
	x1=x+(height*sin(fAngle))+(width*cos(fAngle))+fxrot+fxmove+fxr1;
	y1=y-(height*cos(fAngle))+(width*sin(fAngle))+fyrot+fymove+fyr1;
	x2=x+(height*sin(fAngle))+fxrot+fxmove+fxr2;
	y2=y-(height*cos(fAngle))+fyrot+fymove+fyr2;
	x3=x+(width*cos(fAngle))+fxrot+fxmove+fxr3;
	y3=y+(width*sin(fAngle))+fyrot+fymove+fyr3;

		vert[0]=vertex(x0,y0,z+fzrot+fzmove,0.0f,0.0f,D3DCOLOR_XRGB(256,0,0));
	vert[1]=vertex(x1,y1,z+fzrot+fzmove,1.0f,1.0f,D3DCOLOR_XRGB(256,0,0));
	vert[2]=vertex(x2,y2,z+fzrot+fzmove,0.0f,1.0f,D3DCOLOR_XRGB(256,0,0));
	vert[3]=vertex(x3,y3,z+fzrot+fzmove,1.0f,0.0f,D3DCOLOR_XRGB(256,0,0));*/
	//launch code end


	
	
	
	
	
	
	
	
	
	
	
	/*final psotion of x and y for all objects created*/


	fxpos=x+fxrot+fxmove+fx;
	fypos=y+fyrot+fymove+fy;

	
	
	vb->Unlock();

	ib->Lock(0,0,(void**)&ind,0);
	ind[0] = 0;
	ind[1] = 1;
	ind[2] = 2;

	ind[3] = 0;
	ind[4] = 3;
	ind[5] = 1;
	ib->Unlock();

	device->SetIndices(ib);
}

void dxtexture::setstream(IDirect3DDevice9 *device)
{
		device->SetStreamSource(0,vb,0,sizeof(vertex));
		device->SetFVF(D3DFVF_XYZ|D3DFVF_TEX1);
	device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT);
	
		device->SetRenderState(D3DRS_LIGHTING,false);
	
}

void dxtexture::settexture(LPCSTR texf,IDirect3DDevice9 *device)
{
	//loading up texture once
	if(texturecreated == false)
	{
		D3DXCreateTextureFromFile(device,texf,&tex);
				

		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	device->SetRenderState(D3DRS_ALPHAREF, (DWORD)50);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
	

		texturecreated = true;
	}
	device->SetTexture(0,tex);
	device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_POINT);
}
void dxtexture::drawobject(LPCSTR texf,IDirect3DDevice9 *device)
{
	settexture(texf,device);
	
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);
}



void dxtexture::clearmem(IDirect3DDevice9 *device)
{
	device->Release();    // clearing all memory

	tex->Release();
	vb->Release();
	ib->Release();
}

dxtexture::~dxtexture()
{

}



