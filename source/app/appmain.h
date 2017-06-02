


/*******************************************************
 * Copyright (C) 2015 DMD 'Ware <dmdware@gmail.com>
 * 
 * This file is part of Caloric Metabolic Graph.
 * 
 * You are entitled to use this source code to learn.
 *
 * You are not entitled to duplicate or copy this source code 
 * into your own projects, commercial or personal, UNLESS you 
 * give credit.
 *
 *******************************************************/



#ifndef GMAIN_H
#define GMAIN_H

#include "../window.h"
#include "../version.h"
#include "../language.h"

#define VER_MAJ				0	//major version / sequal
#define VER_FS				1	//feature set
#define VER_ATT				0	//attempt
#define VER_STAT			VERSTAT_A		//status
#define VER_POST			0	//post-release number, or release candidate number, or hotfix?, local change only, should be compatible in multiplayer with other post-numbers
//#define VERSION				MakeVer(VER_MAJ, VER_FS, VER_ATT, VER_STAT, VER_POST)
#define VERSION				1

#define CONFIGFILE			"config.ini"
//#define TITLE				"Caloric Metabolic Graph"
#define TITLE				(STRTABLE[STR_TITLE].rawstr().c_str())
#define SERVER_ADDR			"corpstates.com"

//#define TRUCK_DEBUG
//#define TRUCK_DEBUG2
//#define PATH_DEBUG

//#define LOCAL_TRANSX	//define this if you want only player-specific transaction hovering notifications to appear

class Heightmap;

#define APPMODE_LOGO		0
#define APPMODE_INTRO		1
#define APPMODE_LOADING		2
#define APPMODE_RELOADING	3
#define APPMODE_MENU		4
#define APPMODE_PLAY		5
#define APPMODE_PAUSE		6
#define APPMODE_EDITOR		7
#define APPMODE_JOINING		8
extern int g_mode;
extern int g_reStage;

void LoadConfig();
void LoadName();
void WriteConfig();
void WriteName();
void DrawScene(Matrix projection, Matrix viewmat, Matrix modelmat, Matrix modelviewinv, float lightpos[3], float lightdir[3]);
void DrawSceneDepth();
void SkipLogo();
void Scroll(int dx, int dy);
int HandleEvent(void *userdata, SDL_Event *e);

#endif
