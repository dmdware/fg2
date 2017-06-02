


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



#include "appmain.h"
#include "../gui/gui.h"
#include "../gui/keymap.h"
#include "../render/shader.h"
#include "gres.h"
#include "../gui/font.h"
#include "../texture.h"
#include "../math/frustum.h"
#include "../gui/gui.h"
#include "../debug.h"
#include "../math/camera.h"
#include "../window.h"
#include "../utils.h"
#include "../texture.h"
#include "../render/graph.h"
#include "../sys/unicode.h"
#include "../ustring.h"
#include "../language.h"
#include "../render/sprite.h"
#include "../gui/layouts/ggui.h"
#include "../math/matrix.h"
#include "../gui/cursor.h"
#include "../sim/simdef.h"

//int g_mode = APPMODE_LOADING;
int g_mode = APPMODE_LOGO;

//static long long g_lasttime = GetTicks();

#ifdef DEMO
static long long g_demostart = GetTicks();
#endif

void SkipLogo()
{
	g_mode = APPMODE_LOADING;
	GUI* gui = &g_gui;
	gui->hideall();
	gui->show("loading");
}

void UpdLogo()
{
	static int stage = 0;

	SkipLogo();

	GUI* gui = &g_gui;

	if(stage < 60)
	{
		float a = (float)stage / 60.0f;
		gui->get("logo")->get("logo")->m_rgba[3] = a;
	}
	else if(stage < 120)
	{
		float a = 1.0f - (float)(stage-60) / 60.0f;
		gui->get("logo")->get("logo")->m_rgba[3] = a;
	}
	else
		SkipLogo();

	stage++;
}

int g_restage = 0;
void UpdLoad()
{
	GUI* gui = &g_gui;

	switch(g_restage)
	{
	case 0:
		if(!Load1Sprite()) g_restage++;
		break;
	case 1:
		if(!Load1Texture())
		{
			g_spriteload.clear();
			g_texload.clear();
			g_lastLSp = -1;
			g_lastLTex = -1;

			g_mode = APPMODE_MENU;
			gui->hideall();
			gui->show("main");
			//g_mode = APPMODE_PLAY;
			//Click_NewGame();
			//Click_OpenEd();
		}
		break;
	}
}

void UpdReload()
{
	g_restage = 0;
	g_lastLTex = -1;
	//g_lastmodelload = -1;
	g_lastLSp = -1;
	g_gui.freech();
	//FreeModels();
	FreeSprites();
	FreeTextures();
	BreakWin(TITLE);
	//ReloadTextures();
	MakeWin(TITLE);
	//ReloadModels();	//Important - VBO only possible after window GL context made.
	g_mode = APPMODE_LOADING;
	//LoadFonts();	//already called in MakeWin
	//ReloadTextures();
	LoadSysRes();
	FillGUI();
	char exepath[DMD_MAX_PATH+1];
	ExePath(exepath);
	Log("ExePath %s\r\n", exepath);
	
	GUI* gui = &g_gui;
	gui->hideall();
	gui->show("loading");

	/*
	TODO
	Overhaul ui system
	Make scrollable everywhere
	Croppable pipeline 
	*/
	
}

void UpdSim()
{
	UpdGraph();
}

void UpdEd()
{
}

void Update()
{
	UpdSim();

	if(g_mode == APPMODE_LOGO)
		UpdLogo();
	//else if(g_mode == APPMODE_INTRO)
	//	UpdateIntro();
	else if(g_mode == APPMODE_LOADING)
		UpdLoad();
	else if(g_mode == APPMODE_RELOADING)
		UpdReload();
	else if(g_mode == APPMODE_PLAY)
		UpdSim();
	else if(g_mode == APPMODE_EDITOR)
		UpdEd();

#ifdef DEMO
	if(GetTicks() - g_demostart > DEMOTIME)
		g_quit = true;
#endif
}

void DrawScene(Matrix projection, Matrix viewmat, Matrix modelmat, Matrix modelviewinv, float lightpos[3], float lightdir[3])
{
	//return;	//temp

}

// get a power of 2 number that is big enough to hold 'lowerbound' but does not exceed 2048
int Max2Pow(int lowerbound)
{
	int twopow = 2;
	
	while( twopow < lowerbound
#if 0
		  && twopow < 2048
#endif
		  )
		twopow *= 2;
	
	return twopow;
}

int Max2Pow32(int lowerbound)
{
	int twopow = 32;
	
	while( twopow < lowerbound
#if 0
		  && twopow < 2048
#endif
		  )
		twopow *= 2;
	
	return twopow;
}

void MakeFBO(unsigned int* rendertex, unsigned int* renderrb, unsigned int* renderfb, unsigned int* renderdepthtex, int w, int h)
{
#if 0   //OpenGL 3.0 way
#if 1
	glGenTextures(1, rendertex);
	glBindTexture(GL_TEXTURE_2D, *rendertex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	CHECKGLERROR();
#if 0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	CheckGLError(__FILE__, __LINE__);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffersEXT(1, renderrb);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, *renderrb);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, w, h);
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	CheckGLError(__FILE__, __LINE__);

	glGenFramebuffersEXT(1, renderfb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, *renderfb);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, g_rendertex, 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, g_renderrb);
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	CheckGLError(__FILE__, __LINE__);
#else
	//RGBA8 2D texture, 24 bit depth texture, 256x256
	glGenTextures(1, &g_rendertex);
	glBindTexture(GL_TEXTURE_2D, g_rendertex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//NULL means reserve texture memory, but texels are undefined
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//-------------------------
	glGenFramebuffersEXT(1, &g_renderfb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_renderfb);
	//Attach 2D texture to this FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, g_rendertex, 0);
	//-------------------------
	glGenRenderbuffersEXT(1, &g_renderrb);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, g_renderrb);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, w, h);
	//-------------------------
	//Attach depth buffer to FBO
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, g_renderrb);
	//-------------------------
