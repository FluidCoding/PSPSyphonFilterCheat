//Includes
/**
	Author: Brian Sonnie
	Date: 8/29/2009
*/
#define _PSP_FW_VERSION 150
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspdisplay_kernel.h>
#include <pspkerneltypes.h>
#include <pspmoduleinfo.h>
#include <pspiofilemgr.h>
#include <pspmodulemgr.h>
#include <pspthreadman.h>
#include <math.h>
#include <time.h> 
#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>
#include <string.h>
#include <pspctrl_kernel.h>
#include <psppower.h>
#include "menu.h"
#define print pspDebugScreenPuts
#define tcolor pspDebugScreenSetTextColor


PSP_MODULE_INFO("SFCD", 0x3010, 1, 2); 
PSP_MAIN_THREAD_ATTR(0);
PSP_HEAP_SIZE_KB(1012);

//Threads
SceUID thid;
SceCtrlData pad;
void *vram;
unsigned char buffer[10];
int fd;
char menudrawn=0;
int offcolor=0xFF000000;
unsigned char gameId[10];
int running=0;
int module_start(SceSize args, void *argp) __attribute__((alias("_start")));
int module_stop(SceSize args, void *argp) __attribute__((alias("_stop")));


//Colors
unsigned int lgreen = 0x0066ff00;
//unsigned int green = 0x
unsigned int white = 0xFFFFFFFF;
unsigned int black = 0x00000000;
unsigned int red =  0x000000FF;
unsigned int dred = 0x00000080;
unsigned int grey = 0x00808080;
unsigned int lblue = 0x00FFFF00;
unsigned int blue = 0x00FF0000;
unsigned int yellow = 0xFF00FFFF;
//Vars
int pctab=1;

//Code Values
unsigned int jrra = 0x03e00008;
unsigned int respawntime0 = 0x00000000;
unsigned int respawntime5 = 0x40a00000;
unsigned int nop = 0x00000000;

unsigned int on="[ENABLED]";
unsigned int off="[DISABLED]";

//CODES
//Code Display Data
unsigned int isInfiniteHealthON = "[DISABLED]";
unsigned int isInfiniteAmmoON = "[DISABLED]";
unsigned int isNoFallDamageON = "[DISABLED]";
unsigned int isInstantStartON = "[DISABLED]";
unsigned int isGameOpsON = "[DISABLED]";
unsigned int isTeamDamageON = "[DISABLED]";
unsigned int isInstantRespawnON = "[DISABLED]";
unsigned int isTargetLockON = "[DISABLED]";
unsigned int isVisionON = "[DISABLED]";
unsigned int isKillsON = "[DISABLED]";
unsigned int isInfiniteGAmmoON = "[DISABLED]";
unsigned int isStwON="[DISABLED]";
unsigned int isSwON = "[DISABLED]";
unsigned int isSwJON ="[DISABLED]";
unsigned int isRfON = "[DISABLED]";
unsigned int isT2EON =  "[DISABLED]";
unsigned int isMarON =  "[DISABLED]";
unsigned int isZmON = "[DISABLED]";

//NON DMA CODES
unsigned int *InfiniteHealth=(unsigned int*) (0x003011c4+0x08800000);
unsigned int *infammo=(unsigned int*) (0x00298114+0x08800000);
unsigned int *infgammo = (unsigned int*) (0x002c47e8 + 0x08800000);
unsigned int *NoFallDamage=(unsigned int*) (0x0012CBE4+0x08800000);
unsigned int *cleargoggles =(unsigned int*) (0x08e96050);
unsigned int *vision = (unsigned int*) (0x08e14174); 
unsigned int *stw = (unsigned int*) (0x00564A8C+0x08800000);
unsigned int *sw1 = (unsigned int*) (0x0034DA34+0x08800000);
unsigned int *sw2 = (unsigned int*) (0x00420170+0x08800000);
unsigned int *sw3 = (unsigned int*) (0x00420174+0x08800000);
unsigned int *sw4 = (unsigned int*) (0x0034D830+0x08800000);
unsigned int *sw5 = (unsigned int*) (0x0034D85C+0x08800000);
unsigned int *sw6 = (unsigned int*) (0x0034D784+0x08800000);
unsigned int *rf = (unsigned int*) (0x08a972e0); 
unsigned int *Zm = (unsigned int*) (0x08e15138); 


