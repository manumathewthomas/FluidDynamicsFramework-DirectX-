
#include "Game1.h"
#include "directxHelper.h"
#include "windows.h"

//variables decalration

dxtexture ball,cannon,background,ship,ship2,ball2;

float iangle=0.00000f,iangle2=0,iTime=0,iTime2=0,fHighest2=0,fHighest1=0;

float fpower=0.3,fpower2=0.3;

float yFall=0,x,y,pointX,pointY;

bool bpressed=0,launch=0,newcannon=0,launch2=0,bpressed2=0,b1=0,b2=0;

int foo=0,f=0,threadCount=0,threadWaterCount=0;

int hit1=0,hit2=0,block=0;

water w[30]; 

texture2D bg,rockTex,shipTex;

POINT cursorPos;

bool waterCheck = false;

//objects are created

void textureLoad();

void waterInit();

void waterWaves();

void collision(dxtexture,dxtexture);



void waterWaves()
{
	for(int i=0;i<100;i++)
	{
		wavesPropogation(w);
			
		Sleep(10);
	
	}
		
}

Game1::Game1(void)
{	
	//objects of dxtexutre are created
	//their postion and width and height inspace is given
	
	background=dxtexture(-12.0f,12.0f,0.0f,25,23);
	
	ship=dxtexture(0.0f,2.0f,0.0f,6.0f,4.0f);
	
	ship2=dxtexture(-10.0f,2.0f,0.0f,6.0f,4.0f);
	
 	ball=dxtexture(ship.x+2,ship.y-1,0.0f,1.0f,1.0f);
 	
 	ball2=dxtexture(ship2.x+2,ship2.y-1,0.0f,1.0f,1.0f);
 	
	//camera variabes
	pos.x = 0.0f;
	
	pos.y = 0.0f;
	
	pos.z = -10.0f;

	tgt.x = 0.0f;
	
	tgt.y = 0.0f;
	
	tgt.z = 0.0f;

	up.x = 0.0f;
	
	up.y = 1.0f;
	
	up.z = 0.0f;


}

//contains the game logics

void Game1::UpdateGame(IDirect3DDevice9 *device)
{
	/* player 1 new ball*/
	
	if(GetAsyncKeyState(VK_UP))
	{
		ball=dxtexture(ship.fxpos+2,ship.fypos-1,0.0f,1.0f,1.0f);
		
		iTime=0;
		
		iangle=0.00003f;
		
		launch=0;
		
		fpower=0.3f;

		block=0;
		
		b1=0;

	}

	/* player 2 new ball*/
	
	if(GetAsyncKeyState(0x57))
	{
		/*2 ship*/
		
		ball2=dxtexture(ship2.fxpos+2,ship2.fypos-1,0.0f,1.0f,1.0f);
		
		iTime2=0;
		
		iangle2=0.00003f;
		
		launch2=0;
		
		fpower2=0.3f;
		
		b2=0;
		
		block=0;
		
	}

	/*ball rotation for player 1*/


	 if(GetAsyncKeyState(VK_LBUTTON) && launch==0 )
	{
		GetCursorPos(&cursorPos);
		
		x = 0;
		
        	x = cursorPos.x; 
        	
         	y = 0;
         	
        	y = cursorPos.y;

		bpressed=1;
	
		iangle+=0.00005f;
	
		ball.fxrot=0;
		
		ball.fyrot=0;
	
		ball.rotate(iangle);
		
		fpower+=0.000003;

	}


	 /*ball rotation for player 2*///shift key
	 
	  if(GetAsyncKeyState(VK_SHIFT) && launch2==0 )
	{
		bpressed2=1;
	
	
		iangle2+=0.00005f;
	
		ball2.fxrot=0;
		
		ball2.fyrot=0;
	
		ball2.rotate(iangle2);
		
		fpower2+=0.000003;

	}






	/*player 1 launch*/
	
		if(GetAsyncKeyState(VK_SPACE)&&launch2==0)
	{
	
		launch=1;
		
		waterCheck = false;


	}

		/*player 2 launch*/

	if(GetAsyncKeyState(0x45) &&launch==0)
	{
	
		launch2=1;

	}



	/*player 1 movements*/
	
	 if(GetAsyncKeyState(VK_RIGHT))
	 {
		 ship.fxmove+=0.0004;
	
		 if(launch==0)
		 
		 	ball.x+=0.0004;
	 
	 
	 }
	 
	 else if(GetAsyncKeyState(VK_LEFT))
	 {
	 
	 	ship.fxmove-=0.0004;
	 	
	 	if(launch==0)
	  		ball.x-=0.0004;
	 }

	 /*player 2 movements*/
	 
	if(GetAsyncKeyState(0x44))//right 'a'
	 {
		 ship2.fxmove+=0.0004;
		 
		 if(launch2==0)
		 
		 	ball2.x+=0.0004;
	 
	 
	 }
	 
	 else if(GetAsyncKeyState(0x41))//left 's'
	 {
	 	ship2.fxmove-=0.0004;
	 	
	 	if(launch2==0)
	 	
	  		ball2.x-=0.0004;
	 }


	/*launch for player  cannon*/

	 if(launch==1 &&launch2==0)
	 {
		 if(ship2.fxpos+ship2.width>ball.fxpos &&block==0&&hit2<=3&&ship2.fypos>ball.fypos+ball.height)
		 {
		 	block=1;
		 	
			hit2++;
			
			ship2.texturecreated=false;
		 }
		 
		 fHighest1=fpower*fpower*sin(iangle*180/3.14)*sin(iangle*180/3.14)/(2*0.009);
		 
		 if(ball.fypos>-3)
		 {
		 	iTime+=0.005;
		 	
			ball.fxmove=(iTime*fpower*cos(iangle*180/3.14));//0.3 v
		
			ball.fymove=(iTime*fpower*sin(iangle*180/3.14))-(0.5*0.009*iTime*iTime);//gravity 0.001
			
			waterCheck=true;
			
			if(ball.fxpos<ship.fxpos+ship.height)
			
				waterSplash(ball.fxpos,-fHighest1/2,-1,w);
		 }
		 
		 if(ball.fypos<-3)
		 {
			//DrawGame(device,1024,768);
			
			wavesPropogation(w);
					
			Sleep(30);
		}
	 }


	 /*launch for player 2 cannon*/

	  if(launch2==1 &&launch==0)
	 {
		 if(ship.fxpos<ball2.fxpos &&block==0 &&hit1<=3&&ship.fypos>ball2.fypos+ball2.height)
		 { 
			block=1;
			
			hit1++;
				
			ship.texturecreated=false;
		  }
		  
		 fHighest2=fpower2*fpower2*sin(iangle2*180/3.14)*sin(iangle2*180/3.14)/(2*0.009);
		 
		 if(ball2.fypos>-3)
		 {
		 	iTime2+=0.005;
		 	
			ball2.fxmove=(iTime2*fpower2*cos(iangle2*180/3.14));//0.3 v
		
			ball2.fymove=(iTime2*fpower2*sin(iangle2*180/3.14))-(0.5*0.009*iTime2*iTime2);//gravity 0.001



			if(ball2.fxpos<ship2.fxpos+ship.height)
			
				waterSplash(ball2.fxpos,-fHighest2/2,-1,w);
		 }
		 
		 if(ball2.fypos<-3)
		 {
				//DrawGame(device,1024,768);

			
						wavesPropogation(w);
						
			
							Sleep(30);
			
		}
	 }
	
	  	int shipPos,shipPos2;
						shipPos=ship.fxpos+ship.width/2;
						ship.fymove = w[shipPos+10].height+2;

	  
						shipPos2=ship2.fxpos+ship2.width/2;
						ship2.fymove = w[shipPos2+10].height+2;
	
	

}
//contains all the code for displaying
void Game1::DrawGame(IDirect3DDevice9 *device,float screenwidth,float screenheight)
{



	//Create Buffers for each objects
	
	//setting camera
	SetCamera(device,screenwidth,screenheight);

	
	


	
	//begin Scene
	//the water is called first after which other objects  are drawn.

	device->BeginScene();
		device->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);

	

		