#endif
	
#elif defined(PLATFORM_MOBILE)	//for mobile
	
	glGenFramebuffers(1, renderfb);
	glBindFramebuffer(GL_FRAMEBUFFER, *renderfb);
	
	//NSLog(@"1 glGetError() = %d", glGetError());
	
	w = Max2Pow(w);
	h = Max2Pow(h);
	
	w = 512;
	h = 512;
	
	glGenTextures(1, rendertex);
	glBindTexture(GL_TEXTURE_2D, *rendertex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	//NSLog(@"2 glGetError() = %d", glGetError());
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *rendertex, 0);
	
	//NSLog(@"3 glGetError() = %d", glGetError());
	/*
	glGenRenderbuffers(1, renderrb);
	glBindRenderbuffer(GL_RENDERBUFFER, *renderrb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, *renderrb);
	
	NSLog(@"4 glGetError() = %d", glGetError());
	
	glGenRenderbuffers(1, renderdepthtex);
	glBindRenderbuffer(GL_RENDERBUFFER, *renderdepthtex);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *renderdepthtex);
	
	NSLog(@"5 glGetError() = %d", glGetError());
	*/
	unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	//GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		char mess[128];
		sprintf(mess, "Couldn't create framebuffer for render, error: %u.", status);
		ErrMess("Error", mess);
		return;
	}
	
	//InfoMess("suc","fbosuc");
	
#else   //OpenGL 1.4 way

	glGenTextures(1, rendertex);
	glBindTexture(GL_TEXTURE_2D, *rendertex);
#ifndef PLATFORM_MOBILE
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8_OES, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//g_applog<<"gge1 "<<glGetError());

	glGenTextures(1, renderdepthtex);
	glBindTexture(GL_TEXTURE_2D, *renderdepthtex);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, g_deswidth, g_desheight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
#ifndef PLATFORM_MOBILE
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	//g_applog<<"gge2 "<<glGetError());
	//glDrawBuffer(GL_NONE); // No color buffer is drawn
	//glReadBuffer(GL_NONE);

	glGenFramebuffers(1, renderfb);
	glBindFramebuffer(GL_FRAMEBUFFER, *renderfb);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *rendertex, 0);
#ifndef PLATFORM_MOBILE
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *renderdepthtex, 0);
#endif

	//g_applog<<"gge3 "<<glGetError());

#ifndef PLATFORM_MOBILE
	//glDrawBuffers requires OpenGL ES 3.0
	//GL_DEPTH_ATTACHMENT_EXT
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
	glDrawBuffers(1, DrawBuffers);
#else
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);
#endif
	
	//g_applog<<"gge4 "<<glGetError());

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ErrMess("Error", "Couldn't create framebuffer for render.");
		return;
	}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

#ifdef APPDEBUG
	g_applog<<__FILE__<<":"<<__LINE__<<"create check frame buf stat: "<<glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT)<<" ok="<<(int)(GL_FRAMEBUFFER_COMPLETE));
	g_applog<<__FILE__<<":"<<__LINE__<<"create check frame buf stat ext: "<<glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)<<" ok="<<(int)(GL_FRAMEBUFFER_COMPLETE));
#endif
}

void DelFBO(unsigned int* rendertex, unsigned int* renderrb, unsigned int* renderfb, unsigned int* renderdepthtex)
{
#if 0
	CHECKGLERROR();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CHECKGLERROR();
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	CHECKGLERROR();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CHECKGLERROR();
	glDeleteFramebuffers(1, &g_renderfb);
	CHECKGLERROR();
	glDeleteRenderbuffers(1, &g_renderrb);
	CHECKGLERROR();
	glDeleteTextures(1, &g_rendertex);
	CHECKGLERROR();
	
#elif defined(PLATFORM_MOBILE)
	
	glDeleteTextures(1, rendertex);
	//glDeleteRenderbuffers(1, renderrb);
	glDeleteFramebuffers(1, renderfb);
	//glDeleteRenderbuffers(1, renderdepthtex);
	CHECKGLERROR();
	
#else
	
	//Delete resources
	glDeleteTextures(1, rendertex);
#ifndef PLATFORM_MOBILE
	glDeleteTextures(1, renderdepthtex);
#endif
#ifdef PLATFORM_MOBILE
	//corpd fix sped fix
	glDeleteRenderbuffers(1, renderrb);
#endif
#ifndef PLATFORM_MOBILE
	//Bind 0, which means render to back buffer, as a result, fb is unbound
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
	glDeleteFramebuffers(1, renderfb);
	CHECKGLERROR();
#endif
}