//Skywalker+Height control
unsigned int *heightjoker1 = (unsigned int*) (0x00000200 + 0x08800000);
unsigned int *heightjoker2 = (unsigned int*) (0x00000204 + 0x08800000);
unsigned int *heightjoker3 = (unsigned int*) (0x00000208 + 0x08800000);
unsigned int *heightjoker4 = (unsigned int*) (0x0000020c + 0x08800000);
unsigned int *heightjoker5 = (unsigned int*) (0x00000210 + 0x08800000);
unsigned int *heightjoker6 = (unsigned int*) (0x00000214 + 0x08800000);
unsigned int *heightjoker7 = (unsigned int*) (0x00000218 + 0x08800000);
unsigned int *heightjoker8 = (unsigned int*) (0x0000021c + 0x08800000);
unsigned int *heightjoker9 = (unsigned int*) (0x00000220 + 0x08800000);
unsigned int *heightjoker10 = (unsigned int*) (0x00000224 + 0x08800000);
unsigned int *heightjoker11 = (unsigned int*) (0x00000228 + 0x08800000);
unsigned int *heightjoker12 = (unsigned int*) (0x0000022c + 0x08800000);
unsigned int *heightjoker13 = (unsigned int*) (0x00000230 + 0x08800000);
unsigned int *heightjoker14 = (unsigned int*) (0x00000234 + 0x08800000);
unsigned int *heightjoker15 = (unsigned int*) (0x00000238 + 0x08800000);
unsigned int *heightjoker16 = (unsigned int*) (0x0000023c + 0x08800000);
unsigned int *heightjoker17 = (unsigned int*) (0x00000240 + 0x08800000);
unsigned int *heightjoker18 = (unsigned int*) (0x00000244 + 0x08800000);
unsigned int *heightjoker19 = (unsigned int*) (0x00000248 + 0x08800000);
unsigned int *heightjoker20 = (unsigned int*) (0x0000024c + 0x08800000);
unsigned int *heightjoker21 = (unsigned int*) (0x00000250 + 0x08800000);
unsigned int *heightjokerhk = (unsigned int*) (0x00449be8 + 0x08800000); 

//Teleport To Enemy
unsigned int *t2e1 = (unsigned int*) (0x00000280 +0x08800000);
unsigned int *t2e2 = (unsigned int*) (0x00000284 +0x08800000);
unsigned int *t2e3 = (unsigned int*) (0x00000288 +0x08800000);
unsigned int *t2e4 = (unsigned int*) (0x0000028c +0x08800000);
unsigned int *t2e5 = (unsigned int*) (0x00000290 +0x08800000);
unsigned int *t2e6 = (unsigned int*) (0x00000294 +0x08800000);
unsigned int *t2e7 = (unsigned int*) (0x00000298 +0x08800000);
unsigned int *t2e8 = (unsigned int*) (0x0000029c +0x08800000);
unsigned int *t2e9 = (unsigned int*) (0x000002a0 +0x08800000);
unsigned int *t2e10 = (unsigned int*) (0x000002a4 +0x08800000);
unsigned int *t2e11 = (unsigned int*) (0x000002a8 +0x08800000);
unsigned int *t2e12 = (unsigned int*) (0x000002ac +0x08800000);
unsigned int *t2e13 = (unsigned int*) (0x000002b0 +0x08800000);
unsigned int *t2e14 = (unsigned int*) (0x000002b4 +0x08800000);
unsigned int *t2e15 = (unsigned int*) (0x000002b8 +0x08800000);
unsigned int *t2e16 = (unsigned int*) (0x000002bc +0x08800000);
unsigned int *t2e17 = (unsigned int*) (0x000002c0 +0x08800000);
unsigned int *t2e18 = (unsigned int*) (0x000002c4 +0x08800000);
unsigned int *t2e19 = (unsigned int*) (0x000002c8 +0x08800000);
unsigned int *t2e20 = (unsigned int*) (0x000002cc +0x08800000);
unsigned int *t2e21 = (unsigned int*) (0x000002d0 +0x08800000);
unsigned int *t2e22 = (unsigned int*) (0x000002d4 +0x08800000);
unsigned int *t2e23 = (unsigned int*) (0x000002d8 +0x08800000);
unsigned int *t2e24 = (unsigned int*) (0x000002dc +0x08800000);
unsigned int *t2e25 = (unsigned int*) (0x000002e0 +0x08800000);
unsigned int *t2e26 = (unsigned int*) (0x000002e4 +0x08800000);
unsigned int *t2ehk = (unsigned int*) (0x00449be8  +0x08800000);