//vertex and indices are created for the objects.and then texture is also loaded.
	background.setcoordinates(device);
		background.setstream(device);
	background.drawobject("sky.jpg",device);

			
			startPt=-10;
	
	for(int waterCount=1;waterCount<endPt;waterCount++)
	{
		if(waterCount>1)
		w[waterCount].heightPrev = w[waterCount-1].height;
		else
		w[waterCount].heightPrev = -3;

		w[waterCount].initWater(startPt++,maxHeight,-3.0,-3.0,0,device);

	}

	for(int waterCount=1;waterCount<endPt;waterCount++)
	{

	device->SetStreamSource(0,w[waterCount].vb1,0,sizeof(VertexColor));
	colorVertexInit(SHADE_GOURAUD,device);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	}


		if(bpressed==1)//to flag t ocheck whther to print ball or not.
	ball.setcoordinates(device);
	if(bpressed2==1)
	ball2.setcoordinates(device);
		ship.setcoordinates(device);
		ship2.setcoordinates(device);
	
	cannon.setstream(device);
cannon.drawobject("cb.png",device);
	

	ship.setstream(device);
	if(hit1==0)
	ship.drawobject("ps.png",device);
	else if(hit1==1)
ship.drawobject("pss.png",device);
	else if(hit1==2)
ship.drawobject("pss2.png",device);
	else 
ship.drawobject("pss3.png",device);

	ship2.setstream(device);
	if(hit2==0)
	ship2.drawobject("ps2.png",device);
	else if(hit2==1)
		ship2.drawobject("ps2s2.png",device);
	else if(hit2==2)
		ship2.drawobject("ps2s3.png",device);
	else 
		ship2.drawobject("ps2s4.png",device);

	if(bpressed==1 && ball.fypos+ball.height>ship.fypos-2.5 &&b1==0)//to check whether ball is gone below ship level.
	{
	
	ball.setstream(device);
	ball.drawobject("cb.png",device);

	}
	else
		b1=1;
	if(bpressed2==1&& ball2.fypos+ball2.height>ship2.fypos-2.5 && b2==0)//to check whether ball is gone below ship level.
	{
	
	ball2.setstream(device);
	ball2.drawobject("cb.png",device);

	}
	else
		b2=1;
	




	device->EndScene();
	device->Present(0,0,0,0);



}
//camera is set here.
void Game1::SetCamera(IDirect3DDevice9 *device,float screenwidth,float screenheight)
{
	D3DXMatrixLookAtLH(&v,&pos,&tgt,&up);
	device->SetTransform(D3DTS_VIEW,&v);

	D3DXMatrixPerspectiveFovLH(&proj,D3DX_PI*0.5,1024/768,1.0f,1000.f);
	device->SetTransform(D3DTS_PROJECTION,&proj);
}
Game1::~Game1(void)
{
}

IDirect3DDevice9 *getDev()
{
	return device;
}