void Draw()
{
	if(g_quit)
		return;
	
	StartTimer(TIMER_DRAWSETUP);

	CHECKGLERROR();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CHECKGLERROR();

	GUI* gui = &g_gui;
	Camera* c = &g_cam;

	StopTimer(TIMER_DRAWSETUP);

	if(g_mode == APPMODE_PLAY || g_mode == APPMODE_EDITOR)
	{
		StartTimer(TIMER_DRAWSETUP);

		StopTimer(TIMER_DRAWSETUP);
		CHECKGLERROR();
		//Ortho(g_width, g_height, 1, 1, 1, 1);
#if 1


#if 0
		//draw protectionism graphs
		UseS(SHADER_COLOR2D);
		Shader* s = &g_shader[g_curS];
		glUniform1f(s->slot[SSLOT_WIDTH], (float)g_width);
		glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_height);
		glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);
		float graphbounds[4];
		graphbounds[0] = (float)g_width/2;
		graphbounds[1] = (float)g_height/2 - 200;
		graphbounds[2] = (float)g_width - 30;
		graphbounds[3] = (float)g_height/2 + 130;
		float highest=0;
		//for(int i=1; i<PLAYERS; i+=(FIRMSPERSTATE+1))
		for(int i=1; i<(FIRMSPERSTATE+1)*4; i+=(FIRMSPERSTATE+1))
		{
			for(auto pit=g_protecg[i].points.begin(); pit!=g_protecg[i].points.end(); pit++)
			{
				highest = fmax(highest,  *pit);
				highest = fmax(highest,  -*pit);
			}
		}
		for(int i=1; i<(FIRMSPERSTATE+1)*4; i+=(FIRMSPERSTATE+1))
		{
			float* c = g_player[i].color;
			//glUniform4f(s->slot[SSLOT_COLOR], c[0], c[1], c[2], 1);
			DrawGraph(&g_protecg[i], graphbounds[0], graphbounds[1], graphbounds[2], graphbounds[3], highest, c);
		}
		EndS();
#endif

	}
	CHECKGLERROR();
#endif

#ifdef DEBUGLOG
	Log("draw "<<__FILE__<<" "<<__LINE__);
	
#endif

	StartTimer(TIMER_DRAWGUI);
	gui->frameupd();

#ifdef DEBUGLOG
	Log("draw "<<__FILE__<<" "<<__LINE__);
	
#endif

	CHECKGLERROR();
	//if(!(g_mode == APPMODE_PLAY && g_speed == SPEED_FAST))
	gui->draw();
	StopTimer(TIMER_DRAWGUI);

	CHECKGLERROR();
	Ortho(g_width, g_height, 1, 1, 1, 1);
	CHECKGLERROR();
	glDisable(GL_DEPTH_TEST);
	CHECKGLERROR();

#ifdef DEBUGLOG
	Log("draw "<<__FILE__<<" "<<__LINE__);
	
#endif

#ifdef DEMO
	{
		unsigned int msleft = DEMOTIME - (GetTicks() - g_demostart);
		char msg[128];
		sprintf(msg, "Demo time %d:%02d", msleft / 1000 / 60, (msleft % (1000 * 60)) / 1000);
		RichText msgrt(msg);
		float color[] = {0.5f,1.0f,0.5f,1.0f};
		DrawShadowedText(MAINFONT16, g_width - 130, g_height-16, &msgrt, color);
	}
#endif

	EndS();
	CHECKGLERROR();

#ifdef DEBUGLOG
	Log("draw "<<__FILE__<<" "<<__LINE__);
	
#endif

	//SDL_GL_SwapWindow(g_window);

	//CheckNum("post draw");
}

bool OverMinimap()
{
	return false;
}

void LoadConfig()
{
	
	//g_selres.width = 568;
	//g_selres.height = 320;
	
	//g_selres.height = 568;
	//g_selres.width = 320;
	
	g_lang = "eng";
	SwitchLang(g_lang.c_str());
	
	char cfgfull[DMD_MAX_PATH+1];
	FullWritePath(CONFIGFILE, cfgfull);

	std::ifstream f(cfgfull);

	if(!f)
		return;

	std::string line;
	char keystr[128];
	char actstr[128];

	while(!f.eof())
	{
		strcpy(keystr, "");
		strcpy(actstr, "");

		getline(f, line);

		if(line.length() > 127)
			continue;

		actstr[0] = 0;

		sscanf(line.c_str(), "%s %s", keystr, actstr);

		float valuef = StrToFloat(actstr);
		int valuei = StrToInt(actstr);
		bool valueb = valuei ? true : false;

		if(stricmp(keystr, "fullscreen") == 0)					g_fullscreen = valueb;
		else if(stricmp(keystr, "client_width") == 0)			g_width = g_selres.width = valuei;
		else if(stricmp(keystr, "client_height") == 0)			g_height = g_selres.height = valuei;
		else if(stricmp(keystr, "screen_bpp") == 0)				g_bpp = valuei;
		//else if(stricmp(keystr, "volume") == 0)					SetVol(valuei);
		else if(stricmp(keystr, "language") == 0)				SwitchLang(actstr);
	}

	f.close();

	//g_lang = LANG_ENG;
}