//Mark & Recall
unsigned int *mar1 = (unsigned int*) (0x08800310);
unsigned int *mar2 = (unsigned int*) (0x08800314);
unsigned int *mar3 = (unsigned int*) (0x08800318);
unsigned int *mar4 = (unsigned int*) (0x0880031c);
unsigned int *mar5 = (unsigned int*) (0x08800320);
unsigned int *mar6 = (unsigned int*) (0x08800324);
unsigned int *mar7 = (unsigned int*) (0x08800328);
unsigned int *mar8 = (unsigned int*) (0x0880032c);
unsigned int *mar9 = (unsigned int*) (0x08800330);
unsigned int *mar10 = (unsigned int*) (0x08800334);
unsigned int *mar11 = (unsigned int*) (0x08800338);
unsigned int *mar12 = (unsigned int*) (0x0880033c);
unsigned int *mar13 = (unsigned int*) (0x08800340);
unsigned int *mar14 = (unsigned int*) (0x08800344);
unsigned int *mar15 = (unsigned int*) (0x08800348);
unsigned int *mar16 = (unsigned int*) (0x0880034c);
unsigned int *mar17 = (unsigned int*) (0x08800350);
unsigned int *mar18 = (unsigned int*) (0x08800354);
unsigned int *mar19 = (unsigned int*) (0x08800358);
unsigned int *mar20 = (unsigned int*) (0x0880035c);
unsigned int *mar21 = (unsigned int*) (0x08800360);
unsigned int *mar22 = (unsigned int*) (0x08800364);
unsigned int *mar23 = (unsigned int*) (0x08800368);
unsigned int *mar24 = (unsigned int*) (0x0880036c);
unsigned int *mar25 = (unsigned int*) (0x08800370);
unsigned int *mar26 = (unsigned int*) (0x08800374);
unsigned int *mar27 = (unsigned int*) (0x08800378);
unsigned int *mar28 = (unsigned int*) (0x0880037c);
unsigned int *mar29 = (unsigned int*) (0x08800380);
unsigned int *mar30 = (unsigned int*) (0x08800384);
unsigned int *mar31 = (unsigned int*) (0x08800388);
unsigned int *mar32 = (unsigned int*) (0x0880038c);
unsigned int *mar33 = (unsigned int*) (0x08800390);
unsigned int *mar34 = (unsigned int*) (0x08800394);
unsigned int *mar35 = (unsigned int*) (0x08800398);
unsigned int *mar36 = (unsigned int*) (0x0880039c);
unsigned int *mar37 = (unsigned int*) (0x088003a0);
unsigned int *mar38 = (unsigned int*) (0x088003a4);
unsigned int *mar39 = (unsigned int*) (0x088003a8);
unsigned int *mar40 = (unsigned int*) (0x088003ac);
unsigned int *mar41 = (unsigned int*) (0x088003b0);
unsigned int *mar42 = (unsigned int*) (0x088003b4);
unsigned int *mar43 = (unsigned int*) (0x088003b8);
unsigned int *mar44 = (unsigned int*) (0x088003bc);
unsigned int *mar45 = (unsigned int*) (0x088003c0);
unsigned int *mar46 = (unsigned int*) (0x088003c4);
unsigned int *mar47 = (unsigned int*) (0x088003c8);
unsigned int *mar48 = (unsigned int*) (0x088003cc);
unsigned int *mar49 = (unsigned int*) (0x088003d0);
unsigned int *mar50 = (unsigned int*) (0x088003d4);
unsigned int *mar51 = (unsigned int*) (0x088003d8);
unsigned int *marhk = (unsigned int*) (0x00449bd0 + 0x08800000);

