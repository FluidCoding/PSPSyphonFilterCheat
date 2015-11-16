#include <pspkernel.h>
#include <pspkerneltypes.h>
#include <pspmoduleinfo.h>
#include <pspiofilemgr.h>
#include <pspmodulemgr.h>
#include <pspthreadman.h>
#include <pspwlan.h>
#include <stdlib.h>
#include <pspchnnlsv.h>
#include <pspctrl.h>
#include <string.h>
#include <pspdebug.h>
#include <pspctrl_kernel.h>
#include <pspdisplay.h>
#include <pspdisplay_kernel.h>
#include <pspthreadman_kernel.h>
#include <pspumd.h>
#include "float.h"

#define print pspDebugScreenPuts
#define tcolor pspDebugScreenSetTextColor



int backcolor=0xFFFFFFFF;
unsigned int starttext=0x00660000;
unsigned int startback=0x00000000;
int startMenuRunning=1;
SceCtrlData pad;

int startMenu()
{
	while(startMenuRunning)
	{
		refresh();
		if(pad.Buttons & PSP_CTRL_START)
		{
startMenuRunning=0;
pspDebugScreenClear();
continue;
}
pspDebugScreenSetXY(0, 0);
pspDebugScreenSetTextColor(0x0066ff00);
pspDebugScreenSetBackColor(startback);
print(" Welcome to SFCD Rev1... \n");
print("Coded By SonniE & xWhite_Shadowx of www.CODINGHS.com\n\n");
tcolor(0xFFFFFFFF);
print("                     PRESS START TO Open Menu!!!");
sceKernelDelayThread(100000);
}
}