void WriteConfig()
{
	char cfgfull[DMD_MAX_PATH+1];
	FullWritePath(CONFIGFILE, cfgfull);
	FILE* fp = fopen(cfgfull, "w");
	if(!fp)
		return;
	fprintf(fp, "fullscreen %d \r\n\r\n", g_fullscreen ? 1 : 0);
	fprintf(fp, "client_width %d \r\n\r\n", g_selres.width);
	fprintf(fp, "client_height %d \r\n\r\n", g_selres.height);
	fprintf(fp, "screen_bpp %d \r\n\r\n", g_bpp);
	//fprintf(fp, "volume %d \r\n\r\n", g_volume);
	fprintf(fp, "language %s\r\n\r\n", g_lang.c_str());
	fclose(fp);
}

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void SignalCallback(int signum)
{
	//printf("Caught signal %d\n",signum);
	// Cleanup and hide up stuff here

	// Terminate program
	//g_quit = true;
	exit(0);	//force quit NOW
}

#ifdef PLATFORM_MOBILE
void CopyFile(const char* from, const char* to)
{
	FILE* fromfp = fopen(from, "rb");
	FILE* tofp = fopen(to, "wb");
	
	unsigned char byte = 0;

	int i = 0;
	int len = 0;

	fseek(fromfp, 0L, SEEK_END);
	len = ftell(fromfp);
	fseek(fromfp, 0L, SEEK_SET);
	
	//check remark about feof in CheckSum(...)
	//while(!feof(fromfp))
	for(i=0; i<len; i++)
	{
		fread(&byte, 1, 1, fromfp);
		fwrite(&byte, 1, 1, tofp);
	}
	
	fclose(fromfp);
	fclose(tofp);
}