int pccheat = 1;

#define MAX_THREAD 64

//Refresh the buttons
void refresh()
{
	pad.Buttons=0;
	sceCtrlPeekBufferPositive(&pad, 1);
	sceCtrlSetSamplingCycle(0); 
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

void codedisp()
{
	if(pccheat==1){tcolor(red);}
	else{tcolor(blue);}
	print("\nInfinite Health  ");
	print(isInfiniteHealthON);
	
	if(pccheat==2){tcolor(red);}
	else{tcolor(blue);}
	print("\nInfinite Ammuntion  ");
	print(isInfiniteAmmoON);

	if(pccheat==3){tcolor(red);}
	else{tcolor(blue);}
	print("\nNo Fall Damage  ");
	print(isNoFallDamageON);

	if(pccheat==4){tcolor(red);}
	else{tcolor(blue);}
	print("\nInfinite Grenades  ");
	print(isInfiniteGAmmoON);

	if(pccheat==5){tcolor(red);}
	else{tcolor(blue);}
	print("\nShoot Through Walls  ");
	print(isStwON);

	if(pccheat==6){tcolor(red);}
	else{tcolor(blue);}
	print("\nSkywalk+Height Control  ");
	print(isSwJON);

	if(pccheat==7){tcolor(red);}
	else{tcolor(blue);}
	print("\nSkywalker  ");
	print(isSwON);

	if(pccheat==8){tcolor(red);}
	else{tcolor(blue);}
	print("\nRapid Fire  ");
	print(isRfON);

	if(pccheat==9){tcolor(red);}
	else{tcolor(blue);}
	print("\nTeleport To Enemies  ");
	print(isT2EON);

	if(pccheat==10){tcolor(red);}
	else{tcolor(blue);}
	print("\nMark & Recall  ");
	print(isMarON);
	
	if(pccheat==11){tcolor(red);}
	else{tcolor(blue);}
	print("\nZoom W/O Scope  ");
	print(isZmON);

	switch(pccheat)
	{
	case 1:
	if((pad.Buttons & PSP_CTRL_DOWN))
		{
			pccheat=2;
		}
	if((pad.Buttons & PSP_CTRL_LEFT))
		{
			isInfiniteHealthON="[DISABLED]";
		}
	if((pad.Buttons & PSP_CTRL_RIGHT))
		{
			isInfiniteHealthON="[ENABLED]";
		}
	break;
	case 2:
		if((pad.Buttons & PSP_CTRL_DOWN))
		{
			pccheat=3;
		}
		else if((pad.Buttons & PSP_CTRL_UP))
		{
			pccheat=1;
		}
		if((pad.Buttons & PSP_CTRL_RIGHT))
		{
			isInfiniteAmmoON="[ENABLED]";
		}
		if((pad.Buttons & PSP_CTRL_LEFT))
		{
			isInfiniteAmmoON="[DISABLED]";
		}
	break;
	case 3:
		if((pad.Buttons & PSP_CTRL_UP))
		{
			pccheat=2;
		}
		if((pad.Buttons & PSP_CTRL_DOWN))
		{
			pccheat=4;
		}
		if((pad.Buttons & PSP_CTRL_RIGHT))
		{
			isNoFallDamageON="[ENABLED]";
		}
		if((pad.Buttons & PSP_CTRL_LEFT))
		{
			isNoFallDamageON="[DISABLED]";
		}
	break;
	case 4:
		if((pad.Buttons & PSP_CTRL_UP))
		{
			pccheat=3;
		}
		if((pad.Buttons & PSP_CTRL_DOWN))
		{
			pccheat=5;
		}
		if((pad.Buttons & PSP_CTRL_RIGHT))
		{
			isInfiniteGAmmoON="[ENABLED]";
		}
		if((pad.Buttons & PSP_CTRL_LEFT))
		{
			isInfiniteGAmmoON="[DISABLED]";
		}
	break;
	case 5:
		if((pad.Buttons & PSP_CTRL_UP))
		{
			pccheat=4;
		}
		if((pad.Buttons & PSP_CTRL_DOWN))
		{
			pccheat=6;
		}
		if((pad.Buttons & PSP_CTRL_RIGHT))
		{
			isStwON="[ENABLED]";
		}
		if((pad.Buttons & PSP_CTRL_LEFT))
		{
			isStwON="[DISABLED]";
		}
	break;
	case 6:
		if((pad.Buttons & PSP_CTRL_UP))
		{
			pccheat=5;
		}
		if((pad.Buttons & PSP_CTRL_DOWN))
		{
			pccheat=7;
		}
		if((pad.Buttons & PSP_CTRL_RIGHT))
		{
			isSwJON="[ENABLED]";
		}
		if((pad.Buttons & PSP_CTRL_LEFT))
		{
			isSwJON="[DISABLED]";
		}
	break;
		case 7:
		if((pad.Buttons & PSP_CTRL_UP))
		{
			pccheat=6;
		}
		if((pad.Buttons & PSP_CTRL_DOWN))
		{
			pccheat=8;
		}
		if((pad.Buttons & PSP_CTRL_RIGHT))
		{
			isSwON="[ENABLED]";
		}
		if((pad.Buttons & PSP_CTRL_LEFT))
		{
			isSwON="[DISABLED]";
		}
	break;
	case 8:
		if((pad.Buttons & PSP_CTRL_UP))
		{
			pccheat=7;
		}
		if((pad.Buttons & PSP_CTRL_DOWN))
		{
			pccheat=9;
		}
		if((pad.Buttons & PSP_CTRL_RIGHT))
		{
			isRfON="[ENABLED]";
		}
		if((pad.Buttons & PSP_CTRL_LEFT))
		{
			isRfON="[DISABLED]";
		}
	break;
	case 9:
		if((pad.Buttons & PSP_CTRL_UP))
		{
			pccheat=8;
		}
		if((pad.Buttons & PSP_CTRL_DOWN))
		{
			pccheat=10;
		}
		if((pad.Buttons & PSP_CTRL_RIGHT))
		{
			isT2EON="[ENABLED]";
		}
		if((pad.Buttons & PSP_CTRL_LEFT))
		{
			isT2EON="[DISABLED]";
		}
	break;
	case 10:
		if((pad.Buttons & PSP_CTRL_UP))
		{
			pccheat=9;
		}
		if((pad.Buttons & PSP_CTRL_DOWN))
		{
			pccheat=11;
		}
		if((pad.Buttons & PSP_CTRL_RIGHT))
		{
			isMarON="[ENABLED]";
		}
		if((pad.Buttons & PSP_CTRL_LEFT))
		{
			isMarON="[DISABLED]";
		}
	break;
	case 11:
		if((pad.Buttons & PSP_CTRL_UP))
		{
			pccheat=10;
		}
		if((pad.Buttons & PSP_CTRL_DOWN))
		{
			pccheat=11;
		}
		if((pad.Buttons & PSP_CTRL_RIGHT))
		{
			isZmON="[ENABLED]";
		}
		if((pad.Buttons & PSP_CTRL_LEFT))
		{
			isZmON="[DISABLED]";
		}
	break;

	}
}





void enablecheat()
{
	if(isInfiniteHealthON==on)
	{
	    *InfiniteHealth=0x10000005; 
	}
	else if(isInfiniteHealthON=off)
	{
		if(*InfiniteHealth=0x10000005)
		{
		  *InfiniteHealth=0x50850005; 
	
		}
	}
	if(isNoFallDamageON==on)
	{
		*NoFallDamage=jrra;
	}
	else if(isNoFallDamageON==off)
	{
		if(*NoFallDamage!=0x27bdff30)
		{
		*NoFallDamage=0x27bdff30;
		}
	}
	if(isInfiniteAmmoON==on)
	{
		*infammo=0x24450000;
	}
	else if(isInfiniteAmmoON==off)
	{
		if(*infammo!=0x2445FFFF)
		{
		*infammo=0x2445FFFF;
		}
	}
		if(isInfiniteGAmmoON==on)
	{
		*infgammo=0x24840000;
	}
	else if(isInfiniteGAmmoON==off)
	{
		if(*infgammo!=0x2484FFFF)
		{
		*infgammo=0x2484FFFF;
		}
	}
	if(isStwON==on)
	{
		*stw=jrra;
	}
	else if(isStwON==off)
	{
		if(*stw!=0x27bdff30)
		{
		*stw=0x27bdff30;
		}
	}
	if(isSwJON==on)
	{
		
		*sw1 = nop;
		*sw2 = jrra;
		*sw3 = nop;
		*sw4 = nop;
		*sw5 = nop;
		*sw6 = nop;
		*heightjokerhk = 0x0e200080;
		*heightjoker1 = 0x3c0808e2;
		*heightjoker2 = 0x85089398;
		*heightjoker3 = 0x3c0908e0;
		*heightjoker4 = 0x340b0090;
		*heightjoker5 = 0x256c0030;
		*heightjoker6 = 0x150b0006;
		*heightjoker7 = 0x3c190200;
		*heightjoker8 = 0x8d296398;
		*heightjoker9 = 0x8d2904c4;
		*heightjoker10 = 0x8d2a0028;
		*heightjoker11 = 0x01596820;
		*heightjoker12 = 0xad2d0028;
		*heightjoker13 = 0x150c0006;
		*heightjoker14 = nop;
		*heightjoker15 = 0x8d296398;
		*heightjoker16 = 0x8d2904c4;
		*heightjoker17 = 0x8d2a0028;
		*heightjoker18 = 0x01596822;
		*heightjoker19 = 0xad2d0028;
		*heightjoker20 = 0x03e00008;
		*heightjoker21 = 0x0e313e83;

	}
	else if(isSwJON==off && *heightjokerhk!=0x0e313e83)
	{
		
		*sw1 = 0x0e2d434e;
		*sw2 = 0x27bdff90;
		*sw3 = 0xafb3005c;
		*sw4 = 0x10800007;
		*sw5 = 0x10800006;
		*sw6 = 0x10a00008;
		*heightjokerhk=0x0e313e83;
	}

	if(isSwON==on)
	{
		*sw1 = nop;
		*sw2 = jrra;
		*sw3 = nop;
		*sw4 = nop;
		*sw5 = nop;
		*sw6 = nop;
	}
	else if(isSwON==off && *sw1 != 0x0e2d434e && *heightjokerhk==0x0e313e83)
	{
		*sw1 = 0x0e2d434e;
		*sw2 = 0x27bdff90;
		*sw3 = 0xafb3005c;
		*sw4 = 0x10800007;
		*sw5 = 0x10800006;
		*sw6 = 0x10a00008;
	}
	if(isRfON==on)
	{
		*rf=0x3c043e00;
	}
	if(isRfON==off&&*rf!=0x3c043f80)
	{
		*rf=0x3c043f80;
	}
	if(isT2EON==on)
	{
		*t2e1 = 0x3c0808e2;
		*t2e2 = 0x85089398;
		*t2e3 = 0x340b0210;
		*t2e4 = 0x3c0908e0;
		*t2e5 = 0x3c1908ee;
		*t2e6 = 0x150b0012; 
		*t2e7 = nop;
		*t2e8 = 0x8d296398;
		*t2e9 = 0x1120000f;
		*t2e10 = nop;
		*t2e11 = 0x8f39b978;
		*t2e12 = 0x1320000c;
		*t2e13 = nop;
		*t2e14 = 0x8f3904c4;
		*t2e15 = 0x8d2904c4;
		*t2e16 = 0x8f2c0020;
		*t2e17 = 0x8f2d0024;
		*t2e18 = 0x8f2e0028;
		*t2e19 = 0xad2c0020;
		*t2e20 = 0xad2c0050;
		*t2e21 = 0xad2d0024;
		*t2e22 = 0xad2d0054;
		*t2e23 = 0xad2e0028;
		*t2e24 = 0xad2e0058;
		*t2e25 = jrra;
		*t2e26 = 0x0e313e83;
		*t2ehk = 0x0e2000a0;
	}
	
	else if(isT2EON == off && *t2ehk != 0x0e313e83)
	{
		*t2ehk = 0x0e313e83;
	}

	if(isMarON==on)
	{
		*mar1 =0x3c0808e2;
		*mar2 =0x85089398;
		*mar3 =0x8d296398;
		*mar4 =0x3c0908e0;
		*mar5 =0x8d296398;
		*mar6 =0x8d2904c4;
		*mar7 =0x34190110;
		*mar8 = 0x34180140;
		*mar9 = 0x340f0210;
		*mar10=0x340e0240;
		*mar11=0x3c0a0880;
		*mar12 =0x15190007;
		*mar13 =nop;
		*mar14 =0x8d2b0020;
		*mar15 =0xad4b03e0;
		*mar16 =0x8d2b0024;
		*mar17 =0xad4b03e4;
		*mar18 =0x8d2b0028;
		*mar19 =0xad4b03e8;
		*mar20 =0x15180007;
		*mar21 =nop;
		*mar22 =0x8d2b0020;
		*mar23 =0xad4b03ec;
		*mar24 =0x8d2b0024;
		*mar25 =0xad4b03f0;
		*mar26 =0x8d2b0028;
		*mar27 =0xad4b03f4;
		*mar28 =0x150f000a;
		*mar29 =nop;
		*mar30 =0x8d4b03e0;
		*mar31 =0x8d4c03e4;
		*mar32 =0x8d4d03e8;
		*mar33 =0xad2b0020;
		*mar34 =0xad2b0050;
		*mar35 =0xad2c0024;
		*mar36 =0xad2c0054;
		*mar37 =0xad2d0028;
		*mar38 =0xad2d0058;
		*mar39 =0x150e000a;
		*mar40 =nop;
		*mar41 =0x8d4b03ec;
		*mar42 =0x8d4c03f0;
		*mar43 =0x8d4d03f4;
		*mar44 =0xad2b0020;
		*mar45 =0xad2b0050;
		*mar46 =0xad2c0024;
		*mar47 =0xad2c0054;
		*mar48 =0xad2d0028;
		*mar49 =0xad2d0058;
		*mar50 =0x03e00008;
		*mar51 =0x0e3127fd;
		*marhk = 0x0e2000c4;
	}
	if(isZmON==on)
	{
		*Zm=0x40B33333;
	}
	else if(isZmON==off && *Zm != 0x3fb33333)
	{
		*Zm = 0x3fb33333;
	}
	

}


void imposterdisp()
{
	tcolor(yellow);
	print("This Will Be implemented in Rev2 of sfcd ;]");

}

void settingdisp()
{
	



}

//Draw the menu
int drawmeup()
{
while(menudrawn)
{
	startMenu();
	pad.Buttons=0;
	refresh();
	if(pad.Buttons & PSP_CTRL_CIRCLE)
	{
	//Return the standard VRAM
    sceDisplaySetFrameBufferInternal(0, 0, 512, 0, 1);
      
    //Allow the game to receive input
    sceCtrlSetButtonMasks(0x10000, 0); // Unset HOME key
    sceCtrlSetButtonMasks(0xFFFF, 0);  // Unset mask
	if(menudrawn) menudrawn=0;
	continue;
	}
	else if((pad.Buttons & PSP_CTRL_START) && (pad.Buttons & PSP_CTRL_SELECT))
	{
	running=0;
	menudrawn=0;
	}
drawmenu();


//Tab Display
	pad.Buttons=0;
	refresh();
	switch(pctab)
	{
	case 1:
		if(pad.Buttons & PSP_CTRL_RTRIGGER)
		{
			pctab=2;
pspDebugScreenClear();
		}
	break;
	case 2:
		if(pad.Buttons & PSP_CTRL_RTRIGGER)
		{
			pctab=3;
pspDebugScreenClear();
		}
		else if(pad.Buttons & PSP_CTRL_LTRIGGER)
		{
			pctab=1;
pspDebugScreenClear();
		}
	break;
	case 3:
		if(pad.Buttons & PSP_CTRL_LTRIGGER)
		{
			pctab=2;
pspDebugScreenClear();
		}
		break;
	}
	refresh();
	sceKernelDelayThread(10000);
	
}
}


int drawmenu()
{

	pspDebugScreenSetXY(9, 0);
	tcolor(white);
	print("[SFCD Rev1] http://www.psphs.com");
	pspDebugScreenSetXY(0, 1);
	//Draw Tabs
	if(pctab==1){pspDebugScreenSetTextColor(red);}
	else{pspDebugScreenSetTextColor(dred);}
	print("[CODES]    ");
	if(pctab==2){pspDebugScreenSetTextColor(red);}
	else{pspDebugScreenSetTextColor(dred);}
	print("[IMPOSTOR]    ");
	if(pctab==3){pspDebugScreenSetTextColor(red);}
	else{pspDebugScreenSetTextColor(dred);}
	print("[SETTINGS]    ");
	pspDebugScreenSetTextColor(red);
	print("\n--------------------------------------------------------------------");//END DRAW TABS
	refresh();
	if(menudrawn){
	if(pctab==1){codedisp();}
	if(pctab==2){imposterdisp();}
	if(pctab==3){settingdisp();}
	}
	sceKernelDelayThread(100000);
	refresh();



}

int menucheck()
{
if((pad.Buttons & PSP_CTRL_LTRIGGER) && (pad.Buttons & PSP_CTRL_VOLUP))
{
if(!menudrawn) menudrawn=1;
}
if(menudrawn)
{
//Stop the game from receiving input (USER mode input block)
      sceCtrlSetButtonMasks(0xFFFF, 1);  // Mask lower 16bits
      sceCtrlSetButtonMasks(0x10000, 2); // Always return HOME key
      
		//Setup a custom VRAM
    	sceDisplaySetFrameBufferInternal(0, vram, 512, 0, 1);
    	pspDebugScreenInitEx(vram, 0, 0);
	}
	drawmeup();
}


int mainThread(){
    signed int fd;
	running=1;
	// Pause while game loads
	sceKernelDelayThread(15000000);
	while(!sceKernelFindModuleByName("sceKernelLibrary"))
	sceKernelDelayThread(100000);
	sceKernelDelayThread(100000);
	do{
		fd=sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777); 
		sceKernelDelayThread(10000);
	} while(fd<=0);
	
	sceIoRead(fd, gameId, 10);
	sceIoClose(fd);
  //Game ID Check
	if(strncmp(gameId, "UCUS-98641", 10)){               
		running=0;
		if(running) {
		pspDebugScreenInit();
		pspDebugScreenInitEx(0x44000000, 0, 0);
		vram=NULL;
		}
	}
	while(running){
	
	if(vram == NULL)
    {
      //Has the HOME button been pressed?
      unsigned int a_address=0;
      unsigned int a_bufferWidth=0;
      unsigned int a_pixelFormat=0;
      unsigned int a_sync;
      
   	  sceDisplayGetFrameBufferInternal(0, &a_address, &a_bufferWidth, &a_pixelFormat, &a_sync);
      
      if(a_address)
      {
        vram=(void*)(0xA0000000 | a_address);
      }
      else
      {
        sceDisplayGetMode(&a_pixelFormat, &a_bufferWidth, &a_bufferWidth);
        pspDebugScreenSetColorMode(a_pixelFormat);
      }
	  sceKernelDelayThread(1500);
      continue;
    }
		pad.Buttons=0;
		refresh();
		menucheck();
		enablecheat();
		sceKernelDelayThread(100000);
		
	}
	sceDisplaySetFrameBufferInternal(0, 0, 512, 0, 1);
	sceCtrlSetButtonMasks(0x10000, 0);
	sceCtrlSetButtonMasks(0xFFFF, 0);
	sceKernelTerminateThread(thid);
}

// Start and stop the thread

int _start(SceSize args, void *argp){
	thid=sceKernelCreateThread("Main", &mainThread, 0x18, 0x512, 0, NULL);
	if(thid >= 0) 
		sceKernelStartThread(thid, 0, NULL);	
	return 0;
}

int _stop(SceSize args, void *argp){

	running=0;
 	sceKernelTerminateThread(thid);
	return 0;
	
}