void FirstRun()
{
	char checkfull[DMD_MAX_PATH+1];
	char checkfile[128];
	sprintf(checkfile, "first%llu.txt", VERSION);
	FullWritePath(checkfile, checkfull);
	
	FILE* fp = fopen(checkfull, "rb");
	
	if(fp)
	{
		fclose(fp);
		return;
	}
	
	//else, this is the first run for this version
	fp = fopen(checkfull, "wb");
	fwrite("1", sizeof(char), 2, fp);
	fclose(fp);
	
	std::list<std::string> maps;
	std::list<std::string> saves;
	
	char mapswritefull[DMD_MAX_PATH+1];
	char saveswritefull[DMD_MAX_PATH+1];
	char screenwritefull[DMD_MAX_PATH+1];
	FullWritePath("maps", mapswritefull);
	FullWritePath("saves", saveswritefull);
	FullWritePath("screenshot", screenwritefull);
	mkdir(mapswritefull, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir(saveswritefull, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir(screenwritefull, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	
	char mapsreadfull[DMD_MAX_PATH+1];
	char savesreadfull[DMD_MAX_PATH+1];
	FullPath("maps", mapsreadfull);
	FullPath("saves", savesreadfull);
	
	ListFiles(mapsreadfull, maps);
	ListFiles(savesreadfull, saves);
	
	for(auto fit=maps.begin(); fit!=maps.end(); fit++)
	{
		char fullfrom[DMD_MAX_PATH+1];
		char fullto[DMD_MAX_PATH+1];
		sprintf(fullfrom, "%s/%s", mapsreadfull, fit->c_str());
		sprintf(fullto, "%s/%s", mapswritefull, fit->c_str());
		CopyFile(fullfrom, fullto);
	}
	
	for(auto fit=saves.begin(); fit!=saves.end(); fit++)
	{
		char fullfrom[DMD_MAX_PATH+1];
		char fullto[DMD_MAX_PATH+1];
		sprintf(fullfrom, "%s/%s", savesreadfull, fit->c_str());
		sprintf(fullto, "%s/%s", saveswritefull, fit->c_str());
		CopyFile(fullfrom, fullto);
	}
}
#endif

void Init()
{
#ifdef PLATFORM_LINUX
	signal(SIGINT, SignalCallback);
#endif
	
#ifdef PLATFORM_IOS
	//SDL_SetEventFilter(HandleEvent, NULL);
#endif
	
#ifdef PLATFORM_MOBILE
	FirstRun();
#endif

#if 0
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
	{
		[director enableRetinaDisplay:NO];
		
		
	} else {
		
		[director enableRetinaDisplay:YES];
		
	}
#endif
	//SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
	
#if 0
	if(!SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1"))
		ErrMess("failed to set dpi hint", "failed to set dpi hint");
	else
		ErrMess("set to set dpi hint", "set to set dpi hint");
#endif
	
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		char msg[1280];
		sprintf(msg, "SDL_Init: %s\n", SDL_GetError());
		ErrMess("Error", msg);
	}

	if(SDLNet_Init() == -1)
	{
		char msg[1280];
		sprintf(msg, "SDLNet_Init: %s\n", SDLNet_GetError());
		ErrMess("Error", msg);
	}

	SDL_version compile_version;
	const SDL_version *link_version=Mix_Linked_Version();
	SDL_MIXER_VERSION(&compile_version);
	printf("compiled with SDL_mixer version: %d.%d.%d\n",
		compile_version.major,
		compile_version.minor,
		compile_version.patch);
	printf("running with SDL_mixer version: %d.%d.%d\n",
		link_version->major,
		link_version->minor,
		link_version->patch);

	// load support for the OGG and MOD sample/music formats
	//int flags=MIX_INIT_OGG|MIX_INIT_MOD|MIX_INIT_MP3;
	int flags=MIX_INIT_OGG|MIX_INIT_MP3;
	int initted=Mix_Init(flags);
	if( (initted & flags) != flags)
	{
		char msg[1280];
		sprintf(msg, "Mix_Init: Failed to init required ogg and mod support!\nMix_Init: %s", Mix_GetError());
		//ErrMess("Error", msg);
		// handle error
	}

	// start SDL with audio support
	if(SDL_Init(SDL_INIT_AUDIO)==-1) {
		char msg[1280];
		sprintf(msg, "SDL_Init: %s\n", SDL_GetError());
		ErrMess("Error", msg);
		// handle error
		//exit(1);
	}
	// show 44.1KHz, signed 16bit, system byte order,
	//      stereo audio, using 1024 byte chunks
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
		char msg[1280];
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		ErrMess("Error", msg);
		// handle error
		//exit(2);
	}

	Mix_AllocateChannels(8);

	if(!g_log.is_open())
		OpenLog("log.txt", VERSION);

	srand((unsigned int)GetTicks());
	LoadConfig();
	LoadData();

	//g_os = ObjectScript::OS::create();
	//g_os->pushCFunction(testfunc);
	//g_os->setGlobal("testfunc");
	//os->eval("testfunc();");
	//os->eval("function require(){ /* if(relative == \"called.os\") */ { testfunc(); } }");
	char autoexecpath[DMD_MAX_PATH+1];
	FullPath("scripts/autoexec.os", autoexecpath);
	//g_os->require(autoexecpath);
	//g_os->release();

	//EnumerateMaps();
	//EnumerateDisplay();
	MapKeys();

	InitProfiles();
}

void Deinit()
{
	g_gui.freech();

	WriteProfiles(-1, 0);
	BreakWin(TITLE);

	// Clean up

	Mix_CloseAudio();

	// force a quit
	//while(Mix_Init(0))
	//	Mix_Quit();
	Mix_Quit();

	SDLNet_Quit();
	SDL_Quit();
}

void CaptureZoom()
{
#if 1
#ifdef PLATFORM_MOBILE
	//if(g_zoomdrawframe)
	{
		//glFlush();
		//glFinish();
		
		Vec2i zoombox[2];
		
		zoombox[0].x = g_mouse.x - ZOOMBOX;
		zoombox[0].y = g_mouse.y - ZOOMBOX;
		zoombox[1].x = g_mouse.x + ZOOMBOX;
		zoombox[1].y = g_mouse.y + ZOOMBOX;
		
		zoombox[0].x = imax(0, zoombox[0].x);
		zoombox[0].y = imax(0, zoombox[0].y);
		zoombox[1].x = imin(g_width, zoombox[1].x);
		zoombox[1].y = imin(g_height, zoombox[1].y);
		
		LoadedTex zoompix;
		zoompix.sizex = zoombox[1].x - zoombox[0].x;
		zoompix.sizey = zoombox[1].y - zoombox[0].y;
		zoompix.channels = 4;
		zoompix.data = (unsigned char*)malloc( sizeof(unsigned char) * 4 * zoompix.sizex * zoompix.sizey );
		
		//glReadBuffer( GL_FRONT );
		glReadPixels(zoombox[0].x, g_height - zoombox[1].y, zoompix.sizex, zoompix.sizey, GL_RGBA, GL_UNSIGNED_BYTE, zoompix.data);
		//CreateTex(&zoompix, &zoomtex, true, false);
		
		glBindTexture(GL_TEXTURE_2D, g_zoomtex);
		glTexSubImage2D(GL_TEXTURE_2D, 0, zoombox[0].x - (g_mouse.x - ZOOMBOX), zoombox[0].y - (g_mouse.y - ZOOMBOX), zoompix.sizex, zoompix.sizey, GL_RGBA, GL_UNSIGNED_BYTE, zoompix.data);
	}
#endif
#endif
}

int HandleEvent(void *userdata, SDL_Event *e)
{
	GUI* gui = &g_gui;
	InEv ie;
	ie.intercepted = false;
	ie.curst = CU_DEFAULT;
	Vec2i old;
	
	switch(e->type)
	{
#ifdef PLATFORM_IOS
		case SDL_APP_TERMINATING:
			/* Terminate the app.
			 Shut everything down before returning from this function.
			 */
			g_quit = true;
			g_background = true;
			exit(0);
			return 0;
		case SDL_APP_LOWMEMORY:
			/* You will get this when your app is paused and iOS wants more memory.
			 Release as much memory as possible.
			 */
			exit(0);
			return 0;
		case SDL_APP_WILLENTERBACKGROUND:
			/* Prepare your app to go into the background.  Stop loops, etc.
			 This gets called when the user hits the home button, or gets a call.
			 */
			g_quit = true;
			g_background = true;
			exit(0);
			return 0;
		case SDL_APP_DIDENTERBACKGROUND:
			/* This will get called if the user accepted whatever sent your app to the background.
			 If the user got a phone call and canceled it, you'll instead get an SDL_APP_DIDENTERFOREGROUND event and restart your loops.
			 When you get this, you have 5 seconds to save all your state or the app will be terminated.
			 Your app is NOT active at this point.
			 */
			g_quit = true;
			g_background = true;
			exit(0);
			return 0;
		case SDL_APP_WILLENTERFOREGROUND:
			/* This call happens when your app is coming back to the foreground.
			 Restore all your state here.
			 */
			g_background = false;
			return 0;
		case SDL_APP_DIDENTERFOREGROUND:
			/* Restart your loops here.
			 Your app is interactive and getting CPU again.
			 */
			g_background = false;
			return 0;
#endif
		case SDL_QUIT:
			g_quit = true;
			break;
		case SDL_KEYDOWN:
			ie.type = INEV_KEYDOWN;
			ie.key = e->key.keysym.sym;
			ie.scancode = e->key.keysym.scancode;
			
			//Handle copy
			if( e->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
			{
				//SDL_SetClipboardText( inputText.c_str() );
				ie.type = INEV_COPY;
			}
			//Handle paste
			if( e->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
			{
				//inputText = SDL_GetClipboardText();
				//renderText = true;
				ie.type = INEV_PASTE;
			}
			//Select all
			if( e->key.keysym.sym == SDLK_a && SDL_GetModState() & KMOD_CTRL )
			{
				//inputText = SDL_GetClipboardText();
				//renderText = true;
				ie.type = INEV_SELALL;
			}
			
			gui->inev(&ie);
			
			if(!ie.intercepted)
				g_keys[e->key.keysym.scancode] = true;
			
			g_keyintercepted = ie.intercepted;
			break;
		case SDL_KEYUP:
			ie.type = INEV_KEYUP;
			ie.key = e->key.keysym.sym;
			ie.scancode = e->key.keysym.scancode;
			
			gui->inev(&ie);
			
			if(!ie.intercepted)
				g_keys[e->key.keysym.scancode] = false;
			
			g_keyintercepted = ie.intercepted;
			break;
		case SDL_TEXTINPUT:
			//g_GUI.charin(e->text.text);	//UTF8
			ie.type = INEV_TEXTIN;
			ie.text = e->text.text;
			
			gui->inev(&ie);
			break;
#if 0
		case SDL_TEXTEDITING:
			//g_GUI.charin(e->text.text);	//UTF8
			ie.type = INEV_TEXTED;
			//ie.text = e->text.text;
			ie.text = e->edit.text;
			//Log("texed: "<<e->text.text<<" [0]="<<(unsigned int)e->text.text[0]);
			//
			ie.cursor = e->edit.start;
			ie.sellen = e->edit.length;
			
			
			gui->inev(&ie);
#if 0
			ie.intercepted = false;
			ie.type = INEV_TEXTIN;
			ie.text = e->text.text;
			
			gui->inev(&ie);
#endif
			break;
#endif
#if 0
		case SDL_TEXTINPUT:
			/* Add new text onto the end of our text */
			strcat(text, event.text.text);
#if 0
			ie.type = INEV_CHARIN;
			ie.key = wParam;
			ie.scancode = 0;
			
			gui->inev(&ie);
#endif
			break;
		case SDL_TEXTEDITING:
			/*
			 Update the composition text.
			 Update the cursor position.
			 Update the selection length (if any).
			 */
			composition = event.edit.text;
			cursor = event.edit.start;
			selection_len = event.edit.length;
			break;
#endif
			//else if(e->type == SDL_BUTTONDOWN)
			//{
			//}
		case SDL_MOUSEWHEEL:
			ie.type = INEV_MOUSEWHEEL;
			ie.amount = e->wheel.y;
			
			gui->inev(&ie);
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (e->button.button)
		{
			case SDL_BUTTON_LEFT:
				g_mousekeys[MOUSE_LEFT] = true;
				g_moved = false;
				
				ie.type = INEV_MOUSEDOWN;
				ie.key = MOUSE_LEFT;
				ie.amount = 1;
				ie.x = g_mouse.x;
				ie.y = g_mouse.y;
				
#if 0	//attempt at zoombox coordinate translation
#ifdef PLATFORM_MOBILE
				if(e->button.x > g_mouse.x - ZOOMBOX &&
				   e->button.x < g_mouse.x + ZOOMBOX &&
				   e->button.y > g_mouse.y - ZOOMBOX &&
				   e->button.y < g_mouse.y + ZOOMBOX)
				{
					//ie.x = (e->button.x - g_mouse.x) * 2 + g_mouse.x;
					//ie.y = (e->button.y - g_mouse.y) * 2 + g_mouse.y;
					ie.x = (e->button.x/2 - g_mouse.x) * 2 + g_mouse.x;
					ie.y = (e->button.y/2 - g_mouse.y) * 2 + g_mouse.y;
					
					g_mouse.x = ie.x;
					g_mouse.y = ie.y;
				}
#endif
#endif
				
				gui->inev(&ie);
				
				g_keyintercepted = ie.intercepted;
				break;
			case SDL_BUTTON_RIGHT:
				g_mousekeys[MOUSE_RIGHT] = true;
				
				ie.type = INEV_MOUSEDOWN;
				ie.key = MOUSE_RIGHT;
				ie.amount = 1;
				ie.x = g_mouse.x;
				ie.y = g_mouse.y;
				
				gui->inev(&ie);
				break;
			case SDL_BUTTON_MIDDLE:
				g_mousekeys[MOUSE_MIDDLE] = true;
				
				ie.type = INEV_MOUSEDOWN;
				ie.key = MOUSE_MIDDLE;
				ie.amount = 1;
				ie.x = g_mouse.x;
				ie.y = g_mouse.y;
				
				gui->inev(&ie);
				break;
		}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (e->button.button)
		{
				case SDL_BUTTON_LEFT:
					g_mousekeys[MOUSE_LEFT] = false;
					
					ie.type = INEV_MOUSEUP;
					ie.key = MOUSE_LEFT;
					ie.amount = 1;
					ie.x = g_mouse.x;
					ie.y = g_mouse.y;
					
					gui->inev(&ie);
					break;
				case SDL_BUTTON_RIGHT:
					g_mousekeys[MOUSE_RIGHT] = false;
					
					ie.type = INEV_MOUSEUP;
					ie.key = MOUSE_RIGHT;
					ie.amount = 1;
					ie.x = g_mouse.x;
					ie.y = g_mouse.y;
					
					gui->inev(&ie);
					break;
				case SDL_BUTTON_MIDDLE:
					g_mousekeys[MOUSE_MIDDLE] = false;
					
					ie.type = INEV_MOUSEUP;
					ie.key = MOUSE_MIDDLE;
					ie.amount = 1;
					ie.x = g_mouse.x;
					ie.y = g_mouse.y;
					
					gui->inev(&ie);
					break;
			}
			break;
		case SDL_MOUSEMOTION:
			//g_mouse.x = e->motion.x;
			//g_mouse.y = e->motion.y;
			
			if(g_mouseout)
			{
				//TrackMouse();
				g_mouseout = false;
			}
			
			old = g_mouse;
			
			if(MousePosition())
			{
				g_moved = true;
				
				ie.type = INEV_MOUSEMOVE;
				ie.x = g_mouse.x;
				ie.y = g_mouse.y;
				ie.dx = g_mouse.x - old.x;
				ie.dy = g_mouse.y - old.y;
				
				gui->inev(&ie);
				
				g_curst = ie.curst;
			}
			break;
		}

	return 0;
}

void EventLoop()
{
	
#ifdef PLATFORM_MOBILE
	int width = g_width;
	int height = g_height;
	g_width >>= 1;
	g_height >>= 1;
#endif
	
#if 0
	key->keysym.scancode
		SDLMod  e->key.keysym.mod
		key->keysym.unicode

		if( mod & KMOD_NUM ) printf( "NUMLOCK " );
	if( mod & KMOD_CAPS ) printf( "CAPSLOCK " );
	if( mod & KMOD_LCTRL ) printf( "LCTRL " );
	if( mod & KMOD_RCTRL ) printf( "RCTRL " );
	if( mod & KMOD_RSHIFT ) printf( "RSHIFT " );
	if( mod & KMOD_LSHIFT ) printf( "LSHIFT " );
	if( mod & KMOD_RALT ) printf( "RALT " );
	if( mod & KMOD_LALT ) printf( "LALT " );
	if( mod & KMOD_CTRL ) printf( "CTRL " );
	if( mod & KMOD_SHIFT ) printf( "SHIFT " );
	if( mod & KMOD_ALT ) printf( "ALT " );
#endif

	//SDL_EnableUNICODE(SDL_ENABLE);

	while (!g_quit)
	{
		StartTimer(TIMER_FRAME);
		StartTimer(TIMER_EVENT);

//#ifndef PLATFORM_IOS
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			HandleEvent(NULL, &e);
		}
//#endif

		StopTimer(TIMER_EVENT);
		
		if(g_quit)
			break;
		
#if 1
		//if ((g_mode == APPMODE_LOADING || g_mode == APPMODE_RELOADING) || true /* DrawNextFrame(DRAW_FRAME_RATE) */ )
		if ( !g_background &&
			( (g_mode == APPMODE_LOADING || g_mode == APPMODE_RELOADING) || DrawNextFrame() ) )
#endif
		{
			StartTimer(TIMER_DRAW);

#ifdef DEBUGLOG
			Log("main "<<__FILE__<<" "<<__LINE__);
			
#endif
			CalcDrawRate();

			CHECKGLERROR();

#ifdef DEBUGLOG
			Log("main "<<__FILE__<<" "<<__LINE__);
			
#endif
			
#ifndef PLATFORM_MOBILE
			Draw();
			SDL_GL_SwapWindow(g_window);
#endif
			
			
#ifdef PLATFORM_MOBILE
			/*
			GLint deffbo;
			//GL_FRAMEBUFFER_BINDING_OES
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &deffbo);
			
			unsigned int offtex;
			unsigned int offrb;
			unsigned int offfb;
			unsigned int offdepth;
			
			MakeFBO(&offtex, &offrb, &offfb, &offdepth, g_width, g_height);
			glBindFramebuffer(GL_FRAMEBUFFER, offfb);
			 */

			//g_zoomdrawframe = false;
			Draw();
			SDL_GL_SwapWindow(g_window);
#if 0
			if(!g_zoomdrawframe)
			{
				SDL_GL_SwapWindow(g_window);
				CaptureZoom();
			}
			
			//glBindFramebuffer(GL_FRAMEBUFFER, deffbo);
			
			//int highw = Max2Pow(g_width);
			//int highh = Max2Pow(g_height);
			
			//DrawImage(offtex, 0, 0, g_width-1, g_height-1, 0, 1, 1, 0, g_gui.m_crop);
			//DrawImage(offtex, 0, 0, g_width-1, g_height-1, 0, (float)g_width/(float)highw, (float)g_height/(float)highh, 0, g_gui.m_crop);
			else
			{
				DrawZoom();
				SDL_GL_SwapWindow(g_window);
			}
#elif 0
			if(g_zoomdrawframe)
			{
				DrawZoom();
				SDL_GL_SwapWindow(g_window);
				CaptureZoom();
			}
#endif
			
			g_zoomdrawframe = !g_zoomdrawframe;

			//DelFBO(&offtex, &offrb, &offfb, &offdepth);
			//glBindFramebuffer(GL_FRAMEBUFFER, deffbo);
			
			
			
#endif	//PLATFORM_MOBILE
			
			CHECKGLERROR();

			if(g_mode == APPMODE_PLAY || g_mode == APPMODE_EDITOR)
			{
#ifdef DEBUGLOG
				Log("main "<<__FILE__<<" "<<__LINE__);
				
#endif
#ifdef DEBUGLOG
				Log("main "<<__FILE__<<" "<<__LINE__);
				
#endif
			}

			StopTimer(TIMER_DRAW);
		}

		//if((g_mode == APPMODE_LOADING || g_mode == APPMODE_RELOADING) || true /* UpdNextFrame(SIM_FRAME_RATE) */ )
		if((g_mode == APPMODE_LOADING || g_mode == APPMODE_RELOADING) || UpdNextFrame() )
		{
			StartTimer(TIMER_UPDATE);

			CalcUpdRate();
			Update();

			StopTimer(TIMER_UPDATE);
		}
		
		StopTimer(TIMER_FRAME);
	}
	
#ifdef PLATFORM_MOBILE
	g_width = width;
	g_height = height;
#endif
}

#ifdef USESTEAM
//-----------------------------------------------------------------------------
// Purpose: callback hook for debug text emitted from the Steam API
//-----------------------------------------------------------------------------
extern "C" void __cdecl SteamAPIDebugTextHook( int nSeverity, const char *pchDebugText )
{
	// if you're running in the debugger, only warnings (nSeverity >= 1) will be sent
	// if you add -debug_steamapi to the command-line, a lot of extra informational messages will also be sent
#ifdef PLATFORM_WIN
	::OutputDebugString( pchDebugText );
#endif

	if(!g_log.is_open())
		OpenLog("log.txt", VERSION);

	Log(pchDebugText);
	

	if ( nSeverity >= 1 )
	{
		// place to set a breakpoint for catching API errors
		int x = 3;
		x = x;
	}
}
#endif

void Main()
{
	//*((int*)0) = 0;

#ifdef USESTEAM

	if ( SteamAPI_RestartAppIfNecessary( k_uAppIdInvalid ) )
	{
		// if Steam is not running or the game wasn't started through Steam, SteamAPI_RestartAppIfNecessary starts the 
		// local Steam client and also launches this game again.

		// Once you get a public Steam AppID assigned for this game, you need to replace k_uAppIdInvalid with it and
		// removed steam_appid.txt from the game depot.

		return;
	}

	// Init Steam CEG
	if ( !Steamworks_InitCEGLibrary() )
	{
#ifdef PLATFORM_WIN
		OutputDebugString( "Steamworks_InitCEGLibrary() failed\n" );
#endif
		ErrMess( "Fatal Error", "Steam must be running to play this game (InitDrmLibrary() failed).\n" );
		return;
	}

	// Initialize SteamAPI, if this fails we bail out since we depend on Steam for lots of stuff.
	// You don't necessarily have to though if you write your code to check whether all the Steam
	// interfaces are NULL before using them and provide alternate paths when they are unavailable.
	//
	// This will also load the in-game steam overlay dll into your process.  That dll is normally
	// injected by steam when it launches games, but by calling this you cause it to always load,
	// even when not launched via steam.
	if ( !SteamAPI_Init() )
	{
#ifdef PLATFORM_WIN
		OutputDebugString( "SteamAPI_Init() failed\n" );
#endif
		ErrMess( "Fatal Error", "Steam must be running to play this game (SteamAPI_Init() failed).\n" );
		return;
	}

	// set our debug handler
	SteamClient()->SetWarningMessageHook( &SteamAPIDebugTextHook );

#endif

	Init();
	g_log << "MakeWin: " << std::endl;
	g_log.flush();
	
	MakeWin(TITLE);
	
	g_log << "FillGUI: "    << std::endl;
	
	g_log << "Queue: "      << std::endl;
	
	SDL_ShowCursor(false);
	LoadSysRes();
	Queue();
	FillGUI();
	g_log << "EventLoop: "  << std::endl;
	
	EventLoop();
	g_log << "Deinit: "     << std::endl;
	
	Deinit();
	SDL_ShowCursor(true);
}

#ifdef PLATFORM_WIN
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char* argv[])
#endif
{
#ifdef PLATFORM_IOS
	NSLog(@"running");
#endif
	
	//g_log << "Log start"    << std::endl; /* TODO, include date */
	//g_log << "Init: "       << std::endl;
	//

#ifdef PLATFORM_WIN
	if ( IsDebuggerPresent() )
	{
		// We don't want to mask exceptions (or report them to Steam!) when debugging.
		// If you would like to step through the exception handler, attach a debugger
		// after running the game outside of the debugger.	

		Main();
		return 0;
	}
#endif

#ifdef PLATFORM_WIN
#ifdef USESTEAM
	_set_se_translator( MiniDumpFunction );

	try  // this try block allows the SE translator to work
	{
#endif
#endif
		Main();
#ifdef PLATFORM_WIN
#ifdef USESTEAM
	}
	catch( ... )
	{
		return -1;
	}
#endif
#endif
	
#ifdef PLATFORM_IOS
	exit(0);
#endif

	return 0;
}
